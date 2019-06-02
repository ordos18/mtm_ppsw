#include <LPC21xx.H>
#include "timer_interrupts.h"

// TIMER
#define mCOUNTER_ENABLE 0x00000001
#define mCOUNTER_RESET  0x00000002

// CompareMatch
#define mINTERRUPT_ON_MR0 0x00000001
#define mRESET_ON_MR0     0x00000002
#define mMR0_INTERRUPT    0x00000001

// VIC (Vector Interrupt Controller) VICIntEnable
#define VIC_TIMER0_CHANNEL_NR 4

// VICVectCntlx Vector Control Registers
#define mIRQ_SLOT_ENABLE 0x00000020

struct Watch sWatch;

void (*ptrTimer0InterruptFunction)(void);

/**********************************************/
//(Interrupt Service Routine) of Timer 0 interrupt
__irq void Timer0IRQHandler(void){

	T0IR=mMR0_INTERRUPT; 	// skasowanie flagi przerwania 
	ptrTimer0InterruptFunction();		// cos do roboty
	VICVectAddr=0x00; 	// potwierdzenie wykonania procedury obslugi przerwania
}
/**********************************************/
void Timer0Interrupts_Init(unsigned int uiPeriod, void (*ptrInterruptFunction)(void)){ // microseconds
	
	ptrTimer0InterruptFunction = ptrInterruptFunction;
	
        // interrupts

	VICIntEnable |= (0x1 << VIC_TIMER0_CHANNEL_NR);            // Enable Timer 0 interrupt channel 
	VICVectCntl2  = mIRQ_SLOT_ENABLE | VIC_TIMER0_CHANNEL_NR;  // Enable Slot 0 and assign it to Timer 0 interrupt channel
	VICVectAddr2  =(unsigned long)Timer0IRQHandler; 	   // Set to Slot 0 Address of Interrupt Service Routine 

        // match module

	T0MR0 = 15 * uiPeriod;                 	      // value 
	T0MCR |= (mINTERRUPT_ON_MR0 | mRESET_ON_MR0); // action 

        // timer

	T0TCR |=  mCOUNTER_ENABLE; // start 

}

void WatchUpdate (void) {
	sWatch.ucSeconds = sWatch.ucSeconds + 1;
	sWatch.fSecondsValueChanged = 1;
	if (60 == sWatch.ucSeconds) {
		sWatch.ucSeconds = 0;
		sWatch.ucMinutes = (sWatch.ucMinutes + 1) % 60;
		sWatch.fMinutesValueChanged = 1;
	}
}
