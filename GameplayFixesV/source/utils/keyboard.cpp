#include "keyboard.h"

constexpr int KEYS_SIZE = 255;

struct {
	ULONGLONG time;
	BOOL isWithAlt;
	BOOL wasDownBefore;
	BOOL isUpNow;
} keyStates[KEYS_SIZE];

void OnKeyboardMessage(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow)
{
	if (key >= 1 && key < KEYS_SIZE)
	{
		keyStates[key].time = GetTickCount64();
		keyStates[key].isWithAlt = isWithAlt;
		keyStates[key].wasDownBefore = wasDownBefore;
		keyStates[key].isUpNow = isUpNow;
	}
}

constexpr int NOW_PERIOD = 100, MAX_DOWN = 5000; // ms

bool IsKeyDown(DWORD key)
{
	return (key >= 1 && key < KEYS_SIZE) ? ((GetTickCount64() < keyStates[key].time + MAX_DOWN) && !keyStates[key].isUpNow) : false;
}

bool IsKeyJustUp(DWORD key, bool exclusive)
{
	bool b = (key >= 1 && key < KEYS_SIZE) ? (GetTickCount64() < keyStates[key].time + NOW_PERIOD && keyStates[key].isUpNow) : false;
	if (b && exclusive)
		ResetKeyState(key);
	return b;
}

void ResetKeyState(DWORD key)
{
	if (key >= 1 && key < KEYS_SIZE)
		memset(&keyStates[key], 0, sizeof(keyStates[0]));
}