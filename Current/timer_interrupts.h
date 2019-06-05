
struct Watch {
	unsigned char ucMinutes,ucSeconds;
	unsigned char fSecondsValueChanged, fMinutesValueChanged;
};

void Timer0Interrupts_Init(unsigned int uiPeriod, void (*ptrInterruptFunction1)(void), void (*ptrInterruptFunction2)(void));
void Timer1Interrupts_Init(unsigned int uiPeriod, void (*ptrInterruptFunction1)(void));
void WatchUpdate (void);
