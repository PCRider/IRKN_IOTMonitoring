//********************************************************************************************
//
// File : AVR Compatibility Routins
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


#ifndef AVR_COMPAT_H
#define AVR_COMPAT_H

// uncomment the following line if you have avr-1.2.x or less:
//#define ALIBC_OLD 1

//https://www.avrfreaks.net/forum/sbi-cbi
// #define cbi(port, bit) (port) &= ~(1 << (bit))
// cbi\((.+),(.+)\)
// \1 &= ~(1<<\2)
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

// #define sbi(port, bit) (port) |= (1 << (bit))
// sbi\((.+),(.+)\)
// \1 |= (1<<\2)
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif





// inb\((.+?)\)
// \1
#ifndef inb
#define inb(sfr) _SFR_BYTE(sfr)
#endif
// outb\((.+),(.+)\)
// \1 = \2
#ifndef outb
#define outb(sfr, val) (_SFR_BYTE(sfr) = (val))
#endif





//
#ifndef inw
#define inw(sfr) _SFR_WORD(sfr)
#endif

//
#ifndef outw
#define outw(sfr, val) ( _SFR_WORD(sfr) = (val))
#endif





// same as outb but arguments are in reverse
#ifndef outp
#define outp(val, sfr) outb(sfr, val)
#endif

// same as inb
#ifndef inp
#define inp(sfr) inb(sfr)
#endif






// BV\((.*?)\)
// (1<<\1)
#ifndef BV
#define BV(bit) _BV(bit)
#endif

/* compatibility macro for libc 1.0 to 1.2 */
#ifndef PRG_RDB
#define PRG_RDB(addr)       pgm_read_byte(addr)
#endif


#define __bss_end   __heap_start
extern void *__heap_start;
// SFR Special Function Register
// It's not a minus sign ("-"), it's a bitwise NOT sign ("~"), Logical NOT sign ("!").
#ifndef uint8_t
 #define uint8_t   unsigned char
#endif
#ifndef uint16_t
 #define uint16_t   unsigned int
#endif

#ifdef __AVR_ENHANCED__

/* Nut/OS is still using the original ATmega103 register names for
   backward compatibility. */
#if  defined(__AVR_AT90USB1287__)
/* AT90USB1287 without USART0 */
#ifndef UDR
#define UDR     UDR1
#endif
#ifndef UBRR
#define UBRR    UBRR1L
#endif
#ifndef USR
#define USR     UCSR1A
#endif
#ifndef UCR
#define UCR     UCSR1B
#endif
#ifndef EICR
#define EICR    EICRB
#endif
#ifndef RXC
#define RXC     RXC1
#endif
#ifndef UDRE
#define UDRE    UDRE1
#endif
#ifndef FE
#define FE      FE1
#endif
#ifndef DOR
#define DOR     DOR1
#endif
#ifndef RXCIE
#define RXCIE   RXCIE1
#endif
#ifndef TXCIE
#define TXCIE   TXCIE1
#endif
#ifndef UDRIE
#define UDRIE   UDRIE1
#endif
#ifndef RXEN
#define RXEN    RXEN1
#endif
#ifndef TXEN
#define TXEN    TXEN1
#endif
#else
#ifndef UDR
#define UDR     UDR0
#endif
#ifndef UBRR
#define UBRR    UBRR0L
#endif
#ifndef USR
#define USR     UCSR0A
#endif
#ifndef UCR
#define UCR     UCSR0B
#endif
#ifndef EICR
#define EICR    EICRB
#endif
#ifndef RXC
#define RXC     RXC0
#endif
#ifndef UDRE
#define UDRE    UDRE0
#endif
#ifndef FE
#define FE      FE0
#endif
#ifndef DOR
#define DOR     DOR0
#endif
#ifndef RXCIE
#define RXCIE   RXCIE0
#endif
#ifndef TXCIE
#define TXCIE   TXCIE0
#endif
#ifndef UDRIE
#define UDRIE   UDRIE0
#endif
#ifndef RXEN
#define RXEN    RXEN0
#endif
#ifndef TXEN
#define TXEN    TXEN0
#endif
#endif

/* Some ATC90CAN128 SFR names are different to ATMEGA128. Define some
   compatibilty macros. */
#if defined(__AVR_AT90CAN128__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) ||  defined(__AVR_AT90USB1287__)
#ifndef ADCW
#define ADCW    ADC
#endif
#ifndef ADCSR
#define ADCSR   ADCSRA
#endif
#ifndef ADFR
#define ADFR    ADATE
#endif
#ifndef OCIE0
#define OCIE0   OCIE0A
#endif
#ifndef TCCR0
#define TCCR0   TCCR0A
#endif
#ifndef TCCR2
#define TCCR2   TCCR2A
#endif
#ifndef OCR0
#define OCR0    OCR0A
#endif
#ifndef TIMSK
#define TIMSK   TIMSK0
#endif
#ifndef TIFR
#define TIFR   TIFR0
#endif
#endif /* __AVR_AT90CAN128__ || __AVR_ATmega2560__ || __AVR_ATmega2561__ || __AVR_AT90USB1287__)*/


#endif /* __AVR_ENHANCED__ */


#endif //AVR_COMPAT_H
//@}
