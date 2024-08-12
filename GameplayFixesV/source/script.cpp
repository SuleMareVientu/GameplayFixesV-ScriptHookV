//ScriptHook
#include <natives.h>
// #include <types.h> //Already included in globals.h
#include <main.h>
//Custom
#include "script.h"
#include "utils\functions.h"
#include "utils\peds.h"
#include "utils\ini.h"
#include "globals.h"

namespace nPlayerGlobals
{
Player player = NULL;
Ped playerPed = NULL;
Vector3 playerCoords{ NULL, NULL, NULL, NULL, NULL, NULL };
}

static void update()
{
	nPlayerGlobals::player = PLAYER_ID();
	nPlayerGlobals::playerPed = PLAYER_PED_ID();

	// Check if player ped exists and control is on (e.g. not in a cutscene)
	if (!DOES_ENTITY_EXIST(nPlayerGlobals::playerPed))// || !IS_PLAYER_CONTROL_ON(GetPlayer()))
		return;

	nPlayerGlobals::playerCoords = GET_ENTITY_COORDS(nPlayerGlobals::playerPed, false);
	
	//Update player options
	UpdatePlayerOptions();

	//Update ped pool every frame... was set to 500ms to save on performance but it's needed for ped disarm
	UpdatePedsPool();
	return;
}

void ScriptMain()
{
	ReadINI();
	SetupPedFunctions();
	SET_THIS_SCRIPT_CAN_BE_PAUSED(false);
	while (true)
	{
		update();
		WAIT(0);
	}
	return;
}