#include <LPC21xx.H>
#include "keyboard.h"
#include "servo.h"
#include "uart.h"
#include "string.h"
#include "command_decoder.h"
#include "timer.h"
#include "timer_interrupts.h"


/**********************************************/
int main () {
	
	extern char cOdebranyZnak;
	extern Token asToken[];
	extern unsigned char ucTokenNr;
	extern struct Watch sWatch;
	
	char RxString[RECEIVER_SIZE];
	char cStringToSend[15];
	char cSecWord[] = "sec ";
	char cMinWord[] = "min ";
	
	KeyboardInit();
	ServoInit(50);
	InitTimer0();
	Timer0Interrupts_Init(1000000, &WatchUpdate);
	UART_InitWithInt(9600);
	

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
						if (ucTokenNr > 1) {
							ServoGoTo(asToken[1].uValue.uiNumber);
						}
						break;
					default: {}
				}
			}
		}
		if (FREE == eTransmitter_GetStatus()) {
			if (sWatch.fSecondsValueChanged) {
				sWatch.fSecondsValueChanged = 0;
				CopyString(cSecWord, cStringToSend);
				AppendUIntToString(sWatch.ucSeconds, cStringToSend);
				Transmitter_SendString(cStringToSend);
			} else if (sWatch.fMinutesValueChanged) {
				sWatch.fMinutesValueChanged = 0;
				CopyString(cMinWord, cStringToSend);
				AppendUIntToString(sWatch.ucMinutes, cStringToSend);
				Transmitter_SendString(cStringToSend);
			}
		}
	}
}
