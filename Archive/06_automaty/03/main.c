// v6.3

#include "led.h"
#include "keyboard.h"

enum LedState{STEP_LEFT, STEP_RIGHT};
enum LedState eLedState = STEP_LEFT;

void Delay (int delayMs) {
	
	int iLoopCounter, msLoopCounter;
	for (msLoopCounter = 0; msLoopCounter<delayMs; msLoopCounter++) {
		for (iLoopCounter=0; iLoopCounter<7498; iLoopCounter++) {}
	}
}

int main () {

	unsigned char ucStepCounter = 1;
	
	LedInit();
	KeyboardInit();

	while(1){
		switch( eLedState ) {
			case STEP_RIGHT:
				if(3==ucStepCounter) {
					eLedState = STEP_LEFT;
					ucStepCounter = 0;
				}
				LedStepRight();
				break;
			case STEP_LEFT:
				if(3==ucStepCounter) {
					eLedState = STEP_RIGHT;
					ucStepCounter = 0;
				}
				LedStepLeft();
				break;
			default: {}
		}
		ucStepCounter = ucStepCounter + 1;
		Delay(250);
	}
}
