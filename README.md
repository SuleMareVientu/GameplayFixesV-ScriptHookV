# GameplayFixesV-ScriptHookV [![License: GPL v3][GPL-3.0-shield]][GPL-3.0] [![GitHub all releases](https://img.shields.io/github/downloads/SuleMareVientu/GameplayFixesV-ScriptHookV/total?label=Total%20Downloads&logo=GitHub)](https://github.com/SuleMareVientu/GameplayFixesV-ScriptHookV/releases/latest)
![Screenshot](https://github.com/SuleMareVientu/GameplayFixesV-ScriptHookV/blob/images/GameplayFixesV.png?raw=true)
Open-source script that aims to address some of the numerous issues GTA:V has.

This mod is **does not** patch the game directly, and should thus last more without active maintenance. It only makes use of in-game natives.

#### Use the INI to configure it to your liking. All options can be enabled or disabled independently from one another

**Requires [ScriptHookV](http://www.dev-c.com/gtav/scripthookv/)**

## Features
**Input:**
- Reload the INI config with F12 (key can be changed)

**Player:**
- Enable friendly fire
- Disable action mode
- Allow enemies that hit the player's weapon (or hands) to disarm the player, similar to RDR and GTA:IV
- Clean the player's wounds and dirt as he gradually enters the water
- Sprint inside interiors
    - Works in b1180 & later, but doesn't in b372. Don't know in between
- Allow weapons inside the safehouse
    - **Will break some story missions**, but shouldn't otherwise have problems in normal free-roam

**Player Controls:**
- Disable assisted movement (when a player follows a path automatically while moving forward, eg. Floyd's apartment entrance)
- Toggle walking for first person by pressing (and not holding) the sprint button (X on PS / A on XBOX)
- Disable auto-centering of the gameplay camera
- Force the gameplay camera to follow the vehicle when using the handbrake
- Disable the First Person View
    - Requires at least b372
- Disable the idle camera that activates when the player stands still for too long
- Disable the replay recording feature completely
- Disable the player's mobile phone

**Player Vehicle:**
- Disable mid-air car controls and the ability to rotate the player's vehicle while stuck/upside down
- Disable the automatic explosion after the player's vehicle collides with something at high speeds
- Disable engine fire/smoke
- Leave the engine on when exiting a vehicle unless the player keeps the exit vehicle button pressed for 250ms. Exactly like GTA:IV
- Disable the auto-centering of the player vehicle's wheels on exit 
    - While this is universally compatible, **it could cause some issues** with mission vehicles. Disable if necessary
- Keeps the current position of the player vehicle's hydraulics on enter/exit
    - Requires at least b505
- Allow helicopter's rotors to affect the movement of dynamic water bodies, similar to GTA:IV
- Block player from entering ragdoll when standing on car roofs
    - The max speed the player can reach before entering ragdoll is customizable inside the INI
- Prevent the player from flying through the windscreen
- Prevent the player from falling off bikes
- Prevent other peds from dragging the player out of his vehicle
- Disable the forced jump out when entering a water body with a bike (the player will still jump out if completely submerged)
- Disable all stunt jumps around the map

**HUD:**
- Allow the game to continue when the pause menu is active, similar to GTA:Online
    - The game can still be paused by pressing the "ESC" key instead of the "P" key.
- Disable the blur and color filter on the pause menu
- Disable all on-screen effects in menus and HUD (weapon wheel, radio wheel, character switch, etc.)
- Disable the on-screen effects when the player is using a special ability
- Allow the player to switch between normal minimap size and extended, similar to GTA:Online
- Force a specified zoom level of the minimap
- Flatten the minimap view.
- Reveal all the map (hide the "Fog Of War")
- Hide direction and distance indicator under the minimap when a waypoint is set
- Hide the depth indicators under the minimap when inside a submersible
- Hide the health, armour and ability bar under the minimap
    - Requires at least b372
- Show remaining stamina instead of armour in the blue bar below the minimap
- Hide specific HUD component(s)
- Hide the weapon reticle
- Hide enemy's blips from the (mini)map. Also applies to police

**Audio:**
- Disable the dynamic music that plays when wanted
- Disable the police scanner voice that plays when wanted
- Disable the dynamic music that plays when flying
- Disable all non-music radio content (ads, etc.)
- Disable vehicle's radio auto-start (radio will be turned off by default, unless the engine is running)

**Peds:**
- Block peds from entering the "writhe" state and consequently dying
- Block peds from entering the "hurt" state when shot while standing and consequently dying (or entering writhe)
    - Should be enabled in tandem with "DisableWritheWhenInjured", makes shootouts last much longer 
- Force peds to get up instead of shooting when they fall on the ground with their backs
- Make (most) sitting peds behave normally when shot, instead of dying instantly
- Allow enemies that hit the ped's weapon (or hands) to disarm NPCs
- Allow mission peds to be damaged by other entities and not only the player
- Disable dead peds from jumping out of the vehicle they are in

## Changelog:
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
DisableBikeKnockOff, DisableDragOutCar,
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

## Credits:
- The modding community for giving ideas and finding the first methods to circumvent some of the issues listed here
- DurtyFree - [GTA V Data Dumps](https://github.com/DurtyFree/gta-v-data-dumps)
- root-cause - [GTA V Decompiled Scripts](https://github.com/root-cause/v-decompiled-scripts)
- [Grand Theft Auto V Native Database](https://gta5.nativedb.dotindustries.dev/natives)
- [simpleini](https://github.com/brofield/simpleini) (distributed under MIT License)
- Alexander Blade - ScriptHookV

This work is licensed under a
[GNU General Public License v3.0][GPL-3.0].

[GPL-3.0]: https://www.gnu.org/licenses/gpl-3.0
[GPL-3.0-shield]: https://img.shields.io/badge/License-GPL%20v3-blue.svg