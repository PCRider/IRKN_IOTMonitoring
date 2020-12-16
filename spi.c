#include <avr/io.h>

#include <avr/interrupt.h>

#include "spi.h"

// Define the SPI_USEINT key if you want SPI bus operation to be
// interrupt-driven.  The primary reason for not using SPI in
// interrupt-driven mode is if the SPI send/transfer commands
// will be used from within some other interrupt service routine
// or if interrupts might be globally turned off due to of other
// aspects of your program
//
// Comment-out or uncomment this line as necessary
//#define SPI_USEINT

// global variables
volatile unsigned char spiTransferComplete;

// SPI interrupt service handler
#ifdef SPI_USEINT
ISR(SPI_STC_vect) {
    spiTransferComplete = 1;
}
#endif

// access routines
void spiInit() {
    #ifdef __AVR_ATmega128__
    // setup SPI I/O pins
    PORTB |= (1 << 1); // set SCK hi
    DDRB |= (1 << 1); // set SCK as output
    DDRB &= ~(1 << 3); // set MISO as input
    DDRB |= (1 << 2); // set MOSI as output
    DDRB |= (1 << 0); // SS must be output for Master mode to work
    #elif __AVR_ATmega8__
    // setup SPI I/O pins
    PORTB |= (1 << 5); // set SCK hi
    DDRB |= (1 << 5); // set SCK as output
    DDRB &= ~(1 << 4); // set MISO as input
    DDRB |= (1 << 3); // set MOSI as output
    DDRB |= (1 << 2); // SS must be output for Master mode to work
    #else
    // setup SPI I/O pins
    PORTB |= (1 << 7); // set SCK hi
    DDRB |= (1 << 7); // set SCK as output
    DDRB &= ~(1 << 6); // set MISO as input
    DDRB |= (1 << 5); // set MOSI as output
    DDRB |= (1 << 4); // SS must be output for Master mode to work
    #endif

    // setup SPI interface :
    // master mode
    SPCR |= (1 << MSTR);
    // clock = f/4
    //	SPCR &= ~(1<< SPR0);
    //	SPCR &= ~(1<< SPR1);
    // clock = f/16
    SPCR &= ~(1 << SPR0);
    SPCR |= (1 << SPR1);
    // select clock phase positive-going in middle of data
    SPCR &= ~(1 << CPOL);
    // Data order MSB first
    SPCR &= ~(1 << DORD);
    // enable SPI
    SPCR |= (1 << SPE);

    // some other possible configs
    //outp((1<<MSTR)|(1<<SPE)|(1<<SPR0), SPCR );
    //outp((1<<CPHA)|(1<<CPOL)|(1<<MSTR)|(1<<SPE)|(1<<SPR0)|(1<<SPR1), SPCR );
    //outp((1<<CPHA)|(1<<MSTR)|(1<<SPE)|(1<<SPR0), SPCR );

    // clear status
    SPSR;
    spiTransferComplete = 1;

    // enable SPI interrupt
    #ifdef SPI_USEINT
    SPCR |= (1 << SPIE);
    #endif
}
/*
void spiSetBitrate(u08 spr)
{
	SPCR =  (SPCR & ((1<<SPR0)|(1<<SPR1))) | (spr&((1<<SPR0)|(1<<SPR1))));
}
*/
void spiSendByte(unsigned char data) {
    // send a byte over SPI and ignore reply
    #ifdef SPI_USEINT
    while (!spiTransferComplete);
    spiTransferComplete = 0;
    #else
    while (!(SPSR & (1 << SPIF)));
    #endif

    SPDR = data;
}

unsigned char spiTransferByte(unsigned char data) {
    #ifdef SPI_USEINT
    // send the given data
    spiTransferComplete = 0;
    SPDR = data;
    // wait for transfer to complete
    while (!spiTransferComplete);
    #else
    // send the given data
    SPDR = data;
    // wait for transfer to complete
    while (!(SPSR & (1 << SPIF)));
    #endif
    // return the received data
    return SPDR;
}

unsigned int spiTransferWord(unsigned int data) {
    unsigned int rxData = 0;

    // send MS byte of given data
    rxData = (spiTransferByte((data >> 8) & 0x00FF)) << 8;
    // send LS byte of given data
    rxData |= (spiTransferByte(data & 0x00FF));

    // return the received data
    return rxData;
}
