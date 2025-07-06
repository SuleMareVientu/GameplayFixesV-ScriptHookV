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
	std::filesystem::path gamePath = AbsoluteModulePath(GetModuleHandle(NULL));
	if (std::filesystem::exists(gamePath.replace_filename("GTA5_Enhanced.exe")) ||
		std::filesystem::exists(gamePath.replace_filename("GFSDK_Aftermath_Lib.x64.dll")) ||
		std::filesystem::exists(gamePath.replace_filename("oo2core_5_win64.dll")))
	{
		isEnhancedVersion = true;
	}

	ReadINI();
	srand(static_cast<unsigned int>(GetTickCount64()));

	if (GetIsEnhancedVersion())
	{
		WriteLog("Info",
			"Memory patches and associated functions are not currently available on the Enhanced version of the game."
		);
	}
	else
	{
		GetGameFunctionsAddresses();
		ApplyExePatches();
	}

	while (true)
	{
		update();
		WAIT(0);
	}
	return;
}