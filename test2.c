#include <avr/io.h>
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



int main(void){

        
        unsigned int plen;
        unsigned char i=0;
        // please modify the following two lines. mac and ip have to be unique
// in your local area network. You can not have the same numbers in
// two devices:
static unsigned char mymac[6] = {0x54,0x55,0x58,0x10,0x00,0x24};
static unsigned char myip[4] = {192,168,1,100};
// how did I get the mac addr? Translate the first 3 numbers into ascii is: TUX

#define BUFFER_SIZE 550
static unsigned char buf[BUFFER_SIZE+1];
        

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
	// LEDA=greed LEDB=yellow
	enc28j60PhyWrite(PHLCON,PHLCON_ATBRS139);
        _delay_loop_1(50); // 12ms
        
        //init the ethernet/ip layer:
        //init_ip_arp_udp_tcp(mymac,myip,80);

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
        }
        return (0);
}
