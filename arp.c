//********************************************************************************************
//
// File : arp.c implement for Address Resolution Protocol
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
#include "arp.h"
#include "eth.h"
#include "enc28j60.h"
//********************************************************************************************
// https://en.wikipedia.org/wiki/Address_Resolution_Protocol
// The Address Resolution Protocol (ARP) is a communication protocol used for discovering 
// the link layer address, such as a MAC address, associated with a given internet layer address,
// typically an IPv4 address. 
// This mapping is a critical function in the Internet protocol suite.
// https://tools.ietf.org/html/rfc826
// RFC 826 - Ethernet Address Resolution Protocol, Internet Standard STD 37.
// https://tools.ietf.org/html/rfc903
// RFC 903 - Reverse Address Resolution Protocol, Internet Standard STD 38.
// https://tools.ietf.org/html/rfc2390
// RFC 2390 - Inverse Address Resolution Protocol, draft standard
// https://tools.ietf.org/html/rfc5227
// RFC 5227 - IPv4 Address Conflict Detection, proposed standard
//
// ARP has been implemented with many combinations of network and data link layer technologies,
// such as IPv4, Chaosnet, DECnet and Xerox PARC Universal Packet (PUP) using IEEE 802.11 standards,
// Token Ring, FDDI, X.25, Frame Relay and IP over Asynchronous Transfer Mode (ATM).
//
//
//
// ARP is used in four cases of two hosts communicating:
//
//   1. When two hosts are on the same network and one desires to send a packet to the other
//   2. When two hosts are on different networks and must use a gateway/router to reach the other host
//   3. When a router needs to forward a packet for one host through another router
//   4. When a router needs to forward a packet from one host to the destination host on the same network
//
// +------------+------------+-----------+
// + MAC header + ARP header +  Data ::: +
// +------------+------------+-----------+
//
// ARP header
//
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// +00+01+02+03+04+05+06+07+08+09+10+11+12+13+14+15+16+17+18+19+20+21+22+23+24+25+26+27+28+29+30+31+
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// +             Hardware type 	                   +                    Protocol type             +
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// + HardwareAddressLength + ProtocolAddressLength +	                 Opcode                  +
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// +                                   Source hardware address :::                                 +
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// +                                   Source protocol address :::                                 +
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// +                                Destination hardware address :::                               +
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// +                                Destination protocol address :::                               +
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// +                                             Data :::                                          +
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//
//********************************************************************************************

//*******************************************************************************************
//
// Function : ArpPacketIsArp
// Description : check received packet, that packet is match with arp and avr ip or not?
//
//*******************************************************************************************
unsigned char eth_type_is_arp_and_my_ip(unsigned char *buf, unsigned char *ipaddr, unsigned int len){
        unsigned char i=0;
        //  
        if (len<41){
                return(0);
        }
        if(buf[ETH_TYPE_H_P] != ETHTYPE_ARP_H_V || buf[ETH_TYPE_L_P] != ETHTYPE_ARP_L_V){
                return(0);
        }
        while(i<4){
                if(buf[ARP_DST_IP_P+i] != ipaddr[i]){
                        return(0);
                }
                i++;
        }
        return(1);
}
//*******************************************************************************************
//
// Function : ArpSendReply
// Description : Send reply if recieved packet is ARP and IP address is match with My IP
//
//*******************************************************************************************
void make_arp_answer_from_request(unsigned char *buf, unsigned char *macaddr, unsigned char *ipaddr)
{
        unsigned char i=0;
        //
        make_eth(buf, macaddr);
        buf[ARP_OPCODE_H_P]=ARP_OPCODE_REPLY_H_V;
        buf[ARP_OPCODE_L_P]=ARP_OPCODE_REPLY_L_V;
        // fill the mac addresses:
        while(i<6){
                buf[ARP_DST_MAC_P+i]=buf[ARP_SRC_MAC_P+i];
                buf[ARP_SRC_MAC_P+i]=macaddr[i];
                i++;
        }
        i=0;
        while(i<4){
                buf[ARP_DST_IP_P+i]=buf[ARP_SRC_IP_P+i];
                buf[ARP_SRC_IP_P+i]=ipaddr[i];
                i++;
        }
        // eth+arp is 42 bytes:
        enc28j60PacketSend(42,buf); 
}
