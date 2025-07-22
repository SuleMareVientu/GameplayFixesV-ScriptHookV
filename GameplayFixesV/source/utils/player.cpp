//ScriptHook
#include <shv\natives.h>
#include <shv\types.h>

//std
#include <sstream>
#include <math.h>
#include <algorithm>
#include <random>
#include <numeric>
#include <set>
#include <fstream>
#include <functional>
#include <vector>
#include <memory>

//Custom
#include "utils\player.h"
#include "utils\functions.h"
#include "utils\keyboard.h"
#include "utils\ini.h"
#include "utils\peds.h"

// Globals
bool isPlayerCrouching = false;
bool GetIsPlayerCrouching() { return isPlayerCrouching; }

/////////////////////////////////////////////Player//////////////////////////////////////////////
namespace nGeneral
{
bool CanCrouch(Ped ped)
{
	if (!DOES_ENTITY_EXIST(ped) || IS_ENTITY_DEAD(ped, false) || IS_PED_DEAD_OR_DYING(ped, true) ||
		IS_PED_INJURED(ped) || IS_PED_USING_ANY_SCENARIO(ped) || IS_PED_RAGDOLL(ped) ||
		IS_PED_GETTING_UP(ped) || IS_PED_FALLING(ped) || IS_PED_JUMPING(ped) ||
		IS_PED_DIVING(ped) || IS_PED_SWIMMING(ped) || IS_PED_GOING_INTO_COVER(ped) ||
		IS_PED_CLIMBING(ped) || IS_PED_VAULTING(ped) || IS_PED_HANGING_ON_TO_VEHICLE(ped) ||
		IS_PED_IN_ANY_VEHICLE(ped, true) || IS_PED_IN_COVER(ped, false) || !IS_PED_ON_FOOT(ped) ||
		IS_PED_TAKING_OFF_HELMET(ped) || GET_ENTITY_SUBMERGED_LEVEL(ped) >= 0.7f || IS_PED_PERFORMING_MELEE_ACTION(ped))
		return false;

	return true;
}

// Ensure that movement clipsets are loaded before applying them
constexpr char* crouchedMovementClipSet = "move_ped_crouched";
constexpr char* crouchedStrafingClipSet = "move_ped_crouched_strafing";
constexpr float blendSpeedCrouched = 0.55f;
void SetCrouch(Ped ped, bool state)
{
	if (state)
	{
		DisablePedConfigFlag(ped, PCF_OpenDoorArmIK);
		EnablePedConfigFlag(ped, PCF_PhoneDisableTextingAnimations);
		EnablePedConfigFlag(ped, PCF_PhoneDisableTalkingAnimations);
		EnablePedConfigFlag(ped, PCF_PhoneDisableCameraAnimations);

		isPlayerCrouching = true;
		SET_PED_MOVEMENT_CLIPSET(ped, crouchedMovementClipSet, blendSpeedCrouched);
		SET_PED_STRAFE_CLIPSET(ped, crouchedStrafingClipSet);
	}
	else
	{
		isPlayerCrouching = false;
		SET_PED_STEALTH_MOVEMENT(ped, false, NULL);
		RESET_PED_MOVEMENT_CLIPSET(ped, blendSpeedCrouched);
		RESET_PED_STRAFE_CLIPSET(ped);
		EnablePedConfigFlag(ped, PCF_OpenDoorArmIK);
		DisablePedConfigFlag(ped, PCF_PhoneDisableTextingAnimations);
		DisablePedConfigFlag(ped, PCF_PhoneDisableTalkingAnimations);
		DisablePedConfigFlag(ped, PCF_PhoneDisableCameraAnimations);
		SET_PED_CAN_PLAY_GESTURE_ANIMS(ped, true);
		SET_PED_CAN_PLAY_AMBIENT_ANIMS(ped, true);
		SET_PED_CAN_PLAY_AMBIENT_BASE_ANIMS(ped, true);
		SET_PLAYER_NOISE_MULTIPLIER(GetPlayer(), 1.0f);
	}
	return;
}

Timer timerCrouch;
constexpr int crouchHold = 250;
bool stealthState = false;
void EnableCrouching()
{
	if (!RequestClipSet(crouchedMovementClipSet) || !RequestClipSet(crouchedStrafingClipSet))
		return;

	if (!IS_PED_HUMAN(GetPlayerPed()))
		return;

	if (IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_DUCK))
	{
		if (isPlayerCrouching)
			SetCrouch(GetPlayerPed(), false);
		else
		{
			timerCrouch.Reset();
			if (GET_PED_STEALTH_MOVEMENT(GetPlayerPed()))
			{
				stealthState = true;
				SET_PED_STEALTH_MOVEMENT(GetPlayerPed(), false, NULL);	// Fixes bug when crouching near objects where the player would not be able to stand up
			}
		}
	}
	else if (IS_CONTROL_JUST_RELEASED(PLAYER_CONTROL, INPUT_DUCK) && Between(timerCrouch.Get(), 0, crouchHold) && stealthState)
	{
		stealthState = false;
		// Enable stealth mode if control is released early and ensure compatibility with non-story peds
		if (IsPedMainProtagonist(GetPlayerPed()))
			SET_PED_STEALTH_MOVEMENT(GetPlayerPed(), true, NULL);
		else if (Ini::EnablePlayerActionsForAllPeds)
			SET_PED_STEALTH_MOVEMENT(GetPlayerPed(), true, "DEFAULT_ACTION");
	}
	else if (IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_DUCK) && timerCrouch.Get() > crouchHold)
	{
		timerCrouch.Set(INT_MIN);
		if (!isPlayerCrouching && CanCrouch(GetPlayerPed()))
			SetCrouch(GetPlayerPed(), true);
	}

	if (isPlayerCrouching)
	{
		if (!CanCrouch(GetPlayerPed()))
			SetCrouch(GetPlayerPed(), false);
		else
		{
			EnablePedResetFlag(GetPlayerPed(), PRF_DisableActionMode);
			EnablePedResetFlag(GetPlayerPed(), PRF_DontUseSprintEnergy);
			EnablePedResetFlag(GetPlayerPed(), PRF_ScriptDisableSecondaryAnimationTasks);
			EnablePedResetFlag(GetPlayerPed(), PRF_DisableDustOffAnims);
			EnablePedResetFlag(GetPlayerPed(), PRF_DisableWallHitAnimation);

			SET_PED_CAN_PLAY_AMBIENT_IDLES(GetPlayerPed(), true, true);	// Resets every frame
			SET_PED_CAN_PLAY_GESTURE_ANIMS(GetPlayerPed(), false);
			SET_PED_CAN_PLAY_AMBIENT_ANIMS(GetPlayerPed(), false);
			SET_PED_CAN_PLAY_AMBIENT_BASE_ANIMS(GetPlayerPed(), false);

			SET_PLAYER_NOISE_MULTIPLIER(GetPlayer(), 0.0f);
			DISABLE_ON_FOOT_FIRST_PERSON_VIEW_THIS_UPDATE();

			if (IsPlayerAiming(true, true) || IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_ATTACK) || IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_ATTACK2))
				SET_PED_MAX_MOVE_BLEND_RATIO(GetPlayerPed(), 0.25f);
			else
				SET_PED_MAX_MOVE_BLEND_RATIO(GetPlayerPed(), PEDMOVEBLENDRATIO_RUN);
		}
	}

	// Ensure action compatibility with non-story peds. THIS MUST BE AT THE BOTTOM
	if (Ini::EnablePlayerActionsForAllPeds && !IsPedMainProtagonist(GetPlayerPed()) && !GetIsPlayerCrouching())
	{
		if (!GET_PED_STEALTH_MOVEMENT(GetPlayerPed()) && !stealthState)
		{
			if (IS_PED_IN_MELEE_COMBAT(GetPlayerPed()) || COUNT_PEDS_IN_COMBAT_WITH_TARGET(GetPlayerPed()) > 0 || IS_PED_SHOOTING(GetPlayerPed()))
				SET_PED_USING_ACTION_MODE(GetPlayerPed(), true, 10000, "DEFAULT_ACTION");
		}
	}
	return;
}

void EnablePlayerActionsForAllPeds()
{
	if (Ini::EnableCrouching || !IS_PED_HUMAN(GetPlayerPed()) || DOES_ENTITY_EXIST(GetVehiclePedIsUsing(GetPlayerPed())))
		return;

	if (!IsPedMainProtagonist(GetPlayerPed()))
	{
		if (GET_PED_STEALTH_MOVEMENT(GetPlayerPed()))
			SET_PED_STEALTH_MOVEMENT(GetPlayerPed(), true, "DEFAULT_ACTION");
		else if (IS_PED_IN_MELEE_COMBAT(GetPlayerPed()) || COUNT_PEDS_IN_COMBAT_WITH_TARGET(GetPlayerPed()) > 0 || IS_PED_SHOOTING(GetPlayerPed()))
			SET_PED_USING_ACTION_MODE(GetPlayerPed(), true, 10000, "DEFAULT_ACTION");
	}
	return;
}

inline void DisableActionMode() { EnablePedResetFlag(GetPlayerPed(), PRF_DisableActionMode); return; }

constexpr int timeClearDirtDecal = 120000;	//2min
Timer timerDirtDecal(timeClearDirtDecal);
int oldHealthWounds = 0;
void DynamicallyCleanWoundsAndDirt()
{
	if (IS_PLAYER_SWITCH_IN_PROGRESS())
	{
		timerDirtDecal.Set(120000);
		return;
	}

	const float subLevel = GET_ENTITY_SUBMERGED_LEVEL(GetPlayerPed());
	if (subLevel >= 0.3f)
	{
		CLEAR_PED_BLOOD_DAMAGE_BY_ZONE(GetPlayerPed(), PDZ_RIGHT_LEG);
		CLEAR_PED_BLOOD_DAMAGE_BY_ZONE(GetPlayerPed(), PDZ_LEFT_LEG);
	}

	if (subLevel >= 0.7f || IS_PED_SWIMMING(GetPlayerPed()))
	{
		timerDirtDecal.Reset();
		CLEAR_PED_BLOOD_DAMAGE_BY_ZONE(GetPlayerPed(), PDZ_TORSO);
		CLEAR_PED_BLOOD_DAMAGE_BY_ZONE(GetPlayerPed(), PDZ_RIGHT_ARM);
		CLEAR_PED_BLOOD_DAMAGE_BY_ZONE(GetPlayerPed(), PDZ_LEFT_ARM);
		CLEAR_PED_BLOOD_DAMAGE_BY_ZONE(GetPlayerPed(), PDZ_MEDALS);
	}

	if (subLevel > 0.9f)
		CLEAR_PED_BLOOD_DAMAGE_BY_ZONE(GetPlayerPed(), PDZ_HEAD);

	if (subLevel == 1.0f || IS_PED_SWIMMING_UNDER_WATER(GetPlayerPed()))
		CLEAR_PED_BLOOD_DAMAGE(GetPlayerPed());

	// Also clean when player picks up a medkit
	const int health = GET_ENTITY_HEALTH(GetPlayerPed());
	if (health > oldHealthWounds && health == GET_ENTITY_MAX_HEALTH(GetPlayerPed()))
	{
		timerDirtDecal.Reset();
		CLEAR_PED_BLOOD_DAMAGE(GetPlayerPed());
	}

	// Dirt ad oldHealth should always be at the bottom
	oldHealthWounds = health;
	if (timerDirtDecal.Get() <= timeClearDirtDecal)
	{
		if (timerDirtDecal.Get() <= (timeClearDirtDecal / 2))
			SET_PED_SWEAT(GetPlayerPed(), 0.0f);

		CLEAR_PED_ENV_DIRT(GetPlayerPed());	//Must be called every frame
		//CLEAR_PED_DAMAGE_DECAL_BY_ZONE(GetPlayerPed(), PDZ_TORSO, "basic_dirt_cloth");
		//CLEAR_PED_DAMAGE_DECAL_BY_ZONE(GetPlayerPed(), PDZ_TORSO, "basic_dirt_skin");
	}
	return;
}

inline void EnableSprintInsideInteriors() { EnablePedConfigFlag(GetPlayerPed(), PCF_IgnoreInteriorCheckForSprinting); return; }

void SilentWanted()
{
	SET_POLICE_RADAR_BLIPS(false);
	SET_BLOCK_WANTED_FLASH(true);
	FORCE_OFF_WANTED_STAR_FLASH(true);
	HIDE_HUD_COMPONENT_THIS_FRAME(HUD_WANTED_STARS);

	SET_AUDIO_FLAG("WantedMusicDisabled", true);
	SET_AUDIO_FLAG("WantedMusicOnMission", false);
	IS_PED_IN_ANY_POLICE_VEHICLE(GetPlayerPed()) ? SET_AUDIO_FLAG("PoliceScannerDisabled", false) : SET_AUDIO_FLAG("PoliceScannerDisabled", true);
	return;
}
}	//END nGeneral

namespace nWeapons
{
void FriendlyFire()
{
	SET_CAN_ATTACK_FRIENDLY(GetPlayerPed(), true, true);	// Sets PCF_CanAttackFriendly, PCF_AllowLockonToFriendlyPlayers

	auto EnableLockOn = [](Ped ped)
		{
			//int relationship = GET_RELATIONSHIP_BETWEEN_PEDS(GetPlayerPed(), ped);
			//if (relationship == ACQUAINTANCE_TYPE_PED_RESPECT || relationship == ACQUAINTANCE_TYPE_PED_LIKE)
			SET_PED_CAN_BE_TARGETTED(ped, true);
			SET_PED_CAN_BE_TARGETTED_BY_PLAYER(ped, GetPlayer(), true);
			SET_PED_CAN_BE_TARGETED_WHEN_INJURED(ped, true);
			SET_ALLOW_LOCKON_TO_PED_IF_FRIENDLY(ped, true);	// same as setting PCF_AllowPlayerLockOnIfFriendly
			//SET_ENTITY_IS_TARGET_PRIORITY(ped, false, 1000.0f);
			SET_PED_CAN_RAGDOLL(ped, true);
			CLEAR_RAGDOLL_BLOCKING_FLAGS(ped, RAGDOLL_BLOCKING_FLAGS_ALL);
			SET_ENTITY_PROOFS(ped, false, false, false, false, false, false, false, false);
			SET_PED_DIES_WHEN_INJURED(ped, true);
			// SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, false); // might not be desirable
			DisablePedConfigFlag(ped, PCF_PreventAllMeleeTaunts);
			DisablePedConfigFlag(ped, PCF_NeverEverTargetThisPed);
			DisablePedConfigFlag(ped, PCF_DisablePlayerLockon);
			DisablePedResetFlag(ped, PRF_DisablePlayerLockon);
		};

	Ped ped = NULL;
	SET_SCENARIO_PEDS_TO_BE_RETURNED_BY_NEXT_COMMAND(true);
	if (GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(GetPlayer(), &ped))
		EnableLockOn(ped);

	SET_SCENARIO_PEDS_TO_BE_RETURNED_BY_NEXT_COMMAND(true);
	if (GET_PLAYER_TARGET_ENTITY(GetPlayer(), &ped))
		EnableLockOn(ped);

	const Vector3 tmpCoords = GetPlayerCoords();
	SET_SCENARIO_PEDS_TO_BE_RETURNED_BY_NEXT_COMMAND(true);
	if (GET_CLOSEST_PED(tmpCoords.x, tmpCoords.y, tmpCoords.z, 10.0f, true, true, &ped, false, true, GET_PED_TYPE(GetPlayerPed())))
		EnableLockOn(ped);

	return;
}

int lastPlayerArmor = 0;
void LocationalDamage()
{
	int armor = GET_PED_ARMOUR(GetPlayerPed());
	if (!HAS_PED_BEEN_DAMAGED_BY_WEAPON(GetPlayerPed(), NULL, GENERALWEAPON_TYPE_ANYWEAPON))
	{
		lastPlayerArmor = armor;
		return;
	}

	ClearEntityLastWeaponDamage(GetPlayerPed());

	int bone = NULL;
	if (!GET_PED_LAST_DAMAGE_BONE(GetPlayerPed(), &bone))
	{
		lastPlayerArmor = armor;
		return;
	}

	ClearPedLastDamageBone(GetPlayerPed());

	switch (GetGeneralDamageFromBoneTag(bone))
	{
	case DZ_HEAD:
		if (Ini::DeadlyPlayerHeadshots)
		{
			APPLY_DAMAGE_TO_PED(GetPlayerPed(), 100000, true, NULL);
			//EXPLODE_PED_HEAD(GetPlayerPed(), Joaat("WEAPON_SNIPERRIFLE"));
		}
		break;
	case DZ_UPPER_BODY:
	case DZ_LOWER_BODY:
	{
		const int armorDm = lastPlayerArmor - armor;
		if (armorDm > 0)
		{
			SET_PED_ARMOUR(GetPlayerPed(), lastPlayerArmor);
			SET_ENTITY_HEALTH(GetPlayerPed(), (GET_ENTITY_HEALTH(GetPlayerPed()) - armorDm), NULL);
		}
		break;
	}
	default:
		lastPlayerArmor = armor;
		break;
	}
	return;
}

void EnableWeaponRecoil()
{
	if (!IS_PED_SHOOTING(GetPlayerPed()) || IsFirstPersonActive())
		return;

	const bool isInVeh = DOES_ENTITY_EXIST(GetVehiclePedIsIn(GetPlayerPed(), false, false));
	if (!Ini::EnableRecoilInVehicles && isInVeh)
		return;

	Hash weapon = NULL;
	if (!GET_CURRENT_PED_WEAPON(GetPlayerPed(), &weapon, false))
		return;

	const Entity wpObj = GET_CURRENT_PED_WEAPON_ENTITY_INDEX(GetPlayerPed(), false);
	const Hash wpGrp = GET_WEAPONTYPE_GROUP(weapon);

	// Check if it's a valid weapon
	switch (wpGrp)
	{
	case WEAPONGROUP_INVALID: case WEAPONGROUP_MELEE: case WEAPONGROUP_THROWN:
	case WEAPONGROUP_FIREEXTINGUISHER: case WEAPONGROUP_PETROLCAN: case WEAPONGROUP_LOUDHAILER:
	case WEAPONGROUP_DIGISCANNER: case WEAPONGROUP_NIGHTVISION: case WEAPONGROUP_PARACHUTE:
	case WEAPONGROUP_JETPACK: case WEAPONGROUP_METALDETECTOR:
		return; break;
	}

	float multPistol = 1.0f;
	float multSMG = multPistol;
	float multRifle = 1.5f;
	float multMG = multRifle;
	float multShotgun = 4.0f;
	float multSniper = multRifle;
	float multHeavy = 3.0f;
	float multMinigun = multSMG * 0.33f;

	float multTwoHanded = 1.0f;
	float multOneHanded = 1.5f;
	float multShortGuns = 1.5f;
	float multLongGuns = 0.75f;
	float multAimedShot = 0.75f;
	float multSilenced = 0.75f;
	float multGrip = 0.5f;
	float multShootingAbility = 0.5f;	// Max multiplier for shooting ability

	if (Ini::WeaponRecoilMultipliers.size() >= 16)
	{
		multPistol = Ini::WeaponRecoilMultipliers[0];
		multSMG = Ini::WeaponRecoilMultipliers[1];
		multRifle = Ini::WeaponRecoilMultipliers[2];
		multMG = Ini::WeaponRecoilMultipliers[3];
		multShotgun = Ini::WeaponRecoilMultipliers[4];
		multSniper = Ini::WeaponRecoilMultipliers[5];
		multHeavy = Ini::WeaponRecoilMultipliers[6];
		multMinigun = Ini::WeaponRecoilMultipliers[7];

		multTwoHanded = Ini::WeaponRecoilMultipliers[8];
		multOneHanded = Ini::WeaponRecoilMultipliers[9];
		multShortGuns = Ini::WeaponRecoilMultipliers[10];
		multLongGuns = Ini::WeaponRecoilMultipliers[11];
		multAimedShot = Ini::WeaponRecoilMultipliers[12];
		multSilenced = Ini::WeaponRecoilMultipliers[13];
		multGrip = Ini::WeaponRecoilMultipliers[14];
		multShootingAbility = Ini::WeaponRecoilMultipliers[15];	// Max multiplier for shooting ability
	}

	float recoilMult = Ini::WeaponRecoilGlobalMultiplier;
	if (GET_PED_AMMO_TYPE_FROM_WEAPON(GetPlayerPed(), weapon) == AMMOTYPE_MINIGUN)
		recoilMult *= multMinigun;
	else
	{
		switch (wpGrp)
		{
		case WEAPONGROUP_PISTOL: case WEAPONGROUP_RUBBERGUN: case WEAPONGROUP_STUNGUN:
			recoilMult *= multPistol; break;
		case WEAPONGROUP_SMG:
			recoilMult *= multSMG; break;
		case WEAPONGROUP_RIFLE:
			recoilMult *= multRifle; break;
		case WEAPONGROUP_MG:
			recoilMult *= multMG; break;
		case WEAPONGROUP_SHOTGUN:
			recoilMult *= multShotgun; break;
		case WEAPONGROUP_SNIPER:
			recoilMult *= multSniper; break;
		case WEAPONGROUP_HEAVY:
			recoilMult *= multHeavy; break;
		}
	}

	const Vector3 handLoc = GET_PED_BONE_COORDS(GetPlayerPed(), BONETAG_PH_L_HAND, 0.0f, 0.0f, 0.0f);
	if (std::abs(GET_OFFSET_FROM_ENTITY_GIVEN_WORLD_COORDS(wpObj, handLoc.x, handLoc.y, handLoc.z).y) < 0.4f)	// Check offset for two-handed weapons
		recoilMult *= multTwoHanded;
	else
		recoilMult *= multOneHanded;

	Vector3 min; Vector3 max; GET_MODEL_DIMENSIONS(GET_ENTITY_MODEL(wpObj), &min, &max);
	const Vector3 dim = max - min;
	if (dim.x > 0.5f)
		recoilMult *= multLongGuns;
	else if (dim.x <= 0.2f)
		recoilMult *= multShortGuns;

	if (IsPlayerAiming(false, false))
		recoilMult *= multAimedShot;

	if (IS_PED_CURRENT_WEAPON_SILENCED(GetPlayerPed()))
		recoilMult *= multSilenced;

	if (DoesPedWeaponHaveComponentType(GetPlayerPed(), weapon, WAP_Grip, true))
		recoilMult *= multGrip;

	int shootingAbility = 0;
	if (STAT_GET_INT(GetCharacterStatHash("SHOOTING_ABILITY"), &shootingAbility, -1))
		recoilMult *= (1.0f - (static_cast<float>(shootingAbility) * 0.01f * multShootingAbility));

	// Relative to RAGE coordinates system - EULER_XYZ
	// +X is roll right., -Y is pitch UP, Z is yaw, 
	//Vector3 rot = GET_ENTITY_ROTATION(wpObj, EULER_XYZ);

	/* tilted Recoil
	float roll = GET_ENTITY_ROLL(wpObj);
	if (roll > 90.0f)
		roll = -(roll - 90.0f);
	else if (roll < -90.0f)
		roll = -(roll + 90.0f);

	roll = roll / 90.0f;
	const float tiltedRecoil = recoilMult * roll;
	recoilMult *= (1.0f - std::abs(roll));
	const float finalHeading = GET_GAMEPLAY_CAM_RELATIVE_HEADING() + tiltedRecoil;
	float finalPitch = GET_GAMEPLAY_CAM_RELATIVE_PITCH() + recoilMult;
	if (IS_FOLLOW_VEHICLE_CAM_ACTIVE() && !isFPS)
		finalPitch += 2.25f;	// Offset for vehicle cam (don't know why it's like this, but it is)

	recoilMult = static_cast<float>(std::round(recoilMult * 10.0f)) * 0.1f;	// Round to first decimal place
	tiltedRecoil = static_cast<float>(std::round(tiltedRecoil * 10.0f)) * 0.1f;

	FORCE_CAMERA_RELATIVE_HEADING_AND_PITCH(finalHeading, finalPitch, 0.1f);	// requires VER_1_0_505_2_STEAM
	*/

	// Add offset for vehicle cam (don't know why it's like this, but it is)
	const float finalPitch = isInVeh ? (GET_GAMEPLAY_CAM_RELATIVE_PITCH() + recoilMult + 2.75f) : (GET_GAMEPLAY_CAM_RELATIVE_PITCH() + recoilMult);
	SET_GAMEPLAY_CAM_RELATIVE_PITCH(finalPitch, 0.1f);
	return;
}

void DisarmPlayerWhenShot()
{
	RestorePlayerRetrievedWeapon(Ini::AutoEquipDroppedWeapon);

	bool drop = false;
	if (HasEntityBeenDamagedByWeaponThisFrame(GetPlayerPed(), WEAPON_STUNGUN, GENERALWEAPON_TYPE_INVALID))
		drop = true;
	else if (CanDisarmPed(GetPlayerPed(), true) && GetWeightedBool(Ini::DisarmPlayerChance))
		drop = true;

	if (!drop)
		return;

	Hash wp = NULL; GET_CURRENT_PED_WEAPON(GetPlayerPed(), &wp, false);
	if (ShouldWeaponSpawnPickupWhenDropped(wp, true))
		DropPlayerWeapon(wp, true, GET_ENTITY_ROTATION(GET_CURRENT_PED_WEAPON_ENTITY_INDEX(GetPlayerPed(), false), EULER_YXZ));

	return;
}

//Compatibility with custom NM reactions
int lastNMReactionTime = 0;
Timer NMReactionTimer;

Hash lastRagdollWp = NULL;
Vector3 lastRagdollWpRot = Vector3();
void DropPlayerWeaponWhenRagdolling()
{
	RestorePlayerRetrievedWeapon(Ini::AutoEquipDroppedWeapon);

	//Compatibility with custom NM reactions
	if (GetNMReactionTime() != lastNMReactionTime)
	{
		lastNMReactionTime = GetNMReactionTime();
		NMReactionTimer.Reset();
	}

	Hash tmp = NULL;
	const bool res = GET_CURRENT_PED_WEAPON(GetPlayerPed(), &tmp, false);

	if (tmp == lastRagdollWp && NMReactionTimer.Get() > (GetNMReactionTime() + 100))
	{
		if (IS_PED_RAGDOLL(GetPlayerPed()) && ShouldWeaponSpawnPickupWhenDropped(lastRagdollWp, true))
			DropPlayerWeapon(lastRagdollWp, false, lastRagdollWpRot);
	}

	// GET_CURRENT_PED_WEAPON returns true when a weapon is usable (i.e. in their hand). We do this to check if
	// the player was holding the gun correctly (since for two-handed weapons their model is hidden on the same frame a ragdoll starts)
	if (res)
	{
		lastRagdollWp = tmp;
		lastRagdollWpRot = GET_ENTITY_ROTATION(GET_CURRENT_PED_WEAPON_ENTITY_INDEX(GetPlayerPed(), false), EULER_YXZ);
	}
	else
	{
		lastRagdollWp = NULL;
		lastRagdollWpRot = { 0.0f, 0.0f, 0.0f };
	}
	return;
}

//This approach isn't great, and could cause a lot of issues with the game's story... Too bad
//MUST BE CALLED EVERY FRAME
//A different and more aggressive approach would be to terminate these scripts, which are responsible for the family scenes
//inside the safehouses and disable the player's control: family_scene_f0, family_scene_f1, family_scene_m, family_scene_t0, family_scene_t1
Weapon lastPlayerWeapon = NULL;
Timer timerLastPlayerWeapon;
Timer timerCharacterSwitch;
void AllowWeaponsInsideSafeHouse()
{
	//Force selected player weapon for 1000ms upon exit/enter of safehouse
	if (timerLastPlayerWeapon.Get() < 1000 && lastPlayerWeapon != WEAPON_UNARMED)
		SET_CURRENT_PED_WEAPON(GetPlayerPed(), lastPlayerWeapon, true);

	//Checks if player is inside valid interior
	int playerInterior = GET_INTERIOR_FROM_ENTITY(GetPlayerPed());
	if (!IS_VALID_INTERIOR(playerInterior) ||
		//reset fakewanted also if player is inside the strip club but not inside the office
		(GET_INTERIOR_AT_COORDS(113.53f, -1287.61f, 28.64f) == playerInterior /*v_strip3*/ && GET_ROOM_KEY_FROM_ENTITY(GetPlayerPed()) != strp3off))
	{
		if (GetFakeWanted())
		{
			timerLastPlayerWeapon.Reset();
			SetFakeWanted(GetPlayer(), false);
		}
		lastPlayerWeapon = GET_SELECTED_PED_WEAPON(GetPlayerPed());
		return;
	}

	//Check if player is inside a safehouse, and not just in a random interior and return if player was already wanted before entering the interior
	if (!IsPlayerInsideSafehouse() || (!GetFakeWanted() && GET_PLAYER_WANTED_LEVEL(GetPlayer()) != 0))
		return;

	//Allow player to switch characters while inside the safehouse
	if (IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_CHARACTER_WHEEL) ||
		IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_SELECT_CHARACTER_MICHAEL) ||
		IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_SELECT_CHARACTER_FRANKLIN) ||
		IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_SELECT_CHARACTER_TREVOR))
	{
		timerCharacterSwitch.Reset();
	}
	else if (timerCharacterSwitch.Get() > 250 &&
		(IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_CHARACTER_WHEEL) ||
			IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_SELECT_CHARACTER_MICHAEL) ||
			IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_SELECT_CHARACTER_FRANKLIN) ||
			IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_SELECT_CHARACTER_TREVOR)))
	{
		if (GetFakeWanted())
			SetFakeWanted(GetPlayer(), false);

		timerLastPlayerWeapon.Reset();
		return;
	}

	//Reset weapon timer
	if (!GetFakeWanted())
		timerLastPlayerWeapon.Reset();
	else
		lastPlayerWeapon = GET_SELECTED_PED_WEAPON(GetPlayerPed());

	//If all checks pass, set fake wanted level
	SetFakeWanted(GetPlayer(), true);
	return;
}
}

//////////////////////////////////////////Player Vehicle/////////////////////////////////////////
namespace nVehicle
{
Vehicle lastVeh = NULL;
void DisableCarMidAirAndRollControl()
{
	Vehicle veh = GetVehiclePedIsIn(GetPlayerPed());
	if (!DOES_ENTITY_EXIST(veh))
		return;

	//Clean up old veh stuck checks, since the game is limited to ~16 of them
	if (lastVeh != veh && DOES_ENTITY_EXIST(lastVeh) && DOES_VEHICLE_HAVE_STUCK_VEHICLE_CHECK(veh))
		REMOVE_VEHICLE_STUCK_CHECK(veh);

	//Check if veh is car and is driveable then proceed
	constexpr int time = 100;
	Hash vehModel = GET_ENTITY_MODEL(veh);
	if (IS_THIS_MODEL_A_CAR(vehModel) && vehModel != 0x586765FB /*deluxo*/)
	{
		if (!DOES_VEHICLE_HAVE_STUCK_VEHICLE_CHECK(veh))
			ADD_VEHICLE_STUCK_CHECK_WITH_WARP(veh, 0.1f, time, false, false, false, -1);

		if (IS_ENTITY_IN_AIR(veh) ||
			IS_VEHICLE_STUCK_TIMER_UP(veh, VEH_STUCK_HUNG_UP, time) ||
			IS_VEHICLE_STUCK_TIMER_UP(veh, VEH_STUCK_ON_ROOF, time) ||
			IS_VEHICLE_STUCK_TIMER_UP(veh, VEH_STUCK_ON_SIDE, time))
		{
			DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_VEH_MOVE_LR, false);
			DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_VEH_MOVE_UD, false);
			DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_VEH_MOVE_UP_ONLY, false);
			DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_VEH_MOVE_DOWN_ONLY, false);
			DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_VEH_MOVE_LEFT_ONLY, false);
			DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_VEH_MOVE_RIGHT_ONLY, false);
			DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_VEH_MOVE_LEFT, false);
			DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_VEH_MOVE_RIGHT, false);
			DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_VEH_MOVE_UP, false);
			DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_VEH_MOVE_DOWN, false);

			//Still enable player to change steering angle  - Control normal must be inverted
			SET_VEHICLE_STEER_BIAS(veh, -0.5f * GET_DISABLED_CONTROL_UNBOUND_NORMAL(PLAYER_CONTROL, INPUT_VEH_MOVE_LR));
		}
	}
	return;
}

bool wasSetAsMissionEntity = false;
Timer timerCarExplosion;
void DisableForcedCarExplosionOnImpact()
{
	Vehicle veh = GetVehiclePedIsUsing(GetPlayerPed());
	if (!DOES_ENTITY_EXIST(veh))
		return;

	//Check if vehicle is boat/car
	Hash vehModel = GET_ENTITY_MODEL(veh);
	if (!IS_THIS_MODEL_A_CAR(vehModel) && !IS_THIS_MODEL_A_BOAT(vehModel))
		return;

	//If not in the air, set it as no longer needed as a mission entity
	if (!IS_ENTITY_IN_AIR(veh) || GET_ENTITY_SPEED(veh) < 10.0f)
	{
		//Wait 500ms before setting vehicle as no longer needed
		if (timerCarExplosion.Get() > 500 && IS_ENTITY_A_MISSION_ENTITY(veh) && wasSetAsMissionEntity)
		{
			wasSetAsMissionEntity = false;
			SET_ENTITY_AS_NO_LONGER_NEEDED(&veh);
		}
		return;
	}

	timerCarExplosion.Reset();
	if (!IS_ENTITY_A_MISSION_ENTITY(veh))
	{
		wasSetAsMissionEntity = true;
		SET_ENTITY_AS_MISSION_ENTITY(veh, true, false);
	}
	return;
}

void DisableEngineSmoke()
{
	Vehicle veh = GetVehiclePedIsIn(GetPlayerPed());
	if (veh == NULL)
		return;

	constexpr float limit = ENGINE_DAMAGE_RADBURST + 50.0f;
	if (GET_VEHICLE_ENGINE_HEALTH(veh) <= limit)
		SET_VEHICLE_ENGINE_HEALTH(veh, limit);

	return;
}

Vehicle lastVehEngine = NULL;
void DisableEngineFire()
{
	Vehicle veh = GetVehiclePedIsIn(GetPlayerPed());
	if (veh == NULL)
		return;

	//Check if vehicle is a car/bike/quad and adjust engine health limit
	// No need to check if it's Amphibious vehicle, normal car and quadbike functions already handle them
	const int vehModel = GET_ENTITY_MODEL(veh);
	if (!IS_THIS_MODEL_A_CAR(vehModel) && !(IS_THIS_MODEL_A_BIKE(vehModel) && !IS_THIS_MODEL_A_BICYCLE(vehModel))
		&& !IS_THIS_MODEL_A_QUADBIKE(vehModel))
		return;

	//Engine health safeguard
	constexpr float limit = ENGINE_DAMAGE_ONFIRE + 50.0f;
	if (GET_VEHICLE_ENGINE_HEALTH(veh) <= limit)
		SET_VEHICLE_ENGINE_HEALTH(veh, limit);

	//Check if we have already disabled engine fire on this vehicle)
	if (veh == lastVehEngine)
		return;

	lastVehEngine = veh;
	if (!IS_ENTITY_A_MISSION_ENTITY(veh))
	{
		SET_ENTITY_AS_MISSION_ENTITY(veh, true, false);
		SET_DISABLE_VEHICLE_ENGINE_FIRES(veh, true);
		SET_ENTITY_AS_NO_LONGER_NEEDED(&veh);
	}
	else
		SET_DISABLE_VEHICLE_ENGINE_FIRES(veh, true);

	return;
}

Timer timerEngineControl;
constexpr int TURN_OFF_ENGINE_DURATION = 250;
void LeaveEngineOnWhenExitingVehicles()
{
	Vehicle veh = GetVehiclePedIsUsing(GetPlayerPed());
	if (!DOES_ENTITY_EXIST(veh))
		return;

	SET_VEHICLE_KEEP_ENGINE_ON_WHEN_ABANDONED(veh, true);	//Same thing as setting PCF_LeaveEngineOnWhenExitingVehicles
	if (IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_VEH_EXIT))
		timerEngineControl.Reset();
	else if (IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_VEH_EXIT) &&  
			timerEngineControl.Get() > TURN_OFF_ENGINE_DURATION &&
			GET_IS_TASK_ACTIVE(GetPlayerPed(), CODE_TASK_EXIT_VEHICLE))
	{ 
		SET_VEHICLE_ENGINE_ON(veh, false, true, false);
		timerEngineControl.Set(INT_MIN);
	}
	return;
}

//Very very bad method of doing this, but it works!
//When a vehicle is physically attached to another vehicle it retains the same steering on exit (thanks to RAGE constraints)
//Only problem is that the vehicle now inherits most veh flags from parent. 
//Don't know if this could cause issues, but better than patching memory
Timer timerWheelsAutoCenter;
Vehicle tmpVeh = NULL;
bool shouldAttachVeh = false;
void DisableWheelsAutoCenterOnCarExit()
{
	constexpr float maxVehSpeedDetach = 10.0f;

	const Vehicle veh = GetVehiclePedIsUsing(GetPlayerPed());
	if (!DOES_ENTITY_EXIST(veh))
		return;

	const int vehHash = GET_ENTITY_MODEL(veh);	//Disable for bikes and bicycles, can cause issues
	if (!IS_THIS_MODEL_A_CAR(vehHash) || !IS_VEHICLE_DRIVEABLE(veh, false) || GET_ENTITY_SUBMERGED_LEVEL(GetPlayerPed()) > 0.7f)
		return;

	const int tmpVehHash = GET_ENTITY_MODEL(tmpVeh);
	if (tmpVehHash != vehHash)
	{
		if (DOES_ENTITY_EXIST(tmpVeh))
		{
			if (IS_ENTITY_ATTACHED_TO_ENTITY(veh, tmpVeh))
				DETACH_ENTITY(veh, false, false);

			SET_ENTITY_AS_MISSION_ENTITY(tmpVeh, false, true);
			DELETE_ENTITY(&tmpVeh);
			SET_MODEL_AS_NO_LONGER_NEEDED(tmpVehHash);
			tmpVeh = NULL;
		}

		if (!HAS_MODEL_LOADED(vehHash))
		{
			REQUEST_MODEL(vehHash);
			return;
		}
		tmpVeh = CREATE_VEHICLE(vehHash, 0.0f, 0.0f, -90.0f, 0.0f, false, false, true);
		SET_ENTITY_AS_MISSION_ENTITY(tmpVeh, false, true);
		SET_ENTITY_VISIBLE(tmpVeh, false, false);
		FREEZE_ENTITY_POSITION(tmpVeh, true);
		return;
	}

	if (GET_IS_TASK_ACTIVE(GetPlayerPed(), CODE_TASK_EXIT_VEHICLE) || timerWheelsAutoCenter.Get() > 2000 || GET_ENTITY_SPEED(veh) > maxVehSpeedDetach)
		shouldAttachVeh = false;

	if (IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_VEH_EXIT) && GetVehiclePedIsIn(GetPlayerPed(), false, false)) //&& !IS_VEHICLE_ATTACHED_TO_TRAILER(veh))
		shouldAttachVeh = true;

	if (shouldAttachVeh && !IS_ENTITY_ATTACHED_TO_ENTITY(veh, tmpVeh) && GET_ENTITY_SPEED(veh) < maxVehSpeedDetach)
	{
		timerWheelsAutoCenter.Reset();

		float yOffset = 100.0f;
		if (GET_ENTITY_SPEED_VECTOR(veh, true).y < 0.0f)
			yOffset *= -1.0f;

		const Vector3 tmpOffset = GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, 0.0f, yOffset, 0.0f);
		SET_ENTITY_COORDS_NO_OFFSET(tmpVeh, tmpOffset.x, tmpOffset.y, GET_ENTITY_COORDS(tmpVeh, false).z, true, true, false);

		ATTACH_ENTITY_TO_ENTITY_PHYSICALLY(veh, tmpVeh, NULL, NULL, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, true, 2);
		return;
	}

	//Detach veh 
	if (IS_ENTITY_ATTACHED_TO_ENTITY(veh, tmpVeh) && !shouldAttachVeh)
	{
		DETACH_ENTITY(veh, false, false);
		return;
	}
	return;
}

struct VehicleHydraulicsState {
	Vehicle veh = NULL;
	bool update = false;
	float hydState[MAX_WHEELS] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	bool operator==(const Vehicle& v) const { return (veh == v); }
};

std::vector<VehicleHydraulicsState> VehHydraulics;
bool wasPlayerEnteringOrExitingVehLastFrame = false;
void KeepCarHydraulicsPosition()
{
	Vehicle veh = GetVehiclePedIsUsing(GetPlayerPed());
	if (DOES_ENTITY_EXIST(veh) &&
		IS_VEHICLE_DRIVEABLE(veh, false))
	{
		VehicleHydraulicsState* ptr = nullptr;
		const auto itr = std::find(VehHydraulics.begin(), VehHydraulics.end(), veh);
		if (itr == VehHydraulics.end())
		{
			VehicleHydraulicsState state;
			state.veh = veh;
			bool isHydraulic = false;
			LOOP(i, MAX_WHEELS)
			{
				state.hydState[i] = GET_HYDRAULIC_SUSPENSION_RAISE_FACTOR(veh, i);
				if (state.hydState[i] != NULL)
					isHydraulic = true;
			}

			if (isHydraulic)
			{
				VehHydraulics.push_back(state);
				ptr = &VehHydraulics.back();
			}
		}
		else
			ptr = &(*itr);

		if (GetVehiclePedIsEnteringOrExiting(GetPlayerPed()) == veh && ptr)
		{
			if (!wasPlayerEnteringOrExitingVehLastFrame)
			{
				LOOP(i, MAX_WHEELS)
				{
					ptr->hydState[i] = GET_HYDRAULIC_SUSPENSION_RAISE_FACTOR(ptr->veh, i);
				}
			}

			ptr->update = true;
			LOOP(i, MAX_WHEELS)
			{
				SET_HYDRAULIC_SUSPENSION_RAISE_FACTOR(ptr->veh, i, ptr->hydState[i]);
			}

			wasPlayerEnteringOrExitingVehLastFrame = true;
		}
		else
			wasPlayerEnteringOrExitingVehLastFrame = false;
	}

	for (auto it = VehHydraulics.begin(); it != VehHydraulics.end(); )
	{
		if (!DOES_ENTITY_EXIST(it->veh))
		{
			it = VehHydraulics.erase(it);
			continue;
		}

		if (it->veh != veh && it->update)
		{
			it->update = false;
			LOOP(i, MAX_WHEELS)
			{
				SET_HYDRAULIC_SUSPENSION_RAISE_FACTOR(it->veh, i, it->hydState[i]);
			}
		}
		++it; // Remember to increment iterator
	}
	return;
}

void EnableBrakeLightsOnStoppedVehicle()
{
	const Vehicle veh = GetVehiclePedIsUsing(GetPlayerPed());
	if (!DOES_ENTITY_EXIST(veh))
		return;

	const Hash model = GET_ENTITY_MODEL(veh);
	if (!IS_THIS_MODEL_A_CAR(model) && !(IS_THIS_MODEL_A_BIKE(model) && !IS_THIS_MODEL_A_BICYCLE(model))
		&& !IS_THIS_MODEL_A_QUADBIKE(model))
		return;

	// Preferrably, do NOT use IS_VEHICLE_STOPPED: "Returns true if the vehicle's current speed is less than, or equal to 0.0025f."
	if (GET_IS_VEHICLE_ENGINE_RUNNING(veh) && GET_ENTITY_SPEED(veh) > 0.1f)
		SET_VEHICLE_BRAKE_LIGHTS(veh, true);

	return;
}

void EnableHeliWaterPhysics()
{
	Vehicle heli = GetVehiclePedIsUsing(GetPlayerPed());
	if (!DOES_ENTITY_EXIST(heli) || !IS_THIS_MODEL_A_HELI(GET_ENTITY_MODEL(heli)) ||
		!GET_IS_VEHICLE_ENGINE_RUNNING(heli))
		return;

	constexpr int arrSize = 4;
	const int rotors[arrSize] = {
		GET_ENTITY_BONE_INDEX_BY_NAME(heli, "ROTOR_MAIN"),
		GET_ENTITY_BONE_INDEX_BY_NAME(heli, "ROTOR_MAIN_2"),
		GET_ENTITY_BONE_INDEX_BY_NAME(heli, "ROTOR_REAR"),
		GET_ENTITY_BONE_INDEX_BY_NAME(heli, "ROTOR_REAR_2")
	};

	Vector3 heliCoords = GET_ENTITY_COORDS(heli, false);
	Vector3 min = nullVec; Vector3 max = nullVec;
	GET_MODEL_DIMENSIONS(GET_ENTITY_MODEL(heli), &min, &max);
	heliCoords.z += min.z;

	float z = NULL;
	if (!GET_GROUND_Z_FOR_3D_COORD(heliCoords.x, heliCoords.y, heliCoords.z, &z, true, true))
		return;

	constexpr float maxHeight = 15.0f;
	const float distGround = heliCoords.z - z;
	if (distGround > maxHeight)
		return;

	const float mult = 1.0f - (distGround / maxHeight);
	const float speed = -2.5f * mult;
	constexpr float rate = 0.9f;
	LOOP(i, arrSize)
	{
		if (rotors[i] < 0)
			continue;

		// Apply only for main rotors first, so that if the helicopter has a tail it doesn't apply the effect.
		// Eg.: Cargobobs have both a front and rear motor, but don't have a tail and thus the effect should be applied to both motors,
		// an helicopter will instead have a rear motors at it's tail that should NOT produce the effect
		if (i <= 1)
		{
			const Vector3 tmpCoords = GET_WORLD_POSITION_OF_ENTITY_BONE(heli, rotors[i]);
			MODIFY_WATER(tmpCoords.x, tmpCoords.y, speed, rate);
		}
		else
		{
			if (GET_ENTITY_BONE_INDEX_BY_NAME(heli, "TAIL") < 0)
			{
				const Vector3 tmpCoords = GET_WORLD_POSITION_OF_ENTITY_BONE(heli, rotors[i]);
				MODIFY_WATER(tmpCoords.x, tmpCoords.y, speed, rate);
			}
		}
	}
	return;
}

int rainShapetestHandle = NULL;
bool rainShapetestLastRes = false;
void DynamicallyCleanVehicles()
{
	auto WashRain = [](const Vehicle veh, const float cleanRatePerSecond, const float speedMult)
		{
			// Rain level: 0.0f -> 1.0f
			const float rain = GET_RAIN_LEVEL();
			if (rain <= 0.001f)
				return;

			const float rainMult = 1.0f + rain;
			const float rateDirt = cleanRatePerSecond * speedMult * rainMult * GET_FRAME_TIME();	// In scale 0.0f -> 1.0f
			const float rateDecals = rateDirt / 2.0f;	// Clean decals at half the rate of dirt

			// There's no way to check for decals, so always wash them
			WASH_DECALS_FROM_VEHICLE(veh, rateDecals);

			// Dirt level: 0.0f -> 15.0f
			const float dirt = GET_VEHICLE_DIRT_LEVEL(veh) / 15.0f;
			if (dirt > 0.0f)
			{
				const float newDirtLevel = (dirt - rateDirt) * 15.0f;
				SET_VEHICLE_DIRT_LEVEL(veh, newDirtLevel);
			}
			return;
		};

	auto WashSubmerged = [](const Vehicle veh, const float cleanRatePerSecond, const float speedMult)
		{
			if (GET_ENTITY_SUBMERGED_LEVEL(veh) <= 0.25f)
				return;

			const float rate = cleanRatePerSecond * (speedMult * 20.0f) * GET_FRAME_TIME();	// Speedup
			WASH_DECALS_FROM_VEHICLE(veh, rate);
			const float dirt = GET_VEHICLE_DIRT_LEVEL(veh) / 15.0f;
			if (dirt > 0.0f)
			{
				const float newDirtLevel = (dirt - rate) * 15.0f;
				SET_VEHICLE_DIRT_LEVEL(veh, newDirtLevel);
			}
			return;
		};

	constexpr int nearbyVehsSize = 17;	// GET_PED_NEARBY_VEHICLES wont return more than 16 vehicles. It includes the player vehicle
	scrValue nearbyVehs[nearbyVehsSize];
	nearbyVehs[0].Int = nearbyVehsSize; // First value has to be initialized as the size of the array, and it will stay that way
	const int count = GET_PED_NEARBY_VEHICLES(GetPlayerPed(), reinterpret_cast<Any*>(nearbyVehs));
	const Vehicle playerVeh = GetVehiclePedIsUsing(GetPlayerPed());
	for (int i = 1; i <= count; ++i)	// Start at index 1, since index 0 is the size of the array. The "i <= count" is NOT an error
	{
		const Hash model = GET_ENTITY_MODEL(nearbyVehs[i].Uns);
		if (IS_THIS_MODEL_A_BOAT(model) || IS_THIS_MODEL_A_PLANE(model) ||
			IS_THIS_MODEL_A_HELI(model) || IS_THIS_MODEL_A_TRAIN(model))
			continue;

		bool shouldSkipRain = false;
		if (GET_INTERIOR_FROM_ENTITY(nearbyVehs[i].Uns))
			shouldSkipRain = true;
		else if (playerVeh == nearbyVehs[i].Uns && GET_RAIN_LEVEL() > 0.001f)
		{
			if (rainShapetestHandle != NULL)
			{
				bool hit = false; Vector3 hitCoords = Vector3(); Vector3 hitNormal = Vector3(); Entity hitEntity = NULL;
				if (GET_SHAPE_TEST_RESULT(rainShapetestHandle, &hit, &hitCoords, &hitNormal, &hitEntity) != SHAPETEST_STATUS_RESULTS_NOTREADY)
				{
					rainShapetestLastRes = hit;
					RELEASE_SCRIPT_GUID_FROM_ENTITY(hitEntity);
					const Vector3 loc = GET_ENTITY_COORDS(nearbyVehs[i].Uns, false);
					rainShapetestHandle = START_SHAPE_TEST_LOS_PROBE(loc.x, loc.y, loc.z, loc.x, loc.y, (loc.z + 10.0f), SCRIPT_INCLUDE_ALL, NULL, SCRIPT_SHAPETEST_OPTION_DEFAULT);
				}
			}
			else
			{
				const Vector3 loc = GET_ENTITY_COORDS(nearbyVehs[i].Uns, false);
				rainShapetestHandle = START_SHAPE_TEST_LOS_PROBE(loc.x, loc.y, loc.z, loc.x, loc.y, (loc.z + 10.0f), SCRIPT_INCLUDE_ALL, NULL, SCRIPT_SHAPETEST_OPTION_DEFAULT);
			}
		}

		constexpr float maxSpeed = 100.0f;
		const float speed = std::clamp(GET_ENTITY_SPEED(nearbyVehs[i].Uns) * 3.6f, 0.0f, maxSpeed);

		constexpr float cleanRatePerSecond = 0.005555555f; // 0.0085f is around 2min (one in-game hour) to clean a vehicle with max dirt level
		const float speedMult = 1.0f + (speed / maxSpeed);

		if (!shouldSkipRain && !rainShapetestLastRes)
			WashRain(nearbyVehs[i].Uns, cleanRatePerSecond, speedMult);

		WashSubmerged(nearbyVehs[i].Uns, cleanRatePerSecond, speedMult);
	}
	return;
}

void DisableRagdollOnVehicleRoof()
{
	//Velocity Unit -> Km/h
	if (Ini::MaxVehicleSpeed > 0.0f)
	{
		const float speed = GET_ENTITY_SPEED(GetPlayerPed()) * 3.6f;	// m\s to Km\h
		if (speed > Ini::MaxVehicleSpeed)
			return;
	}

	EnablePedResetFlag(GetPlayerPed(), PRF_BlockRagdollFromVehicleFallOff);
	return;
}

inline void DisableDragOutCar() { SET_PED_CAN_BE_DRAGGED_OUT(GetPlayerPed(), false); return; }	//Same thing as setting CPED_CONFIG_FLAG_DontDragMeOutCar
inline void DisableFlyThroughWindscreen() { DisablePedConfigFlag(GetPlayerPed(), PCF_WillFlyThroughWindscreen); return; }
inline void DisableBikeKnockOff() { SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(GetPlayerPed(), KNOCKOFFVEHICLE_NEVER); return; }

void DisableShallowWaterBikeJumpOut()
{
	const Vehicle veh = GetVehiclePedIsUsing(GetPlayerPed());
	if (DOES_ENTITY_EXIST(veh))
	{
		if (GET_ENTITY_SUBMERGED_LEVEL(veh) > 0.9f || (IS_ENTITY_IN_WATER(veh) && !IS_VEHICLE_DRIVEABLE(veh, false)))
			return;
	}

	EnablePedResetFlag(GetPlayerPed(), PRF_DisableShallowWaterBikeJumpOutThisFrame);
	return;
}

inline void DisableVehicleJitter() { SET_CAR_HIGH_SPEED_BUMP_SEVERITY_MULTIPLIER(0.0f); return; }

void DisableAirVehicleTurbulence()
{
	const Vehicle veh = GetVehiclePedIsUsing(GetPlayerPed());
	const Hash vehHash = GET_ENTITY_MODEL(veh);
	if (IS_THIS_MODEL_A_PLANE(vehHash))
		SET_PLANE_TURBULENCE_MULTIPLIER(veh, 0.0f);
	else if (IS_THIS_MODEL_A_HELI(vehHash))
		SET_HELI_TURBULENCE_SCALAR(veh, 0.0f);

	return;
}

void DisableAutoEquipHelmets()
{
	if (Ini::DisableAutoEquipHelmets == 1)
		EnablePedConfigFlag(GetPlayerPed(), PCF_DisableAutoEquipHelmetsInBikes);
	else if (Ini::DisableAutoEquipHelmets == 2)
		EnablePedConfigFlag(GetPlayerPed(), PCF_DisableAutoEquipHelmetsInAicraft);
	else
	{
		EnablePedConfigFlag(GetPlayerPed(), PCF_DisableAutoEquipHelmetsInBikes);
		EnablePedConfigFlag(GetPlayerPed(), PCF_DisableAutoEquipHelmetsInAicraft);
	}

	return;
}

inline void DisableStuntJumps() { SET_STUNT_JUMPS_CAN_TRIGGER(false); return; }
}	//END nVehicle

/////////////////////////////////////////Player Controls/////////////////////////////////////////
namespace nControls
{
inline void DisableAssistedMovement() { ASSISTED_MOVEMENT_OVERRIDE_LOAD_DISTANCE_THIS_FRAME(NULL); return; }

Timer radioTapTimer;
void ReplaceRadioTuneForwardWithRadioOff()
{
	if (IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_VEH_RADIO_WHEEL))
		radioTapTimer.Reset();
	else if (IS_CONTROL_JUST_RELEASED(PLAYER_CONTROL, INPUT_VEH_RADIO_WHEEL) && radioTapTimer.Get() <= 250)
	{
		if (DOES_PLAYER_VEH_HAVE_RADIO() && IS_PLAYER_VEH_RADIO_ENABLE() && GET_PLAYER_RADIO_STATION_INDEX() != 255)
		{
			SET_USER_RADIO_CONTROL_ENABLED(false);
			SET_RADIO_TO_STATION_NAME("OFF");
		}
	}
	else if (radioTapTimer.Get() > 500)
	{
		radioTapTimer.Set(INT_MIN);
		SET_USER_RADIO_CONTROL_ENABLED(true);
	}
}

bool isWalking = false;
float playerLastMoveBlend = 0.0f;
Timer timerToggleFPSWalking;
void ToggleFPSWalking()
{
	if (!IS_PED_ON_FOOT(GetPlayerPed()) || !IS_CONTROL_ENABLED(PLAYER_CONTROL, INPUT_SPRINT) ||
		GET_FOLLOW_PED_CAM_VIEW_MODE() != CAM_VIEW_MODE_FIRST_PERSON)
	{
		if (isWalking)
		{
			isWalking = false;
			SET_PED_MAX_MOVE_BLEND_RATIO(GetPlayerPed(), PEDMOVEBLENDRATIO_SPRINT);
		}
		return;
	}

	//Walk Toggle
	if (IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_SPRINT))
	{
		timerToggleFPSWalking.Reset();
		playerLastMoveBlend = GET_PED_DESIRED_MOVE_BLEND_RATIO(GetPlayerPed());
		SET_PED_MAX_MOVE_BLEND_RATIO(GetPlayerPed(), playerLastMoveBlend);
	}
	else if (timerToggleFPSWalking.Get() < 250)
	{
		if (IS_CONTROL_JUST_RELEASED(PLAYER_CONTROL, INPUT_SPRINT))
		{
			if (isWalking)
			{
				SET_PED_MAX_MOVE_BLEND_RATIO(GetPlayerPed(), PEDMOVEBLENDRATIO_SPRINT);
				isWalking = false;
			}
			else
			{
				SET_PED_MAX_MOVE_BLEND_RATIO(GetPlayerPed(), PEDMOVEBLENDRATIO_WALK);
				isWalking = true;
			}
		}
		else
			SET_PED_MAX_MOVE_BLEND_RATIO(GetPlayerPed(), playerLastMoveBlend);
	}
	else if (IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_SPRINT))
	{
		isWalking = false;
		SET_PED_MAX_MOVE_BLEND_RATIO(GetPlayerPed(), PEDMOVEBLENDRATIO_SPRINT);
	}
	else if (isWalking)
		SET_PED_MAX_MOVE_BLEND_RATIO(GetPlayerPed(), PEDMOVEBLENDRATIO_WALK);
	return;
}

void DisableCameraAutoCenter()
{
	switch (Ini::DisableCameraAutoCenter)
	{
	case 1:
		if (IS_PED_ON_FOOT(GetPlayerPed()))
			SET_FOLLOW_CAM_IGNORE_ATTACH_PARENT_MOVEMENT_THIS_UPDATE();
		break;
	case 2:
		if (GetVehiclePedIsIn(GetPlayerPed(), true, true) != NULL)
			SET_FOLLOW_CAM_IGNORE_ATTACH_PARENT_MOVEMENT_THIS_UPDATE();
		break;
	case 3:
		SET_FOLLOW_CAM_IGNORE_ATTACH_PARENT_MOVEMENT_THIS_UPDATE();
		break;
	}
	return;
}

float handbrakeCamHeadingMin = 0.0f;
float handbrakeCamHeadingMax = 0.0f;
void SetCamSmoothHeadingLimit()
{
	const float frametimeMultiplier = TIMESTEP() * 275.0f;
	const float heading = GET_GAMEPLAY_CAM_RELATIVE_HEADING();
	if (heading > 0.0f)
	{
		handbrakeCamHeadingMax = heading - frametimeMultiplier;
		if (handbrakeCamHeadingMax < 0.0f)
			handbrakeCamHeadingMax = 0.0f;

		handbrakeCamHeadingMin = 0.0f;
	}
	else
	{
		handbrakeCamHeadingMin = heading + frametimeMultiplier;
		if (handbrakeCamHeadingMin > 0.0f)
			handbrakeCamHeadingMin = 0.0f;

		handbrakeCamHeadingMax = 0.0f;
	}
	return;
}

Timer timerVehCamA;
Timer timerVehCamB;
void CamFollowVehicleDuringHandbrake()
{
	const int timePressed = Ini::CamFollowVehDelay;
	constexpr int delay = 300;

	const Vehicle veh = GetVehiclePedIsUsing(GetPlayerPed());
	if (!DOES_ENTITY_EXIST(veh))
		return;

	Hash vehModel = GET_ENTITY_MODEL(veh);
	if (!IS_THIS_MODEL_A_CAR(vehModel) && !(IS_THIS_MODEL_A_BIKE(vehModel) && !IS_THIS_MODEL_A_BICYCLE(vehModel))
		&& !IS_THIS_MODEL_A_QUADBIKE(vehModel))
		return;

	if (timePressed > 0)
	{
		if (IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_VEH_HANDBRAKE))
			timerVehCamA.Reset();
		else if (timerVehCamA.Get() > timePressed && IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_VEH_HANDBRAKE))
		{
			SetCamSmoothHeadingLimit();
			SET_THIRD_PERSON_CAM_RELATIVE_HEADING_LIMITS_THIS_UPDATE(handbrakeCamHeadingMin, handbrakeCamHeadingMax);
		}
		else if (timerVehCamA.Get() > timePressed && IS_CONTROL_JUST_RELEASED(PLAYER_CONTROL, INPUT_VEH_HANDBRAKE))
		{
			timerVehCamB.Reset();
			SetCamSmoothHeadingLimit();
			SET_THIRD_PERSON_CAM_RELATIVE_HEADING_LIMITS_THIS_UPDATE(handbrakeCamHeadingMin, handbrakeCamHeadingMax);
		}
		else if (timerVehCamB.Get() < delay)
		{
			SetCamSmoothHeadingLimit();
			SET_THIRD_PERSON_CAM_RELATIVE_HEADING_LIMITS_THIS_UPDATE(handbrakeCamHeadingMin, handbrakeCamHeadingMax);
		}
	}
	else if (IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_VEH_HANDBRAKE))
	{
		SetCamSmoothHeadingLimit();
		SET_THIRD_PERSON_CAM_RELATIVE_HEADING_LIMITS_THIS_UPDATE(handbrakeCamHeadingMin, handbrakeCamHeadingMax);
	}
	return;
}

bool wasUsingHoodCam = false;
void DisableFirstPersonView()
{
	if (GET_FOLLOW_PED_CAM_VIEW_MODE() == CAM_VIEW_MODE_FIRST_PERSON)
		SET_FOLLOW_PED_CAM_VIEW_MODE(CAM_VIEW_MODE_THIRD_PERSON_NEAR);

	if (IS_FOLLOW_VEHICLE_CAM_ACTIVE())
	{
		if (GetGameVersion() >= VER_1_0_372_2_STEAM)
		{
			const bool isUsingHoodCam = GET_IS_USING_HOOD_CAMERA();	//Requires b372
			const int vehModel = GET_ENTITY_MODEL(GetVehiclePedIsUsing(GetPlayerPed()));
			if (IS_THIS_MODEL_A_CAR(vehModel) || (IS_THIS_MODEL_A_BOAT(vehModel) && !IS_THIS_MODEL_A_JETSKI(vehModel)) ||
				IS_THIS_MODEL_A_PLANE(vehModel) || IS_THIS_MODEL_A_HELI(vehModel))
			{
				if (wasUsingHoodCam && isUsingHoodCam)
					SET_FOLLOW_VEHICLE_CAM_VIEW_MODE(CAM_VIEW_MODE_FIRST_PERSON);

				if (IS_CONTROL_JUST_RELEASED(PLAYER_CONTROL, INPUT_NEXT_CAMERA))
				{
					if (GET_FOLLOW_VEHICLE_CAM_VIEW_MODE() == CAM_VIEW_MODE_THIRD_PERSON_FAR)
						wasUsingHoodCam = true;
					else
						wasUsingHoodCam = false;
				}

				if (isUsingHoodCam && !GET_IS_TASK_ACTIVE(GetPlayerPed(), CODE_TASK_EXIT_VEHICLE))
					return;
			}

			if (GET_FOLLOW_VEHICLE_CAM_VIEW_MODE() == CAM_VIEW_MODE_FIRST_PERSON && !GET_IS_TASK_ACTIVE(GetPlayerPed(), CODE_TASK_EXIT_VEHICLE))
			{
				wasUsingHoodCam = false;
				SET_FOLLOW_VEHICLE_CAM_VIEW_MODE(CAM_VIEW_MODE_THIRD_PERSON_NEAR);
			}
		}
		else
			SET_FOLLOW_VEHICLE_CAM_VIEW_MODE(CAM_VIEW_MODE_THIRD_PERSON_NEAR);
	}

	DISABLE_ON_FOOT_FIRST_PERSON_VIEW_THIS_UPDATE(); //Also works in vehicles
	return;
}

void DisableIdleCamera()
{
	INVALIDATE_CINEMATIC_VEHICLE_IDLE_MODE();
	INVALIDATE_IDLE_CAM();
	return;
}

void DisableRecording()
{
	DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_REPLAY_START_STOP_RECORDING, false);
	DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_REPLAY_START_STOP_RECORDING_SECONDARY, false);
	REPLAY_PREVENT_RECORDING_THIS_FRAME();
	return;
}

void DisableMobilePhone()
{
	if (DOES_SCRIPT_EXIST("cellphone_controller"))
		TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("cellphone_controller");

	if (IS_MOBILE_PHONE_CALL_ONGOING()) { STOP_SCRIPTED_CONVERSATION(false); }
	if (IS_PED_RINGTONE_PLAYING(GetPlayerPed())) { STOP_PED_RINGTONE(GetPlayerPed()); }
	DESTROY_MOBILE_PHONE();

	EnablePedConfigFlag(GetPlayerPed(), PCF_PhoneDisableTextingAnimations);
	EnablePedConfigFlag(GetPlayerPed(), PCF_PhoneDisableTalkingAnimations);
	EnablePedConfigFlag(GetPlayerPed(), PCF_PhoneDisableCameraAnimations);

	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_PHONE, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_UP, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_DOWN, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_LEFT, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_RIGHT, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_SELECT, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_CANCEL, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_OPTION, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_EXTRA_OPTION, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_SCROLL_FORWARD, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_SCROLL_BACKWARD, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_CAMERA_FOCUS_LOCK, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_CAMERA_GRID, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_CAMERA_SELFIE, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_CAMERA_DOF, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_CAMERA_EXPRESSION, false);
	return;
}
}	//END nControls

///////////////////////////////////////////////HUD///////////////////////////////////////////////
namespace nHUD
{
void AllowGameExecutionOnPauseMenu()
{
	if (!IS_CONTROL_PRESSED(FRONTEND_CONTROL, INPUT_FRONTEND_PAUSE_ALTERNATE))
		SET_PAUSE_MENU_ACTIVE(false);

	if (IS_CONTROL_JUST_PRESSED(FRONTEND_CONTROL, INPUT_FRONTEND_PAUSE) &&
		GET_CURRENT_FRONTEND_MENU_VERSION() != FE_MENU_VERSION_SP_PAUSE)
	{
		ACTIVATE_FRONTEND_MENU(FE_MENU_VERSION_SP_PAUSE, false, -1);
	}

	return;
}

void DisablePauseMenuPostFX()
{
	CLEAR_ALL_TCMODIFIER_OVERRIDES("hud_def_blur");
	CLEAR_ALL_TCMODIFIER_OVERRIDES("bloommid");
	CLEAR_ALL_TCMODIFIER_OVERRIDES("hud_def_Franklin");
	CLEAR_ALL_TCMODIFIER_OVERRIDES("hud_def_Michael");
	CLEAR_ALL_TCMODIFIER_OVERRIDES("hud_def_Trevor");
	CLEAR_ALL_TCMODIFIER_OVERRIDES("hud_def_desat_Neutral");

	ADD_TCMODIFIER_OVERRIDE("hud_def_blur", "default");
	ADD_TCMODIFIER_OVERRIDE("bloommid", "default");
	switch (GET_PED_TYPE(GetPlayerPed()))
	{
	case PEDTYPE_PLAYER1:			// Michael
		ADD_TCMODIFIER_OVERRIDE("hud_def_Michael", "default"); break;
	case PEDTYPE_PLAYER2:			// Franklin
		ADD_TCMODIFIER_OVERRIDE("hud_def_Franklin", "default"); break;
	case PEDTYPE_PLAYER_UNUSED:		// Trevor
		ADD_TCMODIFIER_OVERRIDE("hud_def_Trevor", "default"); break;
	default:
		ADD_TCMODIFIER_OVERRIDE("hud_def_desat_Neutral", "default"); break;
	}
	return;
}

void DisableHUDPostFX()
{
	LOOP(i, AnimPostFXSize)
	{
		ANIMPOSTFX_STOP_AND_FLUSH_REQUESTS(AnimPostFX[i]);
	}
	return;
}

void DisableSpecialAbilityPostFX()
{
	LOOP(i, AbilityPostFXSize)
	{
		ANIMPOSTFX_STOP_AND_FLUSH_REQUESTS(AbilityPostFX[i]);
	}
	return;
}

Timer timerBigMap;
bool isBigMapActive = false;
void EnableBigMapToggle()
{
	if (IS_CONTROL_JUST_PRESSED(FRONTEND_CONTROL, INPUT_HUD_SPECIAL))
		timerBigMap.Reset();
	else if (IS_CONTROL_JUST_RELEASED(FRONTEND_CONTROL, INPUT_HUD_SPECIAL) && timerBigMap.Get() < 250)
	{
		isBigMapActive ^= true;
		SET_BIGMAP_ACTIVE(isBigMapActive, false);
	}
	return;
}

float speed = 0.0f;
Timer speedTimer;
void MinimapSpeedometer()
{
	if (!IS_MINIMAP_RENDERING() || IS_RADAR_HIDDEN() || !GetVehiclePedIsIn(GetPlayerPed(), false, false))
		return;

	if (speedTimer.Get() > 100)
	{
		speed = GET_ENTITY_SPEED(GetPlayerPed());
		speedTimer.Reset();
	}

	std::string text = "";
	if (SHOULD_USE_METRIC_MEASUREMENTS())
	{
		text = std::to_string(speed * 3.6f);
		text = text.substr(0, (text.find(".") + 2)) + "km/h";
	}
	else
	{
		text = std::to_string(speed * 2.236936f);
		text = text.substr(0, (text.find(".") + 2)) + "mph";
	}

	float txtX = 0.0f, txtY = 0.0f;
	float n = (16.0f / 9.0f) / GET_SCREEN_ASPECT_RATIO();	// Scale X axis if ratio differs from 16/9
	SET_SCRIPT_GFX_ALIGN(UI_ALIGN_LEFT, UI_ALIGN_BOTTOM);
	if (isBigMapActive)
		GET_SCRIPT_GFX_ALIGN_POSITION(0.18f * n, -0.047f, &txtX, &txtY);
	else
		GET_SCRIPT_GFX_ALIGN_POSITION(0.1f * n, -0.047f, &txtX, &txtY);
	
	RESET_SCRIPT_GFX_ALIGN();

	SetTextStyle(TextStyle{ FONT_CONDENSED, 0.44f, 0.44f, RGBA{250, 250, 250, 180}, DROPSTYLE_DROPSHADOWONLY, false, 0.0f, 1.0f });

	BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
	ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text.c_str());
	END_TEXT_COMMAND_DISPLAY_TEXT(txtX, txtY, false);
	return;
}

void SetRadarZoom()
{
	if (Ini::SetRadarZoom < 0.0f)
		return;

	if (Ini::SetRadarZoom >= 100.0f)
		SET_RADAR_ZOOM_PRECISE(100.0f);
	else
		SET_RADAR_ZOOM_PRECISE(Ini::SetRadarZoom);

	DONT_ZOOM_MINIMAP_WHEN_RUNNING_THIS_FRAME();
	DONT_ZOOM_MINIMAP_WHEN_SNIPING_THIS_FRAME();
	return;
}

inline void DisableMinimapTilt() { DONT_TILT_MINIMAP_THIS_FRAME(); return; }
inline void HideMinimapFog() { SET_MINIMAP_HIDE_FOW(true); return; }
inline void HideSatNav() { CALL_SCALEFORM_MOVIE_METHOD(RequestMinimapScaleform(), "HIDE_SATNAV"); return; }
inline void HideMinimapDepth() { CALL_SCALEFORM_MOVIE_METHOD(RequestMinimapScaleform(), "HIDE_DEPTH"); return; }

void HideMinimapBars()
{
	if (BEGIN_SCALEFORM_MOVIE_METHOD(RequestMinimapScaleform(), "SETUP_HEALTH_ARMOUR"))
	{
		SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(3);
		END_SCALEFORM_MOVIE_METHOD();
	}
	return;
}

void AlwaysHideAbilityBar()
{
	if (BEGIN_SCALEFORM_MOVIE_METHOD(RequestMinimapScaleform(), "SET_ABILITY_BAR_VISIBILITY_IN_MULTIPLAYER"))
	{
		SCALEFORM_MOVIE_METHOD_ADD_PARAM_BOOL(false);
		END_SCALEFORM_MOVIE_METHOD();
	}
	return;
}

void HideAbilityBarForNonMainCharacters()
{
	bool shouldHide = true;
	if (IS_SPECIAL_ABILITY_ENABLED(GetPlayer(), 0))
		shouldHide = false;
	else if (GetGameVersion() >= VER_1_0_944_2_STEAM)
	{
		if (DoesVehicleHaveAbility(GetVehiclePedIsIn(GetPlayerPed())))
			shouldHide = false;
	}
	else if (GetGameVersion() >= VER_1_0_372_2_STEAM)
	{
		if (GET_VEHICLE_HAS_KERS(GetVehiclePedIsIn(GetPlayerPed())))
			shouldHide = false;
	}

	if (shouldHide)
	{
		if (BEGIN_SCALEFORM_MOVIE_METHOD(RequestMinimapScaleform(), "SET_ABILITY_BAR_VISIBILITY_IN_MULTIPLAYER"))
		{
			SCALEFORM_MOVIE_METHOD_ADD_PARAM_BOOL(false);
			END_SCALEFORM_MOVIE_METHOD();
		}
		return;
	}
	
	if (BEGIN_SCALEFORM_MOVIE_METHOD(RequestMinimapScaleform(), "MULTIPLAYER_IS_ACTIVE"))
	{
		SCALEFORM_MOVIE_METHOD_ADD_PARAM_BOOL(false);
		SCALEFORM_MOVIE_METHOD_ADD_PARAM_BOOL(false);
		END_SCALEFORM_MOVIE_METHOD();
	}
	return;
}

Timer timerFlashHealth;
constexpr int flashHealthInterval = 400;
void ReplaceArmourBarWithStamina()
{
	int staminaPercentage = ROUND(100.0f - GET_PLAYER_SPRINT_STAMINA_REMAINING(GetPlayer()));				//GET_PLAYER_SPRINT_STAMINA_REMAINING goes from 0 to 100 and then health depletes
	if (Ini::MergeHealthAndArmour)
	{
		int health = GET_ENTITY_HEALTH(GetPlayerPed()) - 100 + GET_PED_ARMOUR(GetPlayerPed());				//We need to subtract 100 because the player fatal health is 100 not 0
		int maxHealth = GET_ENTITY_MAX_HEALTH(GetPlayerPed()) - 100 + GET_PLAYER_MAX_ARMOUR(GetPlayer());
		int newHealthPercentage = ROUND(health * 100.0f / maxHealth);										//Always ensure a 100 offset to fix hud ratio
		int realHealthPercentage = ROUND((GET_ENTITY_HEALTH(GetPlayerPed()) - 100.0f) * 100.0f / (GET_ENTITY_MAX_HEALTH(GetPlayerPed()) - 100.0f));

		//Flash health bar every 400ms if health is below 25%
		if (realHealthPercentage > 25 || timerFlashHealth.Get() > flashHealthInterval)
		{
			if (timerFlashHealth.Get() > (flashHealthInterval * 2))
				timerFlashHealth.Reset();

			SetHealthHudDisplayValues(newHealthPercentage, staminaPercentage);
		}
		else if (realHealthPercentage <= 25 && GET_PED_ARMOUR(GetPlayerPed()) > 10)
		{
			if (timerFlashHealth.Get() <= flashHealthInterval)
				SetHealthHudDisplayValues(realHealthPercentage, staminaPercentage);
			else
				SetHealthHudDisplayValues(newHealthPercentage, staminaPercentage);
		}
	}
	else
	{
		int health = GET_ENTITY_HEALTH(GetPlayerPed()) - 100;
		int maxHealth = GET_ENTITY_MAX_HEALTH(GetPlayerPed()) - 100;
		int healthPercentage = ROUND(health * 100.0f / maxHealth);
		SetHealthHudDisplayValues(healthPercentage, staminaPercentage);
	}
	return;
}

bool InitializedHideHudComponents = false;
std::string hudComponentsStrArr[MAX_HUD_COMPONENTS]{};
unsigned int hudComponentsArr[MAX_HUD_COMPONENTS]{};
void HideHudComponents()
{
	if (!InitializedHideHudComponents)
	{
		SplitString(Ini::HudComponents.c_str(), hudComponentsStrArr, MAX_HUD_COMPONENTS, true);
		LOOP(i, MAX_HUD_COMPONENTS)
		{
			if (!hudComponentsStrArr[i].empty())
				hudComponentsArr[i] = GetHudComponentFromString(hudComponentsStrArr[i].c_str());
		}
		InitializedHideHudComponents = true;
	}

	LOOP(i, MAX_HUD_COMPONENTS)
	{
		if (hudComponentsArr[i] > 0)
			HIDE_HUD_COMPONENT_THIS_FRAME(hudComponentsArr[i]);
	}
	return;
}

void HideWeaponReticle()
{
	Hash playerWeapon = NULL;
	GET_CURRENT_PED_WEAPON(GetPlayerPed(), &playerWeapon, false);
	if (GET_WEAPONTYPE_GROUP(playerWeapon) == WEAPONGROUP_SNIPER && GET_PED_CONFIG_FLAG(GetPlayerPed(), PCF_IsAimingGun, false))
		return;

	HIDE_HUD_COMPONENT_THIS_FRAME(HUD_RETICLE);
	return;
}

inline void HideEnemiesBlips() { SET_POLICE_RADAR_BLIPS(false); return; }
}	//END nHUD

///////////////////////////////////////////////Audio///////////////////////////////////////////////
namespace nAudio
{
inline void DisableWantedMusic() { SET_AUDIO_FLAG("WantedMusicDisabled", true); return; }
inline void DisablePoliceScanner() { SET_AUDIO_FLAG("PoliceScannerDisabled", true); return; }
inline void DisableFlightMusic() { SET_AUDIO_FLAG("DisableFlightMusic", true); return; }

int DisableRadiosCount = 0;
void SetRadiosMusicOnly()
{
	LOOP(i, 5)
	{
		SET_RADIO_STATION_MUSIC_ONLY(RadioStations[DisableRadiosCount], true);

		DisableRadiosCount++;
		if (DisableRadiosCount >= RadioStationsSize)
			DisableRadiosCount = 0;
	}

	SET_RADIO_STATION_MUSIC_ONLY(GET_PLAYER_RADIO_STATION_NAME(), true);
	return;
}

Vehicle lastVehRadioOff = NULL;
void DefaultVehicleRadioOff()
{
	const Vehicle tmpVeh = GetVehiclePedIsUsing(GetPlayerPed());
	if (DOES_ENTITY_EXIST(tmpVeh) && tmpVeh != lastVehRadioOff)
	{
		lastVehRadioOff = tmpVeh;
		if (Ini::DefaultVehicleRadioOff == 2)
			SET_VEH_RADIO_STATION(lastVehRadioOff, "OFF");
		else
		{
			if (!GET_IS_VEHICLE_ENGINE_RUNNING(tmpVeh) && GET_PED_RESET_FLAG(GetPlayerPed(), PRF_IsEnteringVehicle))
				SET_VEH_RADIO_STATION(lastVehRadioOff, "OFF");
		}
	}
	return;
}

bool InitializedMuteSounds = false;
constexpr int maxMutedSounds = 10;
std::string mutedSoundsArr[maxMutedSounds]{};
void MuteSounds()
{
	if (!InitializedMuteSounds)
	{
		SplitString(Ini::Sounds.c_str(), mutedSoundsArr, maxMutedSounds, true);
		LOOP(i, maxMutedSounds)
		{
			if (mutedSoundsArr[i].empty())
				continue;

			switch (Joaat(mutedSoundsArr[i].c_str()))
			{
			case Joaat("AMBIENCE"): mutedSoundsArr[i] = "MUTES_AMBIENCE_SCENE"; break;
			case Joaat("CUTSCENE"): mutedSoundsArr[i] = "MUTES_CUTSCENE_SCENE"; break;
			case Joaat("FRONTEND"): mutedSoundsArr[i] = "MUTES_FRONTEND_SCENE"; break;
			// Mutes all music emitters, including radios (cars, static ones like the stripclub etc.)
			case Joaat("MUSIC"): mutedSoundsArr[i] = "MUTES_MUSIC_SCENE"; break;
			case Joaat("RADIO"): mutedSoundsArr[i] = "MUTES_RADIO_SCENE"; break;
			case Joaat("SFX"): mutedSoundsArr[i] = "MUTES_SFX_SCENE"; break;
			case Joaat("SPEECH"): mutedSoundsArr[i] = "MUTES_SPEECH_SCENE"; break;
			case Joaat("GUNS"): mutedSoundsArr[i] = "MUTES_GUNS_SCENE"; break;
			case Joaat("VEHICLES"): mutedSoundsArr[i] = "MUTES_VEHICLES_SCENE"; break;
			case Joaat("FOOTSTEPS"): mutedSoundsArr[i] = "MUTES_FOOTSTEPS_SCENE"; break;
			default: mutedSoundsArr[i] = ""; break;
			}
		}
		STOP_AUDIO_SCENES();
		InitializedMuteSounds = true;
	}

	LOOP(i, maxMutedSounds)
	{
		if (mutedSoundsArr[i].empty() || IS_AUDIO_SCENE_ACTIVE(mutedSoundsArr[i].c_str()))
			continue;

		START_AUDIO_SCENE(mutedSoundsArr[i].c_str());
	}
	return;
}

void MuteArtificialAmbientSounds()
{
	// Affects: ambience_general, ambience_music, positioned_radio, ambience_oneshot_vehicles, ambience_industrial, ambience_birds, ambience_speech
	if (!IS_AUDIO_SCENE_ACTIVE("CREATOR_SCENES_AMBIENCE"))
		START_AUDIO_SCENE("CREATOR_SCENES_AMBIENCE");

	return;
}

inline void DisablePlayerPainAudio() { DISABLE_PED_PAIN_AUDIO(GetPlayerPed(), true); return; }
}	//END nAudio

namespace nPeds
{
std::set<Ped> jackedPeds;
void DynamicCarJackingReactions()
{
	for (auto it = jackedPeds.begin(); it != jackedPeds.end(); )
	{
		if (!DOES_ENTITY_EXIST(*it))
		{
			it = jackedPeds.erase(it);
			continue;
		}
		++it; // Remember to increment iterator
	}

	const Hash playerVehModel = GET_ENTITY_MODEL(GetVehiclePedIsUsing(GetPlayerPed()));
	if (IS_THIS_MODEL_A_CAR(playerVehModel) || IS_THIS_MODEL_A_QUADBIKE(playerVehModel) ||
		IS_THIS_MODEL_A_BOAT(playerVehModel) || IS_THIS_MODEL_A_PLANE(playerVehModel) ||
		IS_THIS_MODEL_A_HELI(playerVehModel))
		return;

	Entity target = NULL;
	if (!GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(GetPlayer(), &target))
		return;

	// GET_CAN_PED_BE_GRABBED_BY_SCRIPT(target, true, false, false, false, true, false, false, PEDTYPE_ANIMAL)
	if (!IS_ENTITY_A_PED(target) || jackedPeds.count(target) > 0 || IS_ENTITY_A_MISSION_ENTITY(target) ||
		IS_PED_IN_COMBAT(target, GetPlayerPed()) || IS_PED_ARMED(target, 4) || IsPedACop(target) ||
		GET_ENTITY_SPEED(target) > 72 || !IS_PED_FACING_PED(target, GetPlayerPed(), 90.0f) || !IS_PED_FACING_PED(GetPlayerPed(), target, 90.0f))
		return;

	const int relGroup = GET_PED_RELATIONSHIP_GROUP_HASH(target);
	if (relGroup != RELGROUPHASH_CIVMALE && relGroup != RELGROUPHASH_CIVFEMALE && relGroup != RELGROUPHASH_NO_RELATIONSHIP)
		return;

	const Vehicle veh = GetVehiclePedIsIn(target, false, false);
	const Hash vehModel = GET_ENTITY_MODEL(GetVehiclePedIsUsing(target));
	if (!DOES_ENTITY_EXIST(veh) ||
		(!IS_THIS_MODEL_A_CAR(vehModel) && !IS_THIS_MODEL_A_BIKE(vehModel) &&
		!IS_THIS_MODEL_A_QUADBIKE(vehModel) && !IS_THIS_MODEL_A_BICYCLE(vehModel)) ||
		GET_PED_IN_VEHICLE_SEAT(veh, VS_DRIVER, false) != target ||
		!HAS_ENTITY_CLEAR_LOS_TO_ENTITY_IN_FRONT(target, GetPlayerPed())) // We want to do the LOS check at the end of everything to avoid wasting resources
		return;

	if (!GetWeightedBool(65))	// Some peds should not flee
	{
		jackedPeds.emplace(target);
		return;
	}

	constexpr float dist = 12.5f;
	const Vector3 loc = GetPlayerCoords();
	const Vector3 targetLoc = GET_ENTITY_COORDS(target, true);
	if (VDIST2(loc.x, loc.y, loc.z, targetLoc.x, targetLoc.y, targetLoc.z) > (dist * dist))
		return;

	SET_VEHICLE_CAN_BE_USED_BY_FLEEING_PEDS(veh, false);

	const int nSeats = GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(veh);
	for (int i = VS_DRIVER; i < nSeats; ++i)
	{
		if (IS_VEHICLE_SEAT_FREE(veh, i, false))
			continue;

		const Ped ped = GET_PED_IN_VEHICLE_SEAT(veh, i, false);
		STOP_CURRENT_PLAYING_SPEECH(ped);
		STOP_CURRENT_PLAYING_AMBIENT_SPEECH(ped);
		CLEAR_PED_TASKS(ped);
		TASK_LOOK_AT_ENTITY(ped, GetPlayerPed(), -1, SLF_FAST_TURN_RATE, SLF_LOOKAT_VERY_HIGH);

		const int standStillTime = GetRandomNumberInRange(250, 1250);
		const int handsUpTime = GetRandomNumberInRange(1750, 2250);
		const bool shouldReact = GetWeightedBool(50);

		int sequence = 0;
		OPEN_SEQUENCE_TASK(&sequence);
		TASK_SET_DECISION_MAKER(NULL, Joaat("empty"));	// Required to prevent the ped from running the player over, it's NOT necessary to use TASK_SET_BLOCKING_OF_NON_TEMPORARY_EVENTS
		TASK_LEAVE_ANY_VEHICLE(NULL, standStillTime, ECF_RESUME_IF_INTERRUPTED | ECF_DONT_CLOSE_DOOR | ECF_DONT_DEFAULT_WARP_IF_DOOR_BLOCKED);

		if (ped == target && shouldReact)	//Driver
		{
			// Add some dynamism by randomly reacting to the player
			TASK_LOOK_AT_ENTITY(NULL, GetPlayerPed(), -1, SLF_FAST_TURN_RATE, SLF_LOOKAT_VERY_HIGH);
			TASK_HANDS_UP(NULL, handsUpTime, GetPlayerPed(), -1, HANDS_UP_NOTHING);
			TASK_REACT_AND_FLEE_PED(NULL, GetPlayerPed());
		}
		else
			TASK_SMART_FLEE_PED(NULL, GetPlayerPed(), 75.0f, -1, false, false);

		TASK_WANDER_STANDARD(NULL, 40000.0f, 1);
		CLOSE_SEQUENCE_TASK(sequence);
		TASK_PERFORM_SEQUENCE(ped, sequence);
		CLEAR_SEQUENCE_TASK(&sequence);

		if (ped == target)
			jackedPeds.emplace(target);	// Remember jacked driver
	}
	return;
}

void EnableShootingJackedPeds()
{
	const Vehicle veh = GetVehiclePedIsEntering(GetPlayerPed());
	if (!DOES_ENTITY_EXIST(veh))
		return;

	const int seat = GET_SEAT_PED_IS_TRYING_TO_ENTER(GetPlayerPed());
	const Ped target = GET_PED_IN_VEHICLE_SEAT(veh, seat, false);
	if (!DOES_ENTITY_EXIST(target) || IS_PED_DEAD_OR_DYING(target, true))
		return;

	Hash weapon = NULL;
	if (IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_ATTACK) && GET_CURRENT_PED_WEAPON(GetPlayerPed(), &weapon, false))
	{
		const Vector3 loc = GET_WORLD_POSITION_OF_ENTITY_BONE(target, GET_PED_BONE_INDEX(target, BONETAG_HEAD));
		SET_PED_SHOOTS_AT_COORD(GetPlayerPed(), loc.x, loc.y, loc.z, true);
	}
}

// constexpr char* shoveDict = "melee@unarmed@streamed_core";
// constexpr char* shoveAnim = "shove";
constexpr char* shoveDict = "reaction@shove";
constexpr char* shoveAnim = "shove_var_a";
bool alreadyShoved = false;
int shoveShapetestHandle = NULL;
bool shoveHit = false; Vector3 shoveHitCoords = Vector3(); Vector3 shoveHitNormal = Vector3(); Entity shoveHitEntity = NULL;
void EnablePedShove()
{
	if (!RequestAnimDict(shoveDict) || !IS_CONTROL_ENABLED(PLAYER_CONTROL, INPUT_JUMP) || DOES_ENTITY_EXIST(GetVehiclePedIsUsing(GetPlayerPed())))
		return;

	const bool playingAnim = IS_ENTITY_PLAYING_ANIM(GetPlayerPed(), shoveDict, shoveAnim, 3);
	if (playingAnim)
	{
		const float animTime = GET_ENTITY_ANIM_CURRENT_TIME(GetPlayerPed(), shoveDict, shoveAnim);
		if (animTime >= 0.35f)
			STOP_ANIM_TASK(GetPlayerPed(), shoveDict, shoveAnim, REALLY_SLOW_BLEND_OUT);
		else if (animTime >= 0.15f && !alreadyShoved)
		{
			STOP_CURRENT_PLAYING_SPEECH(shoveHitEntity);
			STOP_CURRENT_PLAYING_AMBIENT_SPEECH(shoveHitEntity);
			PLAY_PED_AMBIENT_SPEECH_NATIVE(shoveHitEntity, "GENERIC_CURSE_MED", "SPEECH_PARAMS_FORCE", false);

			/*
			const int pedType = GET_PED_TYPE(shoveHitEntity);
			if ((pedType != PEDTYPE_CIVMALE && pedType != PEDTYPE_CIVFEMALE) || IS_PED_ARMED(shoveHitEntity, 1 | 4))
				TASK_COMBAT_PED(shoveHitEntity, GetPlayerPed(), 0, 16);
			*/

			const Vector3 loc = GetPlayerCoords();
			ADD_SHOCKING_EVENT_AT_POSITION(EVENT_SHOCKING_SEEN_CONFRONTATION, loc.x, loc.y, loc.z, -1.0f);

			if (!IS_PED_RAGDOLL(shoveHitEntity))
				SET_PED_TO_RAGDOLL(shoveHitEntity, -1, GetRandomNumberInRange(500, 1500), TASK_NM_BALANCE, true, true, false);

			const Vector3 force = GET_ENTITY_FORWARD_VECTOR(GetPlayerPed()) * 7.5f;
			//const Vector3 spineVec = GET_PED_BONE_COORDS(shoveHitEntity, BONETAG_SPINE3, 0.0f, 0.0f, 0.0f);
			//const Vector3 off = GET_OFFSET_FROM_ENTITY_GIVEN_WORLD_COORDS(shoveHitEntity, spineVec.x, spineVec.y, spineVec.z);
			//APPLY_FORCE_TO_ENTITY(shoveHitEntity, APPLY_TYPE_IMPULSE, force.x, force.y, 0.0f, off.x, off.y, off.z, RAGDOLL_SPINE3, false, true, true, false, true);
			APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(shoveHitEntity, APPLY_TYPE_IMPULSE, force.x, force.y, 0.0f, RAGDOLL_PELVIS, false, true, false);

			RELEASE_SCRIPT_GUID_FROM_ENTITY(shoveHitEntity);
			alreadyShoved = true;
		}

		EnablePedResetFlag(GetPlayerPed(), PRF_DisablePlayerJumping);
		EnablePedResetFlag(GetPlayerPed(), PRF_DisablePlayerVaulting);
	}
	else
	{
		alreadyShoved = false;
		RELEASE_SCRIPT_GUID_FROM_ENTITY(shoveHitEntity);
	}

	if (GET_SELECTED_PED_WEAPON(GetPlayerPed()) != WEAPON_UNARMED)
		return;

	Vector3 min = Vector3(); Vector3 max = Vector3();
	GET_MODEL_DIMENSIONS(GET_ENTITY_MODEL(GetPlayerPed()), &min, &max);
	const float radius = ((max.x - min.x) * 0.5f) - 0.25f;
	const Vector3 startVec = GET_PED_BONE_COORDS(GetPlayerPed(), BONETAG_SPINE3, 0.0f, 0.0f, 0.0f);
	const Vector3 endVec = startVec + (GET_ENTITY_FORWARD_VECTOR(GetPlayerPed()) * 1.25f);
	if (shoveShapetestHandle != NULL)
	{
		bool hit = false; Vector3 hitCoords = Vector3(); Vector3 hitNormal = Vector3(); Entity hitEntity = NULL;
		SET_SCENARIO_PEDS_TO_BE_RETURNED_BY_NEXT_COMMAND(true);	// Required
		if (GET_SHAPE_TEST_RESULT(shoveShapetestHandle, &hit, &hitCoords, &hitNormal, &hitEntity) != SHAPETEST_STATUS_RESULTS_NOTREADY)
		{
			shoveHit = hit; shoveHitCoords = hitCoords;  shoveHitNormal = hitNormal;  shoveHitEntity = hitEntity;
			shoveShapetestHandle = START_SHAPE_TEST_CAPSULE(startVec.x, startVec.y, startVec.z, endVec.x, endVec.y, endVec.z, radius,
				SCRIPT_INCLUDE_ALL, GetPlayerPed(), SCRIPT_SHAPETEST_OPTION_DEFAULT);
		}
	}
	else
		shoveShapetestHandle = START_SHAPE_TEST_CAPSULE(startVec.x, startVec.y, startVec.z, endVec.x, endVec.y, endVec.z, radius,
			SCRIPT_INCLUDE_ALL, GetPlayerPed(), SCRIPT_SHAPETEST_OPTION_DEFAULT);

	if (DOES_ENTITY_EXIST(shoveHitEntity))
	{
		if (!IS_ENTITY_A_PED(shoveHitEntity) || !IS_PED_HUMAN(shoveHitEntity) || IS_ENTITY_DEAD(shoveHitEntity, false) ||
			DOES_ENTITY_EXIST(GetVehiclePedIsUsing(shoveHitEntity)) || IS_PED_SWIMMING(shoveHitEntity) || IS_PED_SHOOTING(shoveHitEntity) ||
			IS_PED_PRONE(shoveHitEntity) || IS_PED_CLIMBING(shoveHitEntity) || IS_PED_STRAFING(shoveHitEntity) || IS_PED_A_PLAYER(shoveHitEntity) ||
			!IS_ENTITY_UPRIGHT(shoveHitEntity, 60.0f) || IS_ENTITY_A_MISSION_ENTITY(shoveHitEntity) ||
			// IS_PED_IN_MELEE_COMBAT(shoveHitEntity) || COUNT_PEDS_IN_COMBAT_WITH_TARGET(shoveHitEntity) > 0 ||
			!IS_PED_FACING_PED(GetPlayerPed(), shoveHitEntity, 60.0f))
		{
			RELEASE_SCRIPT_GUID_FROM_ENTITY(shoveHitEntity);
			return;
		}

		EnablePedResetFlag(GetPlayerPed(), PRF_DisablePlayerJumping);
		EnablePedResetFlag(GetPlayerPed(), PRF_DisablePlayerVaulting);

		if (IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_JUMP) && !playingAnim)
			PlayScriptedAnim(GetPlayerPed(), shoveDict, shoveAnim, 0.0f, 1.0f, 1.0f, APT_SINGLE_ANIM, FILTER_BOTHARMS_NOSPINE, 0.5f, NORMAL_BLEND_DURATION, -1, upperSecondaryAF);
	}
	return;
}

int DisableScenariosCount = 0;
int DisableScenarioGroupsCount = 0;
void DisableScenarios()
{
	LOOP(i, 10)
	{
		if (IS_SCENARIO_TYPE_ENABLED(Scenarios[DisableScenariosCount]))
			SET_SCENARIO_TYPE_ENABLED(Scenarios[DisableScenariosCount], false);

		DisableScenariosCount++;
		if (DisableScenariosCount >= ScenariosSize)
			DisableScenariosCount = 0;

		if (DOES_SCENARIO_GROUP_EXIST(ScenarioGroups[DisableScenarioGroupsCount]) &&
			IS_SCENARIO_GROUP_ENABLED(ScenarioGroups[DisableScenarioGroupsCount]))
			SET_SCENARIO_GROUP_ENABLED(ScenarioGroups[DisableScenarioGroupsCount], false);

		DisableScenarioGroupsCount++;
		if (DisableScenarioGroupsCount >= ScenarioGroupsSize)
			DisableScenarioGroupsCount = 0;
	}
	return;
}

void DisableWorldPopulation()
{
	SET_PED_POPULATION_BUDGET(0);
	SET_REDUCE_PED_MODEL_BUDGET(0);
	SET_PED_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
	SET_SCENARIO_PED_DENSITY_MULTIPLIER_THIS_FRAME(0.0f, 0.0f);

	SET_VEHICLE_POPULATION_BUDGET(0);
	SET_REDUCE_VEHICLE_MODEL_BUDGET(0);
	SET_RANDOM_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
	SET_PARKED_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
	//SET_AMBIENT_VEHICLE_RANGE_MULTIPLIER_THIS_FRAME(0.0f);

	SetDispatchServices(false);
	SET_RANDOM_BOATS(false);
	SET_RANDOM_TRAINS(false);
	SET_DISABLE_RANDOM_TRAINS_THIS_FRAME(true);
	return;
}
}

namespace nMemory
{
void HUDWheelSlowdownPatch()
{	
	SET_AUDIO_FLAG("AllowAmbientSpeechInSlowMo", true);
	SET_AUDIO_FLAG("AllowScriptedSpeechInSlowMo", true);
	return;
}
}

#define REGISTER_OPTION(mngr, opt, nspace, minVer, en) mngr.RegisterOption(std::make_unique<PlayerOption>(iniValue(Ini::opt), minVer, en, []() { nspace::opt(); }, #opt))
#define REGISTER_OPTION_INI(mngr, opt, nspace, nm, minVer, en) mngr.RegisterOption(std::make_unique<PlayerOption>(iniValue(Ini::nm), minVer, en, []() { nspace::opt(); }, #opt))

OptionManager playerOptionsManager;
void RegisterPlayerOptions()
{
	playerOptionsManager.UnregisterAllOptions();

	REGISTER_OPTION(playerOptionsManager, EnableCrouching, nGeneral, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, EnablePlayerActionsForAllPeds, nGeneral, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DisableActionMode, nGeneral, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DynamicallyCleanWoundsAndDirt, nGeneral, VER_UNK, true);
	REGISTER_OPTION_INI(playerOptionsManager, EnableSprintInsideInteriors, nGeneral, SprintInsideInteriors, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, SilentWanted, nGeneral, VER_UNK, true);

	////////////////////////////////////Player Weapons////////////////////////////////////
	REGISTER_OPTION(playerOptionsManager, FriendlyFire, nWeapons, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, LocationalDamage, nWeapons, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, EnableWeaponRecoil, nWeapons, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DisarmPlayerWhenShot, nWeapons, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DropPlayerWeaponWhenRagdolling, nWeapons, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, AllowWeaponsInsideSafeHouse, nWeapons, VER_UNK, true);

	//////////////////////////////////////Player Vehicle///////////////////////////////////
	REGISTER_OPTION(playerOptionsManager, DisableCarMidAirAndRollControl, nVehicle, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DisableForcedCarExplosionOnImpact, nVehicle, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DisableEngineSmoke, nVehicle, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DisableEngineFire, nVehicle, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, LeaveEngineOnWhenExitingVehicles, nVehicle, VER_UNK, true);
	if (!GetPatchedCenterSteering()) { REGISTER_OPTION(playerOptionsManager, DisableWheelsAutoCenterOnCarExit, nVehicle, VER_UNK, true); }
	REGISTER_OPTION(playerOptionsManager, KeepCarHydraulicsPosition, nVehicle, VER_1_0_2372_0_STEAM, true);
	REGISTER_OPTION(playerOptionsManager, EnableBrakeLightsOnStoppedVehicle, nVehicle, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, EnableHeliWaterPhysics, nVehicle, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DynamicallyCleanVehicles, nVehicle, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DisableRagdollOnVehicleRoof, nVehicle, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DisableDragOutCar, nVehicle, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DisableFlyThroughWindscreen, nVehicle, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DisableBikeKnockOff, nVehicle, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DisableShallowWaterBikeJumpOut, nVehicle, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DisableVehicleJitter, nVehicle, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DisableAirVehicleTurbulence, nVehicle, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DisableAutoEquipHelmets, nVehicle, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DisableStuntJumps, nVehicle, VER_UNK, true);

	//////////////////////////////////////Player Controls//////////////////////////////////
	REGISTER_OPTION(playerOptionsManager, DisableAssistedMovement, nControls, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, ReplaceRadioTuneForwardWithRadioOff, nControls, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, ToggleFPSWalking, nControls, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DisableCameraAutoCenter, nControls, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, CamFollowVehicleDuringHandbrake, nControls, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DisableFirstPersonView, nControls, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DisableIdleCamera, nControls, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DisableRecording, nControls, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DisableMobilePhone, nControls, VER_UNK, true);

	///////////////////////////////////////////HUD/////////////////////////////////////////
	REGISTER_OPTION(playerOptionsManager, AllowGameExecutionOnPauseMenu, nHUD, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DisableHUDPostFX, nHUD, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DisablePauseMenuPostFX, nHUD, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DisableSpecialAbilityPostFX, nHUD, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, EnableBigMapToggle, nHUD, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, MinimapSpeedometer, nHUD, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, SetRadarZoom, nHUD, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DisableMinimapTilt, nHUD, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, HideMinimapFog, nHUD, VER_UNK, true);
	REGISTER_OPTION_INI(playerOptionsManager, HideSatNav, nHUD, HideMinimapSatNav, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, HideMinimapDepth, nHUD, VER_UNK, true);
	if (!REGISTER_OPTION(playerOptionsManager, HideMinimapBars, nHUD, VER_UNK, true))
	{
		if (!REGISTER_OPTION(playerOptionsManager, AlwaysHideAbilityBar, nHUD, VER_UNK, true))
			REGISTER_OPTION(playerOptionsManager, HideAbilityBarForNonMainCharacters, nHUD, VER_UNK, true);

		REGISTER_OPTION(playerOptionsManager, ReplaceArmourBarWithStamina, nHUD, VER_UNK, true);
	}
	REGISTER_OPTION(playerOptionsManager, HideHudComponents, nHUD, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, HideWeaponReticle, nHUD, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, HideEnemiesBlips, nHUD, VER_UNK, true);

	//////////////////////////////////////////Audio////////////////////////////////////////
	REGISTER_OPTION(playerOptionsManager, DisableWantedMusic, nAudio, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DisablePoliceScanner, nAudio, VER_UNK, true);
	REGISTER_OPTION_INI(playerOptionsManager, DisableFlightMusic, nAudio, DisableFlyingMusic, VER_UNK, true);
	REGISTER_OPTION_INI(playerOptionsManager, SetRadiosMusicOnly, nAudio, DisableRadioInterruptions, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, MuteSounds, nAudio, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, MuteArtificialAmbientSounds, nAudio, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DisablePlayerPainAudio, nAudio, VER_UNK, true);

	//////////////////////////////////////////Peds/////////////////////////////////////////
	REGISTER_OPTION(playerOptionsManager, DynamicCarJackingReactions, nPeds, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, EnableShootingJackedPeds, nPeds, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, EnablePedShove, nPeds, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DisableScenarios, nPeds, VER_UNK, true);
	REGISTER_OPTION(playerOptionsManager, DisableWorldPopulation, nPeds, VER_UNK, true);

	/////////////////////////////////////////Memory////////////////////////////////////////
	if (GetPatchedHUDWheelSlowdown()) { REGISTER_OPTION(playerOptionsManager, HUDWheelSlowdownPatch, nMemory, VER_UNK, true); }
	return;
}

bool hasRegisteredPlayerOptions = false;
void UpdatePlayerOptions()	
{
	if (!hasRegisteredPlayerOptions)
	{
		RegisterPlayerOptions();
		hasRegisteredPlayerOptions = true;
	}

	UpdatePlayerVars();
	playerOptionsManager.ApplyOptions();
	return;
}

Timer timerRefreshIni; //Allow reload every 5s
void RefreshIni()
{
	if (IsKeyJustUp(Ini::ReloadIniKey) && timerRefreshIni.Get() > 5000)
	{
		timerRefreshIni.Reset();
		ShowNotification("GameplayFixesV:~n~Reloaded INI");
		ReadINI();
		RegisterPedOptions();
		RegisterPlayerOptions();

		// Reset vars that need to be reset on reload
		nHUD::InitializedHideHudComponents = false;
		nAudio::InitializedMuteSounds = false;
	}
	return;
}