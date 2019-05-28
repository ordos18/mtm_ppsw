// v6.4

#include "led.h"
#include "keyboard.h"

enum LedState{STEP_RIGHT, IDLE};
enum LedState eLedState = IDLE;

void Delay (int delayMs) {
	
	int iLoopCounter, msLoopCounter;
	for (msLoopCounter = 0; msLoopCounter<delayMs; msLoopCounter++) {
		for (iLoopCounter=0; iLoopCounter<7498; iLoopCounter++) {}
	}
}

int main () {

	unsigned char ucStepCounter = 0;
	
	LedInit();
	KeyboardInit();

	while(1){
		switch( eLedState ) {
			case STEP_RIGHT:
				if( 3 == ucStepCounter ) {
					eLedState = IDLE;
				} else {
					eLedState = STEP_RIGHT;
					ucStepCounter = ucStepCounter + 1;
					LedStepRight();
				}
				break;
			case IDLE:
				if( eKeyboardRead() == BUTTON_0 ) {
					eLedState = STEP_RIGHT;
					ucStepCounter = 0;
				}
				break;
			default: {}
		}
		Delay(250);
	}
}
