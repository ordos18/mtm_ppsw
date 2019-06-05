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
	char cStringToSend[TRANSMITER_SIZE];
	char cSecWord[] = "\nsec ";
	char cMinWord[] = "\nmin ";
	char cCalcWord[] = "\ncalc ";
	char cID[] = "\nID: MTM PPSW";
	char cUnknownCommand[] = "\nnieznana komenda";
	unsigned int uiReceivedNumber;
	unsigned char fNumberToCalc = 0;
	unsigned char fIDToSend = 0;
	unsigned char fUnknownToSend = 0;
	
	KeyboardInit();
	ServoInit(50);
	Timer1Interrupts_Init(1000000, &WatchUpdate);
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
			} else if (1 == fIDToSend) {
				fIDToSend = 0;
				CopyString(cID, cStringToSend);
				Transmitter_SendString(cStringToSend);
			} else if (1 == fUnknownToSend) {
				fUnknownToSend = 0;
				CopyString(cUnknownCommand, cStringToSend);
				Transmitter_SendString(cStringToSend);
			}
		}
	}
}
