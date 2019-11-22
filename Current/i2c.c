#include <LPC21xx.H>
#include "i2c.h"

// PINSEL0 Pin Connect Block
#define mI2C_SCL		(1 << 4)
#define mI2C_SDA		(1 << 6)

#define mI2C_INTERRUPT 0x08

// VIC (Vector Interrupt Controller) VICIntEnable
#define VIC_I2C_CHANNEL_NR 9

// VICVectCntlx Vector Control Registers
#define mIRQ_SLOT_ENABLE 0x00000020

unsigned char I2CAddress, I2CData;

void (*ptrI2CInterruptFunction)(void);

//(Interrupt Service Routine) of I2C interrupt
__irq void I2CIRQHandler(void){
	
	switch (I2STAT) {
		case (0x08):						// Start bit
			I2CONCLR = 0x20;			// Clear start bit
			I2DAT = I2CAddress;		// Send address and write bit
			break;
		case (0x18):						// Slave address +W, ACK
			I2DAT = I2CData;			// Write data to TX register
			break;
		case (0x20):						// Slave address +W, Not ACK
			I2DAT = I2CAddress;		// Resend address and write bit
			break;
		case (0x28):						// Data sent, ACK
			I2CONSET = 0x10;			// Stop condition
			break;
		default:
			break;
	}
	
	I2CONCLR = mI2C_INTERRUPT; 	// skasowanie flagi przerwania
	VICVectAddr = 0x00; 				// potwierdzenie wykonania procedury obslugi przerwania
}

void I2CInterrupts_Init(void (*ptrInterruptFunction)(void)) {
	
	ptrI2CInterruptFunction = ptrInterruptFunction;
	
        // interrupts

	VICIntEnable |= (0x1 << VIC_I2C_CHANNEL_NR);            // Enable I2C interrupt channel 
	VICVectCntl4  = mIRQ_SLOT_ENABLE | VIC_I2C_CHANNEL_NR;  // Enable Slot 4 and assign it to I2C interrupt channel
	VICVectAddr4  =(unsigned long)I2CIRQHandler; 	   				// Set to Slot 4 Address of Interrupt Service Routine 
}

void I2C_Init (void) {
	
	// initialize
	PINSEL0 = PINSEL0 | (mI2C_SCL | mI2C_SDA);
	I2CONCLR = 0x6C;
	I2SCLH = 0x80;
	I2SCLL = 0x80;
	
	// interrupts
	VICIntEnable |= (0x1 << VIC_I2C_CHANNEL_NR);            // Enable I2C interrupt channel 
	VICVectCntl4  = mIRQ_SLOT_ENABLE | VIC_I2C_CHANNEL_NR;  // Enable Slot 4 and assign it to I2C interrupt channel
	VICVectAddr4  =(unsigned long)I2CIRQHandler; 	   				// Set to Slot 4 Address of Interrupt Service Routine 
}

void PCF8574_Write(unsigned char ucData) {
	
	I2CAddress = 0x40;
	I2CData = ucData;
	I2CONCLR = 0x6C;
	I2CONSET = 0x40;
	I2CONSET = 0x20;
	
}

void I2C_ExecuteTransaction(struct I2C_Params sTParams) {
	
	switch(sTParams.eI2CTransmissionMode) {
		case TX:
			// TODO
			break;
		case RX:
			
			break;
		case RX_AFTER_TX:
			
			break;
		case TX_AFTER_RX:
			break;
		default:
			break;
	}
}
