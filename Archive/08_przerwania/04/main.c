
#include "timer_interrupts.h"
#include "led.h"


/**********************************************/
int main (){
	
	unsigned int iMainLoopCtr;
	
	LedInit();
	Timer0Interrupts_Init(250000, &LedStepRight);

	while(1){
	 	iMainLoopCtr++;
	}
}
