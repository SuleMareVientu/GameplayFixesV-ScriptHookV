#pragma once
#include <natives.h>

class Timer {
	int gameTimer = 0;
public:
	void Set(int value) { gameTimer = GET_GAME_TIMER() + value; return; };
	int Get() { return (GET_GAME_TIMER() - gameTimer); }
};

void EnablePedConfigFlag(Ped ped, int flag);
void DisablePedConfigFlag(Ped ped, int flag);
void EnablePedResetFlag(Ped ped, int flag);
void DisablePedResetFlag(Ped ped, int flag);

bool ArrayContains(int value, const int a[], int n);
bool GetWeightedBool(int chance, int startRange = 0, int endRange = 100);
Vehicle GetVehiclePedIsIn(Ped ped, bool includeEntering = true);
bool CanDisarmPed(Ped ped, bool includeLeftHand);

void SetPlayerFlags();