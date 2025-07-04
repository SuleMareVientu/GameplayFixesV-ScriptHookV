#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <SimpleIni.h>
#include "ini.h"
#include "functions.h"
#include "globals.h"

static constexpr char* inputGroup = "Input";
static constexpr char* playerGroup = "Player";
static constexpr char* HUDGroup = "HUD";
static constexpr char* AudioGroup = "Audio";
static constexpr char* pedsGroup = "Peds";

namespace Ini
{
//Input Settings
unsigned long ReloadIniKey = VK_F12;
//Player Settings
bool EnableCrouching = true;
bool FriendlyFire = true;
bool EnableStealthForAllPeds = true;
bool DisableActionMode = false;
bool DisarmPlayerWhenShot = true;
bool DropPlayerWeaponWhenRagdolling = true;
bool DynamicallyCleanWoundsAndDirt = true;
bool SprintInsideInteriors = true;
bool AllowWeaponsInsideSafeHouse = false;
bool SilentWanted = false;
//Player Controls
bool DisableAssistedMovement = true;
bool ToggleFPSWalking = true;
int DisableCameraAutoCenter = NULL;
bool CamFollowVehicleDuringHandbrake = false;
int CamFollowVehDelay = 250;
bool DisableFirstPersonView = false;
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
bool EnableBrakeLightsOnStoppedVehicles = true;
bool EnableHeliWaterPhysics = true;
bool DisableRagdollOnVehicleRoof = true;
float MaxVehicleSpeed = 90.0f;
bool DisableDragOutCar = false;
bool DisableFlyThroughWindscreen = false;
bool DisableBikeKnockOff = false;
bool DisableShallowWaterBikeJumpOut = true;
bool DisableVehicleJitter = true;
bool DisableAirVehicleTurbulence = false;
int DisableAutoEquipHelmets = NULL;
bool DisableStuntJumps = false;
//HUD
bool AllowGameExecutionOnPauseMenu = false;
bool DisablePauseMenuPostFX = false;
bool DisableHUDPostFX = false;
bool DisableSpecialAbilityPostFX = false;
bool EnableBigMapToggle = true;
bool MinimapSpeedometer = true;
float SetRadarZoom = -1.0f;
bool DisableMinimapTilt = false;
bool HideMinimapFog = true;
bool HideMinimapSatNav = false;
bool HideMinimapDepth = false;
bool HideMinimapBars = false;
bool AlwaysHideAbilityBar = false;
bool HideAbilityBarForNonMainCharacters = true;
bool ReplaceArmourBarWithStamina = false;
bool MergeHealthAndArmour = true;
bool HideHudComponents = false;
char* HudComponents = "HUD_VEHICLE_NAME, HUD_AREA_NAME, HUD_STREET_NAME";
bool HideWeaponReticle = false;
bool HideEnemiesBlips = false;
//Audio
bool DisableWantedMusic = false;
bool DisablePoliceScanner = false;
bool DisableFlyingMusic = false;
bool DisableRadioInterruptions = false;
int DefaultVehicleRadioOff = 0;
bool MuteSounds = false;
char* Sounds = "AMBIENCE, MUSIC";
bool DisablePlayerPainAudio = false;
bool MuteArtificialAmbientSounds = false;

//Peds Settings
bool DisableWrithe = true;
bool DisableHurt = true;
bool DisableShootFromGround = true;
bool DisableSittingPedsInstantDeath = true;
bool DisarmPedWhenShot = true;
int DisarmChance = 50;
bool DisarmIncludeLeftHand = false;
bool DisablePedOnlyDamagedByPlayer = true;
bool DisableDeadPedsJumpOutOfVehicle = true;
bool DisableScenarios = false;
bool DisableWorldPopulation = false;
}
using namespace Ini;

static CSimpleIniA ini;
void ReadINI()
{
	if (!std::filesystem::exists(GetDllInstanceIniName()))
		WriteINIResource(GetDllInstance(), IDR_INI, GetDllInstanceIniName());
	
	const SI_Error res = ini.LoadFile(GetDllInstanceIniName());
	if (res != SI_OK)
		return;

	constexpr char* DefaultReloadIniKeyStr = "F12"; std::string tmpStrArr[1]{};
	SplitString(const_cast<char*>(ini.GetValue(inputGroup, "ReloadIniKey", DefaultReloadIniKeyStr)), tmpStrArr, 1);
	ReloadIniKey = GetVKFromString(tmpStrArr[0]);

	//////////////////////////////////////Player//////////////////////////////////////////
	EnableCrouching = ini.GetBoolValue(playerGroup, "EnableCrouching", EnableCrouching);
	FriendlyFire = ini.GetBoolValue(playerGroup, "FriendlyFire", FriendlyFire);
	EnableStealthForAllPeds = ini.GetBoolValue(playerGroup, "EnableStealthForAllPeds", EnableStealthForAllPeds);
	DisableActionMode = ini.GetBoolValue(playerGroup, "DisableActionMode", DisableActionMode);
	DisarmPlayerWhenShot = ini.GetBoolValue(playerGroup, "DisarmPlayerWhenShot", DisarmPlayerWhenShot);
	DropPlayerWeaponWhenRagdolling = ini.GetBoolValue(playerGroup, "DropPlayerWeaponWhenRagdolling", DropPlayerWeaponWhenRagdolling);
	DynamicallyCleanWoundsAndDirt = ini.GetBoolValue(playerGroup, "DynamicallyCleanWoundsAndDirt", DynamicallyCleanWoundsAndDirt);
	SprintInsideInteriors = ini.GetBoolValue(playerGroup, "SprintInsideInteriors", SprintInsideInteriors);
	AllowWeaponsInsideSafeHouse = ini.GetBoolValue(playerGroup, "AllowWeaponsInsideSafeHouse", AllowWeaponsInsideSafeHouse);
	SilentWanted = ini.GetBoolValue(playerGroup, "SilentWanted", SilentWanted);

	//////////////////////////////////////Player Controls//////////////////////////////////
	DisableAssistedMovement = ini.GetBoolValue(playerGroup, "DisableAssistedMovement", DisableAssistedMovement);
	ToggleFPSWalking = ini.GetBoolValue(playerGroup, "ToggleFPSWalking", ToggleFPSWalking);
	DisableCameraAutoCenter = static_cast<int>(ini.GetLongValue(playerGroup, "DisableCameraAutoCenter", DisableCameraAutoCenter));
	CamFollowVehicleDuringHandbrake = ini.GetBoolValue(playerGroup, "CamFollowVehicleDuringHandbrake", CamFollowVehicleDuringHandbrake);
	CamFollowVehDelay = static_cast<int>(ini.GetLongValue(playerGroup, "CamFollowVehDelay", CamFollowVehDelay));
	DisableFirstPersonView = ini.GetBoolValue(playerGroup, "DisableFirstPersonView", DisableFirstPersonView);
	DisableIdleCamera = ini.GetBoolValue(playerGroup, "DisableIdleCamera", DisableIdleCamera);
	DisableRecording = ini.GetBoolValue(playerGroup, "DisableRecording", DisableRecording);
	DisableMobilePhone = ini.GetBoolValue(playerGroup, "DisableMobilePhone", DisableMobilePhone);

	//////////////////////////////////////Player Vehicle///////////////////////////////////
	DisableCarMidAirAndRollControl = ini.GetBoolValue(playerGroup, "DisableCarMidAirAndRollControl", DisableCarMidAirAndRollControl);
	DisableForcedCarExplosionOnImpact = ini.GetBoolValue(playerGroup, "DisableForcedCarExplosionOnImpact", DisableForcedCarExplosionOnImpact);
	DisableEngineSmoke = ini.GetBoolValue(playerGroup, "DisableEngineSmoke", DisableEngineSmoke);
	DisableEngineFire = ini.GetBoolValue(playerGroup, "DisableEngineFire", DisableEngineFire);
	EnableBrakeLightsOnStoppedVehicles = ini.GetBoolValue(playerGroup, "EnableBrakeLightsOnStoppedVehicles", EnableBrakeLightsOnStoppedVehicles);
	LeaveEngineOnWhenExitingVehicles = ini.GetBoolValue(playerGroup, "LeaveEngineOnWhenExitingVehicles", LeaveEngineOnWhenExitingVehicles);
	KeepCarHydraulicsPosition = ini.GetBoolValue(playerGroup, "KeepCarHydraulicsPosition", KeepCarHydraulicsPosition);
	DisableWheelsAutoCenterOnCarExit = ini.GetBoolValue(playerGroup, "DisableWheelsAutoCenterOnCarExit", DisableWheelsAutoCenterOnCarExit);
	EnableHeliWaterPhysics = ini.GetBoolValue(playerGroup, "EnableHeliWaterPhysics", EnableHeliWaterPhysics);
	DisableRagdollOnVehicleRoof = ini.GetBoolValue(playerGroup, "DisableRagdollOnVehicleRoof", DisableRagdollOnVehicleRoof);
	MaxVehicleSpeed = static_cast<float>(ini.GetDoubleValue(playerGroup, "MaxVehicleSpeed", MaxVehicleSpeed));
	DisableFlyThroughWindscreen = ini.GetBoolValue(playerGroup, "DisableFlyThroughWindscreen", DisableFlyThroughWindscreen);
	DisableBikeKnockOff = ini.GetBoolValue(playerGroup, "DisableBikeKnockOff", DisableBikeKnockOff);
	DisableDragOutCar = ini.GetBoolValue(playerGroup, "DisableDragOutCar", DisableDragOutCar);
	DisableShallowWaterBikeJumpOut = ini.GetBoolValue(playerGroup, "DisableShallowWaterBikeJumpOut", DisableShallowWaterBikeJumpOut);
	DisableStuntJumps = ini.GetBoolValue(playerGroup, "DisableStuntJumps", DisableStuntJumps);
	DisableVehicleJitter = ini.GetBoolValue(playerGroup, "DisableVehicleJitter", DisableVehicleJitter);
	DisableAirVehicleTurbulence = ini.GetBoolValue(playerGroup, "DisableAirVehicleTurbulence", DisableAirVehicleTurbulence);
	DisableAutoEquipHelmets = static_cast<int>(ini.GetLongValue(playerGroup, "DisableAutoEquipHelmets", DisableAutoEquipHelmets));

	//////////////////////////////////////HUD//////////////////////////////////////////////
	AllowGameExecutionOnPauseMenu = ini.GetBoolValue(HUDGroup, "AllowGameExecutionOnPauseMenu", AllowGameExecutionOnPauseMenu);
	DisablePauseMenuPostFX = ini.GetBoolValue(HUDGroup, "DisablePauseMenuPostFX", DisablePauseMenuPostFX);
	DisableHUDPostFX = ini.GetBoolValue(HUDGroup, "DisableHUDPostFX", DisableHUDPostFX);
	DisableSpecialAbilityPostFX = ini.GetBoolValue(HUDGroup, "DisableSpecialAbilityPostFX", DisableSpecialAbilityPostFX);
	EnableBigMapToggle = ini.GetBoolValue(HUDGroup, "EnableBigMapToggle", EnableBigMapToggle);
	MinimapSpeedometer = ini.GetBoolValue(HUDGroup, "MinimapSpeedometer", MinimapSpeedometer);
	SetRadarZoom = static_cast<float>(ini.GetDoubleValue(HUDGroup, "SetRadarZoom", SetRadarZoom));
	DisableMinimapTilt = ini.GetBoolValue(HUDGroup, "DisableMinimapTilt", DisableMinimapTilt);
	HideMinimapFog = ini.GetBoolValue(HUDGroup, "HideMinimapFog", HideMinimapFog);
	HideMinimapSatNav = ini.GetBoolValue(HUDGroup, "HideMinimapSatNav", HideMinimapSatNav);
	HideMinimapDepth = ini.GetBoolValue(HUDGroup, "HideMinimapDepth", HideMinimapDepth);
	HideMinimapBars = ini.GetBoolValue(HUDGroup, "HideMinimapBars", HideMinimapBars);
	AlwaysHideAbilityBar = ini.GetBoolValue(HUDGroup, "AlwaysHideAbilityBar", AlwaysHideAbilityBar);
	HideAbilityBarForNonMainCharacters = ini.GetBoolValue(HUDGroup, "HideAbilityBarForNonMainCharacters", HideAbilityBarForNonMainCharacters);
	ReplaceArmourBarWithStamina = ini.GetBoolValue(HUDGroup, "ReplaceArmourBarWithStamina", ReplaceArmourBarWithStamina);
	MergeHealthAndArmour = ini.GetBoolValue(HUDGroup, "MergeHealthAndArmour", MergeHealthAndArmour);
	HideHudComponents = ini.GetBoolValue(HUDGroup, "HideHudComponents", HideHudComponents);
	HudComponents = const_cast<char*>(ini.GetValue(HUDGroup, "HudComponents", HudComponents));
	HideWeaponReticle = ini.GetBoolValue(HUDGroup, "HideWeaponReticle", HideWeaponReticle);
	HideEnemiesBlips = ini.GetBoolValue(HUDGroup, "HideEnemiesBlips", HideEnemiesBlips);

	/////////////////////////////////////Audio/////////////////////////////////////////////
	DisableWantedMusic = ini.GetBoolValue(AudioGroup, "DisableWantedMusic", DisableWantedMusic);
	DisablePoliceScanner = ini.GetBoolValue(AudioGroup, "DisablePoliceScanner", DisablePoliceScanner);
	DisableFlyingMusic = ini.GetBoolValue(AudioGroup, "DisableFlyingMusic", DisableFlyingMusic);
	DisableRadioInterruptions = ini.GetBoolValue(AudioGroup, "DisableRadioInterruptions", DisableRadioInterruptions);
	DefaultVehicleRadioOff = static_cast<int>(ini.GetLongValue(AudioGroup, "DefaultVehicleRadioOff", DefaultVehicleRadioOff));
	MuteSounds = ini.GetBoolValue(AudioGroup, "MuteSounds", MuteSounds);
	Sounds = const_cast<char*>(ini.GetValue(AudioGroup, "Sounds", Sounds));
	DisablePlayerPainAudio = ini.GetBoolValue(AudioGroup, "DisablePlayerPainAudio", DisablePlayerPainAudio);
	MuteArtificialAmbientSounds = ini.GetBoolValue(AudioGroup, "MuteArtificialAmbientSounds", MuteArtificialAmbientSounds);

	//////////////////////////////////////Peds/////////////////////////////////////////////
	DisableWrithe = ini.GetBoolValue(pedsGroup, "DisableWrithe", DisableWrithe);
	DisableHurt = ini.GetBoolValue(pedsGroup, "DisableHurt", DisableHurt);
	DisableShootFromGround = ini.GetBoolValue(pedsGroup, "DisableShootFromGround", DisableShootFromGround);
	DisableSittingPedsInstantDeath = ini.GetBoolValue(pedsGroup, "DisableSittingPedsInstantDeath", DisableSittingPedsInstantDeath);
	DisarmPedWhenShot = ini.GetBoolValue(pedsGroup, "DisarmPedWhenShot", DisarmPedWhenShot);
	DisarmChance = static_cast<int>(ini.GetLongValue(pedsGroup, "DisarmChance", DisarmChance));
	DisarmIncludeLeftHand = ini.GetBoolValue(pedsGroup, "DisarmIncludeLeftHand", DisarmIncludeLeftHand);
	DisablePedOnlyDamagedByPlayer = ini.GetBoolValue(pedsGroup, "DisablePedOnlyDamagedByPlayer", DisablePedOnlyDamagedByPlayer);
	DisableDeadPedsJumpOutOfVehicle = ini.GetBoolValue(pedsGroup, "DisableDeadPedsJumpOutOfVehicle", DisableDeadPedsJumpOutOfVehicle);
	DisableScenarios = ini.GetBoolValue(pedsGroup, "DisableScenarios", DisableScenarios);
	DisableWorldPopulation = ini.GetBoolValue(pedsGroup, "DisableWorldPopulation", DisableWorldPopulation);
	return;
}

void WriteINIResource(HINSTANCE hInstance, int resourceID, const char* path)
{
	const HRSRC hRes = FindResource(hInstance, MAKEINTRESOURCE(resourceID), "INI");
	if (!hRes) throw std::runtime_error("Resource not found");

	const HGLOBAL hData = LoadResource(hInstance, hRes);
	if (!hData) throw std::runtime_error("Failed to load resource");

	const void* data = LockResource(hData);
	const DWORD size = SizeofResource(hInstance, hRes);

	FILE* f = fopen(path, "wb");
	if (f)
	{
#pragma warning( suppress : 6387 )
		fwrite(data, 1, size, f);
		fflush(f);
		fclose(f);
	}
	return;
}