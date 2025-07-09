//ScriptHook
#include <shv\natives.h>
#include <shv\types.h>
#include <main.h>
//Custom
#include "script.h"
#include "utils\functions.h"
#include "utils\mem.h"
#include "utils\player.h"
#include "utils\peds.h"
#include "utils\ini.h"
#include "globals.h"

std::string dllInstanceName = "GameplayFixesV.asi";
std::string dllInstanceNameNoExt = "GameplayFixesV";
std::string dllInstanceIniName = "GameplayFixesV.ini";
std::string dllInstanceLogName = "GameplayFixesV.log";
int gameVersion = -1;
bool isEnhancedVersion = false;

const char* GetDllInstanceName() { return dllInstanceName.c_str(); }
const char* GetDllInstanceNameNoExt() { return dllInstanceNameNoExt.c_str(); }
const char* GetDllInstanceIniName() { return dllInstanceIniName.c_str(); }
const char* GetDllInstanceLogName() { return dllInstanceLogName.c_str(); }
const int GetGameVersion() { return gameVersion; }
const bool GetIsEnhancedVersion() { return isEnhancedVersion; }

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

	//Remember to clear last damages
	ClearLastDamages();
	return;
}

void ScriptMain()
{
	//Initialize globals
	const std::filesystem::path modulePath = AbsoluteModulePath(GetDllInstance());
	dllInstanceName = modulePath.filename().u8string();
	dllInstanceNameNoExt = modulePath.filename().replace_extension().u8string();
	dllInstanceIniName = dllInstanceNameNoExt + ".ini";
	dllInstanceLogName = dllInstanceNameNoExt + ".log";
	gameVersion = getGameVersion();
	isEnhancedVersion = (GetGameVersion() >= 1000);

	ReadINI();
	srand(static_cast<unsigned int>(GetTickCount64()));

	if (GetIsEnhancedVersion())
	{
		WriteLog("Info",
			"Memory patches and associated functions are not currently fully compatible with the Enhanced version of the game."
		);
	}

	GetGameFunctionsAddresses();
	ApplyExePatches();

	while (true)
	{
		update();
		WAIT(0);
	}
	return;
}