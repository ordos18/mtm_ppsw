#include <LPC21xx.H>
#include "keyboard.h"
#include "servo.h"
#include "uart.h"


/**********************************************/
int main () {
	
	unsigned int iMainLoopCtr;
	extern char cOdebranyZnak;
	
	KeyboardInit();
	ServoInit(50);
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
			case 'c':
				ServoCalib();
				break;
			case '1':
				ServoGoTo(50);
				break;
			case '2':
				ServoGoTo(100);
				break;
			case '3':
				ServoGoTo(150);
				break;
			default: {}
		}
	}
}
