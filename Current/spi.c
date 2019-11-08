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

void SPI_ConfigMaster (struct SPI_FrameParams sFParams) {
	
	PINSEL0 = PINSEL0 | (mSPI0_SCK | mSPI0_MISO | mSPI0_MOSI | mSPI0_SSEL);
	S0SPCR = (1 << 5) | (sFParams.ucCpha << 3) | (sFParams.ucCpol << 4) | (sFParams.ucLsbf << 6);
	S0SPCCR = sFParams.ucClkDivider;
}

void SPI_ExecuteTransaction (struct SPI_TransactionParams sTParams) {
	
	unsigned char ucIterator, ucMaxBytes;
	
	ucMaxBytes = ( (sTParams.ucNrOfBytesForRx+sTParams.ucRxBytesOffset) > (sTParams.ucNrOfBytesForTx+sTParams.ucTxBytesOffset) ) \
				 ? (sTParams.ucNrOfBytesForRx+sTParams.ucRxBytesOffset) : (sTParams.ucNrOfBytesForTx+sTParams.ucTxBytesOffset);
	
	for(ucIterator = 0; ucIterator < ucMaxBytes; ucIterator++) {
		if( (ucIterator >= sTParams.ucTxBytesOffset) && (ucIterator < (sTParams.ucNrOfBytesForTx+sTParams.ucTxBytesOffset)) ) {
				S0SPDR = *(sTParams.pucBytesForTx);
				sTParams.pucBytesForTx++;
		} else {
				S0SPDR = 0;
		}
		while ( !(S0SPSR & SPI_SPIF_bm) ) {}
			
		if( (ucIterator >= sTParams.ucRxBytesOffset) && (ucIterator < (sTParams.ucNrOfBytesForRx+sTParams.ucRxBytesOffset)) ) {
				*(sTParams.pucBytesForRx) = S0SPDR;
				sTParams.pucBytesForRx++;
		}
	}
}

void Port_MCP23S09_InitCSPin (void) {
	
	IO0DIR = IO0DIR | SPI_CS_MCP23S09_bm;
	IO0SET = SPI_CS_MCP23S09_bm;
}

void Port_MCP23S09_Set (unsigned char ucData) {
	
	unsigned char ucSPIDataTx[3];
	struct SPI_TransactionParams sTransactionParams = {ucSPIDataTx, 3, 0, 0, 0, 0};
	
	ucSPIDataTx[0] = 0x40;
	ucSPIDataTx[1] = 0x00;
	ucSPIDataTx[2] = ucData;

	SPI_ExecuteTransaction(sTransactionParams);
}
