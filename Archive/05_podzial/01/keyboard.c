#include <LPC21xx.H>
#include "keyboard.h"

#define BUTTON0_bm 0x00000010
#define BUTTON1_bm 0x00000040
#define BUTTON2_bm 0x00000020
#define BUTTON3_bm 0x00000080

void KeyboardInit (void) {
	
	IO0DIR = IO0DIR & ~(BUTTON0_bm | BUTTON1_bm | BUTTON2_bm | BUTTON3_bm);
}

enum eButtons eKeyboardRead(void) {
	
	if( 0 == (IO0PIN & BUTTON0_bm) ){
		return BUTTON_0;
	} 
	else if( 0 == (IO0PIN & BUTTON1_bm) ){
		return BUTTON_1;
	}	
	else if( 0 == (IO0PIN & BUTTON2_bm) ){
		return BUTTON_2;
	}	
	else if( 0 == (IO0PIN & BUTTON3_bm) ){
		return BUTTON_3;
	}	
	else {
		return RELEASED;
	}
}
