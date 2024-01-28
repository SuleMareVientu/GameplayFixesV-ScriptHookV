#include <SimpleIni.h>
#include "ini.h"
#include "../globals.h"

static const char* playerGroup = "Player";
static const char* pedsGroup = "Peds";

//Player Settings
bool iniFriendlyFire = true;
bool iniPlayerCanJackFriendlyPeds = true;
bool iniSprintInsideInteriors = true;
bool iniAllowWeaponsInsideSafeHouse = false;
bool iniDisableCarMidAirAndRollControl = true;
bool iniDisableForcedCarExplosionOnImpact = true;
bool iniLeaveEngineOnWhenExitingVehicles = true;
bool iniDisableWheelsAutoCenterOnCarExit = true;
bool iniDisableRagdollOnVehicleRoof = true;
float iniMaxVehicleSpeed = 90.0f;
bool iniDisableShallowWaterBikeJumpOut = true;

//Peds Settings
bool iniDisableWrithe = true;
bool iniDisableHurt = true;
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
	iniSprintInsideInteriors = ini.GetBoolValue(playerGroup, "SprintInsideInteriors", true);
	iniAllowWeaponsInsideSafeHouse = ini.GetBoolValue(playerGroup, "AllowWeaponsInsideSafeHouse", false);
	iniDisableCarMidAirAndRollControl = ini.GetBoolValue(playerGroup, "DisableCarMidAirAndRollControl", true);
	iniDisableForcedCarExplosionOnImpact = ini.GetBoolValue(playerGroup, "DisableForcedCarExplosionOnImpact", true);
	iniLeaveEngineOnWhenExitingVehicles = ini.GetBoolValue(playerGroup, "LeaveEngineOnWhenExitingVehicles", true);
	iniDisableWheelsAutoCenterOnCarExit = ini.GetBoolValue(playerGroup, "DisableWheelsAutoCenterOnCarExit", true);
	iniDisableRagdollOnVehicleRoof = ini.GetBoolValue(playerGroup, "DisableRagdollOnVehicleRoof", true);
	iniMaxVehicleSpeed = ini.GetDoubleValue(playerGroup, "MaxVehicleSpeed", 90.0f);
	iniDisableShallowWaterBikeJumpOut = ini.GetBoolValue(playerGroup, "DisableShallowWaterBikeJumpOut", true);

	//////////////////////////////////////Peds//////////////////////////////////////

	iniDisableWrithe = ini.GetBoolValue(pedsGroup, "DisableWrithe", true);
	iniDisableHurt = ini.GetBoolValue(pedsGroup, "DisableHurt", true);
	iniDisablePedOnlyDamagedByPlayer = ini.GetBoolValue(pedsGroup, "DisablePedOnlyDamagedByPlayer", true);
	iniDisableSittingPedsInstantDeath = ini.GetBoolValue(pedsGroup, "DisableSittingPedsInstantDeath", true);
	iniDisableDeadPedsJumpOutOfVehicle = ini.GetBoolValue(pedsGroup, "DisableDeadPedsJumpOutOfVehicle", true);
	return;
}