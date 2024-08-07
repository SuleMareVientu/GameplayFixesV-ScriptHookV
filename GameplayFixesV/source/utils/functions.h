#pragma once
#include <natives.h>

class Timer {
	int gameTimer = 0;
public:
	void Set(int value) { gameTimer = GET_GAME_TIMER() + value; return; };
	int Get() const { return (GET_GAME_TIMER() - gameTimer); }
};

void EnablePedConfigFlag(Ped ped, int flag);
void DisablePedConfigFlag(Ped ped, int flag);
void EnablePedResetFlag(Ped ped, int flag);
void DisablePedResetFlag(Ped ped, int flag);

void Print(char* string, int ms = 1);
void PrintInt(int value, int ms = 1);
void PrintFloat(float value, int ms = 1);
void PrintHelp(char* string, bool playSound = false, int overrideDuration = -1);

bool ArrayContains(int value, const int a[], int n);
bool GetWeightedBool(int chance);
Vehicle GetVehiclePedIsIn(Ped ped, bool includeEntering = true);
bool HasEntityBeenDamagedByWeaponThisFrame(Ped ped, Hash weaponHash, int weaponType = 0);
bool CanDisarmPed(Ped ped, bool includeLeftHand);

void SetPlayerFlags();