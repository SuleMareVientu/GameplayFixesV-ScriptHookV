#include "ini.h"
#include <SimpleIni.h>
#include "../globals.h"

static constexpr char* playerGroup = "Player";
static constexpr char* HUDGroup = "HUD";
static constexpr char* AudioGroup = "Audio";
static constexpr char* pedsGroup = "Peds";

namespace INI
{
//Player Settings
bool FriendlyFire = true;
bool DisableActionMode = false;
bool DisarmPlayerWhenShot = true;
bool SprintInsideInteriors = true;
bool AllowWeaponsInsideSafeHouse = false;
//Player Controls
bool DisableAssistedMovement = true;
bool ToggleFPSWalking = true;
bool CamFollowVehicleDuringHandbrake = false;
int CamFollowVehDelay = 250;
bool DisableIdleCamera = false;
bool DisableRecording = false;
bool DisableMobilePhone = false;
//Player Vehicle
bool DisableCarMidAirAndRollControl = true;
bool DisableForcedCarExplosionOnImpact = true;
bool DisableEngineSmoke = false;
bool DisableEngineFire = false;
bool LeaveEngineOnWhenExitingVehicles = true;
bool DisableWheelsAutoCenterOnCarExit = true;
bool KeepCarHydraulicsPosition = true;
bool DisableRagdollOnVehicleRoof = true;
float MaxVehicleSpeed = 90.0f;
bool DisableFlyThroughWindscreen = false;
bool DisableBikeKnockOff = false;
bool DisableDragOutCar = false;
bool DisableShallowWaterBikeJumpOut = true;
bool DisableStuntJumps = false;
//HUD
bool AllowGameExecutionOnPauseMenu = false;
bool DisablePauseMenuPostFX = false;
bool DisableHUDPostFX = false;
bool DisableSpecialAbilityPostFX = false;
bool EnableBigMapToggle = true;
bool DisableMinimapTilt = false;
bool HideMinimapFog = true;
bool HideMinimapSatNav = false;
bool HideMinimapDepth = false;
bool HideMinimapBars = false;
bool AlwaysHideAbilityBar = false;
bool HideAbilityBarForNonMainCharacters = true;
bool ReplaceArmourBarWithStamina = false;
bool MergeHealthAndArmour = true;
//Audio
bool DisableWantedMusic = false;
bool DisablePoliceScanner = false;
bool DisableFlyingMusic = false;
bool DisableRadioInterruptions = false;
bool DefaultVehicleRadioOff = false;

//Peds Settings
bool DisableWrithe = true;
bool DisableHurt = true;
bool DisableSittingPedsInstantDeath = true;
bool DisarmPedWhenShot = true;
int DisarmChance = 50;
bool DisarmIncludeLeftHand = false;
bool DisablePedOnlyDamagedByPlayer = true;
bool DisableDeadPedsJumpOutOfVehicle = true;
}
using namespace INI;

void ReadINI()
{
	CSimpleIniA ini;
	SI_Error res = ini.LoadFile("GameplayFixesV.ini");

	if (res != SI_OK)
		return;

	//////////////////////////////////////Player//////////////////////////////////////////
	FriendlyFire = ini.GetBoolValue(playerGroup, "FriendlyFire", FriendlyFire);
	DisableActionMode = ini.GetBoolValue(playerGroup, "DisableActionMode", DisableActionMode);
	DisarmPlayerWhenShot = ini.GetBoolValue(playerGroup, "DisarmPlayerWhenShot", DisarmPlayerWhenShot);
	SprintInsideInteriors = ini.GetBoolValue(playerGroup, "SprintInsideInteriors", SprintInsideInteriors);
	AllowWeaponsInsideSafeHouse = ini.GetBoolValue(playerGroup, "AllowWeaponsInsideSafeHouse", AllowWeaponsInsideSafeHouse);

	//////////////////////////////////////Player Controls//////////////////////////////////
	DisableAssistedMovement = ini.GetBoolValue(playerGroup, "DisableAssistedMovement", DisableAssistedMovement);
	ToggleFPSWalking = ini.GetBoolValue(playerGroup, "ToggleFPSWalking", ToggleFPSWalking);
	CamFollowVehicleDuringHandbrake = ini.GetBoolValue(playerGroup, "CamFollowVehicleDuringHandbrake", CamFollowVehicleDuringHandbrake);
	CamFollowVehDelay = ini.GetLongValue(playerGroup, "CamFollowVehDelay", CamFollowVehDelay);
	DisableIdleCamera = ini.GetBoolValue(playerGroup, "DisableIdleCamera", DisableIdleCamera);
	DisableRecording = ini.GetBoolValue(playerGroup, "DisableRecording", DisableRecording);
	DisableMobilePhone = ini.GetBoolValue(playerGroup, "DisableMobilePhone", DisableMobilePhone);

	//////////////////////////////////////Player Vehicle///////////////////////////////////
	DisableCarMidAirAndRollControl = ini.GetBoolValue(playerGroup, "DisableCarMidAirAndRollControl", DisableCarMidAirAndRollControl);
	DisableForcedCarExplosionOnImpact = ini.GetBoolValue(playerGroup, "DisableForcedCarExplosionOnImpact", DisableForcedCarExplosionOnImpact);
	DisableEngineSmoke = ini.GetBoolValue(playerGroup, "DisableEngineSmoke", DisableEngineSmoke);
	DisableEngineFire = ini.GetBoolValue(playerGroup, "DisableEngineFire", DisableEngineFire);
	LeaveEngineOnWhenExitingVehicles = ini.GetBoolValue(playerGroup, "LeaveEngineOnWhenExitingVehicles", LeaveEngineOnWhenExitingVehicles);
	KeepCarHydraulicsPosition = ini.GetBoolValue(playerGroup, "KeepCarHydraulicsPosition", KeepCarHydraulicsPosition);
	DisableWheelsAutoCenterOnCarExit = ini.GetBoolValue(playerGroup, "DisableWheelsAutoCenterOnCarExit", DisableWheelsAutoCenterOnCarExit);
	DisableRagdollOnVehicleRoof = ini.GetBoolValue(playerGroup, "DisableRagdollOnVehicleRoof", DisableRagdollOnVehicleRoof);
	#pragma warning(suppress: 4244)
	MaxVehicleSpeed = ini.GetDoubleValue(playerGroup, "MaxVehicleSpeed", MaxVehicleSpeed);
	DisableFlyThroughWindscreen = ini.GetBoolValue(playerGroup, "DisableFlyThroughWindscreen", DisableFlyThroughWindscreen);
	DisableBikeKnockOff = ini.GetBoolValue(playerGroup, "DisableBikeKnockOff", DisableBikeKnockOff);
	DisableDragOutCar = ini.GetBoolValue(playerGroup, "DisableDragOutCar", DisableDragOutCar);
	DisableShallowWaterBikeJumpOut = ini.GetBoolValue(playerGroup, "DisableShallowWaterBikeJumpOut", DisableShallowWaterBikeJumpOut);
	DisableStuntJumps = ini.GetBoolValue(playerGroup, "DisableStuntJumps", DisableStuntJumps);

	//////////////////////////////////////HUD//////////////////////////////////////////////
	AllowGameExecutionOnPauseMenu = ini.GetBoolValue(HUDGroup, "AllowGameExecutionOnPauseMenu", AllowGameExecutionOnPauseMenu);
	DisablePauseMenuPostFX = ini.GetBoolValue(HUDGroup, "DisablePauseMenuPostFX", DisablePauseMenuPostFX);
	DisableHUDPostFX = ini.GetBoolValue(HUDGroup, "DisableHUDPostFX", DisableHUDPostFX);
	DisableSpecialAbilityPostFX = ini.GetBoolValue(HUDGroup, "DisableSpecialAbilityPostFX", DisableSpecialAbilityPostFX);
	EnableBigMapToggle = ini.GetBoolValue(HUDGroup, "EnableBigMapToggle", EnableBigMapToggle);
	DisableMinimapTilt = ini.GetBoolValue(HUDGroup, "DisableMinimapTilt", DisableMinimapTilt);
	HideMinimapFog = ini.GetBoolValue(HUDGroup, "HideMinimapFog", HideMinimapFog);
	HideMinimapSatNav = ini.GetBoolValue(HUDGroup, "HideMinimapSatNav", HideMinimapSatNav);
	HideMinimapDepth = ini.GetBoolValue(HUDGroup, "HideMinimapDepth", HideMinimapDepth);
	HideMinimapBars = ini.GetBoolValue(HUDGroup, "HideMinimapBars", HideMinimapBars);
	AlwaysHideAbilityBar = ini.GetBoolValue(HUDGroup, "AlwaysHideAbilityBar", AlwaysHideAbilityBar);
	HideAbilityBarForNonMainCharacters = ini.GetBoolValue(HUDGroup, "HideAbilityBarForNonMainCharacters", HideAbilityBarForNonMainCharacters);
	ReplaceArmourBarWithStamina = ini.GetBoolValue(HUDGroup, "ReplaceArmourBarWithStamina", ReplaceArmourBarWithStamina);
	MergeHealthAndArmour = ini.GetBoolValue(HUDGroup, "MergeHealthAndArmour", MergeHealthAndArmour);

	/////////////////////////////////////Audio/////////////////////////////////////////////
	DisableWantedMusic = ini.GetBoolValue(AudioGroup, "DisableWantedMusic", DisableWantedMusic);
	DisablePoliceScanner = ini.GetBoolValue(AudioGroup, "DisablePoliceScanner", DisablePoliceScanner);
	DisableFlyingMusic = ini.GetBoolValue(AudioGroup, "DisableFlyingMusic", DisableFlyingMusic);
	DisableRadioInterruptions = ini.GetBoolValue(AudioGroup, "DisableRadioInterruptions", DisableRadioInterruptions);
	DefaultVehicleRadioOff = ini.GetBoolValue(AudioGroup, "DefaultVehicleRadioOff", DefaultVehicleRadioOff);

	//////////////////////////////////////Peds/////////////////////////////////////////////
	DisableWrithe = ini.GetBoolValue(pedsGroup, "DisableWrithe", DisableWrithe);
	DisableHurt = ini.GetBoolValue(pedsGroup, "DisableHurt", DisableHurt);
	DisableSittingPedsInstantDeath = ini.GetBoolValue(pedsGroup, "DisableSittingPedsInstantDeath", DisableSittingPedsInstantDeath);
	DisarmPedWhenShot = ini.GetBoolValue(pedsGroup, "DisarmPedWhenShot", DisarmPedWhenShot);
	DisarmChance = ini.GetLongValue(playerGroup, "DisarmChance", DisarmChance);
	DisarmIncludeLeftHand = ini.GetBoolValue(pedsGroup, "DisarmIncludeLeftHand", DisarmIncludeLeftHand);
	DisablePedOnlyDamagedByPlayer = ini.GetBoolValue(pedsGroup, "DisablePedOnlyDamagedByPlayer", DisablePedOnlyDamagedByPlayer);
	DisableDeadPedsJumpOutOfVehicle = ini.GetBoolValue(pedsGroup, "DisableDeadPedsJumpOutOfVehicle", DisableDeadPedsJumpOutOfVehicle);
	return;
}