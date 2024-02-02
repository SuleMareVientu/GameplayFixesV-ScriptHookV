//ScriptHook
#include <natives.h>
//Custom
#include "peds.h"
#include "../globals.h"
#include "functions.h"

static const int arrSizePedFuncs = 10;
static pedFunc arrPedFuncs[arrSizePedFuncs];
static int countPedFuncs = -1;	//Start with -1 for array

static void DisableWrithe(Ped ped)
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

//Inspired by jedijosh920 implementation of "Disarm"
static bool CanDisarmPed(Ped ped)
{
	int bone = NULL;
	GET_PED_LAST_DAMAGE_BONE(ped, &bone);

	if (bone == NULL)
		return false;
	if (iniDisarmIncludeLeftHand)
	{
		switch (bone)
		{
		case SkelRightHand:		case PHRightHand:		case IKRightHand:		case SkelRightFinger00:	case SkelRightFinger01:
		case SkelRightFinger02:	case SkelRightFinger10:	case SkelRightFinger11:	case SkelRightFinger12:	case SkelRightFinger20:
		case SkelRightFinger21:	case SkelRightFinger22:	case SkelRightFinger30:	case SkelRightFinger31:	case SkelRightFinger32:
		case SkelRightFinger40:	case SkelRightFinger41:	case SkelRightFinger42:
		case SkelLeftHand:		case PHLeftHand:		case IKLeftHand:		case SkelLeftFinger00:	case SkelLeftFinger01:
		case SkelLeftFinger02:	case SkelLeftFinger10:	case SkelLeftFinger11:	case SkelLeftFinger12:	case SkelLeftFinger20:
		case SkelLeftFinger21:	case SkelLeftFinger22:	case SkelLeftFinger30:	case SkelLeftFinger31:	case SkelLeftFinger32:
		case SkelLeftFinger40:	case SkelLeftFinger41:	case SkelLeftFinger42:
			return true;
		default:
			return false;
		}
	}
	else
	{
		switch (bone)
		{
		case SkelRightHand:		case PHRightHand:		case IKRightHand:		case SkelRightFinger00:	case SkelRightFinger01:
		case SkelRightFinger02:	case SkelRightFinger10:	case SkelRightFinger11:	case SkelRightFinger12:	case SkelRightFinger20:
		case SkelRightFinger21:	case SkelRightFinger22:	case SkelRightFinger30:	case SkelRightFinger31:	case SkelRightFinger32:
		case SkelRightFinger40:	case SkelRightFinger41:	case SkelRightFinger42:
			/*
			case SkelLeftHand:		case PHLeftHand:		case IKLeftHand:		case SkelLeftFinger00:	case SkelLeftFinger01:
			case SkelLeftFinger02:	case SkelLeftFinger10:	case SkelLeftFinger11:	case SkelLeftFinger12:	case SkelLeftFinger20:
			case SkelLeftFinger21:	case SkelLeftFinger22:	case SkelLeftFinger30:	case SkelLeftFinger31:	case SkelLeftFinger32:
			case SkelLeftFinger40:	case SkelLeftFinger41:	case SkelLeftFinger42:
			*/
			return true;
		default:
			return false;
		}
	}
	return false;
}

static void DisarmPedWhenShot(Ped ped)
{
	if (IS_ENTITY_DEAD(ped, false))
		return;

	Hash weapon = NULL;
	GET_CURRENT_PED_WEAPON(ped, &weapon, false);
	if (weapon == WEAPON_UNARMED)
		return;

	//We don't want peds to always drop their weapons, because they can't pick them back up. So do this probability trick
	SET_RANDOM_SEED(GET_GAME_TIMER());
	int randomInt = GET_RANDOM_INT_IN_RANGE(0, 100);
	bool rand = (iniDisarmChance >= randomInt);
	if (HAS_ENTITY_BEEN_DAMAGED_BY_WEAPON(ped, WEAPON_STUNGUN, 0) || (rand && CanDisarmPed(ped)))
	{
		if (!IS_AMBIENT_SPEECH_PLAYING(ped))
			PLAY_PED_AMBIENT_SPEECH_NATIVE(ped, "GENERIC_CURSE_MED", "SPEECH_PARAMS_FORCE", false);

		SET_PED_DROPS_WEAPON(ped);
		SET_CAN_PED_SELECT_ALL_WEAPONS(ped, true);
		SET_PED_CAN_SWITCH_WEAPON(ped, true);
		CLEAR_PED_LAST_DAMAGE_BONE(ped);
		CLEAR_ENTITY_LAST_WEAPON_DAMAGE(ped);
	}
	return;
}

static void DisablePedOnlyDamagedByPlayer(Ped ped)
{
	SET_ENTITY_ONLY_DAMAGED_BY_PLAYER(ped, false);
	return;
}

static void DisableSittingPedsInstantDeath(Ped ped)
{
	if (PED::GET_PED_CONFIG_FLAG(ped, PCF_IsSitting, false))
		SET_PED_SUFFERS_CRITICAL_HITS(ped, false);	//Same thing as setting CPED_CONFIG_FLAG_NoCriticalHits
	return;
}

static void DisableDeadPedsJumpOutOfVehicle(Ped ped)
{
	EnablePedConfigFlag(ped, PCF_ForceDieInCar);
	return;
}

static void AddPedFuncToArr(pedFunc func)
{
	countPedFuncs++;

	//Check that the number of functions doesn't exceed the size of the array
	if (arrSizePedFuncs < countPedFuncs)
		return;

	#pragma warning(suppress: 6386)
	arrPedFuncs[countPedFuncs] = func;
	return;
}

//Using an array of functions saves us from checking ini bools for every ped
void SetupPedFunctions()
{
	if (iniDisableWrithe)
		AddPedFuncToArr(DisableWrithe);

	if (iniDisableHurt)
		AddPedFuncToArr(DisableHurt);

	if (iniDisarmPedWhenShot)
		AddPedFuncToArr(DisarmPedWhenShot);

	if (iniDisablePedOnlyDamagedByPlayer)
		AddPedFuncToArr(DisablePedOnlyDamagedByPlayer);

	if (iniDisableSittingPedsInstantDeath)
		AddPedFuncToArr(DisableSittingPedsInstantDeath);

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