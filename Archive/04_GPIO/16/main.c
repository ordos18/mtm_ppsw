#include <LPC21xx.H>

#define LED0_bm 0x00010000
#define LED1_bm 0x00020000
#define LED2_bm 0x00040000
#define LED3_bm 0x00080000

#define BUTTON0_bm 0x00000010
#define BUTTON1_bm 0x00000040
#define BUTTON2_bm 0x00000020
#define BUTTON3_bm 0x00000080

void Delay (int delayMs) {
	
	int iLoopCounter, msLoopCounter;
	for (msLoopCounter = 0; msLoopCounter<delayMs; msLoopCounter++) {
		for (iLoopCounter=0; iLoopCounter<7498; iLoopCounter++) {}
	}
}

void LedOn (unsigned char ucLedIndeks) {
	
	IO1CLR = IO1CLR | (LED0_bm | LED1_bm | LED2_bm | LED3_bm);
	switch(ucLedIndeks) {
		case 0:
			IO1SET = IO1SET | LED0_bm;
			break;
		case 1:
			IO1SET = IO1SET | LED1_bm;
			break;
		case 2:
			IO1SET = IO1SET | LED2_bm;
			break;
		case 3:
			IO1SET = IO1SET | LED3_bm;
			break;
	}
}

void LedInit (void) {
	
	IO1DIR = IO1DIR | (LED0_bm | LED1_bm | LED2_bm | LED3_bm);
	LedOn(0);
}

char ReadButton1(void) {
	
	if( 0 == (IO0PIN & BUTTON0_bm) ){
		return 1;
	}
	else {
		return 0;
	}
}

int main () {
	
	LedInit();

	while(1){
		LedOn( ReadButton1() );
	}
}
