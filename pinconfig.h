// ATMEGA128a Pin Configuration 
// SS^  PB0 10
// SCK  PB1 11
// MOSI PB2 12
// MISO PB3 13
// VCC      21,52
// AVCC     64
// GND      22,53,63
// XTAL2    23 // grounded with a ceramic capasitor also
// XTAL1    24 // grounded with a ceramic capasitor also
// PDI PE0  2
// PDO PE1  3
// RST^     20
// we used RST^ pin instead of PEN^ pin for In System Programming 
#define ENC28J60_CONTROL_PORT   PORTB
#define ENC28J60_CONTROL_DDR    DDRB
#define ENC28J60_CONTROL_CS     PORTB0
#define ENC28J60_CONTROL_SCK    PORTB1
#define ENC28J60_CONTROL_SI     PORTB2
#define ENC28J60_CONTROL_SO     PORTB3
