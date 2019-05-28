#include <LPC21xx.H>
#include "timer.h"

#define COUNTER_ENABLE_bm (1 << 0)
#define COUNTER_RESET_bm (1 << 1)

void InitTimer0 (void) {
	
	T0TCR = T0TCR | COUNTER_ENABLE_bm;
}

void WaitOnTimer0 (unsigned int uiTime) {
	
	T0TCR = T0TCR | COUNTER_RESET_bm;
	T0TCR = T0TCR & ~COUNTER_RESET_bm;
	while ((15*uiTime) > T0TC) {}
	return;
}
