//ScriptHook
#include <natives.h>
// #include <types.h> //Already included in globals.h
#include <main.h>
//Custom
#include "script.h"
#include "functions.h"
#include "options.h"
#include "peds.h"
#include "ini.h"
#include "globals.h"

std::string g_hInstanceName = "GameplayFixesV.asi";
std::string g_hInstanceNameNoExt = "GameplayFixesV";
std::string g_hInstanceIniName = "GameplayFixesV.ini";
std::string g_hInstanceLogName = "GameplayFixesV.log";

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
	//Initialize globals
	const std::filesystem::path modulePath = AbsoluteModulePath(g_hInstance);
	g_hInstanceName = modulePath.filename().u8string();
	g_hInstanceNameNoExt = modulePath.filename().replace_extension().u8string();
	g_hInstanceIniName = g_hInstanceNameNoExt + ".ini";
	g_hInstanceLogName = g_hInstanceNameNoExt + ".log";

	ReadINI();
	SetupPedFunctions();
	ApplyMemPatches();
	srand(static_cast<unsigned int>(GetTickCount64()));

	while (true)
	{
		update();
		WAIT(0);
	}
	return;
}