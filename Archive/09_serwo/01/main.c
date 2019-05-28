#include <LPC21xx.H>
#include "timer_interrupts.h"
#include "led.h"
#include "keyboard.h"

#define DETECTOR_bm (1 << 10)

enum LedState {STEP_RIGHT, STEP_LEFT, IDLE, CALLIB};
enum LedState eLedState = CALLIB;
enum DetectorState {ACTIVE, INACTIVE};

void DetectorInit (void) {
	
	IO0DIR = IO0DIR & ~DETECTOR_bm;
}

enum DetectorState eReadDetector (void) {
	
	if( 0 == (IO0PIN & DETECTOR_bm) ){
		return ACTIVE;
	}
	else {
		return INACTIVE;
	}
}

void Automat (void) {
	
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
		case CALLIB:
			if( 0 == eReadDetector() ) {
				eLedState = IDLE;
			} else {
				LedStepLeft();
			}
			break;
		default: {}
	}
}


/**********************************************/
int main () {
	
	unsigned int iMainLoopCtr;
	
	LedInit();
	KeyboardInit();
	Timer0Interrupts_Init(20000, &Automat);

	while(1){
	 	iMainLoopCtr++;
	}
}
