#include <main.h>
#include <script.h>
#include "utils\keyboard.h"
#include "globals.h"
#include "utils\mem.h"
#include "utils\ini.h"
#include "utils\functions.h"

HINSTANCE dllInstance = nullptr;
HINSTANCE GetDllInstance() { return dllInstance; }
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
int GetGameVersion() { return gameVersion; }
bool GetIsEnhancedVersion() { return isEnhancedVersion; }

// Initialize script stuff here
void InitializeScriptPatches()
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
	srand(static_cast<uint32_t>(GetTickCount64()));

	ExtendGamePools();
	GetGameFunctionsAddresses();
	InitHooks();
	ApplyExePatches();
}

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved)
{
	dllInstance = hInstance;
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		InitializeScriptPatches();
		scriptRegister(hInstance, ScriptMain);
		keyboardHandlerRegister(OnKeyboardMessage);
		break;
	case DLL_PROCESS_DETACH:
		ShutdownHooks();
		scriptUnregister(hInstance);
		keyboardHandlerUnregister(OnKeyboardMessage);
		break;
	}		
	return TRUE;
}