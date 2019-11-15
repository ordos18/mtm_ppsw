#include <LPC21xx.H>
#include "spi.h"
#include <math.h>

// PINSEL0 Pin Connect Block
#define mSPI0_SCK			(1 << 8)
#define mSPI0_MISO		(1 << 10)
#define mSPI0_MOSI		(1 << 12)
#define mSPI0_SSEL		(1 << 14)

#define SPI_LSBF_bm (1 << 6)
#define SPI_MSTR_bm (1 << 5)
#define SPI_CPHA_bm (1 << 3)
#define SPI_CPOL_bm (1 << 4)
#define SPI_MCP4921 0x20

#define SPI_CS_MCP4921_bm (1 << 10)
#define SPI_CS_MCP23S09_bm (1 << 11)

#define SPI_SPIF_bm (1 << 7)
#define DAC_config 0x3000

#define PI 3.14159265

void DAC_MCP4921_Set (unsigned int uiVoltage) {
	
	unsigned int uiDataSPI;
	
	//initialize
	PINSEL0 = PINSEL0 | (mSPI0_SCK | mSPI0_MISO | mSPI0_MOSI | mSPI0_SSEL);
	IO0DIR = IO0DIR | SPI_CS_MCP4921_bm;
	S0SPCR = SPI_MCP4921;
	S0SPCCR = 8;
	
	//prepare data
	uiDataSPI = uiVoltage;
	uiDataSPI = uiDataSPI | DAC_config;
	
	//send data
	IO0CLR = SPI_CS_MCP4921_bm;
	
	S0SPDR = (uiDataSPI >> 8) & 0xFF;
	while ( !(S0SPSR & SPI_SPIF_bm) ) {}
	
	S0SPDR = uiDataSPI & 0xFF;
	while ( !(S0SPSR & SPI_SPIF_bm) ) {}
	IO0SET = SPI_CS_MCP4921_bm;
}

void DAC_MCP4921_Set_mV (unsigned int uiVoltage) {
	
	float fDACValue;
	
	fDACValue = uiVoltage * 4095.0 / 3300.0;
	DAC_MCP4921_Set(fDACValue);
}

void DAC_MCP4921_Sine (void) {
	
	unsigned int uiIter;
	
	for(uiIter = 0; uiIter<360; uiIter++) {
		DAC_MCP4921_Set_mV(sin(uiIter*PI/180)*1000+1000);
	}
}

void DAC_MCP4921_SineSaw (void) {
	
	unsigned int uiIter;
	
	for(uiIter = 0; uiIter<360; uiIter++) {
		DAC_MCP4921_Set_mV((PI - 2*(sin(uiIter*PI/180) + 0.5*sin(2*uiIter*PI/180) + 0.33*sin(3*uiIter*PI/180) + 0.25*sin(4*uiIter*PI/180))) *400);
	}
}

void SPI_ConfigMaster (struct SPI_FrameParams sFParams) {
	
	PINSEL0 = PINSEL0 | (mSPI0_SCK | mSPI0_MISO | mSPI0_MOSI | mSPI0_SSEL);
	S0SPCR = (1 << 5) | (sFParams.ucCpha << 3) | (sFParams.ucCpol << 4) | (sFParams.ucLsbf << 6);
	S0SPCCR = sFParams.ucClkDivider;
}

void SPI_ExecuteTransaction (struct SPI_TransactionParams sTParams) {
	
	unsigned char ucIterator, ucMaxBytes, ucByteTx, ucByteRx;
	
	ucMaxBytes = ( (sTParams.ucNrOfBytesForRx+sTParams.ucRxBytesOffset) > (sTParams.ucNrOfBytesForTx+sTParams.ucTxBytesOffset) ) \
				 ? (sTParams.ucNrOfBytesForRx+sTParams.ucRxBytesOffset) : (sTParams.ucNrOfBytesForTx+sTParams.ucTxBytesOffset);
	
	for(ucIterator = 0; ucIterator < ucMaxBytes; ucIterator++) {
		if( (ucIterator >= sTParams.ucTxBytesOffset) && (ucIterator < (sTParams.ucNrOfBytesForTx+sTParams.ucTxBytesOffset)) ) {
				ucByteTx = *(sTParams.pucBytesForTx);
				sTParams.pucBytesForTx++;
		} else {
				ucByteTx = 0;
		}
		ucByteRx = ucSPI_ExchangeByte(ucByteTx);
			
		if( (ucIterator >= sTParams.ucRxBytesOffset) && (ucIterator < (sTParams.ucNrOfBytesForRx+sTParams.ucRxBytesOffset)) ) {
				*(sTParams.pucBytesForRx) = ucByteRx;
				sTParams.pucBytesForRx++;
		}
	}
}

unsigned char ucSPI_ExchangeByte (unsigned char ucByteTx) {
	
	S0SPDR = ucByteTx;
	while ( !(S0SPSR & SPI_SPIF_bm) ) {}
	return S0SPDR;
}

void Port_MCP23S09_InitCSPin (void) {
	
	IO0DIR = IO0DIR | SPI_CS_MCP23S09_bm;
	IO0SET = SPI_CS_MCP23S09_bm;
}

void Port_MCP23S09_Set (unsigned char ucData) {
	
	unsigned char ucSPIDataTx[3];
	struct SPI_TransactionParams sTransactionParams;
	
	ucSPIDataTx[0] = 0x40;		// OpCode
	ucSPIDataTx[1] = 0x00;		// Address
	ucSPIDataTx[2] = 0x00;		// Data
	sTransactionParams.pucBytesForTx = ucSPIDataTx;
	sTransactionParams.ucNrOfBytesForTx = 3;
	sTransactionParams.ucTxBytesOffset = 0;
	sTransactionParams.pucBytesForRx = 0;
	sTransactionParams.ucNrOfBytesForRx = 0;
	sTransactionParams.ucRxBytesOffset = 0;
	
	IO0CLR = SPI_CS_MCP23S09_bm;
	SPI_ExecuteTransaction(sTransactionParams);
	IO0SET = SPI_CS_MCP23S09_bm;
	
	//------------------------//

	ucSPIDataTx[0] = 0x40;		// OpCode
	ucSPIDataTx[1] = 0x09;		// Address
	ucSPIDataTx[2] = ucData;	// Data
	sTransactionParams.pucBytesForTx = ucSPIDataTx;
	sTransactionParams.ucNrOfBytesForTx = 3;
	sTransactionParams.ucTxBytesOffset = 0;
	sTransactionParams.pucBytesForRx = 0;
	sTransactionParams.ucNrOfBytesForRx = 0;
	sTransactionParams.ucRxBytesOffset = 0;
	
	IO0CLR = SPI_CS_MCP23S09_bm;
	SPI_ExecuteTransaction(sTransactionParams);
	IO0SET = SPI_CS_MCP23S09_bm;
}

unsigned char Port_MCP23S09_Get (void) {
	
	unsigned char ucSPIDataTx[3];
	unsigned char ucSPIDataRx[1];
	struct SPI_TransactionParams sTransactionParams;
	
	ucSPIDataTx[0] = 0x40;		// OpCode
	ucSPIDataTx[1] = 0x00;		// Address
	ucSPIDataTx[2] = 0xFF;		// Data
	sTransactionParams.pucBytesForTx = ucSPIDataTx;
	sTransactionParams.ucNrOfBytesForTx = 3;
	sTransactionParams.ucTxBytesOffset = 0;
	sTransactionParams.pucBytesForRx = 0;
	sTransactionParams.ucNrOfBytesForRx = 0;
	sTransactionParams.ucRxBytesOffset = 0;
	
	IO0CLR = SPI_CS_MCP23S09_bm;
	SPI_ExecuteTransaction(sTransactionParams);
	IO0SET = SPI_CS_MCP23S09_bm;
	
	//------------------------//
	
	ucSPIDataTx[0] = 0x41;		// OpCode
	ucSPIDataTx[1] = 0x09;		// Address
	sTransactionParams.pucBytesForTx = ucSPIDataTx;
	sTransactionParams.ucNrOfBytesForTx = 2;
	sTransactionParams.ucTxBytesOffset = 0;
	sTransactionParams.pucBytesForRx = ucSPIDataRx;
	sTransactionParams.ucNrOfBytesForRx = 1;
	sTransactionParams.ucRxBytesOffset = 2;
	
	IO0CLR = SPI_CS_MCP23S09_bm;
	SPI_ExecuteTransaction(sTransactionParams);
	IO0SET = SPI_CS_MCP23S09_bm;

	return ucSPIDataRx[0];
}
