
struct Watch {
	unsigned char ucMinutes,ucSeconds;
	unsigned char fSecondsValueChanged, fMinutesValueChanged;
};

void Timer0Interrupts_Init(unsigned int uiPeriod, void (*ptrInterruptFunction)(void));
void Timer1Interrupts_Init(unsigned int uiPeriod, void (*ptrInterruptFunction)(void));
void WatchUpdate (void);
