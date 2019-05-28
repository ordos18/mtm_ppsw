// v6.2

#include "led.h"
#include "keyboard.h"

enum LedState{STATE0, STATE1, STATE2, STATE3, STATE4, STATE5};
enum LedState eLedState = STATE0;

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
			case STATE0:
				eLedState = STATE1;
				LedStepRight();
				break;
			case STATE1:
				eLedState = STATE2;
				LedStepRight();
				break;
			case STATE2:
				eLedState = STATE3;
				LedStepRight();
				break;
			case STATE3:
				eLedState = STATE4;
				LedStepLeft();
				break;
			case STATE4:
				eLedState = STATE5;
				LedStepLeft();
				break;
			case STATE5:
				eLedState = STATE0;
				LedStepLeft();
				break;
			default: {}
		}
		Delay(250);
	}
}
