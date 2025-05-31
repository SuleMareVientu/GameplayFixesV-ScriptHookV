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

static void update()
{
	// Check if player ped exists
	if (!DOES_ENTITY_EXIST(GetPlayerPed()))
		return;

	RefreshIni();

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
	while (true)
	{
		update();
		WAIT(0);
	}
	return;
}