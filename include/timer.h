#ifndef __TIMER_H__
#define __TIMER_H__

void timerInit(int valueMilliSec);
void timerDestroy();
void timerUpdateTimer(int valueMilliSec);
int timerTimeOver();
void timerPrint();

#endif
