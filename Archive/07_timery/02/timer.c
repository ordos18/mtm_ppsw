#include <LPC21xx.H>
#include "timer.h"

#define COUNTER_ENABLE_bm (1 << 0)
#define COUNTER_RESET_bm (1 << 1)
#define INTERRUPT_ON_MR0_bm (1 << 0)
#define RESET_ON_MR0_bm (1 << 1)
#define MR0_INTERRUPT_bm (1 << 0)

void InitTimer0 (void) {
	
	T0TCR = T0TCR | COUNTER_ENABLE_bm; // aktywacja Timera 0 w Control Register
}

void WaitOnTimer0 (unsigned int uiTime) {
	
	T0TCR = T0TCR | COUNTER_RESET_bm; 												// reset Timera 0
	T0TCR = T0TCR & ~COUNTER_RESET_bm; 												// wylaczenie resetu
	while ((15*uiTime) != T0TC) {} 														// Oczekiwanie az Timer 0 osiagnie wartosc 15*uiTime
	return;
}

void InitTimer0Match0 (unsigned int uiDelayTime) {
	
	T0MR0 = 15 * uiDelayTime;																	// Ustawienie Match Register 0 na 15 * uiDelayTime
	T0MCR = T0MCR | (INTERRUPT_ON_MR0_bm | RESET_ON_MR0_bm);	// Aktywacja przerwania i resetu na Match Register 0
	T0TCR = T0TCR | COUNTER_RESET_bm;													// reset Timera 0
	T0TCR = T0TCR & ~COUNTER_RESET_bm; 												// wylaczenie resetu
}

void WaitOnTimer0Match0 (void) {
	
	while (0 == (T0IR & MR0_INTERRUPT_bm)) {}									// sprawdzanie flagi przerwania MR0
	T0IR = MR0_INTERRUPT_bm;																	// wyzerowanie flagi przerwania MR0
}

