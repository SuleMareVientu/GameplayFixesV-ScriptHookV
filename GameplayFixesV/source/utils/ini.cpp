#include <SimpleIni.h>
#include "ini.h"
#include "../globals.h"

static const char* playerGroup = "Player";
static const char* pedsGroup = "Peds";

//Player Settings
bool iniFriendlyFire = true;
bool iniPlayerCanJackFriendlyPeds = true;
bool iniDisarmPlayerWhenShot = true;
bool iniSprintInsideInteriors = true;
bool iniAllowWeaponsInsideSafeHouse = false;
bool iniToggleFPSWalking = true;
bool iniDisableCarMidAirAndRollControl = true;
bool iniDisableForcedCarExplosionOnImpact = true;
bool iniDisableEngineSmoke = false;
bool iniDisableEngineFire = false;
bool iniLeaveEngineOnWhenExitingVehicles = true;
bool iniDisableWheelsAutoCenterOnCarExit = true;
bool iniDisableRagdollOnVehicleRoof = true;
float iniMaxVehicleSpeed = 90.0f;
bool iniDisableShallowWaterBikeJumpOut = true;
bool iniDisableRecording = false;

//Peds Settings
bool iniDisableWrithe = true;
bool iniDisableHurt = true;
bool iniDisarmPedWhenShot = true;
int iniDisarmChance = 35;
bool iniDisarmIncludeLeftHand = false;
bool iniDisablePedOnlyDamagedByPlayer = true;
bool iniDisableSittingPedsInstantDeath = true;
bool iniDisableDeadPedsJumpOutOfVehicle = true;

void ReadINI()
{
	CSimpleIniA ini;
	SI_Error res = ini.LoadFile("GameplayFixesV.ini");

	if (res != SI_OK)
		return;

	//////////////////////////////////////Player//////////////////////////////////////////
	
	iniFriendlyFire = ini.GetBoolValue(playerGroup, "FriendlyFire", true);
	iniPlayerCanJackFriendlyPeds = ini.GetBoolValue(playerGroup, "PlayerCanJackFriendlyPeds", true);
	iniDisarmPlayerWhenShot = ini.GetBoolValue(playerGroup, "DisarmPlayerWhenShot", true);
	iniSprintInsideInteriors = ini.GetBoolValue(playerGroup, "SprintInsideInteriors", true);
	iniAllowWeaponsInsideSafeHouse = ini.GetBoolValue(playerGroup, "AllowWeaponsInsideSafeHouse", false);
	iniToggleFPSWalking = ini.GetBoolValue(playerGroup, "ToggleFPSWalking", true);
	iniDisableCarMidAirAndRollControl = ini.GetBoolValue(playerGroup, "DisableCarMidAirAndRollControl", true);
	iniDisableForcedCarExplosionOnImpact = ini.GetBoolValue(playerGroup, "DisableForcedCarExplosionOnImpact", true);
	iniDisableEngineSmoke = ini.GetBoolValue(playerGroup, "DisableEngineSmoke", false);
	iniDisableEngineFire = ini.GetBoolValue(playerGroup, "DisableEngineFire", false);
	iniLeaveEngineOnWhenExitingVehicles = ini.GetBoolValue(playerGroup, "LeaveEngineOnWhenExitingVehicles", true);
	iniDisableWheelsAutoCenterOnCarExit = ini.GetBoolValue(playerGroup, "DisableWheelsAutoCenterOnCarExit", true);
	iniDisableRagdollOnVehicleRoof = ini.GetBoolValue(playerGroup, "DisableRagdollOnVehicleRoof", true);
	#pragma warning(suppress: 4244)
	iniMaxVehicleSpeed = ini.GetDoubleValue(playerGroup, "MaxVehicleSpeed", 90.0f);
	iniDisableShallowWaterBikeJumpOut = ini.GetBoolValue(playerGroup, "DisableShallowWaterBikeJumpOut", true);
	iniDisableRecording = ini.GetBoolValue(playerGroup, "DisableRecording", false);

	//////////////////////////////////////Peds//////////////////////////////////////

	iniDisableWrithe = ini.GetBoolValue(pedsGroup, "DisableWrithe", true);
	iniDisableHurt = ini.GetBoolValue(pedsGroup, "DisableHurt", true);
	iniDisarmPedWhenShot = ini.GetBoolValue(pedsGroup, "DisarmPedWhenShot", true);
	iniDisarmChance = ini.GetLongValue(playerGroup, "DisarmChance", 35);
	iniDisarmIncludeLeftHand = ini.GetBoolValue(pedsGroup, "DisarmIncludeLeftHand", false);
	iniDisablePedOnlyDamagedByPlayer = ini.GetBoolValue(pedsGroup, "DisablePedOnlyDamagedByPlayer", true);
	iniDisableSittingPedsInstantDeath = ini.GetBoolValue(pedsGroup, "DisableSittingPedsInstantDeath", true);
	iniDisableDeadPedsJumpOutOfVehicle = ini.GetBoolValue(pedsGroup, "DisableDeadPedsJumpOutOfVehicle", true);
	return;
}