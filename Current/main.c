#include <LPC21xx.H>
#include "keyboard.h"
#include "servo.h"
#include "uart.h"
#include "string.h"
#include "command_decoder.h"
#include "timer.h"
#include "timer_interrupts.h"
#include "adc.h"


/**********************************************/
int main () {
	
	extern char cOdebranyZnak;
	extern Token asToken[];
	extern unsigned char ucTokenNr;
	extern struct Watch sWatch;
	
	char RxString[RECEIVER_SIZE];
	char cStringToSend[TRANSMITER_SIZE];
	char cSecWord[] = "sec ";
	char cMinWord[] = "min ";
	char cCalcWord[] = "calc ";
	unsigned int uiReceivedNumber;
	unsigned char fNumberToCalc = 0;
	
	KeyboardInit();
	ServoInit(50);
	InitTimer0();
	Timer1Interrupts_Init(100000, &ADC_ReadValue);
	//UART_InitWithInt(9600);
	ADC_InitWithInt(&ServoGoTo);

	while (1) {
		/*
		if( eReceiver_GetStatus() == READY ) {
			Receiver_GetStringCopy(RxString);
			DecodeMsg(RxString);
			
			if( (ucTokenNr > 0) && (asToken[0].eType == KEYWORD) ) {
				switch(asToken[0].uValue.eKeyword) {
					case CAL:
						ServoCalib();
						break;
					case GOTO:
						if (ucTokenNr > 1) {
							ServoGoTo(asToken[1].uValue.uiNumber);
						}
						break;
					case CALC:
						if (ucTokenNr > 1) {
							uiReceivedNumber = asToken[1].uValue.uiNumber;
							fNumberToCalc = 1;
						}
						break;
					default: {}
				}
			}
		}
		if (FREE == eTransmitter_GetStatus()) {
			if (1 == sWatch.fSecondsValueChanged) {
				sWatch.fSecondsValueChanged = 0;
				CopyString(cSecWord, cStringToSend);
				AppendUIntToString(sWatch.ucSeconds, cStringToSend);
				Transmitter_SendString(cStringToSend);
			} else if (1 == sWatch.fMinutesValueChanged) {
				sWatch.fMinutesValueChanged = 0;
				CopyString(cMinWord, cStringToSend);
				AppendUIntToString(sWatch.ucMinutes, cStringToSend);
				Transmitter_SendString(cStringToSend);
			} else if (1 == fNumberToCalc) {
				fNumberToCalc = 0;
				CopyString(cCalcWord, cStringToSend);
				AppendUIntToString((uiReceivedNumber << 1), cStringToSend);
				Transmitter_SendString(cStringToSend);
			}
		}
		*/
	}
}
