#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
#ifndef ALIBC_OLD
#include <util/delay.h>
#else
#include <avr/delay.h>
#endif
#include "eth.h"
#include "arp.h"
#include "icmp.h"
#include "enc28j60.h"
#include "ip.h"
#include "tcp.h"
#include "udp.h"
#include "lcd.h"


// listen port for tcp/www (max range 1-254)
#define MYWWWPORT 80
//
// listen port for udp
#define MYUDPPORT 1200

#define BUFFER_SIZE 550
static unsigned char buf[BUFFER_SIZE+1];

// the password string (only the first 5 char checked), (only a-z,0-9,_ characters):
static char password[]="secret"; // must not be longer than 9 char


void send_tcp(char* buf,int datalen,unsigned char* mymac,unsigned char* myip,char wwwport,unsigned char* seqnum,unsigned int plen){
	make_tcp_ack_from_any(buf,datalen,mymac,myip,wwwport,seqnum); // send ack for http get
	make_tcp_ack_with_data(buf,plen); // send data
}

unsigned char verify_password(char *str)
{
        // the first characters of the received string are
        // a simple password/cookie:
        if (strncmp(password,str,5)==0){
                return(1);
        }
        return(0);
}

// takes a string of the form password/commandNumber and analyse it
// return values: -1 invalid password, otherwise command number
//                -2 no command given but password valid
//                -3 valid password, no command and no trailing "/"
int8_t analyse_get_url(char *str)
{
        unsigned char loop=1;
        unsigned char i=0;
        while(loop){
                if(password[i]){
                        if(*str==password[i]){
                                str++;
                                i++;
                        }else{
                                return(-1);
                        }
                }else{
                        // end of password
                        loop=0;
                }
        }
        // is is now one char after the password
        if (*str == '/'){
                str++;
        }else{
                return(-3);
        }
        // check the first char, garbage after this is ignored (including a slash)
        if (*str < 0x3a && *str > 0x2f){
                // is a ASCII number, return it
                return(*str-0x30);
        }
        return(-2);
}

// answer HTTP/1.0 301 Moved Permanently\r\nLocation: password/\r\n\r\n
// to redirect to the url ending in a slash
unsigned int moved_perm(unsigned char *buf)
{
        unsigned int plen;
        plen=fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 301 Moved Permanently\r\nLocation: "));
        plen=fill_tcp_data(buf,plen,password);
        plen=fill_tcp_data_p(buf,plen,PSTR("/\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n"));
        plen=fill_tcp_data_p(buf,plen,PSTR("<h1>301 Moved Permanently</h1>\n"));
        plen=fill_tcp_data_p(buf,plen,PSTR("add a trailing slash to the url\n"));
        return(plen);
}


// prepare the webpage by writing the data to the tcp send buffer
unsigned int print_webpage(unsigned char *buf,unsigned char on_off)
{
        unsigned int plen;
        plen=fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n"));
        plen=fill_tcp_data_p(buf,plen,PSTR("<center><p>Output is: "));
        if (on_off){
                plen=fill_tcp_data_p(buf,plen,PSTR("<font color=\"#00FF00\"> ON</font>"));
        }else{
                plen=fill_tcp_data_p(buf,plen,PSTR("OFF"));
        }
        plen=fill_tcp_data_p(buf,plen,PSTR(" <small><a href=\".\">[refresh status]</a></small></p>\n<p><a href=\"."));
        if (on_off){
                plen=fill_tcp_data_p(buf,plen,PSTR("/0\">Switch off</a><p>"));
        }else{
                plen=fill_tcp_data_p(buf,plen,PSTR("/1\">Switch on</a><p>"));
        }
        plen=fill_tcp_data_p(buf,plen,PSTR("</center><hr><br>\n"));
        return(plen);
}


int main(void){
        
        unsigned int plen;
        unsigned int dat_p;
        unsigned char i=0;
        unsigned char cmd_pos=0;
        char cmd;
        unsigned char payloadlen=0;
        int hdrlen;
        int datalen;
        unsigned char *seqnum; // my initial tcp sequence number
        char str[30];
        char cmdval;
        
        *seqnum = 16;
        DDRA &=0;
        lcd_init(LCD_DISP_ON);
	lcd_clrscr();
	lcd_puts("enc28j60 circuit\n");

        
        // please modify the following two lines. mac and ip have to be unique
// in your local area network. You can not have the same numbers in
// two devices:
static unsigned char mymac[6] = {0x54,0x55,0x58,0x10,0x00,0x24};
// how did I get the mac addr? Translate the first 3 numbers into ascii is: TUX
static unsigned char myip[4] = {192,168,0,225};
//static unsigned char myip[4] = {192,168,255,100};

        // set the clock speed to "no pre-scaler" (8MHz with internal osc or 
        // full external speed)
        // set the clock prescaler. First write CLKPCE to enable setting of clock the
        // next four instructions.
        //CLKPR=(1<<CLKPCE); // change enable
        //CLKPR=0; // "no pre-scaler"
        //_delay_loop_1(50); // 12ms

        /* enable PD2/INT0, as input */
        //DDRD&= ~(1<<DDD2);

        /*initialize enc28j60*/
        enc28j60Init(mymac);
        enc28j60clkout(2); // change clkout from 6.25MHz to 12.5MHz
        _delay_loop_1(50); // 12ms
        
        // LED
        /* enable PC0, LED as output */
        //DDRC|= (1<<DDC0);

        /* set output to Vcc, LED off */
        //PORTC|= (1<<PORTC0);

        // the transistor on PC1
        //DDRC|= (1<<DDC1);
        //PORTC &= ~(1<<PORTC1);// transistor off
        
        /* Magjack leds configuration, see enc28j60 datasheet, page 11 */
        // LEDB=yellow LEDA=green
        //
        // 0x476 is PHLCON LEDA=links status, LEDB=receive/transmit
        // enc28j60PhyWrite(PHLCON,0b0000 0100 0111 01 10);
        enc28j60PhyWrite(PHLCON,PHLCON_ATBRS139);
        _delay_loop_1(50); // 12ms
        
        /* set output to GND, red LED on */
        //PORTC &= ~(1<<PORTC0);
        i=1;

        //init the ethernet/ip layer:
	//lcd_clrscr();
	//lcd_puts(PINA);
        while(1){
        	
	        
                // get the next new packet:
                plen = enc28j60PacketReceive(BUFFER_SIZE, buf);

                /*plen will ne unequal to zero if there is a valid 
                 * packet (without crc error) */
                if(plen==0){
                        continue;
                }
                        
                // arp is broadcast if unknown but a host may also
                // verify the mac address by sending it to 
                // a unicast address.
                if(eth_type_is_arp_and_my_ip(buf,myip,plen)){
                        make_arp_answer_from_request(buf,mymac,myip);
                        continue;
                }

                // check if ip packets are for us:
                if(eth_type_is_ip_and_my_ip(buf,myip,plen)==0){
                        continue;
                }
                // led----------
                if (i){
                        /* set output to Vcc, LED off */
                        //PORTC|= (1<<PORTC0);
                        i=0;
                }else{
                        /* set output to GND, LED on */
                        //PORTC &= ~(1<<PORTC0);
                        i=1;
                }
                
                if(buf[IP_PROTO_P]==IP_PROTO_ICMP_V && buf[ICMP_TYPE_P]==ICMP_TYPE_ECHOREQUEST_V){
                        // a ping packet, let's send pong
                        make_echo_reply_from_request(buf,mymac,myip,plen);
                        continue;
                }
                // tcp port www start, compare only the lower byte
                if (buf[IP_PROTO_P]==IP_PROTO_TCP_V&&buf[TCP_DST_PORT_H_P]==0&&buf[TCP_DST_PORT_L_P]==MYWWWPORT){
                        if (buf[TCP_FLAGS_P] & TCP_FLAGS_SYN_V){
                                make_tcp_synack_from_syn(buf,mymac,myip,MYWWWPORT,seqnum);
                                // make_tcp_synack_from_syn does already send the syn,ack
                                continue;
                        }
                        if (buf[TCP_FLAGS_P] & TCP_FLAGS_ACK_V){
                                //init_len_info(buf); // init some data structures
                                datalen =data_len(buf); // init some data structures
                                hdrlen =hdr_len(buf);
                                // we can possibly have no data, just ack:
                                dat_p=get_tcp_data_pointer(hdrlen,datalen);
                                if (dat_p==0){
                                        if (buf[TCP_FLAGS_P] & TCP_FLAGS_FIN_V){
                                                // finack, answer with ack
                                                make_tcp_ack_from_any(buf,datalen,mymac,myip,MYWWWPORT,seqnum);
                                        }
                                        // just an ack with no data, wait for next packet
                                        continue;
                                }
                                if (strncmp("data ",(char *)&(buf[dat_p]),4)!=0){
                                        // head, post and other methods:
                                        //
                                        // for possible status codes see:
                                        // http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
                                        plen=fill_tcp_data_p(buf,0,PSTR("PINA value is:"));
                                        buf[TCP_CHECKSUM_L_P+3+plen]=PINA;
                                        plen++;
                                        
                                        send_tcp(buf,datalen,mymac,myip,MYWWWPORT,seqnum,plen);
                                        continue;
                                }
                                
                                if (strncmp("GET ",(char *)&(buf[dat_p]),4)!=0){
                                        // head, post and other methods:
                                        //
                                        // for possible status codes see:
                                        // http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
                                        plen=fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>200 OK</h1>"));
                                        send_tcp(buf,datalen,mymac,myip,MYWWWPORT,seqnum,plen);
                                        continue;
                                }
                                if (strncmp("/ ",(char *)&(buf[dat_p+4]),2)==0){
                                        plen=fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n"));
                                        plen=fill_tcp_data_p(buf,plen,PSTR("<h1>PINA value is "));
                                        buf[TCP_CHECKSUM_L_P+3+plen]=PINA;
                                        plen++;
                                        plen=fill_tcp_data_p(buf,plen,PSTR("</h1>\n"));
                                        send_tcp(buf,datalen,mymac,myip,MYWWWPORT,seqnum,plen);
                                        continue;
                                }
                                cmd=analyse_get_url((char *)&(buf[dat_p+5]));
                                // for possible status codes see:
                                // http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
                                if (cmd==-1){
                                        plen=fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 401 Unauthorized\r\nContent-Type: text/html\r\n\r\n<h1>401 Unauthorized</h1>"));
                                        send_tcp(buf,datalen,mymac,myip,MYWWWPORT,seqnum,plen);
                                        continue;
                                }
                                if (cmd==1){
                                        //PORTC|= (1<<PORTC1);// transistor on
                                }
                                if (cmd==0){
                                        //PORTC &= ~(1<<PORTC1);// transistor off
                                }
                                if (cmd==-3){
                                        // redirect to add a trailing slash
                                        plen=moved_perm(buf);
                                        send_tcp(buf,datalen,mymac,myip,MYWWWPORT,seqnum,plen);
                                        continue;
                                }
                                // if (cmd==-2) or any other value
                                // just display the status:
                                plen=print_webpage(buf,(PORTC & (1<<PORTC1)));
                                //
                                send_tcp(buf,datalen,mymac,myip,MYWWWPORT,seqnum,plen);
                                continue;
                        }

                }
                // tcp port www end
                //
                // udp start, we listen on udp port 1200=0x4B0
                if (buf[IP_PROTO_P]==IP_PROTO_UDP_V&&buf[UDP_DST_PORT_H_P]==4&&buf[UDP_DST_PORT_L_P]==0xb0){
                        payloadlen=buf[UDP_LEN_L_P]-UDP_HEADER_LEN;
                        // you must sent a string starting with v
                        // e.g udpcom version 10.0.0.24
                        if (verify_password((char *)&(buf[UDP_DATA_P]))){
                                // find the first comma which indicates 
                                // the start of a command:
                                cmd_pos=0;
                                while(cmd_pos<payloadlen){
                                        cmd_pos++;
                                        if (buf[UDP_DATA_P+cmd_pos]==','){
                                                cmd_pos++; // put on start of cmd
                                                break;
                                        }
                                }
                                // a command is one char and a value. At
                                // least 3 characters long. It has an '=' on
                                // position 2:
                                if (cmd_pos<2 || cmd_pos>payloadlen-3 || buf[UDP_DATA_P+cmd_pos+1]!='='){
                                        strcpy(str,"e=no_cmd");
                                        make_udp_reply_from_request(buf,str,strlen(str),mymac,myip,MYUDPPORT);
                                }
                                // supported commands are
                                // t=1 t=0 t=?
                                if (buf[UDP_DATA_P+cmd_pos]=='t'){
                                        cmdval=buf[UDP_DATA_P+cmd_pos+2];
                                        if(cmdval=='1'){
                                                //PORTC|= (1<<PORTC1);// transistor on
                                                strcpy(str,"t=1");
                                                make_udp_reply_from_request(buf,str,strlen(str),mymac,myip,MYUDPPORT);
                                        }else if(cmdval=='0'){
                                                //PORTC &= ~(1<<PORTC1);// transistor off
                                                strcpy(str,"t=0");
                                                make_udp_reply_from_request(buf,str,strlen(str),mymac,myip,MYUDPPORT);
                                        }else if(cmdval=='?'){
                                                if (PORTC & (1<<PORTC1)){
                                                        strcpy(str,"t=1");
                                                        make_udp_reply_from_request(buf,str,strlen(str),mymac,myip,MYUDPPORT);
                                                }
                                                strcpy(str,"t=0");
                                                make_udp_reply_from_request(buf,str,strlen(str),mymac,myip,MYUDPPORT);
                                        }
                                }
                                strcpy(str,"e=no_such_cmd");
                                make_udp_reply_from_request(buf,str,strlen(str),mymac,myip,MYUDPPORT);
                        }
                        strcpy(str,"e=invalid_pw");
                        make_udp_reply_from_request(buf,str,strlen(str),mymac,myip,MYUDPPORT);
                }
        }
        return (0);
}
