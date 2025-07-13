#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <libs\SimpleIni.h>
#include "utils\ini.h"
#include "utils\functions.h"
#include "globals.h"

static constexpr char* inputGroup = "Input";
static constexpr char* memoryGroup = "Memory";
static constexpr char* playerGroup = "Player";
static constexpr char* HUDGroup = "HUD";
static constexpr char* AudioGroup = "Audio";
static constexpr char* pedsGroup = "Peds";

namespace Ini
{
//Input Settings
unsigned long ReloadIniKey = VK_F12;
//Memory Settings
bool ApplyExePatches = true;
bool LowPriorityPropsPatch = true;
bool CenterSteeringPatch = true;
bool CopBumpSteeringPatch = true;
bool HUDWheelSlowdownPatch = false;
bool HookGameFunctions = true;
bool EnableLogging = false;
//Player Settings
bool EnableCrouching = true;
bool FriendlyFire = true;
bool EnablePlayerActionsForAllPeds = true;
bool DisableActionMode = false;
bool DisarmPlayerWhenShot = true;
int DisarmPlayerChance = 100;
bool DropPlayerWeaponWhenRagdolling = true;
bool AutoEquipDroppedWeapon = true;
bool DroppedWeaponsBlip = true;
float DroppedWeaponsMaxDistance = 512.0f;
bool EnablePlayerNMReactionsWhenShot = true;
int RagdollChance = 30;
int MinimumRagdollTime = 450;
int MaximumRagdollTime = 1500;
bool ShouldRagdollInCover = false;
bool DontDropWeapon = true;
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
bool EnableBrakeLightsOnStoppedVehicle = true;
bool EnableHeliWaterPhysics = true;
bool DynamicallyCleanVehicles = true;
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
std::string HudComponents = "HUD_VEHICLE_NAME, HUD_AREA_NAME, HUD_STREET_NAME";
bool HideWeaponReticle = false;
bool HideEnemiesBlips = false;
//Audio
bool DisableWantedMusic = false;
bool DisablePoliceScanner = false;
bool DisableFlyingMusic = false;
bool DisableRadioInterruptions = false;
int DefaultVehicleRadioOff = 0;
bool MuteSounds = false;
std::string Sounds = "AMBIENCE, MUSIC";
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
bool DynamicCarJackingReactions = true;
bool DisableScenarios = false;
bool DisableWorldPopulation = false;
}
using namespace Ini;

#define GET_INI_BOOL(ini, group, value) ini.GetBoolValue(group, #value, Ini::value);
#define GET_INI_INT(ini, group, value) static_cast<int>(ini.GetLongValue(group, #value, Ini::value));
#define GET_INI_FLOAT(ini, group, value) static_cast<float>(ini.GetDoubleValue(group, #value, Ini::value));
#define GET_INI_STRING(ini, group, value) ini.GetValue(group, #value, Ini::value.c_str());

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

	//////////////////////////////////////Memory//////////////////////////////////////////
	Ini::ApplyExePatches = GET_INI_BOOL(ini, memoryGroup, ApplyExePatches);
	Ini::LowPriorityPropsPatch = GET_INI_BOOL(ini, memoryGroup, LowPriorityPropsPatch);
	Ini::CenterSteeringPatch = GET_INI_BOOL(ini, memoryGroup, CenterSteeringPatch);
	Ini::CopBumpSteeringPatch = GET_INI_BOOL(ini, memoryGroup, CopBumpSteeringPatch);
	Ini::HUDWheelSlowdownPatch = GET_INI_BOOL(ini, memoryGroup, HUDWheelSlowdownPatch);
	Ini::HookGameFunctions = GET_INI_BOOL(ini, memoryGroup, HookGameFunctions);
	Ini::EnableLogging = GET_INI_BOOL(ini, memoryGroup, EnableLogging);

	//////////////////////////////////////Player//////////////////////////////////////////
	Ini::EnableCrouching = GET_INI_BOOL(ini, playerGroup, EnableCrouching);
	Ini::FriendlyFire = GET_INI_BOOL(ini, playerGroup, FriendlyFire);
	Ini::EnablePlayerActionsForAllPeds = GET_INI_BOOL(ini, playerGroup, EnablePlayerActionsForAllPeds);
	Ini::DisableActionMode = GET_INI_BOOL(ini, playerGroup, DisableActionMode);
	Ini::DisarmPlayerWhenShot = GET_INI_BOOL(ini, playerGroup, DisarmPlayerWhenShot);
	Ini::DisarmPlayerChance = GET_INI_INT(ini, playerGroup, DisarmPlayerChance);
	Ini::DropPlayerWeaponWhenRagdolling = GET_INI_BOOL(ini, playerGroup, DropPlayerWeaponWhenRagdolling);
	Ini::AutoEquipDroppedWeapon = GET_INI_BOOL(ini, playerGroup, AutoEquipDroppedWeapon);
	Ini::DroppedWeaponsBlip = GET_INI_BOOL(ini, playerGroup, DroppedWeaponsBlip);
	Ini::DroppedWeaponsMaxDistance = GET_INI_FLOAT(ini, playerGroup, DroppedWeaponsMaxDistance);
	Ini::EnablePlayerNMReactionsWhenShot = GET_INI_BOOL(ini, playerGroup, EnablePlayerNMReactionsWhenShot);
	Ini::RagdollChance = GET_INI_INT(ini, playerGroup, RagdollChance);
	Ini::MinimumRagdollTime = GET_INI_INT(ini, playerGroup, MinimumRagdollTime);
	Ini::MaximumRagdollTime = GET_INI_INT(ini, playerGroup, MaximumRagdollTime);
	Ini::ShouldRagdollInCover = GET_INI_BOOL(ini, playerGroup, ShouldRagdollInCover);
	Ini::DontDropWeapon = GET_INI_BOOL(ini, playerGroup, DontDropWeapon);
	Ini::DynamicallyCleanWoundsAndDirt = GET_INI_BOOL(ini, playerGroup, DynamicallyCleanWoundsAndDirt);
	Ini::SprintInsideInteriors = GET_INI_BOOL(ini, playerGroup, SprintInsideInteriors);
	Ini::AllowWeaponsInsideSafeHouse = GET_INI_BOOL(ini, playerGroup, AllowWeaponsInsideSafeHouse);
	Ini::SilentWanted = GET_INI_BOOL(ini, playerGroup, SilentWanted);

	//////////////////////////////////////Player Controls//////////////////////////////////
	Ini::DisableAssistedMovement = GET_INI_BOOL(ini, playerGroup, DisableAssistedMovement);
	Ini::ToggleFPSWalking = GET_INI_BOOL(ini, playerGroup, ToggleFPSWalking);
	Ini::DisableCameraAutoCenter = GET_INI_INT(ini, playerGroup, DisableCameraAutoCenter);
	Ini::CamFollowVehicleDuringHandbrake = GET_INI_BOOL(ini, playerGroup, CamFollowVehicleDuringHandbrake);
	Ini::CamFollowVehDelay = GET_INI_INT(ini, playerGroup, CamFollowVehDelay);
	Ini::DisableFirstPersonView = GET_INI_BOOL(ini, playerGroup, DisableFirstPersonView);
	Ini::DisableIdleCamera = GET_INI_BOOL(ini, playerGroup, DisableIdleCamera);
	Ini::DisableRecording = GET_INI_BOOL(ini, playerGroup, DisableRecording);
	Ini::DisableMobilePhone = GET_INI_BOOL(ini, playerGroup, DisableMobilePhone);

	//////////////////////////////////////Player Vehicle///////////////////////////////////
	Ini::DisableCarMidAirAndRollControl = GET_INI_BOOL(ini, playerGroup, DisableCarMidAirAndRollControl);
	Ini::DisableForcedCarExplosionOnImpact = GET_INI_BOOL(ini, playerGroup, DisableForcedCarExplosionOnImpact);
	Ini::DisableEngineSmoke = GET_INI_BOOL(ini, playerGroup, DisableEngineSmoke);
	Ini::DisableEngineFire = GET_INI_BOOL(ini, playerGroup, DisableEngineFire);
	Ini::EnableBrakeLightsOnStoppedVehicle = GET_INI_BOOL(ini, playerGroup, EnableBrakeLightsOnStoppedVehicle);
	Ini::LeaveEngineOnWhenExitingVehicles = GET_INI_BOOL(ini, playerGroup, LeaveEngineOnWhenExitingVehicles);
	Ini::KeepCarHydraulicsPosition = GET_INI_BOOL(ini, playerGroup, KeepCarHydraulicsPosition);
	Ini::DisableWheelsAutoCenterOnCarExit = GET_INI_BOOL(ini, playerGroup, DisableWheelsAutoCenterOnCarExit);
	Ini::EnableHeliWaterPhysics = GET_INI_BOOL(ini, playerGroup, EnableHeliWaterPhysics);
	Ini::DynamicallyCleanVehicles = GET_INI_BOOL(ini, playerGroup, DynamicallyCleanVehicles);
	Ini::DisableRagdollOnVehicleRoof = GET_INI_BOOL(ini, playerGroup, DisableRagdollOnVehicleRoof);
	Ini::MaxVehicleSpeed = GET_INI_FLOAT(ini, playerGroup, MaxVehicleSpeed);
	Ini::DisableFlyThroughWindscreen = GET_INI_BOOL(ini, playerGroup, DisableFlyThroughWindscreen);
	Ini::DisableBikeKnockOff = GET_INI_BOOL(ini, playerGroup, DisableBikeKnockOff);
	Ini::DisableDragOutCar = GET_INI_BOOL(ini, playerGroup, DisableDragOutCar);
	Ini::DisableShallowWaterBikeJumpOut = GET_INI_BOOL(ini, playerGroup, DisableShallowWaterBikeJumpOut);
	Ini::DisableStuntJumps = GET_INI_BOOL(ini, playerGroup, DisableStuntJumps);
	Ini::DisableVehicleJitter = GET_INI_BOOL(ini, playerGroup, DisableVehicleJitter);
	Ini::DisableAirVehicleTurbulence = GET_INI_BOOL(ini, playerGroup, DisableAirVehicleTurbulence);
	Ini::DisableAutoEquipHelmets = GET_INI_INT(ini, playerGroup, DisableAutoEquipHelmets);

	//////////////////////////////////////HUD//////////////////////////////////////////////
	Ini::AllowGameExecutionOnPauseMenu = GET_INI_BOOL(ini, HUDGroup, AllowGameExecutionOnPauseMenu);
	Ini::DisablePauseMenuPostFX = GET_INI_BOOL(ini, HUDGroup, DisablePauseMenuPostFX);
	Ini::DisableHUDPostFX = GET_INI_BOOL(ini, HUDGroup, DisableHUDPostFX);
	Ini::DisableSpecialAbilityPostFX = GET_INI_BOOL(ini, HUDGroup, DisableSpecialAbilityPostFX);
	Ini::EnableBigMapToggle = GET_INI_BOOL(ini, HUDGroup, EnableBigMapToggle);
	Ini::MinimapSpeedometer = GET_INI_BOOL(ini, HUDGroup, MinimapSpeedometer);
	Ini::SetRadarZoom = GET_INI_FLOAT(ini, HUDGroup, SetRadarZoom);
	Ini::DisableMinimapTilt = GET_INI_BOOL(ini, HUDGroup, DisableMinimapTilt);
	Ini::HideMinimapFog = GET_INI_BOOL(ini, HUDGroup, HideMinimapFog);
	Ini::HideMinimapSatNav = GET_INI_BOOL(ini, HUDGroup, HideMinimapSatNav);
	Ini::HideMinimapDepth = GET_INI_BOOL(ini, HUDGroup, HideMinimapDepth);
	Ini::HideMinimapBars = GET_INI_BOOL(ini, HUDGroup, HideMinimapBars);
	Ini::AlwaysHideAbilityBar = GET_INI_BOOL(ini, HUDGroup, AlwaysHideAbilityBar);
	Ini::HideAbilityBarForNonMainCharacters = GET_INI_BOOL(ini, HUDGroup, HideAbilityBarForNonMainCharacters);
	Ini::ReplaceArmourBarWithStamina = GET_INI_BOOL(ini, HUDGroup, ReplaceArmourBarWithStamina);
	Ini::MergeHealthAndArmour = GET_INI_BOOL(ini, HUDGroup, MergeHealthAndArmour);
	Ini::HideHudComponents = GET_INI_BOOL(ini, HUDGroup, HideHudComponents);
	Ini::HudComponents = GET_INI_STRING(ini, HUDGroup, HudComponents);
	Ini::HideWeaponReticle = GET_INI_BOOL(ini, HUDGroup, HideWeaponReticle);
	Ini::HideEnemiesBlips = GET_INI_BOOL(ini, HUDGroup, HideEnemiesBlips);

	/////////////////////////////////////Audio/////////////////////////////////////////////
	Ini::DisableWantedMusic = GET_INI_BOOL(ini, AudioGroup, DisableWantedMusic);
	Ini::DisablePoliceScanner = GET_INI_BOOL(ini, AudioGroup, DisablePoliceScanner);
	Ini::DisableFlyingMusic = GET_INI_BOOL(ini, AudioGroup, DisableFlyingMusic);
	Ini::DisableRadioInterruptions = GET_INI_BOOL(ini, AudioGroup, DisableRadioInterruptions);
	Ini::DefaultVehicleRadioOff = GET_INI_INT(ini, AudioGroup, DefaultVehicleRadioOff);
	Ini::MuteSounds = GET_INI_BOOL(ini, AudioGroup, MuteSounds);
	Ini::Sounds = GET_INI_STRING(ini, AudioGroup, Sounds);
	Ini::DisablePlayerPainAudio = GET_INI_BOOL(ini, AudioGroup, DisablePlayerPainAudio);
	Ini::MuteArtificialAmbientSounds = GET_INI_BOOL(ini, AudioGroup, MuteArtificialAmbientSounds);

	//////////////////////////////////////Peds/////////////////////////////////////////////
	Ini::DisableWrithe = GET_INI_BOOL(ini, pedsGroup, DisableWrithe);
	Ini::DisableHurt = GET_INI_BOOL(ini, pedsGroup, DisableHurt);
	Ini::DisableShootFromGround = GET_INI_BOOL(ini, pedsGroup, DisableShootFromGround);
	Ini::DisableSittingPedsInstantDeath = GET_INI_BOOL(ini, pedsGroup, DisableSittingPedsInstantDeath);
	Ini::DisarmPedWhenShot = GET_INI_BOOL(ini, pedsGroup, DisarmPedWhenShot);
	Ini::DisarmChance = GET_INI_INT(ini, pedsGroup, DisarmChance);
	Ini::DisarmIncludeLeftHand = GET_INI_BOOL(ini, pedsGroup, DisarmIncludeLeftHand);
	Ini::DisablePedOnlyDamagedByPlayer = GET_INI_BOOL(ini, pedsGroup, DisablePedOnlyDamagedByPlayer);
	Ini::DisableDeadPedsJumpOutOfVehicle = GET_INI_BOOL(ini, pedsGroup, DisableDeadPedsJumpOutOfVehicle);
	Ini::DynamicCarJackingReactions = GET_INI_BOOL(ini, pedsGroup, DynamicCarJackingReactions);
	Ini::DisableScenarios = GET_INI_BOOL(ini, pedsGroup, DisableScenarios);
	Ini::DisableWorldPopulation = GET_INI_BOOL(ini, pedsGroup, DisableWorldPopulation);
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

	auto file_deleter = [](FILE* f) { if (f) { fclose(f); } };
	std::unique_ptr<FILE, decltype(file_deleter)> f(fopen(path, "wb"), file_deleter);
	if (f)
	{
		fwrite(data, 1, size, f.get());
		fflush(f.get());
	}
	return;
}