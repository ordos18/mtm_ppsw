#include <LPC21xx.H>
#include "keyboard.h"
#include "servo.h"
#include "uart.h"
#include "led.h"


/**********************************************/
int main () {
	
	unsigned int iMainLoopCtr;
	extern char cOdebranyZnak;
	
	KeyboardInit();
	LedInit();
	//ServoInit(50);
	UART_InitWithInt(9600);

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
			case '0':
				LedOn(0);
				break;
			case '1':
				LedOn(1);
				break;
			case '2':
				LedOn(2);
				break;
			case '3':
				LedOn(3);
				break;
			case 'c':
				LedOn(4);
				break;
			default: {}
		}
	}
}
