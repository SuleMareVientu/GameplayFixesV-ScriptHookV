//ScriptHook
#include <shv\natives.h>
#include <shv\types.h>

#include <Windows.h>
#include <Psapi.h>
#include <globals.h>
#include <libs\pattern16\Pattern16.h>
#include "utils\ini.h"
#include "utils\functions.h"
#include "utils\mem.h"

ULONG_PTR FindPattern(std::string signature)
{
	MODULEINFO modInfo;
	if (GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &modInfo, sizeof(MODULEINFO)))
	{
		void* result = Pattern16::scan(modInfo.lpBaseOfDll, modInfo.SizeOfImage, signature);
		if (result)
			return reinterpret_cast<ULONG_PTR>(result);
	}
	else
		WriteLog("Error", "GetModuleInformation() failed! [%d]", GetLastError());

	return 0; // Pattern not found
}

ULONG_PTR FindPatternGlobal(std::string signature)
{
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	const SIZE_T minAddr = reinterpret_cast<SIZE_T>(sysInfo.lpMinimumApplicationAddress);
	const SIZE_T maxAddr = reinterpret_cast<SIZE_T>(sysInfo.lpMaximumApplicationAddress);

	MODULEINFO modInfo = { 0 };
	if (!GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &modInfo, sizeof(MODULEINFO))) {
		WriteLog("Error", "GetModuleInformation() failed! [%d]", GetLastError());
		return 0;
	}

	const SIZE_T exeStart = reinterpret_cast<SIZE_T>(modInfo.lpBaseOfDll);
	const SIZE_T exeEnd = exeStart + modInfo.SizeOfImage;

	MEMORY_BASIC_INFORMATION mbi;
	SIZE_T address = minAddr;

	while (address < maxAddr)
	{
		if (!VirtualQuery(reinterpret_cast<LPCVOID>(address), &mbi, sizeof(mbi)))
			break;

		const bool isAccessible = (!(mbi.Protect & PAGE_NOACCESS) && !(mbi.Protect & PAGE_GUARD)) &&
			((mbi.Protect & PAGE_READWRITE) || (mbi.Protect & PAGE_EXECUTE_READWRITE));

		// Filter: committed, readable, not guarded, private memory, not part of main EXE
		if ((mbi.State == MEM_COMMIT) && isAccessible && (mbi.Type == MEM_PRIVATE) &&
			((address + mbi.RegionSize < exeStart) || (address > exeEnd)))
		{
			LPCVOID result = Pattern16::scan(reinterpret_cast<void*>(address), mbi.RegionSize, signature);
			if (result)
				return reinterpret_cast<ULONG_PTR>(result);
		}

		address += mbi.RegionSize;
	}

	return 0; // Pattern not found
}

#pragma region Game Functions
namespace nUnsafe
{
ULONG_PTR(*GetScriptEntity)(Entity) = nullptr;
int fragInstNmGtaOffset = 0;
ULONG_PTR(*CreateNmMessage)(ULONG_PTR, ULONG_PTR, int) = nullptr;
void (*GivePedNMMessage)(ULONG_PTR, const char*, ULONG_PTR) = nullptr;
bool (*SetNMMessageInt)(ULONG_PTR, const char*, int) = nullptr;
bool (*SetNMMessageBool)(ULONG_PTR, const char*, bool) = nullptr;
bool (*SetNMMessageFloat)(ULONG_PTR, const char*, float) = nullptr;
bool (*SetNMMessageString)(ULONG_PTR, const char*, const char*) = nullptr;
bool (*SetNMMessageVec3)(ULONG_PTR, const char*, float, float, float) = nullptr;
}

bool foundNMFunctions = false;
bool GetFoundNMFunctions() { return foundNMFunctions; }

bool hasSearchedForGameFunctions = false;
void GetGameFunctionsAddresses()
{
	if (hasSearchedForGameFunctions)
		return;

	if (!Ini::HookGameFunctions)
	{
		WriteLog("Info", "Hooking game functions is disabled.");
		hasSearchedForGameFunctions = true;
	}

	foundNMFunctions = true;

	WriteLog("Info", "---------------------- General Functions -----------------------");

	// FiveM - GetScriptEntity
	// ULONG_PTR adr = FindPattern("44 8B C1 49 8B 41 08 41 C1 F8 08 41 38 0C 00");
	// nUnsafe::GetScriptEntity = reinterpret_cast<ULONG_PTR(*)(Entity)>(adr - 12);

	ULONG_PTR adr = FindPattern("85 ED 74 0F 8B CD E8 ?? ?? ?? ?? 48 8B F8 48 85 C0 74 2E");
	if (adr)
	{
		nUnsafe::GetScriptEntity = reinterpret_cast<ULONG_PTR(*)(Entity)>((adr + 11) + (*reinterpret_cast<const int*>(adr + 7)));
		WriteLog("Operation", "Found address of \"GetScriptEntity\" at 0x%X!", nUnsafe::GetScriptEntity);
	}
	else
	{
		WriteLog("Error", "Could not find address of \"GetScriptEntity\"!");
		foundNMFunctions = false;
	}

	WriteLog("Info", "------------------------- NM Functions -------------------------");

	adr = FindPattern("48 83 EC 28 48 8B 42 ?? 48 85 C0 74 09 48 3B 82 ?? ?? ?? ?? 74 21");
	if (adr)
	{
		nUnsafe::fragInstNmGtaOffset = *reinterpret_cast<int*>(adr + 16);
		WriteLog("Operation", "Found address of \"FragInstNmGtaOffset\" at 0x%X!", nUnsafe::fragInstNmGtaOffset);
	}
	else
	{
		WriteLog("Error", "Could not find address of \"FragInstNmGtaOffset\"!");
		foundNMFunctions = false;
	}

	adr = FindPattern("40 53 48 83 EC 20 83 61 0C 00 44 89 41 08 49 63 C0");
	if (adr)
	{
		nUnsafe::CreateNmMessage = reinterpret_cast<ULONG_PTR(*)(ULONG_PTR, ULONG_PTR, int)>(adr);
		WriteLog("Operation", "Found address of \"CreateNmMessage\" at 0x%X!", nUnsafe::CreateNmMessage);
	}
	else
	{
		WriteLog("Error", "Could not find address of \"CreateNmMessage\"!");
		foundNMFunctions = false;
	}

	adr = FindPattern("0F 84 8B 00 00 00 48 8B 47 30 48 8B 48 10 48 8B 51 20 80 7A 10 0A");
	if (adr)
	{
		nUnsafe::GivePedNMMessage = reinterpret_cast<void(*)(ULONG_PTR, const char*, ULONG_PTR)>((adr - 0x1A) + (*reinterpret_cast<const int*>(adr - 0x1E)));
		WriteLog("Operation", "Found address of \"GiveNmMessage\" at 0x%X!", nUnsafe::GivePedNMMessage);
	}
	else
	{
		WriteLog("Error", "Could not find address of \"GiveNmMessage\"!");
		foundNMFunctions = false;
	}

	adr = FindPattern("48 89 5C 24 ?? 57 48 83 EC 20 48 8B D9 48 63 49 0C 41 8B F8");
	if (adr)
	{
		nUnsafe::SetNMMessageInt = reinterpret_cast<bool(*)(ULONG_PTR, const char*, int)>(adr);
		WriteLog("Operation", "Found address of \"SetNmParameterInt\" at 0x%X!", nUnsafe::SetNMMessageInt);
	}
	else
	{
		WriteLog("Error", "Could not find address of \"SetNmParameterInt\"!");
		foundNMFunctions = false;
	}

	adr = FindPattern("48 89 5C 24 ?? 57 48 83 EC 20 48 8B D9 48 63 49 0C 41 8A F8");
	if (adr)
	{
		nUnsafe::SetNMMessageBool = reinterpret_cast<bool(*)(ULONG_PTR, const char*, bool)>(adr);
		WriteLog("Operation", "Found address of \"SetNmParameterBool\" at 0x%X!", nUnsafe::SetNMMessageBool);
	}
	else
	{
		WriteLog("Error", "Could not find address of \"SetNmParameterBool\"!");
		foundNMFunctions = false;
	}

	adr = FindPattern("40 53 48 83 EC 30 48 8B D9 48 63 49 0C");
	if (adr)
	{
		nUnsafe::SetNMMessageFloat = reinterpret_cast<bool(*)(ULONG_PTR, const char*, float)>(adr);
		WriteLog("Operation", "Found address of \"SetNmParameterFloat\" at 0x%X!", nUnsafe::SetNMMessageFloat);
	}
	else
	{
		WriteLog("Error", "Could not find address of \"SetNmParameterFloat\"!");
		foundNMFunctions = false;
	}

	adr = FindPattern("57 48 83 EC 20 48 8B D9 48 63 49 0C 49 8B E8");
	if (adr)
	{
		nUnsafe::SetNMMessageString = reinterpret_cast<bool(*)(ULONG_PTR, const char*, const char*)>(adr - 15);
		WriteLog("Operation", "Found address of \"SetNmParameterString\" at 0x%X!", nUnsafe::SetNMMessageString);
	}
	else
	{
		WriteLog("Error", "Could not find address of \"SetNmParameterString\"!");
		foundNMFunctions = false;
	}

	adr = FindPattern("40 53 48 83 EC 40 48 8B D9 48 63 49 0C");
	if (adr)
	{
		nUnsafe::SetNMMessageVec3 = reinterpret_cast<bool(*)(ULONG_PTR, const char*, float, float, float)>(adr);
		WriteLog("Operation", "Found address of \"SetNmParameterVector\" at 0x%X!", nUnsafe::SetNMMessageVec3);
	}
	else
	{
		WriteLog("Error", "Could not find address of \"SetNmParameterVector\"!");
		foundNMFunctions = false;
	}

	hasSearchedForGameFunctions = true;
	return;
}

namespace nGame
{
ULONG_PTR GetScriptEntity(Entity entity)
{
	if (nUnsafe::GetScriptEntity == nullptr)
	{
		WriteLog("Error", "Script tried to access invalid function \"GetScriptEntity\"!");
		return 0;
	}

	return nUnsafe::GetScriptEntity(entity);
}

int GetFragInstNmGtaOffset()
{
	if (nUnsafe::fragInstNmGtaOffset == 0)
	{
		WriteLog("Error", "Script tried to access invalid variable \"fragInstNmGtaOffset\"!");
		return 0;
	}

	return nUnsafe::fragInstNmGtaOffset;
}

NmMessage CreateNmMessage()
{
	if (nUnsafe::CreateNmMessage == nullptr)
	{
		WriteLog("Error", "Script tried to access invalid function \"CreateNmMessage\"!");
		return 0;
	}

	constexpr size_t msgMemSize = 0x1218;
	NmMessage msgPtr = std::make_unique<unsigned char[]>(msgMemSize);
	if (!msgPtr)
	{
		WriteLog("Error", "Memory allocation for NM message failed!");
		return 0;
	}

	ULONG_PTR ptr = reinterpret_cast<ULONG_PTR>(msgPtr.get());
	nUnsafe::CreateNmMessage(ptr, ptr + 0x18, 0x40);
	return std::move(msgPtr);
}

void GivePedNMMessage(NmMessage msgPtr, const Ped ped, const char* message)
{
	if (nUnsafe::GivePedNMMessage == nullptr)
	{
		WriteLog("Error", "Script tried to access invalid function \"GivePedNMMessage\"!");
		return;
	}

	if (!msgPtr.get())
		return;

	ULONG_PTR pedAddress = nGame::GetScriptEntity(ped);
	if (!pedAddress)
		return;

	ULONG_PTR fragInstNmGtaAddress = *(reinterpret_cast<ULONG_PTR*>(pedAddress + nGame::GetFragInstNmGtaOffset()));
	nUnsafe::GivePedNMMessage(fragInstNmGtaAddress, message, reinterpret_cast<ULONG_PTR>(msgPtr.get()));
	return;
}

void SetNMMessageParam(NmMessagePtr msgPtr, const char* msgParam, int i)
{
	if (nUnsafe::SetNMMessageInt == nullptr)
	{
		WriteLog("Error", "Script tried to access invalid function \"SetNMMessageInt\"!");
		return;
	}

	if (!msgPtr)
		return;

	nUnsafe::SetNMMessageInt(reinterpret_cast<ULONG_PTR>(msgPtr), msgParam, i);
	return;
}

void SetNMMessageParam(NmMessagePtr msgPtr, const char* msgParam, bool b)
{
	if (nUnsafe::SetNMMessageBool == nullptr)
	{
		WriteLog("Error", "Script tried to access invalid function \"SetNMMessageBool\"!");
		return;
	}

	if (!msgPtr)
		return;

	nUnsafe::SetNMMessageInt(reinterpret_cast<ULONG_PTR>(msgPtr), msgParam, b);
	return;
}

void SetNMMessageParam(NmMessagePtr msgPtr, const char* msgParam, float f)
{
	if (nUnsafe::SetNMMessageFloat == nullptr)
	{
		WriteLog("Error", "Script tried to access invalid function \"SetNMMessageFloat\"!");
		return;
	}

	if (!msgPtr)
		return;

	nUnsafe::SetNMMessageFloat(reinterpret_cast<ULONG_PTR>(msgPtr), msgParam, f);
	return;
}

void SetNMMessageParam(NmMessagePtr msgPtr, const char* msgParam, const char* str)
{
	if (nUnsafe::SetNMMessageString == nullptr)
	{
		WriteLog("Error", "Script tried to access invalid function \"SetNMMessageString\"!");
		return;
	}

	if (!msgPtr)
		return;

	nUnsafe::SetNMMessageString(reinterpret_cast<ULONG_PTR>(msgPtr), msgParam, str);
	return;
}

void SetNMMessageParam(NmMessagePtr msgPtr, const char* msgParam, float x, float y, float z)
{
	if (nUnsafe::SetNMMessageVec3 == nullptr)
	{
		WriteLog("Error", "Script tried to access invalid function \"SetNMMessageVec3\"!");
		return;
	}

	if (!msgPtr)
		return;

	nUnsafe::SetNMMessageVec3(reinterpret_cast<ULONG_PTR>(msgPtr), msgParam, x, y, z);
	return;
}
}
#pragma endregion

#pragma region Memory Patching
// Credits FiveM: https://github.com/citizenfx/fivem/blob/master/code/components/gta-streaming-five/src/UnkStuff.cpp
void LowPriorityPropsPatch()
{
	if ((GetModuleHandleW(L"vfs.dll") != nullptr))
	{
		WriteLog("Info", "Lenny's Mod Loader detected, low priority props patch disabled to prevent crashes.");
		return;
	}

	WriteLog("Info", "---------------------- Low Priority Props ----------------------");
	WriteLog("Operation", "Finding prop priority address 1...");

	ULONG_PTR address = FindPattern("BB 02 00 00 00 39 1D");
	if (address)
	{
		WriteLog("Operation", "Found address at 0x%X! Patching 1 byte...", address);

		// Make GTA default rage::fwMapData::ms_entityLevelCap to PRI_OPTIONAL_LOW, not PRI_OPTIONAL_MEDIUM (RAGE suite defaults)
		*reinterpret_cast<char*>(address + 1) = '\x03';	// mov ebx, 0x02 (PRI_OPTIONAL_MEDIUM) to mov ebx, 0x03 (PRI_OPTIONAL_LOW)

		//constexpr char priOptionLow = '\x03';
		//memmove(reinterpret_cast<void*>(address + 1), &priOptionLow, 1);

		WriteLog("Operation", "Finding prop priority address 2...");
		address = FindPattern("0F 2F 47 24 0F 93 05");
		if (address)
		{
			WriteLog("Operation", "Found address at 0x%X! Patching %d bytes...", address, 7);

			// Don't disable low-priority objects when LOD distance is <20%
			memset(reinterpret_cast<void*>(address + 4), 0x90, 7);
			WriteLog("Operation", "Done!");
		}
		else
			WriteLog("Error", "Could not find address!");
	}
	else
		WriteLog("Error", "Could not find address!");

	return;
}

bool patchedCenterSteering = false;
bool GetPatchedCenterSteering() { return patchedCenterSteering; }

// Credits aint-no-other-option: https://github.com/aint-no-other-option/CenterSteeringPatch/
void CenterSteeringPatch()
{
	constexpr int nBytes1 = 7;
	constexpr int nBytes2 = 6;

	/* Address of centering when getting out of car normally */
	WriteLog("Info", "----------------------- Center Steering ------------------------");
	WriteLog("Operation", "Finding steering address 1...");
	ULONG_PTR address = FindPattern("44 89 BB ?? ?? ?? ?? 8B 0D");
	if (address)
	{
		WriteLog("Operation", "Found address at 0x%X! Patching %d bytes...", address, nBytes1);
		memset(reinterpret_cast<void*>(address), 0x90, nBytes1);
		patchedCenterSteering = true;

		/* Address of centering when diving out */
		WriteLog("Operation", "Finding steering address 2...");
		address = FindPattern("89 82 ?? ?? ?? ?? 38 81");
		if (address)
		{
			WriteLog("Operation", "Found address at 0x%X! Patching %d bytes...", address, nBytes2);
			memset(reinterpret_cast<void*>(address), 0x90, nBytes2);
			WriteLog("Operation", "Done!");
		}
		else
			WriteLog("Error", "Could not find address!");
	}
	else
		WriteLog("Error", "Could not find address!");

	return;
}

// Credits aint-no-other-option: https://github.com/aint-no-other-option/CopBumpSteeringPatch
void CopBumpSteeringPatch()
{
	constexpr int nBytes = 8;

	WriteLog("Info", "--------------------------- Cop Bump ---------------------------");
	WriteLog("Operation", "Finding cop bump address...");

	const ULONG_PTR address = FindPattern("33 F6 F3 0F 11 87 ?? ?? ?? ?? 45 84 ED 74 25");
	if (address)
	{
		WriteLog("Operation", "Found address at 0x%X! Patching %d bytes...", address, nBytes);
		memset(reinterpret_cast<void*>(address + 2), 0x90, nBytes);
		WriteLog("Operation", "Done!");
	}
	else
		WriteLog("Error", "Could not find address!");

	return;
}

bool patchedHUDWheelSlowdown = false;
bool GetPatchedHUDWheelSlowdown() { return patchedHUDWheelSlowdown; }

// Credits CamxxCore: https://github.com/CamxxCore/GTAVWeaponWheelMod
// Has to be paired with a function that runs every frame
void HUDWheelSlowdownPatch()
{
	constexpr int nBytes = 12;

	WriteLog("Info", "---------------------- HUD Wheel Slowdown ----------------------");
	WriteLog("Operation", "Finding HUD wheel slowdown address...");

	const ULONG_PTR address = FindPattern("38 51 64 74 19");
	if (address)
	{
		WriteLog("Operation", "Found address at 0x%X! Patching %d total bytes...", address, nBytes);

		// Remove vignetting
		ULONG_PTR vignettingAdr = address + 26;
		vignettingAdr = vignettingAdr + *reinterpret_cast<int*>(vignettingAdr) + 4;
		constexpr int patchVignettingSize = 5;
		constexpr char patchVignetting[patchVignettingSize] = { '\xC3', '\x90', '\x90', '\x90', '\x90' };
		memmove(reinterpret_cast<void*>(vignettingAdr), patchVignetting, patchVignettingSize);

		// Vignetting call patch (NOP)
		memset(reinterpret_cast<void*>(address + 8), 0x90, 5);

		// Timescale override patch
		constexpr int patchTimescaleSize = 2;
		constexpr char patchTimescale[patchTimescaleSize] = { '\x31', '\xD2' };
		memmove(reinterpret_cast<void*>(address + 34), patchTimescale, patchTimescaleSize);

		patchedHUDWheelSlowdown = true;
		WriteLog("Operation", "Done!");
	}
	else
		WriteLog("Error", "Could not find address!");

	return;
}

bool hasAppliedExePatches = false;
void ApplyExePatches()
{
	if (hasAppliedExePatches)
		return;

	if (!Ini::ApplyExePatches)
	{
		WriteLog("Info", "Memory patches are disabled.");
		hasAppliedExePatches = true;
	}

	if (Ini::LowPriorityPropsPatch) { LowPriorityPropsPatch(); }
	if (Ini::CenterSteeringPatch) { CenterSteeringPatch(); }
	if (Ini::CopBumpSteeringPatch) { CopBumpSteeringPatch(); }
	if (Ini::HUDWheelSlowdownPatch) { HUDWheelSlowdownPatch(); }

	hasAppliedExePatches = true;
	return;
}
#pragma endregion

#pragma region Memory Patching Test
/*
void TerminateAllScriptsWithThisName(const char* name, int exceptId = 0)
{
	if (!exceptId)
	{
		TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME(name);
		return;
	}

	// Ensure only one instance of the script is running
	const int n = GetNumberOfScriptInstances(name) - 1;
	int count = 0;
	SCRIPT_THREAD_ITERATOR_RESET();
	while (count <= n)
	{
		const int id = SCRIPT_THREAD_ITERATOR_GET_NEXT_THREAD_ID();
		if (id == exceptId)
			continue;

		if (name == GET_NAME_OF_SCRIPT_WITH_THIS_ID(id))
		{
			TERMINATE_THREAD(id);
			count++;
		}
	}

	return;
}

int GetFirstIdFromScriptName(const char* name)
{
	SCRIPT_THREAD_ITERATOR_RESET();
	int id = 0; int count = 0;
	while (!id && count < 4096)
	{
		const int tmp = SCRIPT_THREAD_ITERATOR_GET_NEXT_THREAD_ID();
		if (name == GET_NAME_OF_SCRIPT_WITH_THIS_ID(tmp))
			id = tmp;

		count++;
	}

	return id;
}


bool patchedF0 = false; bool patchedF1 = false;
bool patchedM = false;
bool patchedT0 = false; bool patchedT1 = false;
void WeaponsInsideSafehousePatch()
{
	bool firstPrint = true;
	auto PatchScript = [&](const char* name, bool& b)
		{
			constexpr int nBytes = 3;
			if (GetNumberOfScriptInstances(name) == 0)
			{
				b = false;
				return;
			}
			else if (b)
				return;

			if (firstPrint)
			{
				WriteLog("Info", "------------------- Weapons Inside Safehouse -------------------");
				firstPrint = false;
			}

			LOOP(i, GetNumberOfScriptInstances(name))
			{
				std::string tmp = name; tmp = "Patching script: " + tmp;
				WriteLog("Info", tmp.c_str());
				WriteLog("Operation", "Finding address...");

				ULONG_PTR address = FindPatternGlobal("71 25 2F 72 2C 0C ?? ?? 71 25 26 72 2C 0C ?? ?? 71 25 16 72");
				if (address)
				{
					WriteLog("Operation", "Found address at 0x%X! Patching %d bytes...", address, nBytes);
					memset(reinterpret_cast<void*>(address - 6), 0x00, nBytes);
					memset(reinterpret_cast<void*>(address), 0x00, 20);	// Prevent pattern from being found again
					WriteLog("Operation", "Done!");
				}
				else
					WriteLog("Error", "Could not find address!");
			}
			b = true;
		};

	PatchScript("family_scene_f0", patchedF0);
	PatchScript("family_scene_f1", patchedF1);
	PatchScript("family_scene_m", patchedM);
	PatchScript("family_scene_t0", patchedT0);
	PatchScript("family_scene_t1", patchedT1);
	return;
}

bool initializedScriptPatches = false;
void ApplyScriptPatches()
{
	if (!initializedScriptPatches)
	{
		initializedScriptPatches = true;
		WriteLog("Info", "------------------------ Script Patches ------------------------");
		WriteLog("Operation", "Applying script patches...");
	}

	WeaponsInsideSafehousePatch();
	return;
}
*/
#pragma endregion