#pragma once
#include <Windows.h>
#include <string>
#include <vector>

namespace Ini
{
//Input Settings
extern unsigned long ReloadIniKey;
//Memory Settings
extern bool ApplyExePatches;
extern bool LowPriorityPropsPatch;
extern bool CenterSteeringPatch;
extern bool CopBumpSteeringPatch;
extern bool HUDWheelSlowdownPatch;
extern bool HookGameFunctions;
extern bool EnableLogging;
//Player Settings
extern bool EnableCrouching;
extern bool EnableMidAirLedgeGrab;
extern bool EnablePlayerActionsForAllPeds;
extern bool DisableActionMode;
extern bool DynamicallyCleanWoundsAndDirt;
extern bool SprintInsideInteriors;
extern bool DisablePlayerAutoVaulting;
extern bool DisablePlayerDropDowns;
extern bool SilentWanted;
//Player Weapons
extern bool FriendlyFire;
extern bool LocationalDamage;
extern bool DeadlyPlayerHeadshots;
extern bool EnableWeaponRecoil;
extern bool EnableRecoilInVehicles;
extern float WeaponRecoilGlobalMultiplier;
extern std::vector<float> WeaponRecoilMultipliers;
extern bool DisarmPlayerWhenShot;
extern int DisarmPlayerChance;
extern bool DropPlayerWeaponWhenRagdolling;
extern bool AutoEquipDroppedWeapon;
extern bool DroppedWeaponsBlip;
extern float DroppedWeaponsMaxDistance;
extern bool EnablePlayerNMReactionsWhenShot;
extern int RagdollChance;
extern int MinimumRagdollTime;
extern int MaximumRagdollTime;
extern bool ShouldRagdollInCover;
extern bool DontDropWeapon;
extern bool AllowWeaponsInsideSafeHouse;
//Player Vehicle
extern bool DisableCarMidAirAndRollControl;
extern bool DisableForcedCarExplosionOnImpact;
extern bool DisableEngineSmoke;
extern bool DisableEngineFire;
extern bool LeaveEngineOnWhenExitingVehicles;
extern bool DisableWheelsAutoCenterOnCarExit;
extern bool KeepCarHydraulicsPosition;
extern bool EnableBrakeLightsOnStoppedVehicle;
extern bool EnableHeliWaterPhysics;
extern bool DynamicallyCleanVehicles;
extern bool DisableRagdollOnVehicleRoof;
extern float MaxVehicleSpeed;
extern bool DisableDragOutCar;
extern bool DisableFlyThroughWindscreen;
extern bool DisableBikeKnockOff;
extern bool DisableShallowWaterBikeJumpOut;
extern bool DisableVehicleJitter;
extern bool DisableAirVehicleTurbulence;
extern int DisableAutoEquipHelmets;
extern bool DisableStuntJumps;
//Player Controls
extern bool DisableAssistedMovement;
extern bool ReplaceRadioTuneForwardWithRadioOff;
extern int DisableCameraAutoCenter;
extern bool ToggleFPSWalking;
extern bool CamFollowVehicleDuringHandbrake;
extern int CamFollowVehDelay;
extern bool DisableFirstPersonView;
extern bool DisableIdleCamera;
extern bool DisableRecording;
extern bool DisableMobilePhone;
//HUD
extern bool AllowGameExecutionOnPauseMenu;
extern bool DisablePauseMenuPostFX;
extern bool DisableHUDPostFX;
extern bool DisableSpecialAbilityPostFX;
extern bool EnableBigMapToggle;
extern bool MinimapSpeedometer;
extern float SetRadarZoom;
extern bool DisableMinimapTilt;
extern bool HideMinimapFog;
extern bool HideMinimapSatNav;
extern bool HideMinimapDepth;
extern bool HideMinimapBars;
extern bool AlwaysHideAbilityBar;
extern bool HideAbilityBarForNonMainCharacters;
extern bool ReplaceArmourBarWithStamina;
extern bool MergeHealthAndArmour;
extern bool HideHudComponents;
extern std::string HudComponents;
extern bool HideWeaponReticle;
extern bool HideEnemiesBlips;
//Audio
extern bool DisableWantedMusic;
extern bool DisablePoliceScanner;
extern bool DisableFlyingMusic;
extern bool DisableRadioInterruptions;
extern int DefaultVehicleRadioOff;
extern bool MuteSounds;
extern std::string Sounds;
extern bool DisablePlayerPainAudio;
extern bool MuteArtificialAmbientSounds;

//Peds Settings
extern bool DynamicCarJackingReactions;
extern bool EnableShootingJackedPeds;
extern bool EnablePedShove;
extern int PedUmbrellas;
extern bool DisableScenarios;
extern bool DisableWorldPopulation;

//Peds Combat
extern bool DisableWrithe;
extern bool DisableHurt;
extern bool DisableShootFromGround;
extern bool DisableSittingPedsInstantDeath;
extern bool DisablePedOnlyDamagedByPlayer;
extern bool DisableDeadPedsJumpOutOfVehicle;
extern bool DeadlyNPCsHeadshots;
extern bool DisarmPedWhenShot;
extern int DisarmChance;
extern bool DisarmIncludeLeftHand;

//Peds Accuracy
extern bool EnablePedsAccuracyOptions;
extern int PedAccuracyMode;
extern int MinAccuracy;
extern int MaxAccuracy;
extern int PedShootRateMode;
extern int MinShootRate;
extern int MaxShootRate;
extern float PedGlobalWeaponDamageModifier;
extern float PedGlobalMeleeWeaponDamageModifier;
}

void ReadINI();
void WriteINIResource(HINSTANCE hInstance, int resourceID, const char* path);
std::vector<float> ParseFloats(const std::string& s);