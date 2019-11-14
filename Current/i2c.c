#include <LPC21xx.H>
#include "i2c.h"

// PINSEL0 Pin Connect Block
#define mI2C_SCL		(1 << 4)
#define mI2C_SDA		(1 << 6)

void I2C_Init (void) {
	
	//initialize
	PINSEL0 = PINSEL0 | (mI2C_SCL | mI2C_SDA);
	I2CONCLR = 0x6C;
}

void PCF8574_Write(unsigned char ucData) {
	
}
