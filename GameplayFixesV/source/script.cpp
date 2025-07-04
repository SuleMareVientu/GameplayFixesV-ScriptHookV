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

char dllInstanceName[MAX_PATH];
char dllInstanceNameNoExt[MAX_PATH];
char dllInstanceIniName[MAX_PATH];
char dllInstanceLogName[MAX_PATH];
const char* GetDllInstanceName() { return dllInstanceName; }
const char* GetDllInstanceNameNoExt() { return dllInstanceNameNoExt; }
const char* GetDllInstanceIniName() { return dllInstanceIniName; }
const char* GetDllInstanceLogName() { return dllInstanceLogName; }

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
	strcpy_s(dllInstanceName, modulePath.filename().u8string().c_str());
	strcpy_s(dllInstanceNameNoExt, modulePath.filename().replace_extension().u8string().c_str());
	strcpy_s(dllInstanceIniName, dllInstanceNameNoExt); strcat_s(dllInstanceIniName, ".ini");
	strcpy_s(dllInstanceLogName, dllInstanceNameNoExt); strcat_s(dllInstanceLogName, ".log");

	ReadINI();
	SetupPedFunctions();
	GetGameFunctionsAddresses();
	ApplyExePatches();
	srand(static_cast<unsigned int>(GetTickCount64()));

	while (true)
	{
		update();
		WAIT(0);
	}
	return;
}