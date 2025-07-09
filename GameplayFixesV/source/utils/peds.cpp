//ScriptHook
#include <shv\natives.h>
#include <shv\types.h>
//Custom
#include "utils\peds.h"
#include "utils\functions.h"
#include "utils\ini.h"
#include <globals.h>

//Compatibility with player disarm
int NMReactionTime = 0;
const int GetNMReactionTime() { return NMReactionTime; }

namespace
{
inline void DisableWrithe(const Ped ped)
{
	EnablePedConfigFlag(ped, PCF_DisableGoToWritheWhenInjured);
	return;
}

void DisableHurt(const Ped ped)
{
	EnablePedConfigFlag(ped, PCF_DisableHurt);
	DisablePedConfigFlag(ped, PCF_ForceDieIfInjured);
	DisablePedConfigFlag(ped, PCF_DieWhenRagdoll);
	//Fix long melee fights
	if (IS_PED_IN_MELEE_COMBAT(ped) || IS_PED_FLEEING(ped))
		SET_PED_DIES_WHEN_INJURED(ped, true);
	else
		SET_PED_DIES_WHEN_INJURED(ped, false);
	return;
}

void DisableShootFromGround(const Ped ped)
{
	if (!IS_PED_ARMED(ped, WF_INCLUDE_GUN))
		return;

	if (GET_PED_RESET_FLAG(ped, PRF_ShootFromGround) ||
		IS_ENTITY_PLAYING_ANIM(ped, "get_up@aim_from_ground@pistol@back", "intro_0", 3)		||
		IS_ENTITY_PLAYING_ANIM(ped, "get_up@aim_from_ground@pistol@back", "intro_90r", 3)	||
		IS_ENTITY_PLAYING_ANIM(ped, "get_up@aim_from_ground@pistol@back", "intro_90l", 3)	||
		IS_ENTITY_PLAYING_ANIM(ped, "get_up@aim_from_ground@pistol@back", "intro_180r", 3)	||
		IS_ENTITY_PLAYING_ANIM(ped, "get_up@aim_from_ground@pistol@back", "intro_180l", 3)	||
		IS_ENTITY_PLAYING_ANIM(ped, "get_up@aim_from_ground@rifle@back", "intro_0", 3)		||
		IS_ENTITY_PLAYING_ANIM(ped, "get_up@aim_from_ground@rifle@back", "intro_90r", 3)	||
		IS_ENTITY_PLAYING_ANIM(ped, "get_up@aim_from_ground@rifle@back", "intro_90l", 3)	||
		IS_ENTITY_PLAYING_ANIM(ped, "get_up@aim_from_ground@rifle@back", "intro_180r", 3)	||
		IS_ENTITY_PLAYING_ANIM(ped, "get_up@aim_from_ground@rifle@back", "intro_180l", 3))
	{
		SET_PED_TO_RAGDOLL(ped, 0, 75, TASK_RELAX, false, false, false);
	}
	return;
}

void DisableSittingPedsInstantDeath(const Ped ped)
{
	if (GET_PED_CONFIG_FLAG(ped, PCF_IsSitting, false))
		SET_PED_SUFFERS_CRITICAL_HITS(ped, false);	//Same thing as setting CPED_CONFIG_FLAG_NoCriticalHits
	return;
}

void DisarmPedWhenShot(const Ped ped)
{
	if (IS_ENTITY_DEAD(ped, false))
		return;

	//Check if ped has a weapon and can be disarmed
	Hash weapon = NULL;
	GET_CURRENT_PED_WEAPON(ped, &weapon, false);
	if (weapon == WEAPON_UNARMED || !HAS_ENTITY_BEEN_DAMAGED_BY_WEAPON(ped, NULL, GENERALWEAPON_TYPE_ANYWEAPON))
		return;

	//We don't want peds to always drop their weapons, because they can't pick them back up. So do this probability trick (GetWeightedBool)
	bool shoudDisarm = false;
	if (HasEntityBeenDamagedByWeaponThisFrame(ped, WEAPON_STUNGUN, GENERALWEAPON_TYPE_INVALID))
		shoudDisarm = true;
	else if (CanDisarmPed(ped, Ini::DisarmIncludeLeftHand))
	{
		ClearEntityLastWeaponDamage(ped);
		if (GetWeightedBool(Ini::DisarmChance))
			shoudDisarm = true;
	}

	if (shoudDisarm)
	{
		if (!IS_AMBIENT_SPEECH_PLAYING(ped))
			PLAY_PED_AMBIENT_SPEECH_NATIVE(ped, "GENERIC_CURSE_MED", "SPEECH_PARAMS_FORCE", false);

		SET_PED_DROPS_WEAPON(ped);
		SET_PED_COMBAT_ATTRIBUTES(ped, CA_ALWAYS_EQUIP_BEST_WEAPON, true);
		SET_CAN_PED_SELECT_ALL_WEAPONS(ped, true);
		SET_PED_CAN_SWITCH_WEAPON(ped, true);
	}
	return;
}

inline void DisablePedOnlyDamagedByPlayer(const Ped ped)
{
	SET_ENTITY_ONLY_DAMAGED_BY_PLAYER(ped, false);
	return;
}

inline void DisableDeadPedsJumpOutOfVehicle(const Ped ped)
{
	EnablePedConfigFlag(ped, PCF_ForceDieInCar);
	return;
}

void EnablePlayerNMReactionsWhenShot(const Ped shooter)
{
	const Ped ped = GetPlayerPed();
	//SET_ENTITY_HEALTH(ped, GET_ENTITY_MAX_HEALTH(ped), 0);
	
	if (IS_PED_RAGDOLL(ped) || !HasEntityBeenDamagedByWeaponThisFrame(ped, NULL, GENERALWEAPON_TYPE_ANYWEAPON) ||
		!HAS_ENTITY_BEEN_DAMAGED_BY_ENTITY(ped, shooter, true))
		return;

	ClearEntityLastDamageEntity(ped);

	int boneTag = NULL;
	Vector3 hitLoc = { 0.0f, 0.0f, 0.0f };
	if (!GET_PED_LAST_DAMAGE_BONE(ped, &boneTag) ||
		!GET_PED_LAST_WEAPON_IMPACT_COORD(shooter, &hitLoc))
		return;

	ClearPedLastDamageBone(ped);

	if (!Ini::ShouldRagdollInCover && IS_PED_IN_COVER(ped, false))
		return;

	Hash weapon = NULL;
	GET_CURRENT_PED_WEAPON(shooter, &weapon, false);
	const Hash wpGroup = GET_WEAPONTYPE_GROUP(weapon);
	switch (wpGroup)
	{
	case WEAPONGROUP_MELEE: case WEAPONGROUP_THROWN: case WEAPONGROUP_FIREEXTINGUISHER:
	case WEAPONGROUP_PETROLCAN: case WEAPONGROUP_LOUDHAILER: case WEAPONGROUP_DIGISCANNER:
	case WEAPONGROUP_NIGHTVISION: case WEAPONGROUP_PARACHUTE: case WEAPONGROUP_JETPACK:
	case WEAPONGROUP_METALDETECTOR:
		return; break;
	}

	if (!GetWeightedBool(Ini::RagdollChance))
		return;

	Vector3 shotStartLoc = { 0.0f, 0.0f, 0.0f };
	const Entity wpObject = GET_CURRENT_PED_WEAPON_ENTITY_INDEX(shooter, false);
	if (!DOES_ENTITY_EXIST(wpObject))
		shotStartLoc = GET_WORLD_POSITION_OF_ENTITY_BONE(wpObject, GET_ENTITY_BONE_INDEX_BY_NAME(wpObject, "Gun_Muzzle"));	//  "Gun_Muzzle" is the correct one
	else
		shotStartLoc = GET_PED_BONE_COORDS(shooter, BONETAG_PH_R_HAND, 0.0f, 0.0f, 0.0f);

	const bool isPedAiming = IS_PED_SHOOTING(ped) || IsPlayerAiming();
	const bool isPedCrouching = GetIsPlayerCrouching();

	const Vector3 impulseNorm = Normalize(Vector3{ hitLoc.x - shotStartLoc.x, hitLoc.y - shotStartLoc.y, hitLoc.z - shotStartLoc.z });
	const int partIndex = GetNMPartIndexFromBoneTag(boneTag);
	//const Vector3 partIndexLoc = GET_PED_BONE_COORDS(ped, GetBoneTagFromNMPartIndex(partIndex), 0.0f, 0.0f, 0.0f);

	const int minShotReactTime = Ini::MinimumRagdollTime;	// CTaskNMShot MinimumShotReactionTimePlayerMS 450
	const int maxShotReactTime = Ini::MaximumRagdollTime;
	const int reactTime = GetRandomIntInRange(minShotReactTime, maxShotReactTime);
	
	if (Ini::DontDropWeapon)
		NMReactionTime = reactTime;	//Store for compatibility with player disarm

	if (wpGroup == WEAPONGROUP_STUNGUN)
	{
		SET_PED_TO_RAGDOLL(ped, minShotReactTime, 4500, TASK_NM_SCRIPT, true, true, false);
		GIVE_PLAYER_RAGDOLL_CONTROL(GetPlayer(), true);
		TaskNMElectrocute(ped);
	}
	else
	{
		SET_PED_TO_RAGDOLL(ped, minShotReactTime, reactTime, TASK_NM_SCRIPT, true, true, false);
		GIVE_PLAYER_RAGDOLL_CONTROL(GetPlayer(), true);
		TaskNMShot(ped, weapon, partIndex, hitLoc, impulseNorm, isPedAiming, isPedCrouching);
	}
	/*
	char arr[1024];
	snprintf(arr, 1024, "Part %d; HitLoc %f, %f, %f; Impulse %f, %f, %f;", partIndex, hitLoc.x, hitLoc.y, hitLoc.z, impulseNorm.x, impulseNorm.y, impulseNorm.z);
	Print(arr, 5000);
	*/
	return;
}
}

#define REGISTER_OPTION(mngr, opt, pedptr) mngr.RegisterOption(std::make_unique<PedOption>(iniValue(Ini::opt), [](Ped ped) { opt(ped); }, pedptr, #opt))
#define REGISTER_OPTION_INI(mngr, opt, pedptr, nm) mngr.RegisterOption(std::make_unique<PedOption>(iniValue(Ini::nm), [](Ped ped) { opt(ped); }, pedptr, #opt))

OptionManager pedOptionsManager;
Ped currentPed = 0;
void RegisterPedOptions()
{
	pedOptionsManager.UnregisterAllOptions();

	REGISTER_OPTION(pedOptionsManager, DisableWrithe, &currentPed);
	REGISTER_OPTION(pedOptionsManager, DisableHurt, &currentPed);
	REGISTER_OPTION(pedOptionsManager, DisableShootFromGround, &currentPed);
	REGISTER_OPTION(pedOptionsManager, DisableSittingPedsInstantDeath, &currentPed);
	REGISTER_OPTION(pedOptionsManager, DisarmPedWhenShot, &currentPed);
	REGISTER_OPTION(pedOptionsManager, DisablePedOnlyDamagedByPlayer, &currentPed);
	REGISTER_OPTION(pedOptionsManager, DisableDeadPedsJumpOutOfVehicle, &currentPed);
	
	// Player
	if (GetFoundNMFunctions())
		REGISTER_OPTION(pedOptionsManager, EnablePlayerNMReactionsWhenShot, &currentPed);
	else
		WriteLog("Info", "NM functions not found, skipping \"EnablePlayerNMReactionsWhenShot\" option.");

	return;
}

//Heavy, uses gamepools
bool hasRegisteredPedOptions = false;
void UpdatePedsPool()
{
	if (!hasRegisteredPedOptions)
	{
		RegisterPedOptions();
		hasRegisteredPedOptions = true;
	}

	//Get all peds
	constexpr int ARR_SIZE = 1024; Ped peds[ARR_SIZE];
	const int count = worldGetAllPeds(peds, ARR_SIZE);

	LOOP(i, count)
	{
		if (!IS_ENTITY_A_PED(peds[i]) || !IS_PED_HUMAN(peds[i]) || IS_ENTITY_DEAD(peds[i], false) || IS_PED_A_PLAYER(peds[i]))
			continue;

		currentPed = peds[i];
		pedOptionsManager.ApplyOptions();
	}
	return;
}