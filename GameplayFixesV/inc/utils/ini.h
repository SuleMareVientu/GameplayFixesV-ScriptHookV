#pragma once
#include <Windows.h>
#include <string>

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
extern bool FriendlyFire;
extern bool EnablePlayerActionsForAllPeds;
extern bool DisableActionMode;
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
extern bool DynamicallyCleanWoundsAndDirt;
extern bool SprintInsideInteriors;
extern bool AllowWeaponsInsideSafeHouse;
extern bool SilentWanted;
//Player Controls
extern bool DisableAssistedMovement;
extern int DisableCameraAutoCenter;
extern bool ToggleFPSWalking;
extern bool CamFollowVehicleDuringHandbrake;
extern int CamFollowVehDelay;
extern bool DisableFirstPersonView;
extern bool DisableIdleCamera;
extern bool DisableRecording;
extern bool DisableMobilePhone;
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
extern bool DisableWrithe;
extern bool DisableHurt;
extern bool DisableShootFromGround;
extern bool DisableSittingPedsInstantDeath;
extern bool DisarmPedWhenShot;
extern int DisarmChance;
extern bool DisarmIncludeLeftHand;
extern bool DisablePedOnlyDamagedByPlayer;
extern bool DisableDeadPedsJumpOutOfVehicle;
extern bool DynamicCarJackingReactions;
extern bool EnableShootingJackedPeds;
extern bool DisableScenarios;
extern bool DisableWorldPopulation;
}

void ReadINI();
void WriteINIResource(HINSTANCE hInstance, int resourceID, const char* path);