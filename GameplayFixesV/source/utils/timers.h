#pragma once

class Timer {
	int gameTimer = 0;
public:
	void Set(int value);
	int Get();
};

extern Timer TimerA;
extern Timer TimerB;
extern Timer TimerC;
extern Timer TimerD;
extern Timer TimerE;
extern Timer TimerF;
//extern Timer TimerG;