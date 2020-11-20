#ifndef TIMER_H
#define TIMER_H

#define FREQ_TIMER1 250
#define FREQ_TIMER4 0.5

void InitTimer23(void);
void InitTimer1(void);
void SetFreqTimer1(float freq); // Hz
void InitTimer4(void);
void SetFreqTimer4(float freq); // Hz
extern unsigned long timestamp;

#endif // TIMER_H