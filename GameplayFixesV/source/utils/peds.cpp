//ScriptHook
#include <natives.h>
//Custom
#include "peds.h"
#include "../globals.h"
#include "functions.h"

static pedFunc arrPedFuncs[10];
static int countPedFuncs = -1;

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

/*
static void FriendlyMelee(Ped ped)
{
	DisablePedConfigFlag(ped, PCF_NeverEverTargetThisPed);
	SET_PED_CAN_BE_TARGETTED(ped, true);
	SET_PED_CAN_BE_TARGETED_WHEN_INJURED(ped, true);
	SET_ALLOW_LOCKON_TO_PED_IF_FRIENDLY(ped, true);
	return;
}
*/

static void AddPedFuncToArr(pedFunc func)
{
	countPedFuncs++;
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

	if (iniDisablePedOnlyDamagedByPlayer)
		AddPedFuncToArr(DisablePedOnlyDamagedByPlayer);

	if (iniDisableSittingPedsInstantDeath)
		AddPedFuncToArr(DisableSittingPedsInstantDeath);

	if (iniDisableDeadPedsJumpOutOfVehicle)
		AddPedFuncToArr(DisableDeadPedsJumpOutOfVehicle);
	return;
}

//Heavy, uses gamepools, should only be called ONCE every ~15 frames
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