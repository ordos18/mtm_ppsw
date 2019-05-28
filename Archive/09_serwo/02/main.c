#include <LPC21xx.H>
#include "timer_interrupts.h"
#include "led.h"
#include "keyboard.h"

#define DETECTOR_bm (1 << 10)

enum ServoState {CALLIB, IDLE, IN_PROGRESS};
enum DetectorState {ACTIVE, INACTIVE};

struct Servo {
	
	enum ServoState eState;
	unsigned int uiCurrentPosition;
	unsigned int uiDesiredPosition;
};

struct Servo sServo = {CALLIB, 0, 0};

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
	
	switch( sServo.eState ) {
		case IDLE:
			if( sServo.uiCurrentPosition != sServo.uiDesiredPosition ) {
				sServo.eState = IN_PROGRESS;
			} 
			break;
		case CALLIB:
			if( 0 == eReadDetector() ) {
				sServo.eState = IDLE;
				sServo.uiCurrentPosition = 0;
				sServo.uiDesiredPosition = 0;
			} else {
				LedStepLeft();
			}
			break;
		case IN_PROGRESS:
			if( sServo.uiCurrentPosition < sServo.uiDesiredPosition ) {
				LedStepRight();
				sServo.uiCurrentPosition++;
			} 
			else if ( sServo.uiCurrentPosition > sServo.uiDesiredPosition ) {
				LedStepLeft();
				sServo.uiCurrentPosition--;
			}
			else {
				sServo.eState = IDLE;
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
	 	switch( eKeyboardRead() ) {
			case BUTTON_0:
				sServo.eState = CALLIB;
				break;
			case BUTTON_1:
				sServo.uiDesiredPosition = 12;
				break;
			case BUTTON_2:
				sServo.uiDesiredPosition = 24;
				break;
			case BUTTON_3:
				sServo.uiDesiredPosition = 36;
				break;
			case RELEASED: {}
			default: {}
		}
	}
}
