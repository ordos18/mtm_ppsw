#include <LPC21xx.H>
#include "keyboard.h"
#include "servo.h"
#include "uart.h"

// test git

/**********************************************/
int main () {
	
	unsigned int iMainLoopCtr;
	extern char cOdebranyZnak;
	char RxString[RECEIVER_SIZE];
	
	KeyboardInit();
	ServoInit(50);
	UART_InitWithInt(9600);
	/*
	while(1){
	 	switch( eKeyboardRead() ) {
			case BUTTON_0:
				ServoCalib();
				break;
			case BUTTON_1:
				ServoGoTo(12);
				break;
			case BUTTON_2:
				ServoGoTo(24);
				break;
			case BUTTON_3:
				ServoGoTo(36);
				break;
			case RELEASED: {}
			default: {}
		}
		
		switch( cOdebranyZnak ) {
			case 'c':
				cOdebranyZnak = '\0';
				ServoCalib();
				break;
			case '1':
				cOdebranyZnak = '\0';
				ServoMoveDegrees(90);
				break;
			default: {}
		}
	}*/
	while (1) {
		if( eReceiver_GetStatus() == READY ) {
			Receiver_GetStringCopy(RxString);
			
			if( eCompareString(RxString, "callib") == EQUAL ) {
				ServoCalib();
			} else if( eCompareString(RxString, "left") == EQUAL ) {
				ServoGoTo(50);
			} else if( eCompareString(RxString, "right") == EQUAL ) {
				ServoGoTo(150);
			}
		}
}
}
