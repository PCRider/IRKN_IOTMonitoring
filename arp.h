//@{

#ifndef ARP_H
#define ARP_H


// ******* ARP *******
#define ARP_OPCODE_REPLY_H_V 0x0
#define ARP_OPCODE_REPLY_L_V 0x02
//
#define ETHTYPE_ARP_L_V 0x06
// arp.dst.ip
#define ARP_DST_IP_P 0x26
// arp.opcode
#define ARP_OPCODE_H_P 0x14
#define ARP_OPCODE_L_P 0x15
// arp.src.mac
#define ARP_SRC_MAC_P 0x16
#define ARP_SRC_IP_P 0x1c
#define ARP_DST_MAC_P 0x20
#define ARP_DST_IP_P 0x26



#define ETH_TYPE_H_P 12
#define ETH_TYPE_L_P 13

#define ETHTYPE_ARP_H_V 0x08
#define ETHTYPE_ARP_L_V 0x06


extern unsigned char eth_type_is_arp_and_my_ip(unsigned char *buf, unsigned char *ipaddr, unsigned int len);
extern void make_arp_answer_from_request(unsigned char *buf, unsigned char *macaddr, unsigned char *ipaddr);

#endif
//@}
