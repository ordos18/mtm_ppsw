#include <LPC21xx.H>
#include "timer.h"

#define COUNTER_ENABLE_bm (1 << 0)
#define COUNTER_RESET_bm (1 << 1)
#define INTERRUPT_ON_MR0_bm (1 << 0)
#define RESET_ON_MR0_bm (1 << 1)
#define MR0_INTERRUPT_bm (1 << 0)

void InitTimer0 (void) {
	
	T0TCR = T0TCR | COUNTER_ENABLE_bm;
}

void WaitOnTimer0 (unsigned int uiTime) {
	
	T0TCR = T0TCR | COUNTER_RESET_bm;
	T0TCR = T0TCR & ~COUNTER_RESET_bm;
	while ((15*uiTime) > T0TC) {}
	return;
}

void InitTimer0Match0 (unsigned int uiDelayTime) {
	
	T0MR0 = 15 * uiDelayTime;
	T0MCR = T0MCR | (INTERRUPT_ON_MR0_bm | RESET_ON_MR0_bm);
	T0TCR = T0TCR | COUNTER_RESET_bm;
	T0TCR = T0TCR & ~COUNTER_RESET_bm;
}

void WaitOnTimer0Match0 (void) {
	
	while (0 == (T0IR & MR0_INTERRUPT_bm)) {}
	T0IR = T0IR | MR0_INTERRUPT_bm;
}

