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

	while (1) {
		if( eReceiver_GetStatus() == READY ) {
			Receiver_GetStringCopy(RxString);
			DecodeMsg(RxString);
			
			if( (ucTokenNr > 0) && (asToken[0].eType == KEYWORD) ) {
				switch(asToken[0].uValue.eKeyword) {
					case CAL:
						ServoCalib();
						break;
					case GOTO:
						ServoGoTo(asToken[1].uValue.uiNumber);
						break;
					case CALC:
						uiReceivedNumber = asToken[1].uValue.uiNumber;
						fNumberToCalc = 1;
						break;
					case ID:
						fIDToSend = 1;
						break;
					default:
						fUnknownToSend = 1;
				}
			} else {
				fUnknownToSend = 1;
			}
		}
		if (FREE == eTransmitter_GetStatus()) {
			if (1 == sWatch.fSecondsValueChanged) {
				sWatch.fSecondsValueChanged = 0;
				CopyString("sec ", cStringToSend);
				AppendUIntToString(sWatch.ucSeconds, cStringToSend);
				Transmitter_SendString(cStringToSend);
			} else if (1 == sWatch.fMinutesValueChanged) {
				sWatch.fMinutesValueChanged = 0;
				CopyString("min ", cStringToSend);
				AppendUIntToString(sWatch.ucMinutes, cStringToSend);
				Transmitter_SendString(cStringToSend);
			} else if (1 == fNumberToCalc) {
				fNumberToCalc = 0;
				CopyString("calc ", cStringToSend);
				AppendUIntToString((uiReceivedNumber << 1), cStringToSend);
				Transmitter_SendString(cStringToSend);
			} else if (1 == fIDToSend) {
				fIDToSend = 0;
				CopyString("ID: MTM PPSW", cStringToSend);
				Transmitter_SendString(cStringToSend);
			} else if (1 == fUnknownToSend) {
				fUnknownToSend = 0;
				CopyString("nieznana komenda", cStringToSend);
				Transmitter_SendString(cStringToSend);
			}
		}
	}
}
