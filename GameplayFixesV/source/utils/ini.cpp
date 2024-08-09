#include <SimpleIni.h>
#include "ini.h"
#include "../globals.h"

static constexpr char* playerGroup = "Player";
static constexpr char* HUDGroup = "HUD";
static constexpr char* AudioGroup = "Audio";
static constexpr char* pedsGroup = "Peds";

//Player Settings
bool iniFriendlyFire = true;
bool iniDisableActionMode = false;
bool iniDisarmPlayerWhenShot = true;
bool iniSprintInsideInteriors = true;
bool iniAllowWeaponsInsideSafeHouse = false;
//Player Controls
bool iniToggleFPSWalking = true;
bool iniCamFollowVehicleDuringHandbrake = false;
int iniCamFollowVehDelay = 250;
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
bool iniDisableFlyThroughWindscreen = false;
bool iniDisableBikeKnockOff = false;
bool iniDisableDragOutCar = false;
bool iniDisableShallowWaterBikeJumpOut = true;
bool iniDisableStuntJumps = false;
//HUD
bool iniHideMinimapSatNav = false;
bool iniHideMinimapBars = false;
bool iniHideAbilityBarForNonMainCharacters = true;
bool iniAlwaysHideAbilityBar = false;
bool iniReplaceArmourBarWithStamina = false;
bool iniMergeHealthAndArmour = true;
//Audio
bool iniDisableWantedMusic = false;
bool iniDisablePoliceScanner = false;
bool iniDisableFlyingMusic = false;
bool iniDisableRadioInterruptions = false;
bool iniDefaultVehicleRadioOff = false;

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
	iniFriendlyFire = ini.GetBoolValue(playerGroup, "FriendlyFire", iniFriendlyFire);
	iniDisableActionMode = ini.GetBoolValue(playerGroup, "DisableActionMode", iniDisableActionMode);
	iniDisarmPlayerWhenShot = ini.GetBoolValue(playerGroup, "DisarmPlayerWhenShot", iniDisarmPlayerWhenShot);
	iniSprintInsideInteriors = ini.GetBoolValue(playerGroup, "SprintInsideInteriors", iniSprintInsideInteriors);
	iniAllowWeaponsInsideSafeHouse = ini.GetBoolValue(playerGroup, "AllowWeaponsInsideSafeHouse", iniAllowWeaponsInsideSafeHouse);

	//////////////////////////////////////Player Controls//////////////////////////////////
	iniToggleFPSWalking = ini.GetBoolValue(playerGroup, "ToggleFPSWalking", iniToggleFPSWalking);
	iniCamFollowVehicleDuringHandbrake = ini.GetBoolValue(playerGroup, "CamFollowVehicleDuringHandbrake", iniCamFollowVehicleDuringHandbrake);
	iniCamFollowVehDelay = ini.GetLongValue(playerGroup, "CamFollowVehDelay", iniCamFollowVehDelay);
	iniDisableRecording = ini.GetBoolValue(playerGroup, "DisableRecording", iniDisableRecording);
	iniDisableMobilePhone = ini.GetBoolValue(playerGroup, "DisableMobilePhone", iniDisableMobilePhone);

	//////////////////////////////////////Player Vehicle///////////////////////////////////
	iniDisableCarMidAirAndRollControl = ini.GetBoolValue(playerGroup, "DisableCarMidAirAndRollControl", iniDisableCarMidAirAndRollControl);
	iniDisableForcedCarExplosionOnImpact = ini.GetBoolValue(playerGroup, "DisableForcedCarExplosionOnImpact", iniDisableForcedCarExplosionOnImpact);
	iniDisableEngineSmoke = ini.GetBoolValue(playerGroup, "DisableEngineSmoke", iniDisableEngineSmoke);
	iniDisableEngineFire = ini.GetBoolValue(playerGroup, "DisableEngineFire", iniDisableEngineFire);
	iniLeaveEngineOnWhenExitingVehicles = ini.GetBoolValue(playerGroup, "LeaveEngineOnWhenExitingVehicles", iniLeaveEngineOnWhenExitingVehicles);
	iniDisableWheelsAutoCenterOnCarExit = ini.GetBoolValue(playerGroup, "DisableWheelsAutoCenterOnCarExit", iniDisableWheelsAutoCenterOnCarExit);
	iniDisableRagdollOnVehicleRoof = ini.GetBoolValue(playerGroup, "DisableRagdollOnVehicleRoof", iniDisableRagdollOnVehicleRoof);
	#pragma warning(suppress: 4244)
	iniMaxVehicleSpeed = ini.GetDoubleValue(playerGroup, "MaxVehicleSpeed", iniMaxVehicleSpeed);
	iniDisableFlyThroughWindscreen = ini.GetBoolValue(playerGroup, "DisableFlyThroughWindscreen", iniDisableFlyThroughWindscreen);
	iniDisableBikeKnockOff = ini.GetBoolValue(playerGroup, "DisableBikeKnockOff", iniDisableBikeKnockOff);
	iniDisableDragOutCar = ini.GetBoolValue(playerGroup, "DisableDragOutCar", iniDisableDragOutCar);
	iniDisableShallowWaterBikeJumpOut = ini.GetBoolValue(playerGroup, "DisableShallowWaterBikeJumpOut", iniDisableShallowWaterBikeJumpOut);
	iniDisableStuntJumps = ini.GetBoolValue(playerGroup, "DisableStuntJumps", iniDisableStuntJumps);

	//////////////////////////////////////HUD//////////////////////////////////////////////
	iniHideMinimapSatNav = ini.GetBoolValue(HUDGroup, "HideMinimapSatNav", iniHideMinimapSatNav);
	iniHideMinimapBars = ini.GetBoolValue(HUDGroup, "HideMinimapBars", iniHideMinimapBars);
	iniHideAbilityBarForNonMainCharacters = ini.GetBoolValue(HUDGroup, "HideAbilityBarForNonMainCharacters", iniHideAbilityBarForNonMainCharacters);
	iniAlwaysHideAbilityBar = ini.GetBoolValue(HUDGroup, "AlwaysHideAbilityBar", iniAlwaysHideAbilityBar);
	iniReplaceArmourBarWithStamina = ini.GetBoolValue(HUDGroup, "ReplaceArmourBarWithStamina", iniReplaceArmourBarWithStamina);
	iniMergeHealthAndArmour = ini.GetBoolValue(HUDGroup, "MergeHealthAndArmour", iniMergeHealthAndArmour);

	/////////////////////////////////////Audio/////////////////////////////////////////////
	iniDisableWantedMusic = ini.GetBoolValue(AudioGroup, "DisableWantedMusic", iniDisableWantedMusic);
	iniDisablePoliceScanner = ini.GetBoolValue(AudioGroup, "DisablePoliceScanner", iniDisablePoliceScanner);
	iniDisableFlyingMusic = ini.GetBoolValue(AudioGroup, "DisableFlyingMusic", iniDisableFlyingMusic);
	iniDisableRadioInterruptions = ini.GetBoolValue(AudioGroup, "DisableRadioInterruptions", iniDisableRadioInterruptions);
	iniDefaultVehicleRadioOff = ini.GetBoolValue(AudioGroup, "DefaultVehicleRadioOff", iniDefaultVehicleRadioOff);

	//////////////////////////////////////Peds/////////////////////////////////////////////
	iniDisableWrithe = ini.GetBoolValue(pedsGroup, "DisableWrithe", iniDisableWrithe);
	iniDisableHurt = ini.GetBoolValue(pedsGroup, "DisableHurt", iniDisableHurt);
	iniDisableSittingPedsInstantDeath = ini.GetBoolValue(pedsGroup, "DisableSittingPedsInstantDeath", iniDisableSittingPedsInstantDeath);
	iniDisarmPedWhenShot = ini.GetBoolValue(pedsGroup, "DisarmPedWhenShot", iniDisarmPedWhenShot);
	iniDisarmChance = ini.GetLongValue(playerGroup, "DisarmChance", iniDisarmChance);
	iniDisarmIncludeLeftHand = ini.GetBoolValue(pedsGroup, "DisarmIncludeLeftHand", iniDisarmIncludeLeftHand);
	iniDisablePedOnlyDamagedByPlayer = ini.GetBoolValue(pedsGroup, "DisablePedOnlyDamagedByPlayer", iniDisablePedOnlyDamagedByPlayer);
	iniDisableDeadPedsJumpOutOfVehicle = ini.GetBoolValue(pedsGroup, "DisableDeadPedsJumpOutOfVehicle", iniDisableDeadPedsJumpOutOfVehicle);
	return;
}