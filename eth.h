//@{

#ifndef ETH_H
#define ETH_H

// ******* ETH *******
#define ETH_HEADER_LEN	14
// values of certain bytes:

#define ETHTYPE_IP_H_V  0x08
#define ETHTYPE_IP_L_V  0x00
// byte positions in the ethernet frame:
//
// Ethernet type field (2bytes):
#define ETH_TYPE_H_P 12
#define ETH_TYPE_L_P 13
//
#define ETH_DST_MAC 0
#define ETH_SRC_MAC 6


#ifndef IP_DST_P
#define IP_DST_P 0x1e
#endif

#ifndef IP_HEADER_LEN_VER_P
#define IP_HEADER_LEN_VER_P 0xe
#endif

extern void make_eth(unsigned char *buf, unsigned char *macaddr);
extern unsigned char eth_type_is_ip_and_my_ip(unsigned char *buf, unsigned char *ipaddr, unsigned int len);
#endif
//@}
