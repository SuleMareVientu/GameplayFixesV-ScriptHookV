#include <main.h>
#include <script.h>
#include "utils\keyboard.h"
#include "globals.h"
#include "utils\mem.h"

HINSTANCE dllInstance = nullptr;
HINSTANCE GetDllInstance() { return dllInstance; }

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID lpReserved)
{
	dllInstance = hInstance;
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		scriptRegister(hInstance, ScriptMain);
		keyboardHandlerRegister(OnKeyboardMessage);
		break;
	case DLL_PROCESS_DETACH:
		scriptUnregister(hInstance);
		keyboardHandlerUnregister(OnKeyboardMessage);
		ShutdownHooks();
		break;
	}		
	return TRUE;
}