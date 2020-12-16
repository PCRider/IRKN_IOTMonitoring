// notation: _P = position of a field
//           _V = value of a field
//@{
#ifndef TCP_H
#define TCP_H




// ******* TCP *******
#define TCP_SRC_PORT_H_P 0x22
#define TCP_SRC_PORT_L_P 0x23
#define TCP_DST_PORT_H_P 0x24
#define TCP_DST_PORT_L_P 0x25
// the tcp seq number is 4 bytes 0x26-0x29
#define TCP_SEQ_H_P 0x26
#define TCP_SEQACK_H_P 0x2a
// flags: SYN=2
#define TCP_FLAGS_P 0x2f
#define TCP_FLAGS_SYN_V 2
#define TCP_FLAGS_FIN_V 1
#define TCP_FLAGS_PUSH_V 8
#define TCP_FLAGS_SYNACK_V 0x12
#define TCP_FLAGS_ACK_V 0x10
#define TCP_FLAGS_PSHACK_V 0x18
//  plain len without the options:
#define TCP_HEADER_LEN_PLAIN 20
#define TCP_HEADER_LEN_P 0x2e
#define TCP_CHECKSUM_H_P 0x32
#define TCP_CHECKSUM_L_P 0x33
#define TCP_OPTIONS_P 0x36



extern void make_tcp_synack_from_syn(unsigned char *buf, unsigned char *macaddr, unsigned char *ipaddr, unsigned char wwwport,unsigned char *seqnum);
extern int data_len(unsigned char *buf);
extern int hdr_len(unsigned char *buf);
extern unsigned int get_tcp_data_pointer(int info_hdr_len, int info_data_len);
extern unsigned int fill_tcp_data_p(unsigned char *buf,unsigned int pos, const char *progmem_s);
extern unsigned int fill_tcp_data(unsigned char *buf,unsigned int pos, const char *s);
extern void make_tcp_ack_from_any(unsigned char *buf,int info_data_len,unsigned char *macaddr,unsigned char *ipaddr,unsigned char wwwport,unsigned char *seqnum);
extern void make_tcp_ack_with_data(unsigned char *buf,unsigned int dlen);


#endif /* IP_ARP_UDP_TCP_H */
//@}
