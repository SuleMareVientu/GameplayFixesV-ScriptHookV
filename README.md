# GameplayFixesV-ScriptHookV [![CC BY-NC-SA 4.0][cc-by-nc-sa-shield]][cc-by-nc-sa]
![Screenshot](https://github.com/SuleMareVientu/GameplayFixesV-ScriptHookV/blob/images/GameplayFixesV.png?raw=true)
Open-source script that aims to address some of the numerous issues GTA:V has.

This mod is **does not** patch the game directly, and should thus last more without active maintenance. It only makes use of in-game natives.

#### Use the INI to configure it to your liking. All options can be enabled or disabled independently from one another

**Requires [ScriptHookV](http://www.dev-c.com/gtav/scripthookv/)**

**v1.1** - Changelog:
- DisarmPlayerWhenShot / DisarmPedWhenShot - Allows enemies that hit the player's weapon (or hands) to disarm the player, similar to RDR and GTA:IV. Adjust DisarmChance and DisarmIncludeLeftHand at your liking. Inspired by jedijosh920's [Disarm](https://www.gta5-mods.com/scripts/disarm)
- ToggleFPSWalking - Allow player to toggle between jogging and walking in FPS mode. Inspired by chinagreenelvis's [Walk Toggle](https://www.gta5-mods.com/scripts/walk-toggle)
- DisableEngineFire - Disables engine fires when engine healts reaches below 0. Also disables automatic vehicle explosion after engine healt reaches -4000
- DisableEngineSmoke - Same as DisableEngineFire, but the threshold is set to 400. Prevents the engine from smoking and catching fire
- DisableRecording - Disables the replay recording feature completely
**Fixes:**
- Improved friendly fire code allowing player to perform melee lockon on friendly peds
- Allow player to still have control of wheel steering when vehicle is stuck
- Allow player to switch characters while inside the safehouse (AllowWeaponsInsideSafeHouse is now disabled by default)
- Made the LeaveEngineOnWhenExitingVehicles option more similar to GTA:IV's

## Features
**Player:**
- Enable friendly fire
- Allow player to jack friendly peds vehicles
- Sprint inside interiors
    - Works in b1180 & later, but doesn't in b372. Don't know in between
- Allow weapons inside safehouse
    -  **Could (and probably will) break some story missions**, but shouldn't otherwise have problems in normal freeroam

**Player Vehicle:**
- Disable mid-air car controls and the ability to rotate the player's vehicle while stuck/upside down
- Disable the automatic explosion after the players vehicle collides with something at high speeds
- Leave engine on when exiting vehicle unless the player keeps the exit vehicle button pressed for 250ms. Similar to GTA:IV
- Disable the auto-centering of the player vehicle's wheels on exit. 
    - While this is universally compatible, **it could cause some issues** with mission vehicles. Disable if necessary
- Block player from entering ragdoll when standing on car roofs
    - The max speed the player can reach before entering ragdoll is customizable inside the INI
- Disable the forced jump out when entering a water body with a bike (player will still jump out if completely submerged)

**Peds:**
- Block peds from entering the "writhe" state and consequently dying
- Blocks peds from entering the "hurt" state when shot while standing and consequently dying (or entering writhe)
    - Should be enabled in tandem with "DisableWritheWhenInjured", makes shootouts last much longer 
- Allow mission peds to be damaged by other entities' and not only the player
- Make (most) sitting peds behave normally when shot, instead of dying instantly
- Disable dead peds from jumping out of the vehicle they are in

## Credits:
- The modding community for giving ideas and finding the first methods to circument some of the issues listed here
- DurtyFree - [GTA V Data Dumps](https://github.com/DurtyFree/gta-v-data-dumps)
- root-cause - [GTA V Decompiled Scripts](https://github.com/root-cause/v-decompiled-scripts)
- [Grand Theft Auto V Native Database](https://gta5.nativedb.dotindustries.dev/natives)
- [simpleini](https://github.com/brofield/simpleini) (distributed under MIT License)
- Alexander Blade - ScriptHookV

This work is licensed under a
[Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License][cc-by-nc-sa].

[![CC BY-NC-SA 4.0][cc-by-nc-sa-image]][cc-by-nc-sa]

[cc-by-nc-sa]: http://creativecommons.org/licenses/by-nc-sa/4.0/
[cc-by-nc-sa-image]: https://licensebuttons.net/l/by-nc-sa/4.0/88x31.png
[cc-by-nc-sa-shield]: https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg
