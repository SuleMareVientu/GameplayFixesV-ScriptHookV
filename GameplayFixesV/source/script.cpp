//ScriptHook
#include <shv\natives.h>
#include <shv\types.h>
#include <main.h>
//Custom
#include "script.h"
#include "utils\functions.h"
#include "utils\player.h"
#include "utils\peds.h"
#include "utils\mem.h"
#include "utils\ini.h"
#include "utils\profiler.h"
#include "globals.h"

static void ResetScriptState()
{
	Profiler::Reset();
	ResetWeaponDrops();
	ResetLastDamages();
	ResetPedState();
	ResetPlayerState();
	ResetSafehouseState();
}

static void update()
{
	// Guard against uninitialized state during save load, screen transitions, or dead/inactive player
	if (GET_IS_LOADING_SCREEN_ACTIVE() || !IS_PLAYER_PLAYING(PLAYER_ID()) || !DOES_ENTITY_EXIST(GetPlayerPed()))
	{
		ResetScriptState();
		return;
	}

	Profiler::BeginFrame();

	{
		PROFILE_SCOPE("RefreshIni");
		RefreshIni();
	}

	//Update player options
	{
		PROFILE_SCOPE("UpdatePlayerOptions (Total)");
		UpdatePlayerOptions();
	}

	//Update ped pool every frame
	{
		PROFILE_SCOPE("UpdatePedsPool (Total)");
		UpdatePedsPool();
	}

	//Remember to clear last damages
	{
		PROFILE_SCOPE("ClearLastDamages");
		ClearLastDamages();
	}

	Profiler::EndFrame();
	return;
}

void ScriptMain()
{
	ResetScriptState();

	while (true)
	{
		update();
		WAIT(0);
	}
	return;
}