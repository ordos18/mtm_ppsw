#include <LPC21xx.H>
#include "adc.h"

// PINSEL1 Pin Connect Block
#define mADC_AIN0								   (1 << 22)
#define VIC_ADC_CHANNEL_NR  18
#define mIRQ_SLOT_ENABLE                           0x00000020
#define ADC_ENABLE_bm	(1 << 21)
#define ADC_START_bm	(1 << 24)
#define ADC_DONE_bm		(1 << 31)
#define ADC_LSB_bm 		6

///////////////////////////////////////////
__irq void ADC_Interrupt (void) {
	// jesli przerwanie z ADC
	
	
	
	VICVectAddr = 0; // Acknowledge Interrupt
}

////////////////////////////////////////////
void ADC_InitWithInt(unsigned int uiBaudRate){

	// ADC
	PINSEL1 = PINSEL1 | mADC_AIN0;								// ustawic pina na odbiornik uart0
	ADCR = ADCR | ADC_ENABLE_bm;
	
	// INT
	VICVectAddr7  = (unsigned long) ADC_Interrupt;
	VICVectCntl7  = mIRQ_SLOT_ENABLE | VIC_ADC_CHANNEL_NR;
	VICIntEnable |= (0x1 << VIC_ADC_CHANNEL_NR);
	/*
   U0LCR  |= m8BIT_UART_WORD_LENGTH | mDIVISOR_LATCH_ACCES_BIT; // dlugosc slowa, DLAB = 1
   U0DLL   = ((15000000)/16)/uiBaudRate;                        // predkosc transmisji
   U0LCR  &= (~mDIVISOR_LATCH_ACCES_BIT);                       // DLAB = 0
   U0IER  |= mRX_DATA_AVALIABLE_INTERRUPT_ENABLE;               // aktywacja przerwan po odebraniu danych (RDA)
   U0IER  |= mTHRE_INTERRUPT_PENDING;            			    // aktywacja przerwan po wyslaniu danych (THRE)

   // INT
   VICVectAddr1  = (unsigned long) UART0_Interrupt;             // set interrupt service routine address
   VICVectCntl1  = mIRQ_SLOT_ENABLE | VIC_UART0_CHANNEL_NR;     // use it for UART 0 Interrupt
   VICIntEnable |= (0x1 << VIC_UART0_CHANNEL_NR);               // Enable UART 0 Interrupt Channel
	*/
}
