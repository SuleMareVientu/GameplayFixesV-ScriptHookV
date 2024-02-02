//ScriptHook
#include <natives.h>
//Custom
#include "timers.h"

//Custom implementation of TIMERA and TIMERB natives
void Timer::Set(int value)
{
	gameTimer = GET_GAME_TIMER() + value;
	return;
}

int Timer::Get()
{
	return (GET_GAME_TIMER() - gameTimer);
}

Timer TimerA;
Timer TimerB;
Timer TimerC;
Timer TimerD;
Timer TimerE;
Timer TimerF;
//Timer TimerG;