#ifndef SPI_H
#define SPI_H

// function prototypes

// SPI interface initializer
void spiInit(void);

// spiSendByte(u08 data) waits until the SPI interface is ready
// and then sends a single byte over the SPI port.  This command
// does not receive anything.
void spiSendByte(unsigned char data);

// spiTransferByte(u08 data) waits until the SPI interface is ready
// and then sends a single byte over the SPI port.  The function also
// returns the byte that was received during transmission.
unsigned char spiTransferByte(unsigned char data);

// spiTransferWord(u08 data) works just like spiTransferByte but
// operates on a whole word (16-bits of data).
unsigned int spiTransferWord(unsigned int data);

#endif
