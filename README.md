# GameplayFixesV-ScriptHookV [![License: GPL v3][GPL-3.0-shield]][GPL-3.0] [![GitHub all releases](https://img.shields.io/github/downloads/SuleMareVientu/GameplayFixesV-ScriptHookV/total?label=Total%20Downloads&logo=GitHub)](https://github.com/SuleMareVientu/GameplayFixesV-ScriptHookV/releases/latest)
![Screenshot](https://github.com/SuleMareVientu/GameplayFixesV-ScriptHookV/blob/images/GameplayFixesV.png?raw=true)
Open-source script that aims to address some of the numerous issues GTA:V has.

The advantages of using this mod are:
  - It's compatible with **both Legacy and Enhanced**.
    - It's tested on Legacy b1180 & b3521, Enhanced b812.
    - It's theoretically compatible with every game version since b335 (ScriptHook base).  
  - Functionalities are **thoroughly tested**, and blend-in with the game as if R* implemented them.
  - It's **faster** than its C#/.NET counterparts because it's written in **CPP**, with great attention at keeping it performant.
  - It merges functionalities from many other mods, meaning that mods can now share resources (e.g., entity pools), making the code faster and allowing **cross-compatibility** between different options.
  - Almost all functionality **works out-of-the-box**, without the need to patch the game's memory. Options that do so are clearly labelled, complexity is not hidden from the user.
  - The lack of reliance on ScriptHookV .NET (which is essentially a fancy wrapper for normal ScriptHook) means **less compatibility issues** across game versions.
  - It's **open-source**.

To my knowledge it's also the only mod that fixes the "Low Priority Props" bug in Enhanced and implements NM reactions without relying on ScriptHookV .NET.

#### Use the INI to configure it to your liking. All options can be enabled or disabled independently from one another.

**Requires [ScriptHookV](http://www.dev-c.com/gtav/scripthookv/)**

## Features
**Input:**
- Reload the INI config with F12 (key can be changed).

**Memory:**
- Allow low priority props to be spawned around the map.
    - **It's the only memory patch compatible with Enhanced (currently).**
- Disable the automatic centering of the wheels when a ped exits a vehicle. Applies to all vehicles (not only the player's).
    - If the normal option (DisableWheelsAutoCenterOnCarExit) is enabled, this will override it. If the patch fails and the normal option it's also enabled, it will be used as a fallback.
- Disable cops from randomly changing the player's vehicle steering angle while ramming during a chase.
- Disable the slowdown and screen effects triggered by the weapon and the radio wheel.

**Player:**
- Enable the unused crouched motion state.
- Allow the player to grab onto nearby ledges when jumping. Useful for emulating parkour movement.
    - Keep pressing the "Jump" button (space bar/'X' on controller) when in the air to automatically grab ledges that are in front of the player.
- Allow all ped models to use stealh and action mode.
- Disable action mode.
- Clean the player's wounds and dirt as he gradually enters the water and after picking up a medkit.
- Sprint inside interiors.
    - Works in b1180 & later, but doesn't in b372. Don't know in between.
- Disable auto-vaulting small objects.
    - Vaulting can still be performed manually by pressing the 'Jump' control near the objects.
- Disable automatic drop-down animations when near a ledge at high heights.
- Silent wanted (hide all HUD/music related to the wanted system).

**Player Damage/Weapons:**
- Enable friendly fire.
- Locational damage (e.g., armor won't protect your head, arms and lower body).
    - Headshots can be made fatal for the player (just like NPCs).
- Configurable weapon recoil.
    - Togglable inside vehicles.
    - Separate recoil multipliers for weapon types (pistols, shotguns...) and addons (grips, silencers...).
- Player disarm (allow enemies that hit the player's weapon (or hands) to disarm the player, similar to RDR and GTA:IV).
    - Definable custom 'drop chance' when hit.
    - The weapon will retain all of its customizations (attachments, tints...).
    - Weapon can be auto-equipped after being collected.
    - When dropped, the weapon can be blipped on the minimap.
    - Definable max distance after which a dropped weapon is despawned.
- Drop player weapon when ragdolling.
    - Compatibility with custom NM reactions (read below).
    - The weapon will retain all of its customizations (attachments, tints...).
    - Weapon can be auto-equipped after being collected.
    - When dropped, the weapon can be blipped on the minimap.
    - Definable max distance after which a dropped weapon is despawned.
- Player Euphoria bullet reactions (the player will react to bullets with Euphoria like NPCs)
    - **Not compatible with Enhanced.**
    - Definable 'reaction chance' when hit.
    - Definable minimum and maximum time for reaction.
    - Togglable when in cover.
    - Compatibility with other options that drop the player's weapon (so that the bullet reactions do not drop the player's weapon).
- Allow weapons inside the safehouse.
    - **Will break some story missions**, but shouldn't otherwise have problems in normal free-roam.

**Player Vehicle:**
- Disable mid-air car controls and the ability to rotate the player's vehicle while stuck/upside down.
- Disable the automatic explosion after the player's vehicle collides with something at high speeds.
- Disable engine fire/smoke.
- Leave the engine on when exiting a vehicle unless the player keeps the exit vehicle button pressed for 250ms. Exactly like GTA:IV.
- Disable the auto-centering of the player vehicle's wheels on exit.
    - While this is universally compatible, **it could cause some issues** with mission vehicles. Disable if necessary.
- Keeps the current position of the player vehicle's hydraulics on enter/exit.
    - **Requires at least b2372**.
- Turn on brake lights while the player's vehicle is stopped.
- Helicopter water physics (allow helicopter's rotors to affect the movement of dynamic water bodies, similar to GTA:IV).
- Dynamically clean vehicles (clean dirt and decals (e.g., blood) from the vehicles near the player, including his, while raining).
    - Depends on the amount of rain and the speed of the vehicle.
    - Vehicles can also be cleaned by entering rivers or running along the seaside while partially in the water.
- Disable ragdolls on vehicle roof.
    - Customizable max speed the vehicle the player is standing on before making the player enter ragdoll.
- Prevent other peds from dragging the player out of his vehicle.
- Prevent the player from flying through the windscreen.
- Prevent the player from falling off bikes.
- Disable the forced jump out when entering a water body with a bike (the player will still jump out if completely submerged).
- Disable vehicle jitter effect.
- Disable air vehicle turbulence.
- Disable auto-equip helmets.
    - Togglable on bikes/helicopters.
- Disable all stunt jumps around the map

**Player Controls:**
- Disable assisted movement (when a player follows a path automatically while moving forward, eg. Floyd's apartment entrance).
- Quickly turn off the radio by tapping the radio control button ('Q'/left arrow on controller).
- Toggle walking for first person by pressing (and not holding) the sprint button (X on PS / A on XBOX).
- Disable auto-centering of the gameplay camera.
    - Togglable on foot/in vehicle/always.
- Force the gameplay camera to follow the vehicle when using the handbrake
    - Customizable handbrake delay before centering the camera on the vehicle.
- Disable First Person View.
- Disable the idle camera (it activates when the player stands still for too long).
- Disable the replay recording feature completely.
- Disable the player's mobile phone.
    - **Obviously brakes story missions**.

**HUD:**
- Allow the game to continue when the pause menu is active, similar to GTA:Online.
    - The game can still be paused by pressing the "ESC" key instead of the "P" key.
- Disable the blur and color filter on the pause menu.
- Disable all on-screen effects in menus and HUD (weapon wheel, radio wheel, character switch, etc.).
- Disable the on-screen effects when the player is using a special ability.
- Allow the player to switch between normal minimap size and extended, similar to GTA:Online.
- Enable a non-intrusive, R*-like, speedometer on the minimap.
- Force a specified zoom level of the minimap.
- Flatten the minimap view.
- Reveal all the map (hide the "Fog Of War").
- Hide direction and distance indicator under the minimap when a waypoint is set.
- Hide the depth indicators under the minimap when inside a submersible.
- Hide the health, armour and ability bar under the minimap.
    - Overrides all other 'minimap bars' options.
- Show remaining stamina instead of armour in the blue bar below the minimap.
- Hide specific HUD component(s).
- Hide the weapon reticle.
- Hide enemy's blips from the (mini)map. Also applies to police.

**Audio:**
- Disable the dynamic music that plays when wanted.
- Disable the police scanner voice that plays when wanted.
    - The scanner still warks while in police vehicles.
- Disable the dynamic music that plays when flying.
- Disable all non-music radio content (ads, etc.).
- Disable vehicle's radio auto-start. Different modes available:
    - Vehicles will start with the radio turned off by default, unless the engine is already running (e.g. when jacking a car).
    - Radio will always be turned off when first entering a vehicle, regardless of engine state.
- Mute specific sound categories.
- Mute artificial ambient sounds (read INI for a deeper explanation).
- Disable player pain audio.

**Peds:**
- Dynamic car jacking reactions (allow peds to dynamically react to the player pointing a gun at their vehicle, just like in GTA:IV). Some will instantly flee, others will react and there's also a chance they'll still run you over.
- Shoot jacked peds (allow the player to kill the driver while jacking a vehicle with a handgun).
    - Weapon jacking animations trigger only with non-customized weapons (e.g. silencers won't work).
- Allow the player to push (shove) peds out of his way, like in GTA:IV.
    - Press the "Jump" button (space bar/'X' on controller) when standing in front of a ped to activate.
- Allow peds to use umbrellas during rainy weather.
    - Definable max number of peds allowed to equip umbrellas.
- Disable all peds scenarios.
- Disable all dynamic population of the game world.

**Peds Combat:**
- Disable writhe state (block peds from entering the "writhe" state and consequently dying).
- Disable hurt state (block peds from entering the "hurt" state when shot while standing and consequently dying, or entering writhe).
- Disable shoot from ground (force peds to get up instead of shooting when they fall on the ground with their backs).
- Disable sitting peds instant death.
- Allow mission peds to be damaged by other entities and not only the player.
- Disable dead peds from jumping out of the vehicle they are in.
- Deadly NPCs headshots (headshots between NPCs will be deadly, just like when hit by the player).
- Ped disarm (allows the player to disarm peds when they are hit on the hand/weapon).
    - Definable custom 'drop chance' when hit.

**Peds Accuracy:**
- Configurable ped accuracy. Modes available:
    - Global: all peds will have the same accuracy.
    - Random: peds will have random accuracy inside the custom-defined range.
    - Clamp: ensure the default accuracy of a ped is inside the custom-defined range.
    - Proportional: scale the ped's default accuracy value inside the the custom-defined range.
    - More info inside the INI.
- Configurable ped shoot rate. 
    - The modes are the same for the option defined right above, excluding "Clamp" and "Proportional" modes.
- Global damage modifier for peds (both gun/melee weapons).

## Credits:
- The modding community for giving ideas and finding the first methods to circumvent some of the issues listed here. Special thanks to (for patches/inspiration):
    - [aint-no-other-option](https://www.gta5-mods.com/users/no_option)
    - [CamxxCore](https://www.gta5-mods.com/users/CamxxCore)
    - [Eddlm](https://www.gta5-mods.com/users/Eddlm)
    - [EnforcerZhukov](https://www.gta5-mods.com/users/EnforcerZhukov)
    - [jedijosh920](https://www.gta5-mods.com/users/jedijosh920)
  
- DurtyFree - [GTA V Data Dumps](https://github.com/DurtyFree/gta-v-data-dumps)
- ScriptHookV .NET (for offsets) - [ScriptHookV .NET](https://github.com/scripthookvdotnet/scripthookvdotnet)
- Cfx.re (for offsets) - [FiveM](https://github.com/citizenfx/fivem)
- root-cause - [GTA V Decompiled Scripts](https://github.com/root-cause/v-decompiled-scripts)
- [Grand Theft Auto V Native Database](https://gta5.nativedb.dotindustries.dev/natives)
- Alexander Blade - ScriptHookV
- Libraries (distributed under MIT License):
    - [simpleini](https://github.com/brofield/simpleini)
    - [json](https://github.com/nlohmann/json)
    - [Pattern16](https://github.com/Dasaav-dsv/Pattern16)
    - [random](https://github.com/ilqvya/random)

This work is licensed under a
[GNU General Public License v3.0][GPL-3.0].

[GPL-3.0]: https://www.gnu.org/licenses/gpl-3.0
[GPL-3.0-shield]: https://img.shields.io/badge/License-GPL%20v3-blue.svg
