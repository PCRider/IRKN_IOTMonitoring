//@{

#ifndef NET_H
#define NET_H
// ******* UDP *******
#define UDP_HEADER_LEN	8
//
#define UDP_SRC_PORT_H_P 0x22
#define UDP_SRC_PORT_L_P 0x23
#define UDP_DST_PORT_H_P 0x24
#define UDP_DST_PORT_L_P 0x25
//
#define UDP_LEN_H_P 0x26
#define UDP_LEN_L_P 0x27
#define UDP_CHECKSUM_H_P 0x28
#define UDP_CHECKSUM_L_P 0x29
#define UDP_DATA_P 0x2a


extern void make_udp_reply_from_request(unsigned char *buf,char *data, unsigned char datalen, unsigned char *macaddr, unsigned char *ipaddr, unsigned int port);

#endif
//@}
