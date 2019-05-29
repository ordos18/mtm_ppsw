#include <LPC21xx.H>
#include "timer_interrupts.h"

void (*ptrTimer0InterruptFunction)(void);
void (*ptrTimer1InterruptFunction)(void);

/**********************************************/
//(Interrupt Service Routine) of Timer 0 interrupt
__irq void Timer0IRQHandler(void){

	T0IR=mMR0_INTERRUPT; 	// skasowanie flagi przerwania 
	ptrTimer0InterruptFunction();		// cos do roboty
	VICVectAddr=0x00; 	// potwierdzenie wykonania procedury obslugi przerwania
}
/**********************************************/
//(Interrupt Service Routine) of Timer 0 interrupt
__irq void Timer1IRQHandler(void){

	T1IR=mMR1_INTERRUPT; 	// skasowanie flagi przerwania 
	ptrTimer1InterruptFunction();		// cos do roboty
	VICVectAddr=0x00; 	// potwierdzenie wykonania procedury obslugi przerwania
}
/**********************************************/
void Timer0Interrupts_Init(unsigned int uiPeriod, void (*ptrInterruptFunction)(void)){ // microseconds
	
	ptrTimer0InterruptFunction = ptrInterruptFunction;
	
        // interrupts

	VICIntEnable |= (0x1 << VIC_TIMER0_CHANNEL_NR);            // Enable Timer 0 interrupt channel 
	VICVectCntl1  = mIRQ_SLOT_ENABLE | VIC_TIMER0_CHANNEL_NR;  // Enable Slot 0 and assign it to Timer 0 interrupt channel
	VICVectAddr1  =(unsigned long)Timer0IRQHandler; 	   // Set to Slot 0 Address of Interrupt Service Routine 

        // match module

	T0MR0 = 15 * uiPeriod;                 	      // value 
	T0MCR |= (mINTERRUPT_ON_MR0 | mRESET_ON_MR0); // action 

        // timer

	T0TCR |=  mCOUNTER_ENABLE; // start 

}

/**********************************************/
void Timer1Interrupts_Init(unsigned int uiPeriod, void (*ptrInterruptFunction)(void)){ // microseconds
	
	ptrTimer1InterruptFunction = ptrInterruptFunction;
	
        // interrupts

	VICIntEnable |= (0x1 << VIC_TIMER1_CHANNEL_NR);            // Enable Timer 1 interrupt channel 
	VICVectCntl13  = mIRQ_SLOT_ENABLE | VIC_TIMER1_CHANNEL_NR;  // Enable Slot 13 and assign it to Timer 1 interrupt channel
	VICVectAddr13  =(unsigned long)Timer1IRQHandler; 	   // Set to Slot 13 Address of Interrupt Service Routine 

        // match module

	T1MR0 = 15 * uiPeriod;                 	      // value 
	T1MCR |= (mINTERRUPT_ON_MR1 | mRESET_ON_MR1); // action 

        // timer

	T1TCR |=  mCOUNTER_ENABLE; // start 

}
