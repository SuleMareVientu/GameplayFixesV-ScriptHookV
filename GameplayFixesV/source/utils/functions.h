#pragma once
#include <natives.h>
#include <string>

class Timer {
	int gameTimer = 0;
public:
	void Set(int value) { gameTimer = GET_GAME_TIMER() - value; return; };
	void Reset() { gameTimer = GET_GAME_TIMER(); return; };
	int Get() const { return (GET_GAME_TIMER() - gameTimer); }
	Timer(const int startVal = 0) { gameTimer = (startVal * -1); }
};

inline Player GetPlayer() { return PLAYER_ID(); }
inline Ped GetPlayerPed() { return PLAYER_PED_ID(); }
inline Vector3 GetPlayerCoords() { return GET_ENTITY_COORDS(PLAYER_PED_ID(), false); }

void EnablePedConfigFlag(Ped ped, int flag);
void DisablePedConfigFlag(Ped ped, int flag);
void EnablePedResetFlag(Ped ped, int flag);
void DisablePedResetFlag(Ped ped, int flag);

void Print(char* string, int ms = 1);
void PrintInt(int value, int ms = 1);
void PrintFloat(float value, int ms = 1);
void PrintHelp(char* string, bool playSound = false, int overrideDuration = -1);
int ShowNotification(const char* str, bool flash = false);

bool ArrayContains(int value, const int a[], int n);
void SplitString(const char* charStr, std::string arr[], const int arrSize);
bool GetWeightedBool(int chance);
Vehicle GetVehiclePedIsIn(Ped ped, bool includeEntering = true, bool includeExiting = false);
bool HasEntityBeenDamagedByWeaponThisFrame(Ped ped, Hash weaponHash, int weaponType = 0);
bool CanDisarmPed(Ped ped, bool includeLeftHand);

void RefreshIni();
void UpdatePlayerOptions();