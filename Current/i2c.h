
enum I2CTransmissionMode {TX, RX, RX_AFTER_TX, TX_AFTER_RX};

struct I2C_Params {
	enum I2CTransmissionMode eI2CTransmissionMode;
	
	unsigned char ucSlaveAddress;
	
	unsigned char *pucBytesForTx;
	unsigned char ucNrOfBytesForTx;
	
	unsigned char *pucBytesForRx;
	unsigned char ucNrOfBytesForRx;
	
	unsigned char ucDone;
};

void I2C_Init (void);
void PCF8574_Write(unsigned char ucData);
void PCF8574_Read (void);
