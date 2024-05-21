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

	playerLoc = ENTITY::GET_ENTITY_COORDS(playerPed, false);
	
	//Set player options
	SetPlayerFlags();

	// Set ped pool every frame... was set to 500ms to save on performance but it's needed for ped disarm
	SetPedsPoolFlags();
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