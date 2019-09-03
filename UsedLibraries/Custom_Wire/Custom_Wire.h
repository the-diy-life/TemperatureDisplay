#ifndef _Custom_Wire
#define _Custom_Wire


#include <avr/io.h>        // standard AVR header


class AVR_TWI_Class {
public:
void begin(unsigned long Clock);
void beginTransmission(void);
void write(unsigned char cByte);
unsigned char read(unsigned char ack);
void endTransmission(void);

};
extern AVR_TWI_Class customWire;

#endif