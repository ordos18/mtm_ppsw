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
	
	char RxString[RECEIVER_SIZE];
	char cStringToSend[TRANSMITER_SIZE];
	char cNewLine[] = "\n";
	unsigned char fGateStateToSend = 0;
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
	usBinCounter = 0;
	
	DAC_MCP4921_Set(1);
	Port_MCP23S09_InitCSPin();

	while (1) {
		//DAC_MCP4921_Sine();
		//for(uiIter = 0; uiIter < 360; uiIter++) {
		//	DAC_MCP4921_Set_mV(uiT[uiIter]);
		//}
		//Port_MCP23S09_Set(usBinCounter++);
		//WaitOnTimer0(100000);
		
		if( eReceiver_GetStatus() == READY ) {
			Receiver_GetStringCopy(RxString);
			DecodeMsg(RxString);
			
			if( (ucTokenNr > 0) && (asToken[0].eType == KEYWORD) ) {
				switch(asToken[0].uValue.eKeyword) {
					case SPI_S:
						if (ucTokenNr > 1) {
							Port_MCP23S09_Set(asToken[1].uValue.uiNumber);
						}
						break;
					case SPI_G:
						fGateStateToSend = 1;
						break;
					default: {}
				}
			}
		}
		if (FREE == eTransmitter_GetStatus()) {
			if (1 == fGateStateToSend) {
				fGateStateToSend = 0;
				CopyString(cNewLine, cStringToSend);
				AppendUIntToString(Port_MCP23S09_Get(), cStringToSend);
				Transmitter_SendString(cStringToSend);
			}
		}
	}
}
