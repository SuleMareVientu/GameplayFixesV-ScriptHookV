//ScriptHook
#include <shv\natives.h>
#include <shv\types.h>
#include <main.h>
//Custom
#include "script.h"
#include "utils\functions.h"
#include "utils\player.h"
#include "utils\peds.h"
#include "globals.h"

static void update()
{
	// Check if player ped exists
	if (!DOES_ENTITY_EXIST(GetPlayerPed()))
		return;

	RefreshIni();

	//Update player options
	UpdatePlayerOptions();

	//Update ped pool every frame
	UpdatePedsPool();

	//Remember to clear last damages
	ClearLastDamages();
	return;
}

void ScriptMain()
{
	while (true)
	{
		update();
		WAIT(0);
	}
	return;
}