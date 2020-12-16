//********************************************************************************************
//
// File : ethernet.c implement for Ethernet Protocol
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
#include "eth.h"
//********************************************************************************************
//
// Ethernet is a large, diverse family of frame-based computer networking technologies
// that operates at many speeds for local area networks (LANs).
// The name comes from the physical concept of the ether.
// It defines a number of wiring and signaling standards for the physical layer,
// through means of network access at the Media Access Control (MAC)/Data Link Layer,
// and a common addressing format.
//
// Ethernet has been standardized as IEEE 802.3.
// The combination of the twisted pair versions of ethernet for connecting end systems to
// the network with the fiber optic versions for site backbones
// become the most widespread wired LAN technology in use from the 1990s to the present,
// largely replacing competing LAN standards such as coaxial cable Ethernet,
// token ring, FDDI, and ARCNET. In recent years, Wi-Fi,
// the wireless LAN standardized by IEEE 802.11,
// has been used instead of Ethernet for many home and small office networks
// and in addition to Ethernet in larger installations.
//
//
//********************************************************************************************
unsigned char eth_type_is_ip_and_my_ip(unsigned char *buf, unsigned char *ipaddr, unsigned int len){
        unsigned char i=0;
        //eth+ip+udp header is 42
        if (len<42){
                return(0);
        }
        if(buf[ETH_TYPE_H_P]!=ETHTYPE_IP_H_V || buf[ETH_TYPE_L_P]!=ETHTYPE_IP_L_V){
                return(0);
        }
        if (buf[IP_HEADER_LEN_VER_P]!=0x45){
                // must be IP V4 and 20 byte header
                return(0);
        }
        while(i<4){
                if(buf[IP_DST_P+i]!=ipaddr[i]){
                        return(0);
                }
                i++;
        }
        return(1);
}
// make a return eth header from a received eth packet
//********************************************************************************************
//
// Function : eth_generate_header
// Description : generarete ethernet header, contain destination and source MAC address,
// ethernet type.
//
//********************************************************************************************
void make_eth(unsigned char *buf, unsigned char *macaddr)
{
        unsigned char i=0;
        //
        //copy the destination mac from the source and fill my mac into src
        while(i<6){
                buf[ETH_DST_MAC +i]=buf[ETH_SRC_MAC +i];
                buf[ETH_SRC_MAC +i]=macaddr[i];
                i++;
        }
}
