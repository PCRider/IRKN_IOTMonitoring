//********************************************************************************************
//
// File : icmp.c implement for Internet Control Message Protocol
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
#include "icmp.h"
#include "eth.h"
#include "ip.h"
#include "enc28j60.h"
//********************************************************************************************
//
// The Internet Control Message Protocol (ICMP) is one of the core protocols of the
// Internet protocol suite. It is chiefly used by networked computers'
// operating systems to send error messages---indicating, for instance,
// that a requested service is not available or that a host or router could not be reached.
//
// ICMP differs in purpose from TCP and UDP in that it is usually not used directly
// by user network applications. One exception is the ping tool,
// which sends ICMP Echo Request messages (and receives Echo Response messages)
// to determine whether a host is reachable and how long packets take to get to and from that host.
//
// +------------+-----------+-------------+----------+
// + MAC header + IP header + ICMP header + Data ::: +
// +------------+-----------+-------------+----------+
//
// ICMP header
//
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// +00+01+02+03+04+05+06+07+08+09+10+11+12+13+14+15+16+17+18+19+20+21+22+23+24+25+26+27+28+29+30+31+
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// +         Type          +          Code         +               ICMP header checksum            +
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
// +                                            Data :::                                           +
// +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//
//********************************************************************************************


//*******************************************************************************************
//
// Function : icmp_send_reply
// Description : Send PING reply packet to destination.
//
//*******************************************************************************************
void make_echo_reply_from_request(unsigned char *buf, unsigned char *macaddr, unsigned char *ipaddr, unsigned int len)
{
        make_eth(buf, macaddr);
        make_ip(buf, ipaddr);
        buf[ICMP_TYPE_P]=ICMP_TYPE_ECHOREPLY_V;
        // we changed only the icmp.type field from request(=8) to reply(=0).
        // we can therefore easily correct the checksum:
        if (buf[ICMP_CHECKSUM_P] > (0xff-0x08)){
                buf[ICMP_CHECKSUM_P+1]++;
        }
        buf[ICMP_CHECKSUM_P]+=0x08;
        //
        enc28j60PacketSend(len,buf);
}
