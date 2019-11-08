
struct SPI_FrameParams {
	unsigned char ucCpha;
	unsigned char ucCpol;
	unsigned char ucLsbf;
	unsigned char ucClkDivider;
};

struct SPI_TransactionParams {
	unsigned char *pucBytesForTx;		// wskaznik na tablice z bajtami do wyslania
	unsigned char ucNrOfBytesForTx;		// ilosc bajtow do wyslania
	unsigned char ucTxBytesOffset;		// offset bajtow do wyslania
	
	unsigned char *pucBytesForRx;		// wskaznik na tablice na odebrane bajty
	unsigned char ucNrOfBytesForRx;		// ilosc bajtow do odebrania
	unsigned char ucRxBytesOffset;		// offset bajtow do odebrania
};

void DAC_MCP4921_Set (unsigned int uiVoltage);
void DAC_MCP4921_Set_mV (unsigned int uiVoltage);
void DAC_MCP4921_Sine (void);
void SPI_ConfigMaster (struct SPI_FrameParams sParams);
void SPI_ExecuteTransaction (struct SPI_TransactionParams sTParams);
void Port_MCP23S09_InitCSPin (void);
void Port_MCP23S09_Set (unsigned char ucData);
unsigned char Port_MCP23S09_Get (void);
