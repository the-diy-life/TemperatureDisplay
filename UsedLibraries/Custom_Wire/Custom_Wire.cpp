#include <Custom_Wire.h>

AVR_TWI_Class customWire;

void AVR_TWI_Class::begin(unsigned long Clock) {
	
  // set status register
  TWSR = 0x00; // No prescaler
  // set the bit rate register
  TWBR = Clock; // SCL frequency is 50K for XTAL = 8M and 40 for XTAL = 16M

}

void AVR_TWI_Class::beginTransmission(void) {
  // Set the control registers bits TWINT interrupt bit TWEN enable TWI, and TWSTA to set start condtion
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
  while ((TWCR & 0x80) == 0); // wait to finish
}

void AVR_TWI_Class::write(unsigned char cByte) {
  // set the data that want to write in slave in data register
  TWDR = cByte;
  // set interrupt bit and enable TWI
  TWCR = (1 << TWINT) | (1 << TWEN);
  // wait to finish
  while ((TWCR & 0x80) == 0);
}

unsigned char AVR_TWI_Class::read(unsigned char ack){
	// Set the control registers bits TWINT interrupt bit TWEN enable TWI, and TWEA to set Acknowledge
  TWCR = (1 << TWINT) | (1 << TWEN) | (ack << TWEA);
  while ((TWCR & 0x80) == 0); // wait to finish
  // Return the data register
  return TWDR;
}

void AVR_TWI_Class::endTransmission(void){
	// Set the control registers bits TWINT interrupt bit TWEN enable TWI TWSTO to set stop condtion
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}