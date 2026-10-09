#pragma once
#include <shv\natives.h>
#include <globals.h>
#include <script.h>
#include "utils\nm.h"

// Random
#include "libs\random.hpp"
using Random = effolkronium::random_static;

class Timer {
	int gameTimer = 0;
public:
	void Set(int value) {
		if (value <= -1000000000) gameTimer = GET_GAME_TIMER() + 1000000000;
		else if (value >= 1000000000) gameTimer = GET_GAME_TIMER() - 1000000000;
		else gameTimer = GET_GAME_TIMER() - value;
	}
	void Reset() { gameTimer = GET_GAME_TIMER(); }
	int Get() const { return (GET_GAME_TIMER() - gameTimer); }
	Timer(const int startVal = 0) { gameTimer = (startVal * -1); }
};

inline Player GetPlayer() { return PLAYER_ID(); }
inline Ped GetPlayerPed() { return PLAYER_PED_ID(); }
inline Vector3 GetPlayerCoords() { return GET_ENTITY_COORDS(PLAYER_PED_ID(), false); }

#pragma region Generic
inline int Abs(const int n) { return n * ((n > 0) - (n < 0)); }
inline float Abs(const float n) { return n * ((n > 0.0f) - (n < 0.0f)); }

template <typename T>
inline bool ArrayContains(const T value, const T a[], const T n)
{
	unsigned int i = 0;
	while (i < n && a[i] != value) ++i;
	return i == n ? false : true;
}

template <typename T>
inline bool Between(const T val, const T min, const T max)
{
	if (val >= min && val <= max)
		return true;

	return false;
}

template <typename T>
inline bool BetweenExclude(const T val, const T min, const T max)
{
	if (val > min && val < max)
		return true;

	return false;
}
template <typename T>
inline void InvertIfGreater(T& min, T& max)
{
	if (min > max)
	{
		const T tmpMin = min;
		min = max;
		max = tmpMin;
	}
	return;
}

template <typename T>
inline void InvertIfGreater(T& min, T& max, T clampMin, T clampMax)
{
	if (min > max)
	{
		const T tmpMin = min;
		min = max;
		max = tmpMin;
	}

	if (min < clampMin)
		min = clampMin;
	else if (min > clampMax)
		min = clampMax;

	if (max < clampMin)
		max = clampMin;
	else if (max > clampMax)
		max = clampMax;

	return;
}

std::filesystem::path AbsoluteModulePath(HINSTANCE module);
void SplitString(const char* charStr, std::string arr[], const int arrSize, const bool toUpper = false);

// Range is inclusive
template <typename T>
inline int GetRandomNumberInRange(const T min, const T max) {  return Random::get(min, max); }

// Chance out of 100
inline bool GetWeightedBool(int chance) 
{
	if (chance <= 0)
		return false;
	else if (chance >= 100)
		return true;

	return Random::get<bool>(static_cast<float>(chance) * 0.01f); 
}
// int GetRandomIntInRange(int minValue = 0, int maxValue = 65535, bool useRd = false); // 0-65535 is the max range for natives, useRd has no such limit
// bool GetWeightedBool(int chance, bool useRd = false);
// Vector3 Normalize(Vector3 v);

int GetPadControlFromString(const std::string& str);
int GetVKFromString(const std::string& str);
#pragma endregion

#pragma region Log
void ClearLog();
void RawLog(const std::string& szInfo, const std::string& szData);
void WriteLog(const char* szInfo, const char* szFormat, ...);
#pragma endregion

#pragma region JSON
std::string LoadJSONResource(HINSTANCE hInstance, const char* resource);
bool LoadWeaponJson();
#pragma endregion

#pragma region Print
void Print(char* string, int ms = 0);
void Print(const std::string& string, int ms = 0);
void Print(const int value, int ms = 0);
void Print(const float value, int ms = 0);
void PrintHelp(char* string, bool playSound = false, int overrideDuration = -1);
int ShowNotification(const char* str, bool flash = false);
#pragma endregion

#pragma region Assets Request
bool RequestModel(Hash model);
bool RequestAnimDict(char* animDict);
bool RequestClipSet(char* animDict);
//bool RequestScaleform(const char* name, int* handle);
Object CreateObject(Hash model, float locX = 0.0f, float locY = 0.0f, float locZ = 0.0f, float rotX = 0.0f, float rotY = 0.0f, float rotZ = 0.0f);
void DeleteEntity(Entity* obj);
#pragma endregion

#pragma region Ped Flags
void EnablePedConfigFlag(Ped ped, int flag);
void DisablePedConfigFlag(Ped ped, int flag);
void EnablePedResetFlag(Ped ped, int flag);
void DisablePedResetFlag(Ped ped, int flag);
#pragma endregion

#pragma region Weapons
void ClearEntityLastDamageEntity(Entity entity);
void ClearPedLastDamageBone(Ped ped);
void ClearEntityLastWeaponDamage(Entity entity);
void ClearLastDamages();

bool HasEntityBeenDamagedByWeaponThisFrame(Ped ped, Hash weaponHash, int weaponType = 0);
//bool HasEntityBeenDamagedByAnyPedThisFrame(Ped ped);
bool DoesPedWeaponHaveComponentType(const Ped ped, const Hash weaponHash, const Hash attachPart, const bool findAll = true);
bool CanDisarmPed(Ped ped, bool includeLeftHand);
int GetWeaponBlipSprite(const Hash weaponHash);
bool ShouldWeaponSpawnPickupWhenDropped(const Hash weaponHash, const bool checkWeaponType);
void DropPlayerWeapon(Hash weaponHash, const bool shouldCurse, Vector3 wpRot = { 0.0f, 0.0f, 0.0f });
void RestorePlayerRetrievedWeapon(bool autoEquip);

void TaskNMShot(Ped ped, Hash wpHash, int partIndex, Vector3 hitLoc, Vector3 impulseNorm, bool isAiming = false, bool isCrouched = false);
void TaskNMElectrocute(Ped ped);

Hash GetPickupTypeFromWeaponModel(const Hash wpModel);
#pragma endregion

#pragma region Vehicle
inline Vehicle GetVehiclePedIsUsing(const Ped ped) { return GET_VEHICLE_PED_IS_USING(ped); }
Vehicle GetVehiclePedIsIn(const Ped ped, const bool includeEntering = true, const bool includeExiting = false);
Vehicle GetVehiclePedIsEntering(const Ped ped);
Vehicle GetVehiclePedIsExiting(const Ped ped);
Vehicle GetVehiclePedIsEnteringOrExiting(const Ped ped);
bool DoesVehicleHaveAbility(const Vehicle veh);
#pragma endregion

#pragma region HUD
int RequestMinimapScaleform();
void SetTextStyle(TextStyle Style = defaultTextStyle, bool bDrawBeforeFade = false);
int GetHudComponentFromString(const char* str);
void SetHealthHudDisplayValues(int healthPercentage, int armourPercentage, bool showDamage = true);
#pragma endregion

#pragma region Misc
void PlayScriptedAnim(
	const Ped ped,
	const char* dictionary0 = "",
	const char* anim0 = "",
	const float phase0 = 0.0f,
	const float rate0 = 1.0f,
	const float weight0 = 1.0f,

	const int type = APT_EMPTY,
	const int filter = 0,
	const float blendInDelta = NORMAL_BLEND_DURATION,
	const float blendOutDelta = NORMAL_BLEND_DURATION,
	const int timeToPlay = -1,
	const int flags = AF_DEFAULT,
	const int ikFlags = AIK_NONE);
bool IsPedMainProtagonist(const Ped ped);
bool IsPedMissionOrCompanion(const Ped ped);
bool IsPedACop(const Ped ped);
bool IsFirstPersonActive();
bool IsPlayerAiming(bool includeAimGunTask, bool includeShooting);
bool IsPlayerInsideSafehouse();
void SetDispatchServices(bool toggle);
// bool GetFakeWanted();
// void SetFakeWanted(Player player, bool toggle);
inline int GetNumberOfScriptInstances(const char* name) { return (GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH(Joaat(name))); }
Hash GetCharacterStatHash(const char* statName);

enum eDamageZone {
	DZ_HEAD = 0,
	DZ_UPPER_BODY = 1,
	DZ_LOWER_BODY = 2,
	DZ_ARMOR = 3
};

int GetGeneralDamageFromBoneTag(const int boneTag);
int GetNMPartIndexFromBoneTag(const int boneTag);
int GetBoneTagFromNMPartIndex(const int partIndex);
#pragma endregion

void UpdatePlayerVars();
void ResetWeaponDrops();
void ResetLastDamages();
extern bool isPlayerInsideSafehouse;
extern bool isPlayerArmed;
void ResetSafehouseState();