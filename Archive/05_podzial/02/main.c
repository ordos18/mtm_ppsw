#include "led.h"
#include "keyboard.h"

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
		switch( eKeyboardRead() ) {
			case BUTTON_1:
				LedStepRight();
				break;
			case BUTTON_2:
				LedStepLeft();
				break;
			default: {}
		}
		Delay(250);
	}
}
