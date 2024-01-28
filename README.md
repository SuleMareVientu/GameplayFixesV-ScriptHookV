# GameplayFixesV-ScriptHookV [![CC BY-NC-SA 4.0][cc-by-nc-sa-shield]][cc-by-nc-sa]
![Screenshot](https://github.com/SuleMareVientu/GameplayFixesV-ScriptHookV/blob/images/GameplayFixesV.png?raw=true)
Open-source script that aims to address some of the numerous issues GTA:V has.

This mod is **does not** patch the game directly, and should thus last more without active maintenance. It only makes use of in-game natives.

#### Use the INI to configure it to your liking. All options can be enabled or disabled independently from one another

**Requires [ScriptHookV](http://www.dev-c.com/gtav/scripthookv/)**

## Features
**Player:**
- Enable friendly fire
- Allow player to jack friendly peds vehicles
- Sprint inside interiors
    - Works in b1180 & later, but doesn't in b372. I don't know in between
- Allow weapons inside the safehouse
    -  **Could (and probably will) break some story missions**, but shouldn't otherwise have problems in normal free-roam

**Player Vehicle:**
- Disable mid-air car controls and the ability to rotate the player's vehicle while stuck/upside down
- Disable the automatic explosion after the player's vehicle collides with something at high speeds
- Leave the engine on when exiting the vehicle if the player keeps the exit vehicle button pressed for 200ms. Similar to GTA:IV
- Disable the auto-centering of the player vehicle's wheels on exit. 
    - While this is universally compatible, **it could cause some issues** with mission vehicles. Disable if necessary
- Block player from entering ragdoll when standing on car roofs
    - The max speed the player can reach before entering ragdoll is customizable inside the INI
- Disable the forced jump out when entering a water body with a bike (the player will still jump out if completely submerged)

**Peds:**
- Block peds from entering the "writhe" state and consequently dying
- Blocks peds from entering the "hurt" state when shot while standing and consequently dying (or entering writhe)
    - Should be enabled in tandem with "DisableWritheWhenInjured", makes shootouts last much longer 
- Allow mission peds to be damaged by other entities and not only the player
- Make (most) sitting peds behave normally when shot, instead of dying instantly
- Disable dead peds from jumping out of the vehicle they are in

## Credits:
- The modding community for giving ideas and finding the first methods to circumvent some of the issues listed here
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
