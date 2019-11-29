#include <LPC21xx.H>
#include "i2c.h"

// PINSEL0 Pin Connect Block
#define mI2C_SCL		(1 << 4)
#define mI2C_SDA		(1 << 6)

#define mI2C_INTERRUPT 0x08

#define mI2C_INTERRUPT_START 					0x08
#define mI2C_INTERRUPT_SLA_W_ACK			0x18
#define mI2C_INTERRUPT_SLA_W_NACK			0x20
#define mI2C_INTERRUPT_DATA_TX_ACK		0x28

#define mI2C_INTERRUPT_SLA_R_ACK			0x40
#define mI2C_INTERRUPT_SLA_R_NACK			0x48
#define mI2C_INTERRUPT_DATA_RX_ACK		0x50
#define mI2C_INTERRUPT_DATA_RX_NACK		0x58

// VIC (Vector Interrupt Controller) VICIntEnable
#define VIC_I2C_CHANNEL_NR 9

// VICVectCntlx Vector Control Registers
#define mIRQ_SLOT_ENABLE 0x00000020

unsigned char I2CAddress, I2CDataTx, I2CDataRx, I2CDone;
struct I2C_Params sI2C_Params;

void (*ptrI2CInterruptFunction)(void);

//(Interrupt Service Routine) of I2C interrupt
__irq void I2CIRQHandler (void){
	
	switch (I2STAT) {
		case (mI2C_INTERRUPT_START):
			I2CONCLR = 0x20;							// Clear start bit
			I2DAT = I2CAddress;						// Send address and write bit
			break;
		case (mI2C_INTERRUPT_SLA_W_ACK):
			I2DAT = I2CDataTx;							// Write data to TX register
			break;
		case (mI2C_INTERRUPT_SLA_W_NACK):
			I2DAT = I2CAddress;						// Resend address and write bit
			break;
		case (mI2C_INTERRUPT_DATA_TX_ACK):
			I2CONSET = 0x10;							// Stop condition
			I2CDone = 1;
			break;
		
		case (mI2C_INTERRUPT_SLA_R_ACK):
			I2CONSET = 0x04;							// Enable ACK for data byte
			break;
		case (mI2C_INTERRUPT_SLA_R_NACK):
			I2CONSET = 0x20;							// Resend Start condition
			break;
		case (mI2C_INTERRUPT_DATA_RX_ACK):
			I2CDataRx = I2DAT;
			I2CONSET = 0x10;							// Stop condition
			I2CDone = 1;
			break;
		case (mI2C_INTERRUPT_DATA_RX_NACK):
			I2CONSET = 0x20;							// Resend Start condition
			break;
		default:
			break;
	}
	
	I2CONCLR = mI2C_INTERRUPT; 	// skasowanie flagi przerwania
	VICVectAddr = 0x00; 				// potwierdzenie wykonania procedury obslugi przerwania
}

void I2CInterrupts_Init (void (*ptrInterruptFunction)(void)) {
	
	ptrI2CInterruptFunction = ptrInterruptFunction;
	
        // interrupts

	VICIntEnable |= (0x1 << VIC_I2C_CHANNEL_NR);            // Enable I2C interrupt channel 
	VICVectCntl4  = mIRQ_SLOT_ENABLE | VIC_I2C_CHANNEL_NR;  // Enable Slot 4 and assign it to I2C interrupt channel
	VICVectAddr4  = (unsigned long)I2CIRQHandler; 	   				// Set to Slot 4 Address of Interrupt Service Routine 
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
	
	sI2C_Params.ucDone = 1;
}

void I2C_Start (void) {
	
	I2CONCLR = 0x6C;
	I2CONSET = 0x40;
	I2CONSET = 0x20;
}

void I2C_ExecuteTransaction (void) {
	
	unsigned char ucIterator;
	
	sI2C_Params.ucDone = 0;
	I2CAddress = sI2C_Params.ucSlaveAddress;
	
	switch(sI2C_Params.eI2CTransmissionMode) {
		case TX:
			for(ucIterator = 0; ucIterator < sI2C_Params.ucNrOfBytesForTx; ucIterator++) {
				I2C_Start();
				I2CDataTx = *(sI2C_Params.pucBytesForTx);
				sI2C_Params.pucBytesForTx++;
				while(I2CDone != 1) {}
				I2CDone = 0;
			}
			break;
		case RX:
			for(ucIterator = 0; ucIterator < sI2C_Params.ucNrOfBytesForRx; ucIterator++) {
				I2C_Start();
				*(sI2C_Params.pucBytesForRx) = I2CDataRx;
				sI2C_Params.pucBytesForRx++;
				while(I2CDone != 1) {}
				I2CDone = 0;
			}
			break;
		case RX_AFTER_TX:
			for(ucIterator = 0; ucIterator < sI2C_Params.ucNrOfBytesForTx; ucIterator++) {
				I2C_Start();
				I2CDataTx = *(sI2C_Params.pucBytesForTx);
				sI2C_Params.pucBytesForTx++;
				while(I2CDone != 1) {}
				I2CDone = 0;
			}
			for(ucIterator = 0; ucIterator < sI2C_Params.ucNrOfBytesForRx; ucIterator++) {
				I2C_Start();
				*(sI2C_Params.pucBytesForRx) = I2CDataRx;
				sI2C_Params.pucBytesForRx++;
				while(I2CDone != 1) {}
				I2CDone = 0;
			}
			break;
		case TX_AFTER_RX:
			break;
		default:
			break;
	}
	
	
}

void PCF8574_Write (unsigned char ucData) {
	
	sI2C_Params.eI2CTransmissionMode = TX;
	sI2C_Params.ucSlaveAddress = 0x40;
	sI2C_Params.pucBytesForTx = &ucData;
	sI2C_Params.ucNrOfBytesForTx = 1;
	sI2C_Params.pucBytesForRx = 0;
	sI2C_Params.ucNrOfBytesForRx = 0;
	
	I2C_ExecuteTransaction();
}

void PCF8574_Read (void) {
	
	sI2C_Params.eI2CTransmissionMode = RX;
	sI2C_Params.ucSlaveAddress = 0x41;
	sI2C_Params.pucBytesForTx = 0;
	sI2C_Params.ucNrOfBytesForTx = 0;
	sI2C_Params.pucBytesForRx = 0;
	sI2C_Params.ucNrOfBytesForRx = 1;
	
	I2C_ExecuteTransaction();
}
