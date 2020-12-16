//@{

#ifndef ICMP_H
#define ICMP_H

// ******* ICMP *******
#define ICMP_TYPE_ECHOREPLY_V 0
#define ICMP_TYPE_ECHOREQUEST_V 8
//
#define ICMP_TYPE_P 0x22
#define ICMP_CHECKSUM_P 0x24



extern void make_echo_reply_from_request(unsigned char *buf, unsigned char *macaddr, unsigned char *ipaddr, unsigned int len);

#endif
//@}
