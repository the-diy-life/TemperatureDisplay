#include <custom_avr_spi.h>

AVR_SPI_Class AVR_SPI;

void AVR_SPI_Class::begin(void) {
  // Set MOSI, SCK and SS as Output Pins
  DDRB |= (1 << AVR_MOSI) | (1 << AVR_SCK) | (1 << AVR_CS) ;
  DDRB &= ~(1 << AVR_MISO); // Set MISO as an Input Pin
   // Enable SPI, Master mode, Shift Clock = CLK /4
  //SPCR = (1 << SPE) | (1 << MSTR) | (0 << SPR0); or
  SPCR = (1 << SPE) | (1 << MSTR);
  // Enable SPI, Master mode, Shift Clock = CLK /16
  //SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

unsigned char AVR_SPI_Class::transfer(unsigned char cData) {
  SPDR = cData;     // Start transmission
  while (!(SPSR & (1 << SPIF))); // Wait for transmission complete
  return SPDR;      // return the received data
} 
