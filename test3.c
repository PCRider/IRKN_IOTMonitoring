#include <avr/io.h>
#include <stdio.h>
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

        // please modify the following two lines. mac and ip have to be unique
// in your local area network. You can not have the same numbers in
// two devices:
static unsigned char mymac[6] = {0x50,0x52,0x69,0x64,0x65,0x72};
static unsigned char myip[4] = {192,168,1,100};
static unsigned char wwwport=80; // 80 is just a default value. Gets overwritten during init


#define BUFFER_SIZE 550
static unsigned char buf[BUFFER_SIZE+1];
// listen port for www
#define MYWWWPORT 80
//// listen port for udp
#define MYUDPPORT 1200


#ifdef DEBUG
#define DEBUGF(format, args...) printf_P(PSTR(format), ##args)

/* ------------------------------------------------------------------------- */
static int uart_putchar(char c, FILE *stream) {
  if (c == '\n')
    uart_putchar('\r', stream);
  loop_until_bit_is_set(UCSR1A, UDRE1);
  UDR1 = c;
  return 0;
}

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL,
                                         _FDEV_SETUP_WRITE);
#else
#define DEBUGF(format, args...)
#endif

int main(void){
        unsigned int plen=0;
        unsigned int dat_p=0;
        unsigned char i=0;
        unsigned char payloadlen=0;
        int hdrlen;
        int datalen;
unsigned char *seqnum; // my initial tcp sequence number
*seqnum = 0;
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
#ifdef DEBUG
#define BAUD 19200
#define MYUBRR F_CPU/16/BAUD-1
  /* quick'n dirty uart init */
  UCSR1B |= (1<<TXEN1);
  /*Set baud rate */
UBRR1H = (unsigned char)(MYUBRR>>8);
UBRR1L = (unsigned char)MYUBRR;

#endif
#ifdef DEBUG
  stdout = &mystdout;
#endif
lcd_puts("enc28j60 circuit\n");
        // LED
        /* enable PC0, LED as output */
        DDRC|= (1<<DDC0);

        /* set output to Vcc, LED off */
        PORTC|= (1<<PORTC0);

        /*initialize enc28j60*/
        enc28j60Init(mymac);
        enc28j60clkout(2); // change clkout from 6.25MHz to 12.5MHz
        _delay_ms(10);
        
        /* Magjack leds configuration, see enc28j60 datasheet, page 11 */
        // LEDB=yellow LEDA=green
        //
        // 0x476 is PHLCON LEDA=links status, LEDB=receive/transmit
        // enc28j60PhyWrite(PHLCON,0b0000 0100 0111 01 10);
	enc28j60PhyWrite(PHLCON,PHLCON_ATBRS139);
        _delay_loop_1(50); // 12ms
        
        /* set output to GND, red LED on */
        //PORTB &= ~(1<<PORTB1);
        i=1;
        
        //init the ethernet/ip layer:
        //init_ip_arp_udp_tcp(mymac,myip,wwwport);

        while(1){
        	DEBUGF("in loop\n");
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
                // check if ip packets (icmp or udp) are for us:
                if(eth_type_is_ip_and_my_ip(buf,myip,plen)==0){
                        continue;
                }
                
                if (i){
                        /* set output to Vcc, LED off */
                        PORTC|= (1<<PORTC0);
                        i=0;
                }else{
                        /* set output to GND, LED on */
                        PORTC &= ~(1<<PORTC0);
                        i=1;
                }

                        
                if(buf[IP_PROTO_P]==IP_PROTO_ICMP_V && buf[ICMP_TYPE_P]==ICMP_TYPE_ECHOREQUEST_V){
                        // a ping packet, let's send pong
                        make_echo_reply_from_request(buf,mymac,myip,plen);
                        continue;
                }
                // tcp port www start, compare only the lower byte
                if (buf[IP_PROTO_P]==IP_PROTO_TCP_V&&buf[TCP_DST_PORT_H_P]==0&&buf[TCP_DST_PORT_L_P]==wwwport){
                        if (buf[TCP_FLAGS_P] & TCP_FLAGS_SYN_V){
                                make_tcp_synack_from_syn(buf,mymac,myip,wwwport,seqnum);
                                // make_tcp_synack_from_syn does already send the syn,ack
                                continue;
                        }
                        if (buf[TCP_FLAGS_P] & TCP_FLAGS_ACK_V){
                        	    datalen =data_len(buf); // init some data structures
                        	    hdrlen =hdr_len(buf);
                                // we can possibly have no data, just ack:
                                dat_p=get_tcp_data_pointer(hdrlen,datalen);
                                if (dat_p==0){

                                        if (buf[TCP_FLAGS_P] & TCP_FLAGS_FIN_V){
                                                // finack, answer with ack
                                                make_tcp_ack_from_any(buf,datalen,mymac,myip,wwwport,seqnum);
                                        }
                                        // just an ack with no data, wait for next packet
                                        continue;
                                }
                                lcd_puts("before get\n");
                                //DEBUGF("buf[dat_p] %x\n", buf[dat_p]);
                                if (strncmp("GET ",(char *)&(buf[dat_p]),4)!=0){
                                        // head, post and other methods:
                                        //
                                        // for possible status codes see:
                                        // http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
                                        plen=fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>200 OK</h1>"));
                                }else{
                                        // the "get" method
                                        plen=fill_tcp_data_p(buf,0,PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n<p>OK, it works</p>"));
                                }

                                make_tcp_ack_from_any(buf,datalen,mymac,myip,wwwport,seqnum); // send ack for http get
                                make_tcp_ack_with_data(buf,plen); // send data
                                continue;
                        }

                }
                // udp interface:
                if (buf[IP_PROTO_P]==IP_PROTO_UDP_V){
                        payloadlen=buf[UDP_LEN_L_P]-UDP_HEADER_LEN;
                        // the received command has to start with t and be 4 char long
                        // e.g "test\0"
                        if (buf[UDP_DATA_P]=='t' && payloadlen==5){
                                make_udp_reply_from_request(buf,"hello",6,mymac,myip,MYUDPPORT);
                        }
                }
        }
        return (0);
}
