#ifndef _custom_avr_spi
#define _custom_avr_spi


#include <avr/io.h>        // standard AVR header


#define AVR_MOSI 3       // Master Out Slave In 	is pin No.11
#define AVR_MISO 4       // Master In Slave Out     is pin No.12
#define AVR_SCK 5        // Shift Clock         	is pin No.13
#define AVR_CS 2		 // Chip Select				is pin No.9


class AVR_SPI_Class {
public:
void begin(void);
unsigned char transfer(unsigned char cData);
};

extern AVR_SPI_Class AVR_SPI;

#endif