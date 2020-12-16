# Name: Makefile
# Project: ENC28J60
# Author: Ehsan Ghasemlou
# Creation Date: 2020-10-29
# Tabsize: 4
# Copyright: (c) 2020 by PCRider
# License: Proprietary (CommercialLicense.txt)
# This Revision: $Id: Makefile 692 2020-10-29 12:20:40Z cs $

MCU		= atmega128a
F_CPU   	= 11059200	# in Hz
# Fuse atmega128a high byte HFUSE:
# 0xc9 = 1 1 0 0   1 0 0 1 <-- BOOTRST (boot reset vector at 0x0000)
#        ^ ^ ^ ^   ^ ^ ^------ BOOTSZ0
#        | | | |   | +-------- BOOTSZ1
#        | | | |   + --------- EESAVE (don't preserve EEPROM over chip erase)
#        | | | +-------------- CKOPT (full output swing)
#        | | +---------------- SPIEN (allow serial programming)
#        | +------------------ WDTON (WDT not always on)
#        +-------------------- RSTDISBL (reset pin is enabled)
# Fuse atmega128a low byte LFUSE:
# 0x9f = 1 0 0 1   1 1 1 1
#        ^ ^ \ /   \--+--/
#        | |  |       +------- CKSEL 3..0 (external >8M crystal)
#        | |  +--------------- SUT 1..0 (crystal osc, BOD enabled)
#        | +------------------ BODEN (BrownOut Detector enabled)
#        +-------------------- BODLEVEL (2.7V)
FUSE_L  	= 0xf1S
FUSE_H  	= 0xdd
FUSE_EXT	= 0xfeS

PROGRAMMER	= usbasp
PARTNO		= m128
PORT		= usb
AVRDUDE		= avrdude -c ${PROGRAMMER} -p ${PARTNO} -P ${PORT} -V

CFLAGS		=  -save-temps
CFLAGS		+= -Wstrict-prototypes -mcall-prologues
#CFLAGS		+= -Wno-deprecated-declarations -D__PROG_TYPES_COMPAT__
CFLAGS		+= -Wl,--gc-sections
CFLAGS		+= -fdata-sections -ffunction-sections
OBJECTS		=   main.o enc28j60.o eth.o arp.o icmp.o ip.o udp.o tcp.o lcd.o
COMPILE		=   avr-gcc -Wall -Os --std=gnu99 -DF_CPU=$(F_CPU) $(CFLAGS) -mmcu=$(MCU)

# symbolic targets:
help:
	@echo "This Makefile has no default rule. Use one of the following:"
	@echo "make hex ....... to build main.hex"
	@echo "make program ... to flash fuses and firmware"
	@echo "make fuse ...... to flash the fuses"
	@echo "make flash ..... to flash the firmware (use this on metaboard)"
	@echo "make clean ..... to delete objects and hex file"
	@echo "Current values:"
	@echo "       MCU=${MCU}"
	@echo "       LFUSE=${FUSE_L}"
	@echo "       HFUSE=${FUSE_H}"
	@echo "       EFUSE=${FUSE_EXT}"
	@echo "       Clock=${F_CPU}"
	@echo "       Programmer=${PROGRAMMER}"
	@echo "       Port=${PORT}"
	@echo "       Partnumber=${PARTNO}"

hex: main.hex

program: flash fuse

# rule for programming fuse bits:
fuse:
	@[ "$(FUSE_H)" != "" -a "$(FUSE_L)" != "" ] || \
		{ echo "*** Edit Makefile and choose values for FUSE_L and FUSE_H!"; exit 1; }
	$(AVRDUDE) -U hfuse:w:$(FUSE_H):m -U lfuse:w:$(FUSE_L):m -U efuse:w:$(FUSE_EXT):m 

# rule for uploading firmware:
flash: main.hex
	$(AVRDUDE) -e
	$(AVRDUDE) -U flash:w:main.hex:i

# rule for deleting dependent files (those which can be built by Make):
clean:
	rm -f main.hex main.lst main.obj main.cof main.list main.map main.eep.hex main.elf *.o *.i *.s

# Generic rule for compiling C files:
.c.o:
	$(COMPILE) -c $< -o $@

# Generic rule for assembling Assembler source files:
.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

# Generic rule for compiling C to assembler, used for debugging only.
.c.s:
	$(COMPILE) -S $< -o $@

# file targets:


main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS) -Wl,-Map,main.map

main.hex: main.elf
	rm -f main.hex main.eep.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size -C --mcu=$(MCU) main.elf

# debugging targets:

disasm:	main.elf
	avr-objdump -d main.elf

cpp:
	$(COMPILE) -E main.c
