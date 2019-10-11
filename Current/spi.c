#include <LPC21xx.H>
#include "spi.h"

// PINSEL0 Pin Connect Block
#define mSPI0_SCK			(1 << 8)
#define mSPI0_MISO		(1 << 10)
#define mSPI0_MOSI		(1 << 12)
#define mSPI0_SSEL		(1 << 14)

#define SPI_CS_bm (1 << 10)
#define SPI_MSTR_bm (1 << 5)
#define SPI_CPHA_bm (1 << 3)
#define SPI_CPOL_bm (1 << 4)

void DAC_MCP4921_Set (unsigned int uiVoltage) {
	
	PINSEL0 = PINSEL0 | (mSPI0_SCK | mSPI0_MISO | mSPI0_MOSI | mSPI0_SSEL);
	IO1DIR = IO1DIR | SPI_CS_bm;
	S0SPCR = S0SPCR | (SPI_MSTR_bm | SPI_CPHA_bm | SPI_CPOL_bm);
	
}
