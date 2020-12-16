//********************************************************************************************
//
// File : enc28j60.h Definitions and Auxiliary Functions for Microchip ENC28J60 Stand-Alone Ethernet Controller with SPI™ Interface
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
//@{


#ifndef ENC28J60_H
#define ENC28J60_H
#include <inttypes.h>

//There are three types of memory in the ENC28J60:
//• Control Registers (ETH, MAC, MII)
//• Ethernet Buffer
//• PHY Registers
// ENC28J60 Control Registers
// Control register definitions are a combination of Register address,
// bank number, and Ethernet/MAC/PHY indicator bits.
#define ADDR_MASK        0x1F // - Register address	(bits 0-4)
#define BANK_MASK        0x60 // - Bank number        	(bits 5-6)
#define SPRD_MASK        0x80 // - MAC/MII indicator	(bit 7)

// 0x1A address in all bank registers is reserved 
// these addresses are the same in All-bank, these are common registers
#define EIE              0x1B 
#define EIR              0x1C 
#define ESTAT            0x1D 
#define ECON2            0x1E 
#define ECON1            0x1F 

// Bank 0 registers
#define ERDPTL           (0x00|0x00)	// Read Pointer Low Byte			(ERDPT<7:0>)
#define ERDPTH           (0x01|0x00)	// Read Pointer High Byte			(ERDPT<12:8>)
#define EWRPTL           (0x02|0x00)	// Write Pointer Low Byte			(EWRPT<7:0>)
#define EWRPTH           (0x03|0x00)	// Write Pointer High Byte			(EWRPT<12:8>)
#define ETXSTL           (0x04|0x00)	// TX Start Low Byte				(ETXST<7:0>)
#define ETXSTH           (0x05|0x00)	// TX Start High Byte				(ETXST<12:8>)
#define ETXNDL           (0x06|0x00)	// TX End Low Byte				(ETXND<7:0>)
#define ETXNDH           (0x07|0x00)	// TX End High Byte				(ETXND<12:8>)
#define ERXSTL           (0x08|0x00)	// RX Start Low Byte				(ERXST<7:0>)
#define ERXSTH           (0x09|0x00)	// RX Start High Byte				(ERXST<12:8>)
#define ERXNDL           (0x0A|0x00)	// RX End Low Byte				(ERXND<7:0>)
#define ERXNDH           (0x0B|0x00)	// RX End High Byte				(ERXND<12:8>)
#define ERXRDPTL         (0x0C|0x00)	// RX RD Pointer Low Byte			(ERXRDPT<7:0>)
#define ERXRDPTH         (0x0D|0x00)	// RX RD Pointer High Byte			(ERXRDPT<12:8>)
#define ERXWRPTL         (0x0E|0x00)	// RX WR Pointer Low Byte			(ERXWRPT<7:0>)
#define ERXWRPTH         (0x0F|0x00)	// RX WR Pointer High Byte			(ERXWRPT<12:8>)
#define EDMASTL          (0x10|0x00)	// DMA Start Low Byte				(EDMAST<7:0>)
#define EDMASTH          (0x11|0x00)	// DMA Start High Byte				(EDMAST<12:8>)
#define EDMANDL          (0x12|0x00)	// DMA End Low Byte				(EDMAND<7:0>)
#define EDMANDH          (0x13|0x00)	// DMA End High Byte				(EDMAND<12:8>)
#define EDMADSTL         (0x14|0x00)	// DMA Destination Low Byte			(EDMADST<7:0>)
#define EDMADSTH         (0x15|0x00)	// DMA Destination High Byte			(EDMADST<12:8>)
#define EDMACSL          (0x16|0x00)	// DMA Checksum Low Byte			(EDMACS<7:0>)
#define EDMACSH          (0x17|0x00)	// DMA Checksum High Byte			(EDMACS<15:8>)
// 0x18 --
// 0x19 --
// 0x1A r

// Bank 1 registers
#define EHT0             (0x00|0x20)	// Hash Table Byte 0				(EHT<7:0>)
#define EHT1             (0x01|0x20)	// Hash Table Byte 1				(EHT<15:8>)
#define EHT2             (0x02|0x20)	// Hash Table Byte 2				(EHT<23:16>)
#define EHT3             (0x03|0x20)	// Hash Table Byte 3				(EHT<31:24>)
#define EHT4             (0x04|0x20)	// Hash Table Byte 4				(EHT<39:32>)
#define EHT5             (0x05|0x20)	// Hash Table Byte 5				(EHT<47:40>)
#define EHT6             (0x06|0x20)	// Hash Table Byte 6				(EHT<55:48>)
#define EHT7             (0x07|0x20)	// Hash Table Byte 7				(EHT<63:56>)
#define EPMM0            (0x08|0x20)	// Pattern Match Mask Byte 0			(EPMM<7:0>)
#define EPMM1            (0x09|0x20)	// Pattern Match Mask Byte 1			(EPMM<15:8>)
#define EPMM2            (0x0A|0x20)	// Pattern Match Mask Byte 2			(EPMM<23:16>)
#define EPMM3            (0x0B|0x20)	// Pattern Match Mask Byte 3			(EPMM<31:24>)
#define EPMM4            (0x0C|0x20)	// Pattern Match Mask Byte 4			(EPMM<39:32>)
#define EPMM5            (0x0D|0x20)	// Pattern Match Mask Byte 5			(EPMM<47:40>)
#define EPMM6            (0x0E|0x20)	// Pattern Match Mask Byte 6			(EPMM<55:48>)
#define EPMM7            (0x0F|0x20)	// Pattern Match Mask Byte 7			(EPMM<63:56>)
#define EPMCSL           (0x10|0x20)	// Pattern Match Checksum Low Byte		(EPMCS<7:0>)
#define EPMCSH           (0x11|0x20)	// Pattern Match Checksum High Byte		(EPMCS<15:0>)
// 0x12 --
// 0x13 --
#define EPMOL            (0x14|0x20)	// Pattern Match Offset Low Byte			(EPMO<7:0>)
#define EPMOH            (0x15|0x20)	// Pattern Match Offset High Byte		(EPMO<12:8>)
#define EWOLIE           (0x16|0x20)	// EWOLIE: ETHERNET WAKE-UP ON LAN INTERRUPT ENABLE REGISTER
#define EWOLIR           (0x17|0x20)	// EWOLIR: ETHERNET WAKE-UP ON LAN INTERRUPT REQUEST
#define ERXFCON          (0x18|0x20)	// ERXFCON: RECEIVE FILTER CONTROL REGISTER
#define EPKTCNT          (0x19|0x20)	// Ethernet Packet Count
// 0x1A r

// Bank 2 registers
#define MACON1           (0x00|0x40|0x80) // MACON1: MAC CONTROL REGISTER 1
#define MACON2           (0x01|0x40|0x80) // MACON2: MAC CONTROL REGISTER 2
#define MACON3           (0x02|0x40|0x80) // MACON3: MAC CONTROL REGISTER 3
#define MACON4           (0x03|0x40|0x80) // MACON4: MAC CONTROL REGISTER 4
#define MABBIPG          (0x04|0x40|0x80) // Back-to-Back Inter-Packet Gap			(BBIPG<6:0>)
// 0x05 --
#define MAIPGL           (0x06|0x40|0x80) // Non-Back-to-Back Inter-Packet Gap Low Byte	(MAIPGL<6:0>)
#define MAIPGH           (0x07|0x40|0x80) // Non-Back-to-Back Inter-Packet Gap High Byte	(MAIPGH<6:0>)
#define MACLCON1         (0x08|0x40|0x80) // Retransmission Maximum			(RETMAX<3:0>)
#define MACLCON2         (0x09|0x40|0x80) // Collision Window				(COLWIN<5:0>)
#define MAMXFLL          (0x0A|0x40|0x80) // Maximum Frame Length Low Byte			(MAMXFL<7:0>)
#define MAMXFLH          (0x0B|0x40|0x80) // Maximum Frame Length High Byte		(MAMXFL<15:8>)
// 0x0C r
#define MAPHSUP          (0x0D|0x40|0x80) // MAPHSUP: MAC-PHY SUPPORT REGISTER
// 0x0E r
// 0x0F --
// 0x10 r
#define MICON            (0x11|0x40|0x80) // MICON: MII CONTROL REGISTER 
#define MICMD            (0x12|0x40|0x80) // MICMD: MII COMMAND REGISTER
// 0x13 --
#define MIREGADR         (0x14|0x40|0x80) // MII Register Address				(MIREGADR<4:0>)
// 0x15 r
#define MIWRL            (0x16|0x40|0x80) // MII Write Data Low Byte			(MIWR<7:0>)
#define MIWRH            (0x17|0x40|0x80) // MII Write Data High Byte			(MIWR<15:8>)
#define MIRDL            (0x18|0x40|0x80) // MII Read Data Low Byte			(MIRD<7:0>)
#define MIRDH            (0x19|0x40|0x80) // MII Read Data High Byte			(MIRD<15:8>)
// 0x1A r

// Bank 3 registers
#define MAADR1           (0x00|0x60|0x80) // MAC Address Byte 1				(MAADR<15:8>)
#define MAADR0           (0x01|0x60|0x80) // MAC Address Byte 0				(MAADR<7:0>)
#define MAADR3           (0x02|0x60|0x80) // MAC Address Byte 3				(MAADR<31:24>)
#define MAADR2           (0x03|0x60|0x80) // MAC Address Byte 2				(MAADR<23:16>)
#define MAADR5           (0x04|0x60|0x80) // MAC Address Byte 5				(MAADR<48:41>)
#define MAADR4           (0x05|0x60|0x80) // MAC Address Byte 4				(MAADR<40:32>)
#define EBSTSD           (0x06|0x60)	// Built-in Self-Test Fill Seed			(EBSTSD<7:0>)
#define EBSTCON          (0x07|0x60)	// EBSTCON: SELF-TEST CONTROL REGISTER
#define EBSTCSL          (0x08|0x60)	// Built-in Self-Test Checksum Low Byte		(EBSTCS<7:0>)
#define EBSTCSH          (0x09|0x60)	// Built-in Self-Test Checksum High Byte		(EBSTCS<15:8>)
#define MISTAT           (0x0A|0x60|0x80) // MISTAT: MII STATUS REGISTER
// 0x0B --
// 0x0C --
// 0x0D --
// 0x0E --
// 0x0F --
// 0x10 --
// 0x11 --
#define EREVID           (0x12|0x60)	// Ethernet Revision ID				(EREVID<4:0>)
// 0x13 --
// 0x14 --
#define ECOCON           (0x15|0x60)	// ECOCON: CLOCK OUTPUT CONTROL REGISTER
// 0x16 r
#define EFLOCON          (0x17|0x60)	// EFLOCON: ETHERNET FLOW CONTROL REGISTER
#define EPAUSL           (0x18|0x60)	// Pause Timer Value Low Byte			(EPAUS<7:0>)
#define EPAUSH           (0x19|0x60)	// Pause Timer Value High Byte			(EPAUS<15:8>)
// 0x1A r

// PHY registers
#define PHCON1           0x00		// PHCON1: PHY CONTROL REGISTER 1
#define PHSTAT1          0x01		// PHSTAT1: PHYSICAL LAYER STATUS REGISTER 1
#define PHHID1           0x02		// PHY Identifier (PID18:PID3) = 0083h
#define PHHID2           0x03		// PHY Identifier (PID24:PID19) = 000101    PHY P/N (PPN5:PPN0) = 00h     PHY Revision (PREV3:PREV0) = 00h
#define PHCON2           0x10		// HCON2: PHY CONTROL REGISTER 2
#define PHSTAT2          0x11		// PHSTAT2: PHYSICAL LAYER STATUS REGISTER 2
#define PHIE             0x12		// PHIE: PHY INTERRUPT ENABLE REGISTER
#define PHIR             0x13		// PHIR: PHY INTERRUPT REQUEST (FLAG) REGISTER
#define PHLCON           0x14		// PHLCON: PHY MODULE LED CONTROL REGISTER


// ENC28J60 ERXFCON Register Bit Definitions
// ERXFCON: RECEIVE FILTER CONTROL REGISTER
//   R/W-1  R/W-0   R/W-1   R/W-0  R/W-0  R/W-0  R/W-0  R/W-1
// +------+-------+-------+------+------+------+------+------+
// + UCEN + ANDOR + CRCEN + PMEN + MPEN + HTEN + MCEN + BCEN +
// +------+-------+-------+------+------+------+------+------+
// bit 7 UCEN: Unicast Filter Enable bit
// When ANDOR = 1:
// 1 = Packets not having a destination address matching the local MAC address will be discarded
// 0 = Filter disabled
// When ANDOR = 0:
// 1 = Packets with a destination address matching the local MAC address will be accepted
// 0 = Filter disabled
#define ERXFCON_UCEN     0x80
// bit 6 ANDOR: AND/OR Filter Select bit
// 1 = AND: Packets will be rejected unless all enabled filters accept the packet
// 0 = OR: Packets will be accepted unless all enabled filters reject the packet
#define ERXFCON_ANDOR    0x40
// bit 5 CRCEN: Post-Filter CRC Check Enable bit
// 1 = All packets with an invalid CRC will be discarded
// 0 = The CRC validity will be ignored
// When receiving packets, the ENC28J60 will check the CRC of each incoming packet.
// If ERXFCON.CRCEN is set, packets with invalid CRCs will automatically be discarded.
// If CRCEN is clear and the packet meets all other receive filtering criteria, the packet will be written into the receive buffer and the host controller will be able to determine if the CRC was valid by reading the receive status vector
#define ERXFCON_CRCEN    0x20
// bit 4 PMEN: Pattern Match Filter Enable bit
// When ANDOR = 1:
// 1 = Packets must meet the pattern match criteria or they will be discarded
// 0 = Filter disabled
// When ANDOR = 0:
// 1 = Packets which meet the pattern match criteria will be accepted
// 0 = Filter disabled
#define ERXFCON_PMEN     0x10
// bit 3 MPEN: Magic Packet Filter Enable bit
// When ANDOR = 1:
// 1 = Packets must be Magic Packets for the local MAC address or they will be discarded
// 0 = Filter disabled
// When ANDOR = 0:
// 1 = Magic Packets for the local MAC address will be accepted
// 0 = Filter disabled
#define ERXFCON_MPEN     0x08
// bit 2 HTEN: Hash Table Filter Enable bit
// When ANDOR = 1:
// 1 = Packets must meet the hash table criteria or they will be discarded
// 0 = Filter disabled
// When ANDOR = 0:
// 1 = Packets which meet the hash table criteria will be accepted
// 0 = Filter disabled
#define ERXFCON_HTEN     0x04
// bit 1 MCEN: Multicast Filter Enable bit
// When ANDOR = 1:
// 1 = Packets must have the Least Significant bit set in the destination address or they will be discarded
// 0 = Filter disabled
// When ANDOR = 0:
// 1 = Packets which have the Least Significant bit set in the destination address will be accepted
// 0 = Filter disabled
#define ERXFCON_MCEN     0x02
// bit 0 BCEN: Broadcast Filter Enable bit
// When ANDOR = 1:
// 1 = Packets must have a destination address of FF-FF-FF-FF-FF-FF or they will be discarded
// 0 = Filter disabled
// When ANDOR = 0:
// 1 = Packets which have a destination address of FF-FF-FF-FF-FF-FF will be accepted
// 0 = Filter disabled
#define ERXFCON_BCEN     0x01

// ENC28J60 EIE Register Bit Definitions
// EIE: ETHERNET INTERRUPT ENABLE REGISTER
//   R/W-0   R/W-0   R/W-0    R/W-0   R/W-0  R/W-0   R/W-0    R/W-0
// +-------+-------+-------+--------+------+-------+--------+--------+
// + INTIE + PKTIE + DMAIE + LINKIE + TXIE + WOLIE + TXERIE + RXERIE +
// +-------+-------+-------+--------+------+-------+--------+--------+
// bit 7 INTIE: Global INT Interrupt Enable bit
// 1 = Allow interrupt events to drive the INT pin
// 0 = Disable all INT pin activity (pin is continuously driven high)
#define EIE_INTIE        0x80
// bit 6 PKTIE: Receive Packet Pending Interrupt Enable bit
// 1 = Enable receive packet pending interrupt
// 0 = Disable receive packet pending interrupt
#define EIE_PKTIE        0x40
// bit 5 DMAIE: DMA Interrupt Enable bit
// 1 = Enable DMA interrupt
// 0 = Disable DMA interrupt
#define EIE_DMAIE        0x20
// bit 4 LINKIE: Link Status Change Interrupt Enable bit
// 1 = Enable link change interrupt from the PHY
// 0 = Disable link change interrupt
#define EIE_LINKIE       0x10
// bit 3 TXIE: Transmit Enable bit
// 1 = Enable transmit interrupt
// 0 = Disable transmit interrupt
#define EIE_TXIE         0x08
// bit 2 WOLIE: Global WOL Interrupt Enable bit
// 1 = Allow WOL interrupt events to drive the WOL pin
// 0 = Disable all WOL pin activity (pin is continuously driven high)
#define EIE_WOLIE        0x04
// bit 1 TXERIE: Transmit Error Interrupt Enable bit
// 1 = Enable transmit error interrupt
// 0 = Disable transmit error interrupt
#define EIE_TXERIE       0x02
// bit 0 RXERIE: Receive Error Interrupt Enable bit
// 1 = Enable receive error interrupt
// 0 = Disable receive error interrupt
#define EIE_RXERIE       0x01

// ENC28J60 EIR Register Bit Definitions
// EIR: ETHERNET INTERRUPT REQUEST (FLAG) REGISTER
//   U-0    R-0    R/C-0    R-0    R/C-0    R-0    R/C-0    R/C-0
// +-----+-------+-------+--------+------+-------+--------+--------+
// +  —  + PKTIF + DMAIF + LINKIF + TXIF + WOLIF + TXERIF + RXERIF +
// +-----+-------+-------+--------+------+-------+--------+--------+
// bit 6 PKTIF: Receive Packet Pending Interrupt Flag bit
// 1 = Receive buffer contains one or more unprocessed packets; cleared when PKTDEC is set
// 0 = Receive buffer is empty
#define EIR_PKTIF        0x40
// bit 5 DMAIF: DMA Interrupt Flag bit
// 1 = DMA copy or checksum calculation has completed
// 0 = No DMA interrupt is pending
#define EIR_DMAIF        0x20
// bit 4 LINKIF: Link Change Interrupt Flag bit
// 1 = PHY reports that the link status has changed; read PHIR register to clear
// 0 = Link status has not changed
#define EIR_LINKIF       0x10
// bit 3 TXIF: Transmit Interrupt Flag bit
// 1 = Transmit request has ended
// 0 = No transmit interrupt is pending
#define EIR_TXIF         0x08
// bit 2 WOLIF: WOL Interrupt Flag bit
// 1 = Wake-up on LAN interrupt is pending
// 0 = No Wake-up on LAN interrupt is pending
#define EIR_WOLIF        0x04
// bit 1 TXERIF: Transmit Error Interrupt Flag bit
// 1 = A transmit error has occurred
// 0 = No transmit error has occurred
#define EIR_TXERIF       0x02
// bit 0 RXERIF: Receive Error Interrupt Flag bit
// 1 = A packet was aborted because there is insufficient buffer space or the packet count is 255
// 0 = No receive error interrupt is pending
#define EIR_RXERIF       0x01

// ENC28J60 ESTAT Register Bit Definitions
// ESTAT: ETHERNET STATUS REGISTER
//   R-0  R/C-0  R-0    R/C-0    U-0    R-0     R/C-0     R/W-0
// +-----+-----+-----+---------+-----+--------+--------+--------+
// + INT +  r  +  r  + LATECOL +  —  + RXBUSY + TXABRT + CLKRDY +
// +-----+-----+-----+---------+-----+--------+--------+--------+
// bit 7 INT: INT Interrupt Flag bit
// 1 = INT interrupt is pending
// 0 = No INT interrupt is pending
#define ESTAT_INT        0x80
// bit 4 LATECOL: Late Collision Error bit
// 1 = A collision occurred after 64 bytes have been transmitted
// 0 = No collisions after 64 bytes have occurred
#define ESTAT_LATECOL    0x10
// bit 2 RXBUSY: Receive Busy bit
// 1 = Receive logic is receiving a data packet
// 0 = Receive logic is Idle
#define ESTAT_RXBUSY     0x04
// bit 1 TXABRT: Transmit Abort Error bit
// 1 = The transmit request was aborted
// 0 = No transmit abort error
#define ESTAT_TXABRT     0x02
// bit 0 CLKRDY: Clock Ready bit
// 1 = OST has expired; PHY is ready
// 0 = OST is still counting; PHY is not ready
#define ESTAT_CLKRDY     0x01

// ENC28J60 ECON2 Register Bit Definitions
//   R/W-1     W-0      R/W-0   U-0   R/W-0   U-0   U-0   U-0 
// +---------+--------+-------+-----+-------+-----+-----+-----+
// + AUTOINC + PKTDEC + PWRSV +  —  + VRPS  +  —  +  —  +  —  +
// +---------+--------+-------+-----+-------+-----+-----+-----+
// bit 7 AUTOINC: Automatic Buffer Pointer Increment Enable bit
// 1 = Automatically increment ERDPT and EWRPT when the SPI RBM/WBM command is used
// 0 = Do not automatically change ERDPT and EWRPT after the buffer is accessed
#define ECON2_AUTOINC    0x80
// bit 6 PKTDEC: Packet Decrement bit
// 1 = Decrement the EPKTCNT register by one
// 0 = Leave EPKTCNT unchanged
#define ECON2_PKTDEC     0x40
// bit 5 PWRSV: Power Save Enable bit
// 1 = MAC, PHY and control logic are in Low-Power Sleep mode
// 0 = Normal operation
#define ECON2_PWRSV      0x20
// bit 3 VRPS: Voltage Regulator Power Save Enable bit
// When PWRSV = 1:
// 1 = Internal voltage regulator is in Low-Current mode
// 0 = Internal voltage regulator is in Normal Current mode
// When PWRSV = 0:
// The bit is ignored; the regulator always outputs as much current as the device requires.
#define ECON2_VRPS       0x08

// ENC28J60 ECON1 Register Bit Definitions
//  R/W-0   R/W-0   R/W-0    R/W-0   R/W-0   R/W-0  R/W-0   R/W-0
// +------+-------+-------+--------+-------+------+-------+-------+
// +TXRST + RXRST + DMAST + CSUMEN + TXRTS + RXEN + BSEL1 + BSEL0 +
// +------+-------+-------+--------+-------+------+-------+-------+
// TXRST: Transmit Logic Reset bit
// 1 = Transmit logic is held in Reset
// 0 = Normal operation
#define ECON1_TXRST      0x80
// RXRST: Receive Logic Reset bit
// 1 = Receive logic is held in Reset
// 0 = Normal operation
#define ECON1_RXRST      0x40
// DMAST: DMA Start and Busy Status bit
// 1 = DMA copy or checksum operation is in progress
// 0 = DMA hardware is Idle
#define ECON1_DMAST      0x20
// CSUMEN: DMA Checksum Enable bit
// 1 = DMA hardware calculates checksums
// 0 = DMA hardware copies buffer memory
#define ECON1_CSUMEN     0x10
// TXRTS: Transmit Request To Send bit
// 1 = The transmit logic is attempting to transmit a packet
// 0 = The transmit logic is Idle
#define ECON1_TXRTS      0x08
//RXEN: Receive Enable bit
// 1 = Packets which pass the current filter configuration will be written into the receive buffer
// 0 = All packets received will be ignored
#define ECON1_RXEN       0x04
// BSEL1:BSEL0: Bank Select bits
// 11 = SPI accesses registers in Bank 3
// 10 = SPI accesses registers in Bank 2
// 01 = SPI accesses registers in Bank 1
// 00 = SPI accesses registers in Bank 0
#define ECON1_BSEL1      0x02
#define ECON1_BSEL0      0x01

// ENC28J60 MACON1 Register Bit Definitions
// MACON1: MAC CONTROL REGISTER 1
//  U-0 U-0 U-0   R/W-0   R/W-0    R/W-0     R/W-0    R/W-0
// +---+---+---+--------+--------+--------+---------+--------+
// + — + — + — + LOOPBK + TXPAUS + RXPAUS + PASSALL + MARXEN +
// +---+---+---+--------+--------+--------+---------+--------+
// bit 4 LOOPBK: MAC Loopback Enable bit
// 1 = All data transmitted by the MAC will be looped back to the MAC
// 0 = Normal operation
#define MACON1_LOOPBK    0x10
// bit 3 TXPAUS: Pause Control Frame Transmission Enable bit
// 1 = Allow the MAC to transmit pause control frames (needed for flow control in full duplex)
// 0 = Disallow pause frame transmissions
#define MACON1_TXPAUS    0x08
// bit 2 RXPAUS: Pause Control Frame Reception Enable bit
// 1 = Inhibit transmissions when pause control frames are received (normal operation)
// 0 = Ignore pause control frames which are received
#define MACON1_RXPAUS    0x04
// bit 1 PASSALL: Pass All Received Frames Enable bit
// 1 = Control frames received by the MAC will be wirtten into the receive buffer if not filtered out
// 0 = Control frames will be discarded after being processed by the MAC (normal operation)
#define MACON1_PASSALL   0x02
// bit 0 MARXEN: MAC Receive Enable bit
// 1 = Enable packets to be received by the MAC
// 0 = Disable packet reception
#define MACON1_MARXEN    0x01

// ENC28J60 MACON2 Register Bit Definitions
// MACON2: MAC CONTROL REGISTER 2
//   R/W-1    R/W-0   U-0   U-0    R/W-0     R/W-0     R/W-0     R/W-0
// +-------+--------+-----+-----+---------+---------+---------+---------+
// + MARST + RNDRST +  —  +  —  + MARXRST + RFUNRST + MATXRST + TFUNRST +
// +-------+--------+-----+-----+---------+---------+---------+---------+
// bit 7 MARST: MAC Reset bit
// 1 = Entire MAC is held in Reset
// 0 = Normal operation
#define MACON2_MARST     0x80
// bit 6 RNDRST: MAC Random Number Generator Reset bit
// 1 = Random number generator used in transmit function is held in Reset
// 0 = Normal operation
#define MACON2_RNDRST    0x40
// bit 3 MARXRST: MAC Control Sublayer/Receive Logic Reset bit
// 1 = MAC control sublayer and receiver logic are held in Reset
// 0 = Normal operation
#define MACON2_MARXRST   0x08
// bit 2 RFUNRST: MAC Receive Function Reset bit
// 1 = MAC receive logic is held in Reset
// 0 = Normal operation
#define MACON2_RFUNRST   0x04
// bit 1 MATXRST: MAC Control Sublayer/Transmit Logic Reset bit
// 1 = MAC control sublayer and transmit logic are held in Reset
// 0 = Normal operation
#define MACON2_MATXRST   0x02
// bit 0 TFUNRST: MAC Transmit Function Reset bit
// 1 = MAC transmit logic is held in Reset
// 0 = Normal operation
#define MACON2_TFUNRST   0x01

// ENC28J60 MACON3 Register Bit Definitions
// MACON3: MAC CONTROL REGISTER 3 
//    R/W-0     R/W-0     R/W-0     R/W-0     R/W-0     R/W-0    R/W-0    R/W-0
// +---------+---------+---------+---------+---------+--------+---------+--------+
// + PADCFG2 + PADCFG1 + PADCFG0 + TXCRCEN + PHDRLEN + HFRMEN + FRMLNEN + FULDPX +
// +---------+---------+---------+---------+---------+--------+---------+--------+
// bit 7-5 PADCFG2:PACDFG0: Automatic Pad and CRC Configuration bits
// 111 = All short frames will be zero padded to 64 bytes and a valid CRC will then be appended
// 110 = No automatic padding of short frames
// 101 = MAC will automatically detect VLAN Protocol frames which have a 8100h type field and
// automatically pad to 64 bytes. If the frame is not a VLAN frame, it will be padded to
// 60 bytes. After padding, a valid CRC will be appended.
// 100 = No automatic padding of short frames
// 011 = All short frames will be zero padded to 64 bytes and a valid CRC will then be appended
// 010 = No automatic padding of short frames
// 001 = All short frames will be zero padded to 60 bytes and a valid CRC will then be appended
// 000 = No automatic padding of short frames
// When transmitting packets, the ENC28J60 automatically generates zero padding if the MACON3.PADCFG<2:0> bits are configured to do so.
// Otherwise, the host controller should manually add padding to the packet before transmitting it. 
// The ENC28J60 will not prevent the transmission of undersize packets should the host controller command such an action.
// When receiving packets, the ENC28J60 automatically rejects packets which are less than 18 bytes. 
// All packets 18 bytes and larger will be subject to the standard receive filtering criteria and may be accepted as normal traffic.
// When transmitting packets, the ENC28J60 will automatically generate a valid CRC and transmit it if the MACON3.PADCFG<2:0> bits are configured to cause this.
// Otherwise, the host controller must generate the CRC and place it in the transmit buffer.
// Given the complexity of calculating a CRC, it is highly recommended that the PADCFG bits be configured such that the ENC28J60 will automatically generate the CRC field.
#define MACON3_PADCFG2   0x80
#define MACON3_PADCFG1   0x40
#define MACON3_PADCFG0   0x20
// bit 4 TXCRCEN: Transmit CRC Enable bit
// 1 = MAC will apend a valid CRC to all frames transmitted regardless of PADCFG. TXCRCEN
// must be set if PADCFG specifies that a valid CRC will be appended.
// 0 = MAC will not append a CRC. The last 4 bytes will be checked and if it is an invalid CRC, it
// will be reported in the transmit status vector.
#define MACON3_TXCRCEN   0x10
// bit 3 PHDRLEN: Proprietary Header Enable bit
// 1 = Frames presented to the MAC contain a 4-byte proprietary header which will not be used
// when calculating the CRC
// 0 = No proprietary header is present. The CRC will cover all data (normal operation).
#define MACON3_PHDRLEN   0x08
// bit 2 HFRMEN: Huge Frame Enable bit
// 1 = Frames of any size will be allowed to be transmitted and receieved
// 0 = Frames bigger than MAMXFL will be aborted when transmitted or received
// The data field is a variable length field anywhere from 0 to 1500 bytes. Larger data packets will violate Ethernet standards and will be dropped by most Ethernet nodes.
// The ENC28J60, however, is capable of transmitting and receiving larger packets when the Huge Frame Enable bit is set (MACON3.HFRMEN = 1).
#define MACON3_HFRMLEN   0x04
// bit 1 FRMLNEN: Frame Length Checking Enable bit
// 1 = The type/length field of transmitted and received frames will be checked. If it represents a
// length, the frame size will be compared and mismatches will be reported in the
// transmit/receive status vector.
// 0 = Frame lengths will not be compared with the type/length field
#define MACON3_FRMLNEN   0x02
// bit 0 FULDPX: MAC Full-Duplex Enable bit
// 1 = MAC will operate in full-duplex. PHCON1.PDPXMD must also be set.
// 0 = MAC will operate in half-duplex. PHCON1.PDPXMD must also be clear.
#define MACON3_FULDPX    0x01

// ENC28J60 MACON4 Register Bit Definitions
// MACON4: MAC CONTROL REGISTER 4
//  U-0  R/W-0   R/W-0   R/W-0   U-0 U-0   R/W-0     R/W-0
// +---+-------+------+---------+---+---+---------+---------+
// + — + DEFER + BPEN + NOBKOFF + — + — + LONGPRE + PUREPRE +
// +---+-------+------+---------+---+---+---------+---------+
// bit 6 DEFER: Defer Transmission Enable bit (applies to half duplex only)
// 1 = When the medium is occupied, the MAC will wait indefinately for it to become free when
// attempting to transmit
// 0 = When the medium is occupied, the MAC will abort the transmission after the excessive
// deferral limit is reached (2.4287 ms)
#define	MACON4_DEFER	(1<<6)
//bit 5 BPEN: No Backoff During Back Pressure Enable bit (applies to half duplex only)
// 1 = After incidentally causing a collission during back pressure, the MAC will immediately
// begin retransmitting
// 0 = After incidentally causing a collision during back pressure, the MAC will delay using the
// binary exponential backoff algorithm before attempting to retransmit (normal operation)
#define	MACON4_BPEN	(1<<5)
// bit 4 NOBKOFF: No Backoff Enable bit (applies to half duplex only)
// 1 = After any collision, the MAC will immediately begin retransmitting
// 0 = After any collision, the MAC will delay using the binary exponential backoff algorithm
// before attempting to retransmit (normal operation)
#define	MACON4_NOBKOFF	(1<<4)
// bit 1 LONGPRE: Long Preamble Enforcement Enable bit
// 1 = Received packets will be rejected if they are preceded by 12 or more bytes of preamble
// 0 = Received packets will not be rejected if they have 12 or more bytes of preamble (normal operation)
#define	MACON4_LONGPRE	(1<<1)
// bit 0 PUREPRE: Pure Preamble Enforcement Enable bit
// 1 = The preamble of received packets will be checked against 55h. If it contains an error, the packet will be discarded.
// 0 = The preamble of received packets will not be checked
#define	MACON4_PUREPRE	(1<<0)

//  MICON: MII CONTROL REGISTER 
//    R/W-0  U-0 U-0 U-0 U-0 U-0 U-0 U-0
// +--------+---+---+---+---+---+---+---+
// + RSTMII + — + — + — + — + — + — + — +
// +--------+---+---+---+---+---+---+---+
// bit 7 RSTMII: MII Management Module Reset bit 
// 1 = MII management module held in Reset 
// 0 = Normal operation
#define	MICON_RSTMI	(1<<7)

// MABBIPG: MAC BACK-TO-BACK INTER-PACKET GAP REGISTER
//   U-0    R/W-0    R/W-0    R/W-0    R/W-0    R/W-0    R/W-0    R/W-0
// +-----+--------+--------+--------+--------+--------+--------+--------+
// +  —  + BBIPG6 + BBIPG5 + BBIPG4 + BBIPG3 + BBIPG2 + BBIPG1 + BBIPG0 +
// +-----+--------+--------+--------+--------+--------+--------+--------+
// bit 6-0 BBIPG6:BBIPG0: Back-to-Back Inter-Packet Gap Delay Time bits
// When MACON3.FULDPX = 1:
// Nibble time offset delay between the end of one transmission and the beginning of the next in
// a back-to-back sequence. The register value should be programmed to the desired period in
// nibble times minus 3. The recommended setting is 15h which represents the minimum IEEE
// specified Inter-Packet Gap (IPG) of 9.6 μs.
// When MACON3.FULDPX = 0:
// Nibble time offset delay between the end of one transmission and the beginning of the next in
// a back-to-back sequence. The register value should be programmed to the desired period in
// nibble times minus 6. The recommended setting is 12h which represents the minimum IEEE
// specified Inter-Packet Gap (IPG) of 9.6 μs.
#define MABBIPG_BBIPG6	(1<<6)
#define MABBIPG_BBIPG5	(1<<5)
#define MABBIPG_BBIPG4	(1<<4)
#define MABBIPG_BBIPG3	(1<<3)
#define MABBIPG_BBIPG2	(1<<2)
#define MABBIPG_BBIPG1	(1<<1)
#define MABBIPG_BBIPG0	(1)

// ENC28J60 MICMD Register Bit Definitions
// MICMD: MII COMMAND REGISTER
//   U-0   U-0   U-0   U-0   U-0   U-0    R/W-0    R/W-0
// +-----+-----+-----+-----+-----+-----+---------+-------+
// +  —  +  —  +  —  +  —  +  —  +  —  + MIISCAN + MIIRD +
// +-----+-----+-----+-----+-----+-----+---------+-------+
// bit 1 MIISCAN: MII Scan Enable bit
// 1 = PHY register at MIREGADR is continously read and the data is placed in MIRD
// 0 = No MII management scan operation is in progress
#define MICMD_MIISCAN    0x02
// bit 0 MIIRD: MII Read Enable bit
// 1 = PHY register at MIREGADR is read once and the data is placed in MIRD
// 0 = No MII management read operation is in progress
#define MICMD_MIIRD      0x01

// ENC28J60 MISTAT Register Bit Definitions
// MISTAT: MII STATUS REGISTER
//   U-0   U-0   U-0   U-0   R-0    R-0      R-0    R-0
// +-----+-----+-----+-----+-----+--------+------+------+
// +  —  +  —  +  —  +  —  +  r  + NVALID + SCAN + BUSY +
// +-----+-----+-----+-----+-----+--------+------+------+
// bit 1 NVALID: MII Management Read Data Not Valid bit
// 1 = The contents of MIRD are not valid yet
// 0 = The MII management read cycle has completed and MIRD has been updated
#define MISTAT_NVALID    0x04
// bit 1 SCAN: MII Management Scan Operation bit
// 1 = MII management scan operation is in progress
// 0 = No MII management scan operation is in progress
#define MISTAT_SCAN      0x02
// bit 0 BUSY: MII Management Busy bit
// 1 = A PHY register is currently being read or written to
// 0 = The MII management interface is Idle
#define MISTAT_BUSY      0x01

// ENC28J60 PHY PHCON1 Register Bit Definitions
// PHCON1: PHY CONTROL REGISTER 1
//   R/W-0   R/W-0    U-0   U-0      R-1   R/W-0  U-0     R-x   R/W-0  U-0   U-0   U-0   U-0   U-0   U-0   U-0
// +------+---------+-----+------+--------+-----+-----+--------+-----+-----+-----+-----+-----+-----+-----+-----+
// + PRST + PLOOPBK +  —  +  —   + PPWRSV +  r  +  —  + PDPXMD +  r  +  —  +  —  +  —  +  —  +  —  +  —  +  —  +
// +------+---------+-----+------+--------+-----+-----+--------+-----+-----+-----+-----+-----+-----+-----+-----+
// bit 15 PRST: PHY Software Reset bit
// 1 = PHY is processing a Software Reset (automatically resets to ‘0’ when done)
// 0 = Normal operation
#define PHCON1_PRST      0x8000
// bit 14 PLOOPBK: PHY Loopback bit
// 1 = All data transmitted will be returned to the MAC. The twisted pair interface will be disabled.
// 0 = Normal operation
#define PHCON1_PLOOPBK   0x4000
// bit 11 PPWRSV: PHY Power-Down bit
// 1 = PHY is shut-down
// 0 = Normal operation
#define PHCON1_PPWRSV    0x0800
// bit 8 PDPXMD: PHY Duplex Mode bit
// 1 = PHY operates in Full-Duplex mode
// 0 = PHY operates in Half-Duplex mode
// Note 1: The Reset default of PDPXMD depends on the external polarity of the LEDB LED.
#define PHCON1_PDPXMD    0x0100

// ENC28J60 PHY PHSTAT1 Register Bit Definitions
// PHSTAT1: PHYSICAL LAYER STATUS REGISTER 1
//   U-0   U-0   U-0    R-1     R-1    U-0   U-0   U-0   U-0   U-0   U-0   U-0   U-0   R/LL-0   R/LH-0    U-0
// +-----+-----+-----+-------+-------+-----+-----+-----+-----+-----+-----+-----+-----+--------+---------+-----+
// +  —  +  —  +  —  + PFDPX + PHDPX +  —  +  —  +  —  +  —  +  —  +  —  +  —  +  —  + LLSTAT + JBRSTAT +  —  +
// +-----+-----+-----+-------+-------+-----+-----+-----+-----+-----+-----+-----+-----+--------+---------+-----+
// bit 12 PFDPX: PHY Full-Duplex Capable bit
// 1 = PHY is capable of operating at 10 Mbps in Full-Duplex mode (this bit is always set)
#define PHSTAT1_PFDPX    0x1000
// bit 11 PHDPX: PHY Half-Duplex Capable bit
// 1 = PHY is capable of operating at 10 Mbps in Half-Duplex mode (this bit is always set)
#define PHSTAT1_PHDPX    0x0800
// bit 2 LLSTAT: PHY Latching Link Status bit
// 1 = Link is up and has been up continously since PHSTAT1 was last read
// 0 = Link is down or was down for a period since PHSTAT1 was last read
#define PHSTAT1_LLSTAT   0x0004
// bit 1 JBRSTAT: PHY Latching Jabber Status bit
// 1 = PHY has detected a transmission meeting the jabber criteria since PHYSTAT1 was last read
// 0 = PHY has not detected any jabbering transmissions since PHYSTAT1 was last read
#define PHSTAT1_JBSTAT   0x0002

// PHHID1

// PHHID2

// ENC28J60 PHY PHCON2 Register Bit Definitions
// PHCON2: PHY CONTROL REGISTER 2
//  U-0   R/W-0   R/W-0  R/W-x R/W-x   R/W-0  R/W-0  R/W-0     R/W-x   R/W-x  R/W-x   R/W-x   R/W-x   R/W-x   R/W-x   R/W-x
// +---+--------+-------+-----+-----+--------+-----+--------+-------+-------+-------+-------+-------+-------+-------+-------+
// + — + FRCLNK + TXDIS +  r  +  r  + JABBER +  r  + HDLDIS +   r   +   r   +   r   +   r   +   r   +   r   +   r   +   r   +
// +---+--------+-------+-----+-----+--------+-----+--------+-------+-------+-------+-------+-------+-------+-------+-------+
// bit 14 FRCLNK: PHY Force Linkup bit
// 1 = Force linkup even when no link partner is detected
// 0 = Normal operation
#define PHCON2_FRCLINK   0x4000
// bit 13 TXDIS: Twisted Pair Transmitter Disable bit
// 1 = Disable twisted pair transmitter
// 0 = Normal operation
#define PHCON2_TXDIS     0x2000
// bit 10 JABBER: Jabber Correction Disable bit
// 1 = Disable jabber correction
// 0 = Normal operation
#define PHCON2_JABBER    0x0400
// bit 8 HDLDIS: PHY Half-Duplex Loopback Disable bit
// When PHCON1.PDPXMD = 1 or PHCON1.PLOOPBK = 1:
// This bit is ignored.
// When PHCON1.PDPXMD = 0 and PHCON1.PLOOPBK = 0:
// 1 = Transmitted data will only be sent out on the twisted pair interface
// 0 = Transmitted data will be looped back to the MAC and sent out the twisted pair interface
#define PHCON2_HDLDIS    0x0100

// ENC28J60 PHY PHSTAT2 Register Bit Definitions
// PHSTAT2: PHYSICAL LAYER STATUS REGISTER 2
//   U-0   U-0     R-0     R-0       R-0      R-0      R-x        U-0   U-0   U-0   U-0    R-0     U-0   U-0   U-0   U-0
// +-----+-----+--------+--------+---------+-------+------------+-----+-----+-----+-----+--------+-----+-----+-----+-----+
// +  —  +  —  + TXSTAT + RXSTAT + COLSTAT + LSTAT + DPXSTAT(1) +  —  +  —  +  —  +  —  + PLRITY +  —  +  —  +  —  +  —  +
// +-----+-----+--------+--------+---------+-------+------------+-----+-----+-----+-----+--------+-----+-----+-----+-----+
// bit 13 TXSTAT: PHY Transmit Status bit
// 1 = PHY is transmitting data
// 0 = PHY is not transmitting data
#define PHSTAT2_TXSTAT	0x1000
// bit 12 RXSTAT: PHY Receive Status bit
// 1 = PHY is receiving data
// 0 = PHY is not receiving data
#define PHSTAT2_RXSTAT	0x0800
// bit 11 COLSTAT: PHY Collision Status bit
// 1 = A collision is occuring
// 0 = A collision is not occuring
#define PHSTAT2_COLSTAT	0x0400
// bit 10 LSTAT: PHY Link Status bit (non-latching)
// 1 = Link is up
// 0 = Link is down
#define PHSTAT2_LSTAT	0x0200
// bit 9 DPXSTAT: PHY Duplex Status bit(1)
// 1 = PHY is configured for full-duplex operation (PHCON1.PDPXMD is set)
// 0 = PHY is configured for half-duplex operation (PHCON1.PDPXMD is clear)
// Note 1: Reset values of the Duplex mode/status bits depend on the connection of the LED to the LEDB
// pin (see Section 2.6 “LED Configuration” for additional details).
#define PHSTAT2_DPXSTAT	0x0100
// bit 4 PLRITY: Polarity Status bit
// 1 = The polarity of the signal on TPIN+/TPIN- is reversed
// 0 = The polarity of the signal on TPIN+/TPIN- is correct
#define PHSTAT2_PLRITY	0x0010

// PHIE: PHY INTERRUPT ENABLE REGISTER
//  R/W-x R/W-x R/W-x R/W-x R/W-x R/W-x R/W-x R/W-x R/W-x R/W-x R/W-0   R/W-0  R/W-x R/W-x  R/W-0  R/W-0
// +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+--------+-----+-----+-------+-----+
// +  r  +  r  +  r  +  r  +  r  +  r  +  r  +  r  +  r  +  r  +  r  + PLNKIE +  r  +  r  + PGEIE +  r  +
// +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+--------+-----+-----+-------+-----+
// bit 4 PLNKIE: PHY Link Change Interrupt Enable bit
// 1 = PHY link change interrupt is enabled
// 0 = PHY link change interrupt is disabled
#define PHIE_PLNKIE	(1<<4)
// bit 1 PGEIE: PHY Global Interrupt Enable bit
// 1 = PHY interrupts are enabled
// 0 = PHY interrupts are disabled
#define PHIE_PGEIE	(1<<1)

// PHIR: PHY INTERRUPT REQUEST (FLAG) REGISTER
//   R-x   R-x   R-x   R-x   R-x   R-x   R-x   R-x   R-x   R-x   R-0   R/SC-0   R-0  R/SC-0  R-x   R-0
// +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+--------+-----+------+-----+-----+
// +  r  +  r  +  r  +  r  +  r  +  r  +  r  +  r  +  r  +  r  +  r  + PLNKIF +  r  + PGIF +  r  +  r  +
// +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+--------+-----+------+-----+-----+
// bit 4 PLNKIF: PHY Link Change Interrupt Flag bit
// 1 = PHY link status has changed since PHIR was last read; resets to ‘0’ when read
// 0 = PHY link status has not changed since PHIR was last read
#define PHIR_PLNKIF	(1<<4)
// bit 2 PGIF: PHY Global Interrupt Flag bit
// 1 = One or more enabled PHY interrupts have occurred since PHIR was last read; resets to ‘0’ when read
// 0 = No PHY interrupts have occurred
#define PHIR_PGIF	(1<<2)

// PHLCON: PHY MODULE LED CONTROL REGISTER
//  R/W-0 R/W-0 R/W-1 R/W-1   R/W-0    R/W-1    R/W-0    R/W-0    R/W-0   R/W-0    R/W-1    R/W-0    R/W-0   R/W-0   R/W-1  R/W-x
// +-----+-----+-----+-----+--------+--------+--------+--------+--------+--------+--------+--------+-------+-------+-------+-----+
// +  r  +  r  +  r  +  r  + LACFG3 + LACFG2 + LACFG1 + LACFG0 + LBCFG3 + LBCFG2 + LBCFG1 + LBCFG0 + LFRQ1 + LFRQ0 + STRCH +  r  +
// +-----+-----+-----+-----+--------+--------+--------+--------+--------+--------+--------+--------+-------+-------+-------+-----+
// bit 11-8 LACFG3:LACFG0: LEDA Configuration bits
// 0000 = Reserved
// 0001 = Display transmit activity (stretchable)
// 0010 = Display receive activity (stretchable)
// 0011 = Display collision activity (stretchable)
// 0100 = Display link status
// 0101 = Display duplex status
// 0110 = Reserved
// 0111 = Display transmit and receive activity (stretchable)
// 1000 = On
// 1001 = Off
// 1010 = Blink fast
// 1011 = Blink slow
// 1100 = Display link status and receive activity (always stretched)
// 1101 = Display link status and transmit/receive activity (always stretched)
// 1110 = Display duplex status and collision activity (always stretched)
// 1111 = Reserved
#define PHLCON_LACFG3	(1<<11)
#define PHLCON_LACFG2	(1<<10)
#define PHLCON_LACFG1	(1<<9)
#define PHLCON_LACFG0	(1<<8)
#define PHLCON_LATX	PHLCON_LACFG0
#define PHLCON_LARX	PHLCON_LACFG1
#define PHLCON_LACOL	(PHLCON_LACFG1 | PHLCON_LACFG0)
#define PHLCON_LALNK	PHLCON_LACFG2
#define PHLCON_LADPL	(PHLCON_LACFG2 | PHLCON_LACFG0)
#define PHLCON_LATRX	(PHLCON_LACFG2 | PHLCON_LACFG1 | PHLCON_LACFG0)
#define PHLCON_LAON	PHLCON_LACFG3
#define PHLCON_LAOFF	(PHLCON_LACFG3 | PHLCON_LACFG0)
#define PHLCON_LABLF	(PHLCON_LACFG3 | PHLCON_LACFG1)
#define PHLCON_LABLS	(PHLCON_LACFG3 | PHLCON_LACFG1 | PHLCON_LACFG0)
#define PHLCON_LALNKRXS	(PHLCON_LACFG3 | PHLCON_LACFG2)
#define PHLCON_LALNKTRXS (PHLCON_LACFG3 | PHLCON_LACFG2 | PHLCON_LACFG0)
#define PHLCON_LADPLCOLS (PHLCON_LACFG3 | PHLCON_LACFG2 | PHLCON_LACFG1)
// bit 7-4 LBCFG3:LBCFG0: LEDB Configuration bits
// 0000 = Reserved
// 0001 = Display transmit activity (stretchable)
// 0010 = Display receive activity (stretchable)
// 0011 = Display collision activity (stretchable)
// 0100 = Display link status
// 0101 = Display duplex status
// 0110 = Reserved
// 0111 = Display transmit and receive activity (stretchable)
// 1000 = On
// 1001 = Off
// 1010 = Blink fast
// 1011 = Blink slow
// 1100 = Display link status and receive activity (always stretched)
// 1101 = Display link status and transmit/receive activity (always stretched)
// 1110 = Display duplex status and collision activity (always stretched)
// 1111 = Reserved
#define PHLCON_LBCFG3	(1<<7)
#define PHLCON_LBCFG2	(1<<6)
#define PHLCON_LBCFG1	(1<<5)
#define PHLCON_LBCFG0	(1<<4)
#define PHLCON_LBTX	PHLCON_LBCFG0
#define PHLCON_LBRX	PHLCON_LBCFG1
#define PHLCON_LBCOL	(PHLCON_LBCFG1 | PHLCON_LBCFG0)
#define PHLCON_LBLNK	PHLCON_LBCFG2
#define PHLCON_LBDPL	(PHLCON_LBCFG2 | PHLCON_LBCFG0)
#define PHLCON_LBTRX	(PHLCON_LBCFG2 | PHLCON_LBCFG1 | PHLCON_LBCFG0)
#define PHLCON_LBON	PHLCON_LBCFG3
#define PHLCON_LBOFF	(PHLCON_LBCFG3 | PHLCON_LBCFG0)
#define PHLCON_LBBLF	(PHLCON_LBCFG3 | PHLCON_LBCFG1)
#define PHLCON_LBBLS	(PHLCON_LBCFG3 | PHLCON_LBFG1 | PHLCON_LBCFG0)
#define PHLCON_LBLNKRXS	(PHLCON_LBCFG3 | PHLCON_LBCFG2)
#define PHLCON_LBLNKTRXS (PHLCON_LBCFG3 | PHLCON_LBCFG2 | PHLCON_LBCFG0)
#define PHLCON_LBDPLCOLS (PHLCON_LBCFG3 | PHLCON_LBCFG2 | PHLCON_LBCFG1)
// bit 3-2 LFRQ1:LFRQ0: LED Pulse Stretch Time Configuration bits
// 11 = Reserved
// 10 = Stretch LED events to approximately 139 ms
// 01 = Stretch LED events to approximately 73 ms
// 00 = Stretch LED events to approximately 40 ms
#define PHLCON_LFRQ1	(1<<3)
#define PHLCON_LFRQ0	(1<<2)
#define PHLCON_S139	PHLCON_LFRQ1
#define PHLCON_S73	PHLCON_LFRQ0
#define PHLCON_S40	0
// bit 1 STRCH: LED Pulse Stretching Enable bit
// 1 = Stretchable LED events will cause lengthened LED pulses based on the LFRQ configuration
// 0 = Stretchable LED events will only be displayed while they are occurring
#define PHLCON_STRCH	(1<<1)
// 6 * 6 + (4 * 13 *2 *3 ) + 4*13*2 + 3*13*2*3
#define PHLCON_ATBRS139  (PHLCON_LATX | PHLCON_LBRX | PHLCON_STRCH | PHLCON_S139)
#define PHLCON_ATBRS73   (PHLCON_LATX | PHLCON_LBRX | PHLCON_STRCH | PHLCON_S73)
#define PHLCON_ATBRS40   (PHLCON_LATX | PHLCON_LBRX | PHLCON_STRCH | PHLCON_S40)
#define PHLCON_ABOFF 	(PHLCON_LAOFF | PHLCON_LBOFF )
#define PHLCON_ABON	(PHLCON_LAON | PHLCON_LBON )

// ENC28J60 Packet Control Byte Bit Definitions
// FORMAT FOR PER PACKET CONTROL BYTES
// +-----+-----+-----+-----+---------+--------+--------+
// +  —  +  —  +  —  +  —  + PHUGEEN + PPADEN + PCRCEN +
// +-----+-----+-----+-----+---------+--------+--------+
// bit 3 PHUGEEN: Per Packet Huge Frame Enable bit
// When POVERRIDE = 1:
// 1 = The packet will be transmitted in whole
// 0 = The MAC will transmit up to the number of bytes specified by MAMXFL. If the packet is larger than MAMXFL, it will be aborted after MAMXFL is reached.
// When POVERRIDE = 0:
// This bit is ignored.
#define PKTCTRL_PHUGEEN  0x08
// bit 2 PPADEN: Per Packet Padding Enable bit
// When POVERRIDE = 1:
// 1 = The packet will be zero padded to 60 bytes if it is less than 60 bytes
// 0 = The packet will be trasmitted without adding any padding bytes
// When POVERRIDE = 0:
// This bit is ignored.
#define PKTCTRL_PPADEN   0x04
// bit 1 PCRCEN: Per Packet CRC Enable bit
// When POVERRIDE = 1:
// 1 = A valid CRC will be calculated and attached to the frame
// 0 = No CRC will be appended. The last 4 bytes of the frame will be checked for validity as a CRC.
// When POVERRIDE = 0:
// This bit is ignored.
#define PKTCTRL_PCRCEN   0x02
// bit 0 POVERRIDE: Per Packet Override bit
// 1 = The values of PCRCEN, PPADEN and PHUGEEN will override the configuration defined by MACON3
// 0 = The values in MACON3 will be used to determine how the packet will be transmitted
#define PKTCTRL_POVERRIDE 0x01

// ECOCON: CLOCK OUTPUT CONTROL REGISTER
//   U-0   U-0   U-0   U-0   U-0    R/W-1   R/W-0    R/W-0
// +-----+-----+-----+-----+-----+--------+--------+--------+
// +  —  +  —  +  —  +  —  +  —  + COCON2 + COCON1 + COCON0 +
// +-----+-----+-----+-----+-----+--------+--------+--------+
// bit 2-0 COCON2:COCON0: Clock Output Configuration bits
// 111 = Reserved for factory test. Do not use. Glitch prevention not assured.
// 110 = Reserved for factory test. Do not use. Glitch prevention not assured.
// 101 = CLKOUT outputs main clock divided by 8 (3.125 MHz)
// 100 = CLKOUT outputs main clock divided by 4 (6.25 MHz)
// 011 = CLKOUT outputs main clock divided by 3 (8.333333 MHz)
// 010 = CLKOUT outputs main clock divided by 2 (12.5 MHz)
// 001 = CLKOUT outputs main clock divided by 1 (25 MHz)
// 000 = CLKOUT is disabled. The pin is driven low.
#define ECOCON_COCON2	(1<<2)
#define ECOCON_COCON1	(1<<1)
#define ECOCON_COCON0	(1)

// EWOLIE: ETHERNET WAKE-UP ON LAN INTERRUPT ENABLE REGISTER
//    R/W-0    R/W-0    U-0    R/W-0     R/W-0     R/W-0     R/W-0     R/W-0
// +---------+--------+-----+---------+---------+---------+---------+---------+
// + UCWOLIE + AWOLIE +  —  + PMWOLIE + MPWOLIE + HTWOLIE + MCWOLIE + BCWOLIE +
// +---------+--------+-----+---------+---------+---------+---------+---------+
// bit 7 UCWOLIE: Unicast Wake-up on LAN Interrupt Enable bit
// 1 = Enable Unicast Wake-up on LAN interrupt
// 0 = Disable Unicast Wake-up on LAN interrupt
#define EWOLIE_UCWOLIE	(1<<7)
// bit 6 AWOLIE: Any Packet Wake-up on LAN Interrupt Enable bit
// 1 = Enable Any Packet Wake-up on LAN interrupt
// 0 = Disable Any Packet Wake-up on LAN interrupt
#define EWOLIE_AWOLIE	(1<<6)
// bit 4 PMWOLIE: Pattern Match Wake-up on LAN Interrupt Enable bit
// 1 = Enable Pattern Match Wake-up on LAN interrupt
// 0 = Disable Pattern Match Wake-up on LAN interrupt
#define EWOLIE_PMWOLIE	(1<<4)
// bit 3 MPWOLIE: Magic Packet Wake-up on LAN Interrupt Enable bit
// 1 = Enable Magic Packet Wake-up on LAN interrupt
// 0 = Disable Magic Packet Wake-up on LAN interrupt
#define EWOLIE_MPWOLIE	(1<<3)
// bit 2 HTWOLIE: Hash Table Wake-up on LAN Interrupt Enable bit
// 1 = Enable Hash Table Wake-up on LAN interrupt
// 0 = Disable Hash Table Wake-up on LAN interrupt
#define EWOLIE_HTWOLIE	(1<<2)
// bit 1 MCWOLIE: Multicast Packet Wake-up on LAN Interrupt Enable bit
// 1 = Enable Mulitcast Packet Wake-up on LAN interrupt
// 0 = Disable Multicast Packet Wake-up on LAN interrupt
#define EWOLIE_MCWOLIE	(1<<1)
// bit 0 BCWOLIE: Broadcast Packet Wake-up on LAN Interrupt Enable bit
// 1 = Enable Broadcast Packet Wake-up on LAN interrupt
// 0 = Disable Broadcast Packet Wake-up on LAN interrupt
#define EWOLIE_BCWOLIE	(1)

// EWOLIR: ETHERNET WAKE-UP ON LAN INTERRUPT REQUEST
//    R/C-0     R/C-0   U-0    R/C-0     R/C-0     R/C-0     R/C-0     R/C-0 
// +---------+--------+-----+---------+---------+---------+---------+---------+
// + UCWOLIF + AWOLIF +  —  + PMWOLIF + MPWOLIF + HTWOLIF + MCWOLIF + BCWOLIF +
// +---------+--------+-----+---------+---------+---------+---------+---------+
// bit 7 UCWOLIF: Unicast Wake-up on LAN Interrupt Flag bit
// 1 = A packet was received with a destination address matching the local MAC address
// 0 = No unicast packets matching the local MAC address have been received
#define EWOLIR_UCWOLIF	(1<<7)
// bit 6 AWOLIF: Any Packet Wake-up on LAN Interrupt Flag bit
// 1 = A packet was received
// 0 = No packets have been received
#define EWOLIR_AWOLIF	(1<<6)
// bit 4 PMWOLIF: Pattern Match Wake-up on LAN Interrupt Flag bit
// 1 = A packet was received which passed the pattern match filter critieria
// 0 = No packets matching the pattern match criteria have been received
#define EWOLIR_PMWOLIF	(1<<4)
// bit 3 MPWOLIF: Magic Packet Wake-up on LAN Interrupt Flag bit
// 1 = A Magic Packet for the local MAC address was received
// 0 = No Magic Packets for the local MAC address have been received
#define EWOLIR_MPWOLIF	(1<<3)
// bit 2 HTWOLIF: Hash Table Wake-up on LAN Interrupt Flag bit
// 1 = A packet was received which passed the hash table filter criteria
// 0 = No packets matching the hash table filter criteria have been received
#define EWOLIR_HTWOLIF	(1<<2)
// bit 1 MCWOLIF: Multicast Packet Wake-up on LAN Interrupt Flag bit
// 1 = A packet was received with a multicast destination address
// 0 = No packets with a multicast destination address have been received
#define EWOLIR_MCWOLIF	(1<<1)
// bit 0 BCWOLIF: Broadcast Packet Wake-up on LAN Interrupt Flag bit
// 1 = A packet was received with a destination address of FF-FF-FF-FF-FF-FF
// 0 = No packets with a broadcast destination address have been received
#define EWOLIR_BCWOLIF	(1)

// MAPHSUP: MAC-PHY SUPPORT REGISTER
//    R/W-0     U-0   U-0  R/W-1   R/W-0    U-0   U-0  R/W-0
// +----------+-----+-----+-----+---------+-----+-----+-----+
// + RSTINTFC +  —  +  —  +  r  + RSTRMII +  —  +  —  +  r  +
// +----------+-----+-----+-----+---------+-----+-----+-----+
// bit 7 RSTINTFC: Interface Module Reset bit
// 1 = RMII module is held in Reset
// 0 = Normal operation if RSTRMII is also cleared
#define MAPHSUP_RSTINTFC	(1<<7)
// bit 3 RSTRMII: RMII Module Reset bit
// 1 = RMII module is held in Reset
// 0 = Normal operation if RSTINTFC is also cleared
#define MAPHSUP_RSTRMII		(1<<3)

// EBSTCON: SELF-TEST CONTROL REGISTER
//   R/W-0  R/W-0  R/W-0  R/W-0   R/W-0    R/W-0  R/W-0   R/W-0
// +------+------+------+------+--------+--------+-----+--------+
// + PSV2 + PSV1 + PSV0 + PSEL + TMSEL1 + TMSEL0 + TME + BISTST +
// +------+------+------+------+--------+--------+-----+--------+
// bit 7-5 PSV2:PSV0: Pattern Shift Value bits
// When TMSEL = 10:
// The bits in EBSTSD will shift left by this amount after writing to each memory location.
// When TMSEL = 00, 01 or 11:
// This value is ignored.
#define EBSTCON_PSV2	(1<<7)
#define EBSTCON_PSV1	(1<<6)
#define EBSTCON_PSV0	(1<<5)
// bit 4 PSEL: Port Select bit
// 1 = DMA and BIST modules will swap ports when accessing the memory
// 0 = Normal configuration
#define EBSTCON_PSEL	(1<<4)
// bit 3-2 TMSEL1:TMSEL0: Test Mode Select bits
// 11 = Race mode with random data fill
// 10 = Pattern shift fill
// 01 = Address fill
// 00 = Random data fill
#define EBSTCON_TMSEL1	(1<<3)
#define EBSTCON_TMSEL0	(1<<2)
// bit 1 TME: Test Mode Enable bit
// 1 = Enable Test mode
// 0 = Disable Test mode
#define EBSTCON_TME	(1<<1)
// bit 0 BISTST: Built-in Self-Test Start/Busy bit
// 1 = Test in progress; cleared automatically when test is done
// 0 = No test running
#define EBSTCON_BISTST	(1)

// The implementation used on this
// device supports SPI mode 0,0 only. In addition, the SPI
// port requires that SCK be at Idle in a low state;
// selectable clock polarity is not supported.
// SPI operation codes
// SPI™ INSTRUCTION SET FOR THE ENC28J60
//-------------------------------------------------------------------------------------------------------
// Instruction					Byte 0				Byte 1 and Following
// Name and Mnemonic			Opcode		Argument			Data
// Read Control Register (RCR)		000		aaaaa				N/A
// Read Buffer Memory (RBM)		001		11010 				N/A
// Write Control Register (WCR)		010 		aaaaa 				dddddddd
// Write Buffer Memory (WBM)		011 		11010 				dddddddd
// Bit Field Set (BFS)			100 		aaaaa				dddddddd
// Bit Field Clear (BFC)		101		aaaaa				dddddddd
// System Command (Soft Reset) (SC)	111		11111 				N/A
// ------------------------------------------------------------------------------------------------------
// Argument should be or with these values
#define ENC28J60_READ_CTRL_REG       0x00
#define ENC28J60_READ_BUF_MEM        0x3A
#define ENC28J60_WRITE_CTRL_REG      0x40
#define ENC28J60_WRITE_BUF_MEM       0x7A
#define ENC28J60_BIT_FIELD_SET       0x80
#define ENC28J60_BIT_FIELD_CLR       0xA0
#define ENC28J60_SOFT_RESET          0xFF


// The RXSTART_INIT should be zero. See Rev. B4 Silicon Errata
// buffer boundaries applied to internal 8K ram
// the entire available packet buffer space is allocated
//
// start with recbuf at 0/
#define RXSTART_INIT     0x0
// receive buffer end
#define RXSTOP_INIT      (0x1FFF-0x0600-1)
// start TX buffer at 0x1FFF-0x0600, pace for one full ethernet frame (~1500 bytes)
#define TXSTART_INIT     (0x1FFF-0x0600)
// stp TX buffer at end of mem
#define TXSTOP_INIT      0x1FFF
//
// max frame length which the conroller will accept:
#define        MAX_FRAMELEN        1500        // (note: maximum ethernet frame length would be 1518)
//#define MAX_FRAMELEN     600


// functions
extern unsigned char enc28j60ReadOp(unsigned char op, unsigned char address);
extern void enc28j60WriteOp(unsigned char op, unsigned char address, unsigned char data);
extern void enc28j60ReadBuffer(unsigned int len, unsigned char* data);
extern void enc28j60WriteBuffer(unsigned int len, unsigned char* data);
extern void enc28j60SetBank(unsigned char address);
extern unsigned char enc28j60Read(unsigned char address);
extern void enc28j60Write(unsigned char address, unsigned char data);
extern void enc28j60PhyWrite(unsigned char address, unsigned int data);
extern void enc28j60clkout(unsigned char clk);
extern void enc28j60Init(unsigned char* macaddr);
extern void enc28j60PacketSend(unsigned int len, unsigned char* packet);
extern unsigned char enc28j60hasRxPkt(void);
extern unsigned int enc28j60PacketReceive(unsigned int maxlen, unsigned char* packet);
extern unsigned char enc28j60getrev(void);
extern unsigned char enc28j60linkup(void);

#endif
//@}
