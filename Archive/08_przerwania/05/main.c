
#include "timer_interrupts.h"
#include "led.h"
#include "keyboard.h"

enum LedState{STEP_RIGHT, STEP_LEFT, IDLE};
enum LedState eLedState = IDLE;

void Automat(void){
	
	switch( eLedState ) {
		case STEP_RIGHT:
			if( eKeyboardRead() == BUTTON_1 ) {
				eLedState = IDLE;
			} else {
				eLedState = STEP_RIGHT;
				LedStepRight();
			}
			break;
		case STEP_LEFT:
			if( eKeyboardRead() == BUTTON_1 ) {
				eLedState = IDLE;
			} else {
				eLedState = STEP_LEFT;
				LedStepLeft();
			}
			break;
		case IDLE:
			if( eKeyboardRead() == BUTTON_0 ) {
				eLedState = STEP_LEFT;
			} else if( eKeyboardRead() == BUTTON_2 ) {
				eLedState = STEP_RIGHT;
			}
			break;
		default: {}
	}
}


/**********************************************/
int main (){
	
	unsigned int iMainLoopCtr;
	
	LedInit();
	KeyboardInit();
	Timer1Interrupts_Init(250000, &Automat);

	while(1){
	 	iMainLoopCtr++;
	}
}
