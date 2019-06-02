#include <LPC21xx.H>
#include "keyboard.h"
#include "servo.h"
#include "uart.h"
#include "string.h"
#include "command_decoder.h"


/**********************************************/
int main () {
	
	extern char cOdebranyZnak;
	extern Token asToken[];
	extern unsigned char ucTokenNr;
	char RxString[RECEIVER_SIZE];
	unsigned int uiLicznik = 0;
	char cLicznikString[15];
	char cLicznikWord[] = "licznik ";
	
	KeyboardInit();
	ServoInit(50);
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
			CopyString(cLicznikWord, cLicznikString);
			AppendUIntToString(uiLicznik, cLicznikString);
			Transmitter_SendString(cLicznikString);
			uiLicznik = uiLicznik + 1;
		}
	}
}
