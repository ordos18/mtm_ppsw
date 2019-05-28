// v6.6

#include "led.h"
#include "keyboard.h"

enum LedState{STEP_RIGHT, STEP_LEFT, IDLE};
enum LedState eLedState = IDLE;

void Delay (int delayMs) {
	
	int iLoopCounter, msLoopCounter;
	for (msLoopCounter = 0; msLoopCounter<delayMs; msLoopCounter++) {
		for (iLoopCounter=0; iLoopCounter<7498; iLoopCounter++) {}
	}
}

int main () {
	
	LedInit();
	KeyboardInit();

	while(1){
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
		Delay(100);
	}
}
