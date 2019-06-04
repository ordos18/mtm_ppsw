#include <LPC21xx.H>
#include "adc.h"

// PINSEL1 Pin Connect Block
#define mADC_AIN0								   (1 << 22)
#define VIC_ADC_CHANNEL_NR  18
#define mIRQ_SLOT_ENABLE                           0x00000020
#define ADC_ENABLE_bm	(1 << 21)
#define ADC_START_bm	(1 << 24)
#define ADC_DONE_bm		(1 << 31)
#define ADC_LSB 		 6


void (*ptrADCInterruptFunction)(unsigned int uiPosition);

///////////////////////////////////////////
__irq void ADC_Interrupt (void) {
	// jesli przerwanie z ADC
	unsigned int uiValueADC;
	static unsigned int uiADC_min = 0x1FF, uiADC_max = 0x1FF;
	
	uiValueADC = (ADDR >> ADC_LSB) & 0x3FF;
	
	// ADC min/max calibrate
	if (uiValueADC < uiADC_min) {
		uiADC_min = uiValueADC;
	}
	if (uiValueADC > uiADC_max) {
		uiADC_max = uiValueADC;
	}
	
	ptrADCInterruptFunction( (uiValueADC-uiADC_min) * 48 / (uiADC_max - uiADC_min) );
	
	
	VICVectAddr = 0; // Acknowledge Interrupt
}

////////////////////////////////////////////
void ADC_InitWithInt (void (*ptrInterruptFunction)(unsigned int uiPosition)) {

	ptrADCInterruptFunction = ptrInterruptFunction;
	
	// ADC
	PINSEL1 = PINSEL1 | mADC_AIN0;								// ustawic pin na ADC
	ADCR = ADCR | ADC_ENABLE_bm;
	
	// INT
	VICVectAddr7  = (unsigned long) ADC_Interrupt;
	VICVectCntl7  = mIRQ_SLOT_ENABLE | VIC_ADC_CHANNEL_NR;
	VICIntEnable |= (0x1 << VIC_ADC_CHANNEL_NR);
}

void ADC_ReadValue (void) {
	
	ADCR = ADCR | ADC_START_bm;
}
