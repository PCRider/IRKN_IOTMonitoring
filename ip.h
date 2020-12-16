//@{

#ifndef IP_H
#define IP_H

// ******* IP *******
#define IP_HEADER_LEN	20
// ip.src
#define IP_SRC_P 0x1a

#define IP_CHECKSUM_P 0x18
#define IP_TTL_P 0x16
#define IP_FLAGS_P 0x14
#define IP_P 0xe
#define IP_TOTLEN_H_P 0x10
#define IP_TOTLEN_L_P 0x11

#define IP_PROTO_P 0x17  

#define IP_PROTO_ICMP_V 1
#define IP_PROTO_TCP_V 6
// 17=0x11
#define IP_PROTO_UDP_V 17

#ifndef IP_DST_P
#define IP_DST_P 0x1e
#endif

#ifndef IP_HEADER_LEN_VER_P
#define IP_HEADER_LEN_VER_P 0xe
#endif

extern void make_ip(unsigned char *buf, unsigned char *ipaddr);
extern void fill_ip_hdr_checksum(unsigned char *buf);
extern unsigned int checksum(unsigned char *buf, unsigned int len,unsigned char type);


#endif
//@}
