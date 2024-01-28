//ScriptHook
#include <natives.h>
//Custom
#include "timers.h"

//We could define a class here... but we also don't want to make it harder to convert to SC-CL
static int timerC = 0;
static int gameTimerC = 0;
static int timerD = 0;
static int gameTimerD = 0;
static int timerE = 0;
static int gameTimerE = 0;

void UpdateTimers()
{
	timerC = GET_GAME_TIMER() - gameTimerC;
	timerD = GET_GAME_TIMER() - gameTimerD;
	timerE = GET_GAME_TIMER() - gameTimerE;
	return;
}

void SETTIMERC(int value)
{
	gameTimerC = GET_GAME_TIMER() + value;
	UpdateTimers();
	return;
}

int TIMERC()
{
	return timerC;
}

void SETTIMERD(int value)
{
	gameTimerD = GET_GAME_TIMER() + value;
	UpdateTimers();
	return;
}

int TIMERD()
{
	return timerD;
}

void SETTIMERE(int value)
{
	gameTimerE = GET_GAME_TIMER() + value;
	UpdateTimers();
	return;
}

int TIMERE()
{
	return timerE;
}