//ScriptHook
#include <natives.h>
// #include <types.h> //Already included in globals.h
//Custom
#include "peds.h"
#include "functions.h"
#include "ini.h"
#include "../globals.h"

typedef void (*pedFunc)(Ped);

namespace
{
constexpr int arrSizePedFuncs = 10;
pedFunc arrPedFuncs[arrSizePedFuncs];
int countPedFuncs = NULL;

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
		//FORCE_PED_MOTION_STATE(ped, 0xD827C3DB, false, false, false);
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
	else if (CanDisarmPed(ped, INI::DisarmIncludeLeftHand))
	{
		CLEAR_ENTITY_LAST_WEAPON_DAMAGE(ped);
		if (GetWeightedBool(INI::DisarmChance))
			shoudDisarm = true;
	}

	if (shoudDisarm)
	{
		if (!IS_AMBIENT_SPEECH_PLAYING(ped))
			PLAY_PED_AMBIENT_SPEECH_NATIVE(ped, "GENERIC_CURSE_MED", "SPEECH_PARAMS_FORCE", false);

		SET_PED_COMBAT_ATTRIBUTES(ped, CA_ALWAYS_EQUIP_BEST_WEAPON, true);
		SET_CAN_PED_SELECT_ALL_WEAPONS(ped, true);
		SET_PED_CAN_SWITCH_WEAPON(ped, true);
		SET_PED_DROPS_WEAPON(ped);
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

void AddPedFuncToArr(pedFunc func)
{
	//Check that the number of functions doesn't exceed the size of the array
	if (countPedFuncs < arrSizePedFuncs)
		arrPedFuncs[countPedFuncs] = func;

	++countPedFuncs;
	return;
}
}

//Using an array of functions saves us from checking ini bools for every ped
void SetupPedFunctions()
{
	countPedFuncs = NULL;
	if (INI::DisableWrithe) AddPedFuncToArr(DisableWrithe);
	if (INI::DisableHurt) AddPedFuncToArr(DisableHurt);
	if (INI::DisableShootFromGround) AddPedFuncToArr(DisableShootFromGround);
	if (INI::DisableSittingPedsInstantDeath) AddPedFuncToArr(DisableSittingPedsInstantDeath);
	if (INI::DisarmPedWhenShot) AddPedFuncToArr(DisarmPedWhenShot);
	if (INI::DisablePedOnlyDamagedByPlayer) AddPedFuncToArr(DisablePedOnlyDamagedByPlayer);
	if (INI::DisableDeadPedsJumpOutOfVehicle) AddPedFuncToArr(DisableDeadPedsJumpOutOfVehicle);
	return;
}

//Heavy, uses gamepools
void UpdatePedsPool()
{
	//Get all peds
	constexpr int ARR_SIZE = 1024;
	Ped peds[ARR_SIZE];
	const int count = worldGetAllPeds(peds, ARR_SIZE);

	LOOP(i, count)
	{
		if (peds[i] == GetPlayerPed() || !IS_ENTITY_A_PED(peds[i]) || !IS_PED_HUMAN(peds[i]) || IS_ENTITY_DEAD(peds[i], false))
			continue;

		LOOP(j, countPedFuncs)
		{ arrPedFuncs[j](peds[i]); }
	}
	return;
}