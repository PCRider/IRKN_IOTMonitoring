//********************************************************************************************
//
// File : tcp.c implement for Transmission Control Protocol
//
//********************************************************************************************
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "enc28j60.h"
#include "arp.h"
#include "eth.h"
#include "ip.h"
#include "tcp.h"
#include "macconfig.h"
//********************************************************************************************
//
// +------------+-----------+------------+----------+
// + MAC header + IP header + TCP header + Data ::: +
// +------------+-----------+------------+----------+
//
// TCP Header
//
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// +00+01+02+03+04+05+06+07+08+09+10+11+12+13+14+15+16+17+18+19+20+21+22+23+24+25+26+27+28+29+30+31+
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// +               Source Port                     +                Destination Port               +
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// +                                        Sequence Number                                        +
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// +                                     Acknowledgment Number                                     +
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// +Data Offset+reserved+   ECN  +  Control Bits   +                  Window size                  +
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// +                 Checksum                      +                Urgent Pointer                 +
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// +                                   Options and padding :::                                     +
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// +                                             Data :::                                          +
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//
//********************************************************************************************

// make a return tcp header from a received tcp packet
// rel_ack_num is how much we must step the seq number received from the
// other side. We do not send more than 255 bytes of text (=data) in the tcp packet.
// If mss=1 then mss is included in the options list
//
// After calling this function you can fill in the first data byte at TCP_OPTIONS_P+4
// If cp_seq=0 then an initial sequence number is used (should be use in synack)
// otherwise it is copied from the packet we received
void make_tcphead(unsigned char *buf,unsigned int rel_ack_num,unsigned char mss,unsigned char cp_seq, unsigned char wwwport,unsigned char *seqnum)
{
        unsigned char i=0;
        unsigned char tseq;
        while(i<2){
                buf[TCP_DST_PORT_H_P+i]=buf[TCP_SRC_PORT_H_P+i];
                buf[TCP_SRC_PORT_H_P+i]=0; // clear source port
                i++;
        }
        // set source port  (http):
        buf[TCP_SRC_PORT_L_P]=wwwport;
        i=4;
        // sequence numbers:
        // add the rel ack num to SEQACK
        while(i>0){
                rel_ack_num=buf[TCP_SEQ_H_P+i-1]+rel_ack_num;
                tseq=buf[TCP_SEQACK_H_P+i-1];
                buf[TCP_SEQACK_H_P+i-1]=0xff&rel_ack_num;
                if (cp_seq){
                        // copy the acknum sent to us into the sequence number
                        buf[TCP_SEQ_H_P+i-1]=tseq;
                }else{
                        buf[TCP_SEQ_H_P+i-1]= 0; // some preset vallue
                }
                rel_ack_num=rel_ack_num>>8;
                i--;
        }
        if (cp_seq==0){
                // put inital seq number
                buf[TCP_SEQ_H_P+0]= 0;
                buf[TCP_SEQ_H_P+1]= 0;
                // we step only the second byte, this allows us to send packts 
                // with 255 bytes or 512 (if we step the initial seqnum by 2)
                buf[TCP_SEQ_H_P+2]= (*seqnum); 
                buf[TCP_SEQ_H_P+3]= 0;
                // step the inititial seq num by something we will not use
                // during this tcp session:
                (*seqnum) += 2;
        }
        // zero the checksum
        buf[TCP_CHECKSUM_H_P]=0;
        buf[TCP_CHECKSUM_L_P]=0;

        // The tcp header length is only a 4 bit field (the upper 4 bits).
        // It is calculated in units of 4 bytes. 
        // E.g 24 bytes: 24/4=6 => 0x60=header len field
        //buf[TCP_HEADER_LEN_P]=(((TCP_HEADER_LEN_PLAIN+4)/4)) <<4; // 0x60
        if (mss){
                // the only option we set is MSS to 1408:
                // 1408 in hex is 0x580
                buf[TCP_OPTIONS_P]=2;
                buf[TCP_OPTIONS_P+1]=4;
                buf[TCP_OPTIONS_P+2]=0x05; 
                buf[TCP_OPTIONS_P+3]=0x80;
                // 24 bytes:
                buf[TCP_HEADER_LEN_P]=0x60;
        }else{
                // no options:
                // 20 bytes:
                buf[TCP_HEADER_LEN_P]=0x50;
        }
}




void make_tcp_synack_from_syn(unsigned char *buf, unsigned char *macaddr, unsigned char *ipaddr, unsigned char wwwport,unsigned char *seqnum)
{
        unsigned int ck;
        make_eth(buf,macaddr);
        // total length field in the IP header must be set:
        // 20 bytes IP + 24 bytes (20tcp+4tcp options)
        buf[IP_TOTLEN_H_P]=0;
        buf[IP_TOTLEN_L_P]=IP_HEADER_LEN+TCP_HEADER_LEN_PLAIN+4;
        make_ip(buf,ipaddr);
        buf[TCP_FLAGS_P]=TCP_FLAGS_SYNACK_V;
        make_tcphead(buf,1,1,0,wwwport,seqnum);
        // calculate the checksum, len=8 (start from ip.src) + TCP_HEADER_LEN_PLAIN + 4 (one option: mss)
        ck=checksum(&buf[IP_SRC_P], 8+TCP_HEADER_LEN_PLAIN+4,2);
        buf[TCP_CHECKSUM_H_P]=ck>>8;
        buf[TCP_CHECKSUM_L_P]=ck& 0xff;
        // add 4 for option mss:
        enc28j60PacketSend(IP_HEADER_LEN+TCP_HEADER_LEN_PLAIN+4+ETH_HEADER_LEN,buf);
}

// get a pointer to the start of tcp data in buf
// Returns 0 if there is no data
// You must call init_len_info once before calling this function
unsigned int get_tcp_data_pointer(int info_hdr_len, int info_data_len)
{
        if ( info_data_len ){
                return((unsigned int)TCP_SRC_PORT_H_P + info_hdr_len);
        }else{
                return(0);
        }
}

// do some basic length calculations and store the result in static varibales
int data_len(unsigned char *buf)
{
		int info_data_len=0;
		int info_hdr_len = 0;
        info_data_len = (((int)buf[IP_TOTLEN_H_P])<<8)|(buf[IP_TOTLEN_L_P]&0xff);
        info_data_len -= IP_HEADER_LEN;
        info_hdr_len = hdr_len(buf);
        info_data_len -= info_hdr_len;
        if ( info_data_len <=0){
                info_data_len =0;
        }
        return info_data_len;
}
int hdr_len(unsigned char *buf){
	int info_hdr_len;
	info_hdr_len = (buf[TCP_HEADER_LEN_P]>>4)*4; // generate len in bytes;
	return info_hdr_len;
}
// fill in tcp data at position pos. pos=0 means start of
// tcp data. Returns the position at which the string after
// this string could be filled.
unsigned int fill_tcp_data_p(unsigned char *buf,unsigned int pos, const char *progmem_s)
{
        char c;
        // fill in tcp data at position pos
        //
        // with no options the data starts after the checksum + 2 more bytes (urgent ptr)
        while ((c = pgm_read_byte(progmem_s++))) {
                buf[TCP_CHECKSUM_L_P+3+pos]=c;
                pos++;
        }
        return(pos);
}

// fill in tcp data at position pos. pos=0 means start of
// tcp data. Returns the position at which the string after
// this string could be filled.
unsigned int fill_tcp_data(unsigned char *buf,unsigned int pos, const char *s)
{
        // fill in tcp data at position pos
        //
        // with no options the data starts after the checksum + 2 more bytes (urgent ptr)
        while (*s) {
                buf[TCP_CHECKSUM_L_P+3+pos]=*s;
                pos++;
                s++;
        }
        return(pos);
}

// Make just an ack packet with no tcp data inside
// This will modify the eth/ip/tcp header 
void make_tcp_ack_from_any(unsigned char *buf,int info_data_len,unsigned char *macaddr,unsigned char *ipaddr,unsigned char wwwport,unsigned char *seqnum)
{
        unsigned int j;
        make_eth(buf,macaddr);
        // fill the header:
        buf[TCP_FLAGS_P]=TCP_FLAGS_ACK_V;
        if (info_data_len==0){
                // if there is no data then we must still acknoledge one packet
                make_tcphead(buf,1,0,1,wwwport,seqnum); // no options
        }else{
                make_tcphead(buf,info_data_len,0,1,wwwport,seqnum); // no options
        }

        // total length field in the IP header must be set:
        // 20 bytes IP + 20 bytes tcp (when no options) 
        j=IP_HEADER_LEN+TCP_HEADER_LEN_PLAIN;
        buf[IP_TOTLEN_H_P]=j>>8;
        buf[IP_TOTLEN_L_P]=j& 0xff;
        make_ip(buf,ipaddr);
        // calculate the checksum, len=8 (start from ip.src) + TCP_HEADER_LEN_PLAIN + data len
        j=checksum(&buf[IP_SRC_P], 8+TCP_HEADER_LEN_PLAIN,2);
        buf[TCP_CHECKSUM_H_P]=j>>8;
        buf[TCP_CHECKSUM_L_P]=j& 0xff;
        enc28j60PacketSend(IP_HEADER_LEN+TCP_HEADER_LEN_PLAIN+ETH_HEADER_LEN,buf);
}

// you must have called init_len_info at some time before calling this function
// dlen is the amount of tcp data (http data) we send in this packet
// You can use this function only immediately after make_tcp_ack_from_any
// This is because this function will NOT modify the eth/ip/tcp header except for
// length and checksum
void make_tcp_ack_with_data(unsigned char *buf,unsigned int dlen)
{
        unsigned int j;
        // fill the header:
        // This code requires that we send only one data packet
        // because we keep no state information. We must therefore set
        // the fin here:
        buf[TCP_FLAGS_P]=TCP_FLAGS_ACK_V|TCP_FLAGS_PUSH_V|TCP_FLAGS_FIN_V;

        // total length field in the IP header must be set:
        // 20 bytes IP + 20 bytes tcp (when no options) + len of data
        j=IP_HEADER_LEN+TCP_HEADER_LEN_PLAIN+dlen;
        buf[IP_TOTLEN_H_P]=j>>8;
        buf[IP_TOTLEN_L_P]=j& 0xff;
        fill_ip_hdr_checksum(buf);
        // zero the checksum
        buf[TCP_CHECKSUM_H_P]=0;
        buf[TCP_CHECKSUM_L_P]=0;
        // calculate the checksum, len=8 (start from ip.src) + TCP_HEADER_LEN_PLAIN + data len
        j=checksum(&buf[IP_SRC_P], 8+TCP_HEADER_LEN_PLAIN+dlen,2);
        buf[TCP_CHECKSUM_H_P]=j>>8;
        buf[TCP_CHECKSUM_L_P]=j& 0xff;
        enc28j60PacketSend(IP_HEADER_LEN+TCP_HEADER_LEN_PLAIN+dlen+ETH_HEADER_LEN,buf);
}

/* end of tcp.c */
