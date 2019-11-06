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
	
	unsigned short uiT[360];
	unsigned short uiIter;
	/*
	extern char cOdebranyZnak;
	extern Token asToken[];
	extern unsigned char ucTokenNr;
	extern struct Watch sWatch;
	
	char RxString[RECEIVER_SIZE];
	char cStringToSend[TRANSMITER_SIZE];
	unsigned int uiReceivedNumber;
	unsigned char fNumberToCalc = 0;
	unsigned char fIDToSend = 0;
	unsigned char fUnknownToSend = 0;
	
	
	KeyboardInit();
	ServoInit(200);
	InitTimer0();
	Timer1Interrupts_Init(1000000, &WatchUpdate);
	UART_InitWithInt(9600);
	ADC_InitWithInt(&ServoGoTo);
	*/
	
	for(uiIter = 0; uiIter < 360; uiIter++) {
		uiT[uiIter] = sin(uiIter*PI/180)*1000+1000;
	}

	while (1) {
		//DAC_MCP4921_Sine();
		for(uiIter = 0; uiIter < 360; uiIter++) {
			DAC_MCP4921_Set_mV(uiT[uiIter]);
		}
	}
}
