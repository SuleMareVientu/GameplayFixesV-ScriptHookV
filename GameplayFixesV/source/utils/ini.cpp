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
//HUD
bool iniReplaceArmourBarWithStamina = false;
bool iniMergeHealthAndArmour = true;
//Player Controls
bool iniToggleFPSWalking = true;
bool iniCamFollowVehicleDuringHandbrake = false;
int iniCamFollowVehDelay = 200;
bool iniDisableRecording = false;
bool iniDisableMobilePhone = false;
//Player Vehicle
bool iniDisableCarMidAirAndRollControl = true;
bool iniDisableForcedCarExplosionOnImpact = true;
bool iniDisableEngineSmoke = false;
bool iniDisableEngineFire = false;
bool iniLeaveEngineOnWhenExitingVehicles = true;
bool iniDisableWheelsAutoCenterOnCarExit = true;
bool iniDisableRagdollOnVehicleRoof = true;
float iniMaxVehicleSpeed = 90.0f;
bool iniDisableShallowWaterBikeJumpOut = true;
bool iniDisableStuntJumps = false;

//Peds Settings
bool iniDisableWrithe = true;
bool iniDisableHurt = true;
bool iniDisableSittingPedsInstantDeath = true;
bool iniDisarmPedWhenShot = true;
int iniDisarmChance = 50;
bool iniDisarmIncludeLeftHand = false;
bool iniDisablePedOnlyDamagedByPlayer = true;
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
	
	//////////////////////////////////////HUD//////////////////////////////////////////////
	iniReplaceArmourBarWithStamina = ini.GetBoolValue(playerGroup, "ReplaceArmourBarWithStamina", false);
	iniMergeHealthAndArmour = ini.GetBoolValue(playerGroup, "MergeHealthAndArmour", true);

	//////////////////////////////////////Player Controls//////////////////////////////////
	iniToggleFPSWalking = ini.GetBoolValue(playerGroup, "ToggleFPSWalking", true);
	iniCamFollowVehicleDuringHandbrake = ini.GetBoolValue(playerGroup, "CamFollowVehicleDuringHandbrake", false);
	iniCamFollowVehDelay = ini.GetLongValue(playerGroup, "CamFollowVehDelay", 200);
	iniDisableRecording = ini.GetBoolValue(playerGroup, "DisableRecording", false);
	iniDisableMobilePhone = ini.GetBoolValue(playerGroup, "DisableMobilePhone", false);

	//////////////////////////////////////Player Vehicle///////////////////////////////////
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
	iniDisableStuntJumps = ini.GetBoolValue(playerGroup, "DisableStuntJumps", false);

	//////////////////////////////////////Peds/////////////////////////////////////////////
	iniDisableWrithe = ini.GetBoolValue(pedsGroup, "DisableWrithe", true);
	iniDisableHurt = ini.GetBoolValue(pedsGroup, "DisableHurt", true);
	iniDisableSittingPedsInstantDeath = ini.GetBoolValue(pedsGroup, "DisableSittingPedsInstantDeath", true);
	iniDisarmPedWhenShot = ini.GetBoolValue(pedsGroup, "DisarmPedWhenShot", true);
	iniDisarmChance = ini.GetLongValue(playerGroup, "DisarmChance", 50);
	iniDisarmIncludeLeftHand = ini.GetBoolValue(pedsGroup, "DisarmIncludeLeftHand", false);
	iniDisablePedOnlyDamagedByPlayer = ini.GetBoolValue(pedsGroup, "DisablePedOnlyDamagedByPlayer", true);
	iniDisableDeadPedsJumpOutOfVehicle = ini.GetBoolValue(pedsGroup, "DisableDeadPedsJumpOutOfVehicle", true);
	return;
}