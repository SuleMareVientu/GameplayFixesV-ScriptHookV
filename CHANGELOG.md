### **v3.0**
**Added:**  
- Add memory patching options without hiding complexity to the user. These types of patches are known to break easily across different game versions.  
- Implement basic logging for errors.  
- Implement checks for current game version, preventing crashes and allowing some options to work on older versions of the game (with some functionality disabled). This mod is now officially compatible with every game version ScriptHookV supports.  
- Only less than 10% of the options available are incompatible with the Enhanced version of the game. It's always stated when an option is incompatible with Enhanced.
- **Memory:**  
  - LowPriorityPropsPatch, CenterSteeringPatch, CopBumpSteeringPatch, HUDWheelSlowdownPatch  
- **Player:**  
  - EnableCrouching, SilentWanted, LocationalDamage, DeadlyPlayerHeadshots, EnableWeaponRecoil, DropPlayerWeaponWhenRagdolling, EnablePlayerNMReactionsWhenShot, EnableBrakeLightsOnStoppedVehicle, DynamicallyCleanVehicles, DisableVehicleJitter, DisableAirVehicleTurbulence, DisableAutoEquipHelmets  
- **HUD:**  
  - MinimapSpeedometer  
- **Audio:**  
  - MuteSounds, MuteArtificialAmbientSounds, DisablePlayerPainAudio
- **Peds:**  
  - DynamicCarJackingReactions, EnableShootingJackedPeds, PedUmbrellas, DisableScenarios, DisableWorldPopulation, DeadlyNPCsHeadshots, PedAccuracyMode, PedShootRateMode, PedGlobalWeaponDamageModifier, PedGlobalMeleeWeaponDamageModifier  
   
**Improved:**  
- General refactoring 
- ReloadIniKey, DisableWheelsAutoCenterOnCarExit, KeepCarHydraulicsPosition, DisarmPlayerWhenShot, EnablePlayerActionsForAllPeds, DynamicallyCleanWoundsAndDirt, FriendlyFire, DisableFirstPersonView

### **v2.0**
**Added:**  
- ReloadIniKey, FiveM support resource files  
- **Player:**  
  - DisableActionMode, CleanWoundsAndDirtInWater  
- **Player Control:**  
  - DisableAssistedMovement, DisableCameraAutoCenter, DisableFirstPersonView,
DisableIdleCamera  
- **Player Vehicle:**  
  - KeepCarHydraulicsPosition, EnableHeliWaterPhysics, DisableFlyThroughWindscreen, 
DisableBikeKnockOff, DisableDragOutCar
- **HUD:**  
  - AllowGameExecutionOnPauseMenu, DisablePauseMenuPostFX, DisableHUDPostFX, 
DisableSpecialAbilityPostFX, EnableBigMapToggle, SetRadarZoom, 
DisableMinimapTilt, HideMinimapFog, HideMinimapBars, 
HideAbilityBarForNonMainCharacters, AlwaysHideAbilityBar, 
ReplaceArmourBarWithStamina, HideMinimapSatNav, HideMinimapDepth, 
HideHudComponents, HideWeaponReticle, HideEnemiesBlips  
- **Audio:**  
  - DisableWantedMusic, DisablePoliceScanner, DisableFlyingMusic,
DisableRadioInterruptions, DefaultVehicleRadioOff  
- **Peds:**  
  - DisableShootFromGround  
   
**Improved:**  
- DisableWheelsAutoCenterOnCarExit, DisarmPedWhenShot, DisableMobilePhone, 
LeaveEngineOnWhenExitingVehicles  

**Removed:**  
- PlayerCanJackFriendlyPeds

### **v1.2**
- CamFollowVehicleDuringHandbrake / CamFollowVehDelay - Force gameplay camera to follow the vehicle when using the handbrake
- DisableStuntJumps - Disables all stunt jumps around the map
- DisableMobilePhone - Disables the player's mobile phone  
**Fixes:**  
DisableEngineSmoke and DisableEngineFire now work as intended

### **v1.1**
- DisarmPlayerWhenShot / DisarmPedWhenShot - Allows enemies that hit the player's weapon (or hands) to disarm the player, similar to RDR and GTA:IV. Adjust DisarmChance and DisarmIncludeLeftHand to your liking. Inspired by jedijosh920's [Disarm](https://www.gta5-mods.com/scripts/disarm)
- ToggleFPSWalking - Allow player to toggle between jogging and walking in FPS mode. Inspired by chinagreenelvis's [Walk Toggle](https://www.gta5-mods.com/scripts/walk-toggle)
- DisableEngineFire - Disables engine fires when engine health reaches below 0. Also disables automatic vehicle explosion after engine health reaches -4000
- DisableEngineSmoke - Same as DisableEngineFire, but the threshold is 400. It prevents the engine from smoking and catching fire
- DisableRecording - Disables the replay recording feature completely  
**Fixes:**  
- Improved friendly fire code allowing the player to perform melee lock on friendly peds
- Allow player to have control of wheel steering still when a vehicle is stuck
- Allow player to switch characters while inside the safehouse (AllowWeaponsInsideSafeHouse is now disabled by default)
- Made the LeaveEngineOnWhenExitingVehicles option more similar to GTA:IV's