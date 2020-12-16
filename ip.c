//********************************************************************************************
//
// File : ip.c implement for Internet Protocol
// Created: 4/10/2020 12:41:08 PM
// Author: Ehsan Ghasemlou
// enc28j60.h
// Copyright © 2020 Ehsan Ghasemlou. All rights reserved.
//
//                XMMMWMMM0MMMMMMMMMi
//               7MMMMMMMMMMMMMMMMMMW,
//              MMMMMMMMMM0W@@80MMMMMMi
//             SMM0Xrrri:,      ,rWMMM7
//            .MM2.               ,8MMB
//            XMW           .      i0MM:
//            SM2..                iBMM:
//            rM0.              .  :MMM.
//             MM:rXr;:.  .;SZZZaS; XMM
//             XW700MMMM2rXZMBMB22X..M7,.
//            :77.r2r07Sr  :7;2Xr7,  ai,;
//            rX;  .,.. :  .  ...    .;.i
//            ,;i.     .,  ,       ..:,i,
//             ;;:...  :i  ::,  . ..,i:,.
//             i;;:. . ;S77X;, ....,,:
//               :;.... .::     ,.,.::
//                i:.:r7;i:;;77:....:;,S
//                ,;:,:::ii:,.:,,,,;r  @B,
//                 r;:,,,::,,,..,iSr  :MWM2:
//                 87;,,.     .:XWX   BB08B0M8X:
//                :M7rrrr;;;;rXa0r   0W8ZZZZBMMMM0X,
//            .XW ra8:;rXXSS2XXr;:  ZW8ZZZZZBWB0WWM@
//        .78MMM7 ;7MX,iiiiiii;,iS SM8ZZZ8880W000088
//    ,rZMMMMM0M7..:ZWii;i;i;;i rr;@088Z880ZBB0Z088Z
//  XMMMMMWWWWW0MM:,;SX;i;rr;r,,.r00Z08008888B8008ZZ
//.aMM0WWWWW0WWWMMrr:  iiii::,;, aBZ88008Z0Z80008ZZZ
//********************************************************************************************
#include "ip.h"
//********************************************************************************************
//
// +------------+-----------+----------+
// + MAC header + IP header + Data ::: +
// +------------+-----------+----------+
//
// IP Header
//
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// +00+01+02+03+04+05+06+07+08+09+10+11+12+13+14+15+16+17+18+19+20+21+22+23+24+25+26+27+28+29+30+31+
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// + Version   +   IHL     +             TOS       +                Total length                   +
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// +                  Identification               +  Flags +          Fragment offset             +
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// +          TTL          +       Protocol        +                Header checksum                +
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// +                                      Source IP address                                        +
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// +                                    Destination IP address                                     +
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// +                                    Options and padding :::                                    +
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//
//********************************************************************************************
// The Ip checksum is calculated over the ip header only starting
// with the header length field and a total length of 20 bytes
// unitl ip.dst
// You must set the IP checksum field to zero before you start
// the calculation.
// len for ip is 20.
//
// For UDP/TCP we do not make up the required pseudo header. Instead we 
// use the ip.src and ip.dst fields of the real packet:
// The udp checksum calculation starts with the ip.src field
// Ip.src=4bytes,Ip.dst=4 bytes,Udp header=8bytes + data length=16+len
// In other words the len here is 8 + length over which you actually
// want to calculate the checksum.
// You must set the checksum field to zero before you start
// the calculation.
// len for udp is: 8 + 8 + data length
// len for tcp is: 4+4 + 20 + option len + data length
//
// For more information on how this algorithm works see:
// http://www.netfor2.com/checksum.html
// http://www.msc.uky.edu/ken/cs471/notes/chap3.htm
// The RFC has also a C code example: http://www.faqs.org/rfcs/rfc1071.html
unsigned int checksum(unsigned char *buf, unsigned int len,unsigned char type)
{
        // type 0=ip 
        //      1=udp
        //      2=tcp
        unsigned long int sum = 0;

        //if(type==0){
        //        // do not add anything
        //}
        if(type==1){
                sum+=IP_PROTO_UDP_V; // protocol udp
                // the length here is the length of udp (data+header len)
                // =length given to this function - (IP.scr+IP.dst length)
                sum+=len-8; // = real tcp len
        }
        if(type==2){
                sum+=IP_PROTO_TCP_V; 
                // the length here is the length of tcp (data+header len)
                // =length given to this function - (IP.scr+IP.dst length)
                sum+=len-8; // = real tcp len
        }
        // build the sum of 16bit words
        while(len >1){
                sum += 0xFFFF & (((unsigned long int)*buf<<8)|*(buf+1));
                buf+=2;
                len-=2;
        }
        // if there is a byte left then add it (padded with zero)
        if (len){
                sum += ((unsigned long int)(0xFF & *buf))<<8;
        }
        // now calculate the sum over the bytes in the sum
        // until the result is only 16bit long
        while (sum>>16){
                sum = (sum & 0xFFFF)+(sum >> 16);
        }
        // build 1's complement:
        return( (unsigned int) sum ^ 0xFFFF);
}


void fill_ip_hdr_checksum(unsigned char *buf)
{
        unsigned int ck;
        // clear the 2 byte checksum
        buf[IP_CHECKSUM_P]=0;
        buf[IP_CHECKSUM_P+1]=0;
        buf[IP_FLAGS_P]=0x40; // don't fragment
        buf[IP_FLAGS_P+1]=0;  // fragement offset
        buf[IP_TTL_P]=64; // ttl
        // calculate the checksum:
        ck=checksum(&buf[IP_P], IP_HEADER_LEN,0);
        buf[IP_CHECKSUM_P]=ck>>8;
        buf[IP_CHECKSUM_P+1]=ck& 0xff;
}

// make a return ip header from a received ip packet
void make_ip(unsigned char *buf, unsigned char *ipaddr)
{
        unsigned char i=0;
        while(i<4){
                buf[IP_DST_P+i]=buf[IP_SRC_P+i];
                buf[IP_SRC_P+i]=ipaddr[i];
                i++;
        }
        fill_ip_hdr_checksum(buf);
}
