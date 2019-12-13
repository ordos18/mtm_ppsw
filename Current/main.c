#include <LPC21xx.H>
#include "keyboard.h"
#include "servo.h"
#include "uart.h"
#include "string.h"
#include "command_decoder.h"
#include "timer.h"
#include "timer_interrupts.h"
#include "adc.h"
#include "spi.h"
#include "i2c.h"
#include <math.h>

#define PI 3.14159265



/**********************************************/
int main () {
	
	static unsigned short uiT[360];
	unsigned short usIter, usBinCounter;
	extern char cOdebranyZnak;
	extern Token asToken[];
	extern unsigned char ucTokenNr;
	extern struct Watch sWatch;
	extern unsigned char ucPCF8574_Input, ucMC24LC64_Input;
	
	char RxString[RECEIVER_SIZE];
	char cStringToSend[TRANSMITER_SIZE];
	char cNewLine[] = "\n";
	unsigned char fGateStateToSend = 0;
	enum Direction {LEFT, RIGHT} eDirection = LEFT;
	/*
	unsigned int uiReceivedNumber;
	unsigned char fNumberToCalc = 0;
	unsigned char fIDToSend = 0;
	unsigned char fUnknownToSend = 0;
	
	
	KeyboardInit();
	ServoInit(200);
	Timer1Interrupts_Init(1000000, &WatchUpdate);
	ADC_InitWithInt(&ServoGoTo);
	*/
	InitTimer0();
	UART_InitWithInt(9600);
	
	for(usIter = 0; usIter < 360; usIter++) {
		uiT[usIter] = sin(usIter*PI/180)*1000+1000;
	}
	usBinCounter = 1;
	
	//DAC_MCP4921_Set(1);
	//Port_MCP23S09_InitCSPin();
	
	I2C_Init();
	
	while (1) {
		//DAC_MCP4921_Sine();
		//DAC_MCP4921_SineSaw();
		//for(usIter = 0; usIter < 360; usIter++) {
		//	DAC_MCP4921_Set_mV(uiT[usIter]);
		//}
		//Port_MCP23S09_Set(usBinCounter++);
		
		//PCF8574_Write(usBinCounter++);
	  
		
		/*
		// Knight Rider
		if(eDirection == LEFT) {
			usBinCounter = usBinCounter << 1;
		}
		else {
			usBinCounter = usBinCounter >> 1;
		}
		
		if(usBinCounter == 0x80) {
			eDirection = RIGHT;
		}
		else if(usBinCounter == 0x01) {
			eDirection = LEFT;
		}
		
		PCF8574_Write(usBinCounter);
		WaitOnTimer0(100000);
		*/
		
		
		// Terminal
		if( eReceiver_GetStatus() == READY ) {
			Receiver_GetStringCopy(RxString);
			DecodeMsg(RxString);
			
			if( (ucTokenNr > 0) && (asToken[0].eType == KEYWORD) ) {
				switch(asToken[0].uValue.eKeyword) {
					case I2C_W:
						if (ucTokenNr > 1) {
							PCF8574_Write(asToken[1].uValue.uiNumber);
						}
						break;
					case I2C_R:
						fGateStateToSend = 1;
						break;
					case I2C_MW:
						if (ucTokenNr > 1) {
							PCF8574_Write(asToken[1].uValue.uiNumber);
						}
						break;
					case I2C_MR:
						fGateStateToSend = 1;
						break;
					default: {}
				}
			}
		}
		if (FREE == eTransmitter_GetStatus()) {
			if (1 == fGateStateToSend) {
				fGateStateToSend = 0;
				PCF8574_Read();
				CopyString(cNewLine, cStringToSend);
				AppendUIntToString(ucPCF8574_Input, cStringToSend);
				Transmitter_SendString(cStringToSend);
			}
		}
		
	}
}
