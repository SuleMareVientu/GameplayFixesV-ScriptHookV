//ScriptHook
#include <natives.h>
// #include <types.h> //Already included in globals.h
#include <main.h>
//Custom
#include "script.h"
#include "globals.h"
#include "utils\ini.h"
#include "utils\functions.h"
#include "utils\peds.h"
#include "utils\timers.h"

Player player = NULL;
Ped playerPed = NULL;
Vector3 playerLoc{ NULL, NULL, NULL, NULL, NULL, NULL };

static void update()
{
	player = PLAYER_ID();
	playerPed = PLAYER_PED_ID();

	// Check if player ped exists and control is on (e.g. not in a cutscene)
	if (!ENTITY::DOES_ENTITY_EXIST(playerPed) || !IS_PLAYER_CONTROL_ON(player))
		return;

	UpdateTimers();	//Update extra timers (TIMERC etc.)
	playerLoc = ENTITY::GET_ENTITY_COORDS(playerPed, false);
	
	//Set player options
	SetPlayerFlags();

	// Pool every 500ms to save on performance
	if (TIMERA() > 500)
	{
		SETTIMERA(0);
		SetPedsPoolFlags();
	}
	return;
}

void ScriptMain()
{
	ReadINI();
	SetupPedFunctions();
	while (true)
	{
		update();
		WAIT(0);
	}
	return;
}