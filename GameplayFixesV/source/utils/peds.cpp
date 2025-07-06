//ScriptHook
#include <natives.h>
// #include <types.h> //Already included in globals.h
//Custom
#include "peds.h"
#include "functions.h"
#include "ini.h"
#include "globals.h"

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
	else if (CanDisarmPed(ped, Ini::DisarmIncludeLeftHand))
	{
		CLEAR_ENTITY_LAST_WEAPON_DAMAGE(ped);
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
}

#define REGISTER_OPTION(mngr, opt, pedptr) mngr.RegisterOption(std::make_unique<PedOption>(iniValue(Ini::opt), [](Ped ped) { opt(ped); }, pedptr, #opt))
#define REGISTER_OPTION_INI(mngr, opt, pedptr, nm) mngr.RegisterOption(std::make_unique<PedOption>(iniValue(Ini::nm), []() { opt(); }, pedptr, #opt))

OptionManager pedOptionsManager;
Ped initCurrentPed = 0;
Ped* currentPedPtr = &initCurrentPed;	// ensure this is set after calling RegisterPedOptions
void RegisterPedOptions()
{
	pedOptionsManager.UnregisterAllOptions();

	REGISTER_OPTION(pedOptionsManager, DisableWrithe, currentPedPtr);
	REGISTER_OPTION(pedOptionsManager, DisableHurt, currentPedPtr);
	REGISTER_OPTION(pedOptionsManager, DisableShootFromGround, currentPedPtr);
	REGISTER_OPTION(pedOptionsManager, DisableSittingPedsInstantDeath, currentPedPtr);
	REGISTER_OPTION(pedOptionsManager, DisarmPedWhenShot, currentPedPtr);
	REGISTER_OPTION(pedOptionsManager, DisablePedOnlyDamagedByPlayer, currentPedPtr);
	REGISTER_OPTION(pedOptionsManager, DisableDeadPedsJumpOutOfVehicle, currentPedPtr);
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
		if (peds[i] == GetPlayerPed() || !IS_ENTITY_A_PED(peds[i]) || !IS_PED_HUMAN(peds[i]) || IS_ENTITY_DEAD(peds[i], false))
			continue;

		currentPedPtr = &peds[i];
		pedOptionsManager.ApplyOptions();
	}
	return;
}