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

#define mI2C_START										0x20
#define mI2C_STOP											0x10
#define mI2C_ACK											0x04
#define mI2C_I2EN											0x40

// VIC (Vector Interrupt Controller) VICIntEnable
#define VIC_I2C_CHANNEL_NR 9

// VICVectCntlx Vector Control Registers
#define mIRQ_SLOT_ENABLE 0x00000020

unsigned char ucByteCounter;
struct I2C_Params sI2C_Params;
unsigned char ucPCF8574_Input, ucMC24LC64_Input;
unsigned char ucI2CData[3];

void (*ptrI2CInterruptFunction)(void);

//(Interrupt Service Routine) of I2C interrupt
__irq void I2CIRQHandler (void){
	
	switch (I2STAT) {
		case (mI2C_INTERRUPT_START):
			I2CONCLR = mI2C_START;				// Clear start bit
			I2DAT = sI2C_Params.ucSlaveAddress;						// Send address and write bit
			ucByteCounter = 0;
			break;
		case (mI2C_INTERRUPT_SLA_W_ACK):
			I2DAT = *sI2C_Params.pucBytesForTx;							// Write data to TX register
			ucByteCounter++;
			break;
		case (mI2C_INTERRUPT_SLA_W_NACK):
			I2DAT = sI2C_Params.ucSlaveAddress;						// Resend address and write bit
			break;
		case (mI2C_INTERRUPT_DATA_TX_ACK):
			if (sI2C_Params.ucNrOfBytesForTx > ucByteCounter) {
				I2DAT = *(sI2C_Params.pucBytesForTx + ucByteCounter++);
			} else {
				I2CONSET = mI2C_STOP;					// Stop condition
				sI2C_Params.ucDone = 1;
			}
			break;
		
		case (mI2C_INTERRUPT_SLA_R_ACK):
			if (sI2C_Params.ucNrOfBytesForRx > ucByteCounter + 1) {
				*(sI2C_Params.pucBytesForRx + ucByteCounter++) = I2DAT;
				I2CONSET = mI2C_ACK;
			} else {
				I2CONCLR = mI2C_ACK;
			}
			break;
		case (mI2C_INTERRUPT_SLA_R_NACK):
			I2CONSET = mI2C_START;				// Resend Start condition
			break;
		case (mI2C_INTERRUPT_DATA_RX_ACK):
			if (sI2C_Params.ucNrOfBytesForRx > ucByteCounter + 1) {
				*(sI2C_Params.pucBytesForRx + ucByteCounter++) = I2DAT;
				I2CONSET = mI2C_ACK;
			} else {
				I2CONCLR = mI2C_ACK;
			}
			break;
		case (mI2C_INTERRUPT_DATA_RX_NACK):
			*(sI2C_Params.pucBytesForRx + ucByteCounter++) = I2DAT;
			I2CONSET = mI2C_STOP;					// Stop condition
			sI2C_Params.ucDone = 1;
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
	I2CONSET = mI2C_I2EN;
	I2CONSET = mI2C_START;
}

void I2C_ExecuteTransaction (void) {
	
	sI2C_Params.ucDone = 0;
	I2C_Start();
	while(sI2C_Params.ucDone != 1) {}
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
	sI2C_Params.pucBytesForRx = &ucPCF8574_Input;
	sI2C_Params.ucNrOfBytesForRx = 1;
	
	I2C_ExecuteTransaction();
}

void MC24LC64_ByteWrite (unsigned int WordAddress, unsigned char Data) {
	
	ucI2CData[0] = 0x00FF & WordAddress;
	ucI2CData[1] = 0x00FF & (WordAddress >> 8);
	ucI2CData[2] = Data;
	
	sI2C_Params.eI2CTransmissionMode = TX;
	sI2C_Params.ucSlaveAddress = 0xA0;
	sI2C_Params.pucBytesForTx = ucI2CData;
	sI2C_Params.ucNrOfBytesForTx = 3;
	sI2C_Params.pucBytesForRx = 0;
	sI2C_Params.ucNrOfBytesForRx = 0;
	
	I2C_ExecuteTransaction();
}

void MC24LC64_RandomRead (unsigned int WordAddress) {
	
	ucI2CData[0] = 0x00FF & WordAddress;
	ucI2CData[1] = 0x00FF & (WordAddress >> 8);
	
	sI2C_Params.eI2CTransmissionMode = RX;
	sI2C_Params.ucSlaveAddress = 0xA1;
	sI2C_Params.pucBytesForTx = ucI2CData;
	sI2C_Params.ucNrOfBytesForTx = 2;
	sI2C_Params.pucBytesForRx = &ucMC24LC64_Input;
	sI2C_Params.ucNrOfBytesForRx = 1;
	
	I2C_ExecuteTransaction();
}
