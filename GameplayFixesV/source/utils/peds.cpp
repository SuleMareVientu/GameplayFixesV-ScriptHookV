//ScriptHook
#include <natives.h>
// #include <types.h> //Already included in globals.h
//Custom
#include "peds.h"
#include "../globals.h"
#include "functions.h"

static constexpr int arrSizePedFuncs = 10;
static pedFunc arrPedFuncs[arrSizePedFuncs];
static int countPedFuncs = NULL;

static inline void DisableWrithe(Ped ped)
{
	EnablePedConfigFlag(ped, PCF_DisableGoToWritheWhenInjured);
	return;
}

static void DisableHurt(Ped ped)
{
	EnablePedConfigFlag(ped, PCF_DisableHurt);
	DisablePedConfigFlag(ped, PCF_ForceDieIfInjured);
	DisablePedConfigFlag(ped, PCF_DieWhenRagdoll);
	//Fix long melee fights
	if (PED::IS_PED_IN_MELEE_COMBAT(ped) || PED::IS_PED_FLEEING(ped))
		PED::SET_PED_DIES_WHEN_INJURED(ped, true);
	else
		PED::SET_PED_DIES_WHEN_INJURED(ped, false);
	return;
}

static void DisableSittingPedsInstantDeath(Ped ped)
{
	if (PED::GET_PED_CONFIG_FLAG(ped, PCF_IsSitting, false))
		SET_PED_SUFFERS_CRITICAL_HITS(ped, false);	//Same thing as setting CPED_CONFIG_FLAG_NoCriticalHits
	return;
}

static void DisarmPedWhenShot(Ped ped)
{
	if (IS_ENTITY_DEAD(ped, false))
		return;

	//Check if ped has a weapon and can be disarmed
	Hash weapon = NULL;
	GET_CURRENT_PED_WEAPON(ped, &weapon, false);
	if (weapon == WEAPON_UNARMED)
		return;

	//We don't want peds to always drop their weapons, because they can't pick them back up. So do this probability trick (GetWeightedBool)
	if (HAS_ENTITY_BEEN_DAMAGED_BY_WEAPON(ped, WEAPON_STUNGUN, 0)
		|| (GetWeightedBool(iniDisarmChance) && CanDisarmPed(ped, iniDisarmIncludeLeftHand)))
	{
		if (!IS_AMBIENT_SPEECH_PLAYING(ped))
			PLAY_PED_AMBIENT_SPEECH_NATIVE(ped, "GENERIC_CURSE_MED", "SPEECH_PARAMS_FORCE", false);

		SET_PED_COMBAT_ATTRIBUTES(ped, CA_ALWAYS_EQUIP_BEST_WEAPON, true);
		SET_CAN_PED_SELECT_ALL_WEAPONS(ped, true);
		SET_PED_CAN_SWITCH_WEAPON(ped, true);
		SET_PED_DROPS_WEAPON(ped);
	}
	CLEAR_ENTITY_LAST_WEAPON_DAMAGE(ped);
	return;
}

static inline void DisablePedOnlyDamagedByPlayer(Ped ped)
{
	SET_ENTITY_ONLY_DAMAGED_BY_PLAYER(ped, false);
	return;
}

static inline void DisableDeadPedsJumpOutOfVehicle(Ped ped)
{
	EnablePedConfigFlag(ped, PCF_ForceDieInCar);
	return;
}

static void AddPedFuncToArr(pedFunc func)
{
	countPedFuncs++;

	//Check that the number of functions doesn't exceed the size of the array
	if (countPedFuncs < arrSizePedFuncs)
		arrPedFuncs[countPedFuncs] = func;

	return;
}

//Using an array of functions saves us from checking ini bools for every ped
void SetupPedFunctions()
{
	countPedFuncs = -1;	//Start with -1 for array

	if (iniDisableWrithe)
		AddPedFuncToArr(DisableWrithe);

	if (iniDisableHurt)
		AddPedFuncToArr(DisableHurt);

	if (iniDisableSittingPedsInstantDeath)
		AddPedFuncToArr(DisableSittingPedsInstantDeath);

	if (iniDisarmPedWhenShot)
		AddPedFuncToArr(DisarmPedWhenShot);

	if (iniDisablePedOnlyDamagedByPlayer)
		AddPedFuncToArr(DisablePedOnlyDamagedByPlayer);

	if (iniDisableDeadPedsJumpOutOfVehicle)
		AddPedFuncToArr(DisableDeadPedsJumpOutOfVehicle);
	return;
}

//Heavy, uses gamepools
void SetPedsPoolFlags()
{
	//Get all peds
	Ped peds[512];
	int count = worldGetAllPeds(peds, 512);

	for (int i = 0; i < count; i++)
	{
		if (peds[i] == playerPed)
			continue;

		for (int i2 = 0; i2 < countPedFuncs; i2++)
		{
			arrPedFuncs[i2](peds[i]);
		}
	}
	return;
}