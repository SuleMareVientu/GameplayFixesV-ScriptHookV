//ScriptHook
#include <shv\natives.h>
#include <shv\types.h>

#include <Windows.h>
#include <Psapi.h>
#include <globals.h>
#include <libs\pattern16\Pattern16.h>
#include <libs\minhook\include\MinHook.h>
#include "utils\ini.h"
#include "utils\functions.h"
#include "utils\mem.h"

// Undefine Windows min/max macros
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif
#include <algorithm> // for std::max
#include <charconv>
#include <initializer_list>

#pragma region Memory Utils
static const MODULEINFO& GetMainModuleInfo()
{
	static MODULEINFO modInfo = []() {
		MODULEINFO info{};
		HMODULE hMod = GetModuleHandle(NULL);
		if (hMod)
		{
			info.lpBaseOfDll = hMod;
			auto dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(hMod);
			if (dos->e_magic == IMAGE_DOS_SIGNATURE)
			{
				auto nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(reinterpret_cast<const uint8_t*>(hMod) + dos->e_lfanew);
				if (nt->Signature == IMAGE_NT_SIGNATURE)
				{
					info.SizeOfImage = nt->OptionalHeader.SizeOfImage;
					info.EntryPoint = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(hMod) + nt->OptionalHeader.AddressOfEntryPoint);
					return info;
				}
			}
		}

		// Fallback to PSAPI if PE headers were unexpectedly invalid
		if (!GetModuleInformation(GetCurrentProcess(), hMod, &info, sizeof(info)))
			WriteLog("Error", "GetModuleInformation() failed! [%d]", GetLastError());
		return info;
	}();
	return modInfo;
}

inline uintptr_t ResolveCall(uintptr_t callInstruction)
{
	return callInstruction ? (callInstruction + 5 + *reinterpret_cast<const int32_t*>(callInstruction + 1)) : 0;
}

inline uintptr_t ResolveRip(uintptr_t instruction, int dispOffset, int instructionLength)
{
	return instruction ? (instruction + instructionLength + *reinterpret_cast<const int32_t*>(instruction + dispOffset)) : 0;
}

ULONG_PTR FindPattern(const std::string& signature)
{
	const MODULEINFO& modInfo = GetMainModuleInfo();
	if (modInfo.lpBaseOfDll)
	{
		void* result = Pattern16::scan(modInfo.lpBaseOfDll, modInfo.SizeOfImage, signature);
		if (result)
			return reinterpret_cast<ULONG_PTR>(result);
	}

	return 0; // Pattern not found
}

ULONG_PTR FindPatternGlobal(const std::string& signature)
{
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	const SIZE_T minAddr = reinterpret_cast<SIZE_T>(sysInfo.lpMinimumApplicationAddress);
	const SIZE_T maxAddr = reinterpret_cast<SIZE_T>(sysInfo.lpMaximumApplicationAddress);

	const MODULEINFO& modInfo = GetMainModuleInfo();
	if (!modInfo.lpBaseOfDll)
		return 0;

	const SIZE_T exeStart = reinterpret_cast<SIZE_T>(modInfo.lpBaseOfDll);
	const SIZE_T exeEnd = exeStart + modInfo.SizeOfImage;

	MEMORY_BASIC_INFORMATION mbi;
	SIZE_T address = minAddr;

	while (address < maxAddr)
	{
		if (!VirtualQuery(reinterpret_cast<LPCVOID>(address), &mbi, sizeof(mbi)))
			break;

		const DWORD prot = mbi.Protect & ~(PAGE_GUARD | PAGE_NOCACHE | PAGE_WRITECOMBINE);
		const bool isReadable =
			(prot & PAGE_READONLY) ||
			(prot & PAGE_READWRITE) ||
			(prot & PAGE_EXECUTE_READ) ||
			(prot & PAGE_EXECUTE_READWRITE) ||
			(prot & PAGE_EXECUTE_WRITECOPY) ||
			(prot & PAGE_WRITECOPY);

		const bool isGuardedOrNoAccess =
			(mbi.Protect & PAGE_NOACCESS) || (mbi.Protect & PAGE_GUARD);

		if ((mbi.State == MEM_COMMIT) && isReadable && !isGuardedOrNoAccess &&
			(mbi.Type == MEM_PRIVATE) &&
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

class ScopedPageWrite
{
public:
	ScopedPageWrite(void* addr, SIZE_T size)
		: m_addr(addr), m_size(size), m_old(0), m_ok(false)
	{
		m_ok = VirtualProtect(m_addr, m_size, PAGE_EXECUTE_READWRITE, &m_old) != FALSE;
		if (!m_ok)
			WriteLog("Error", "VirtualProtect(RWX) failed at 0x%p [%d]", m_addr, GetLastError());
	}
	~ScopedPageWrite()
	{
		if (m_ok)
		{
			DWORD tmp;
			VirtualProtect(m_addr, m_size, m_old, &tmp);
		}
	}

	// Non-copyable
	ScopedPageWrite(const ScopedPageWrite&) = delete;
	ScopedPageWrite& operator=(const ScopedPageWrite&) = delete;

	bool ok() const { return m_ok; }

private:
	void* m_addr;
	SIZE_T m_size;
	DWORD  m_old;
	bool   m_ok;
};

// Convenience wrappers that guard the write and flush the instruction cache.
static bool SafeMemset(void* dst, uint8_t val, size_t len)
{
	ScopedPageWrite guard(dst, len);
	if (!guard.ok()) return false;
	memset(dst, val, len);
	FlushInstructionCache(GetCurrentProcess(), dst, len);
	return true;
}

static bool SafeMemset(uintptr_t dst, uint8_t val, size_t len)
{
	return SafeMemset(reinterpret_cast<void*>(dst), val, len);
}

static bool SafeMemmove(void* dst, const void* src, size_t len)
{
	ScopedPageWrite guard(dst, len);
	if (!guard.ok()) return false;
	memmove(dst, src, len);
	FlushInstructionCache(GetCurrentProcess(), dst, len);
	return true;
}

static bool SafeMemmove(uintptr_t dst, const void* src, size_t len)
{
	return SafeMemmove(reinterpret_cast<void*>(dst), src, len);
}

template<typename T>
static bool SafeWrite(void* dst, T val)
{
	ScopedPageWrite guard(dst, sizeof(T));
	if (!guard.ok()) return false;
	*reinterpret_cast<T*>(dst) = val;
	FlushInstructionCache(GetCurrentProcess(), dst, sizeof(T));
	return true;
}

template<typename T>
static bool SafeWrite(uintptr_t dst, T val)
{
	return SafeWrite<T>(reinterpret_cast<void*>(dst), val);
}

static bool SafePatch(uintptr_t dst, std::initializer_list<uint8_t> bytes)
{
	return SafeMemmove(dst, bytes.begin(), bytes.size());
}

static bool SafePatch(void* dst, std::initializer_list<uint8_t> bytes)
{
	return SafeMemmove(dst, bytes.begin(), bytes.size());
}
#pragma endregion

#pragma region Game Functions
namespace nUnsafe
{
ULONG_PTR (*GetScriptEntity)(Entity) = nullptr;
ULONG_PTR* pScriptArtMessageParams = 0;
int32_t*  nMessageName = 0;
bool      (*SetNMMessageInt)(ULONG_PTR, const char*, int) = nullptr;
bool      (*SetNMMessageBool)(ULONG_PTR, const char*, bool) = nullptr;
bool      (*SetNMMessageFloat)(ULONG_PTR, const char*, float) = nullptr;
bool      (*SetNMMessageString)(ULONG_PTR, const char*, const char*) = nullptr;
bool      (*SetNMMessageVec3)(ULONG_PTR, const char*, float, float, float) = nullptr;
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
		return;
	}

	foundNMFunctions = true;
	const bool enhanced = GetIsEnhancedVersion();

	WriteLog("Info", "---------------------- General Functions -----------------------");

	// FiveM - GetScriptEntity
	// ULONG_PTR adr = FindPattern("44 8B C1 49 8B 41 08 41 C1 F8 08 41 38 0C 00");
	// nUnsafe::GetScriptEntity = reinterpret_cast<ULONG_PTR(*)(Entity)>(adr - 12);

	ULONG_PTR adr = enhanced
		? FindPattern("41 8B 4C 1C ?? E8")
		: FindPattern("85 ED 74 0F 8B CD E8 ?? ?? ?? ?? 48 8B F8 48 85 C0 74 2E");

	if (adr)
	{
		nUnsafe::GetScriptEntity = reinterpret_cast<ULONG_PTR(*)(Entity)>(
			ResolveCall(adr + (enhanced ? 0x05 : 0x06)));
		WriteLog("Operation", "Found address of \"GetScriptEntity\" at 0x%p!", (void*)nUnsafe::GetScriptEntity);
	}
	else
	{
		WriteLog("Error", "Could not find address of \"GetScriptEntity\"!");
		// Decoupled: failing to find GetScriptEntity does not disable NM functions
	}

	WriteLog("Info", "------------------------- NM Functions -------------------------");

	// Get pScriptArtMessageParams & nMessageName from native CREATE_NM_MESSAGE / GIVE_PED_NM_MESSAGE
	adr = enhanced
		? FindPattern("48 C7 05 ?? ?? ?? ?? 00 00 00 00 C7 05 ?? ?? ?? ?? FF FF FF FF E9")
		: FindPattern("48 83 25 ?? ?? ?? ?? 00 44 89 2D");

	if (adr)
	{
		if (enhanced)
		{
			nUnsafe::pScriptArtMessageParams = reinterpret_cast<ULONG_PTR*>(ResolveRip(adr, 3, 11));
			nUnsafe::nMessageName = reinterpret_cast<int32_t*>(ResolveRip(adr + 11, 2, 10));
		}
		else
		{
			nUnsafe::pScriptArtMessageParams = reinterpret_cast<ULONG_PTR*>(ResolveRip(adr, 3, 8));
			nUnsafe::nMessageName = reinterpret_cast<int32_t*>(ResolveRip(adr + 8, 3, 7));
		}
		WriteLog("Operation", "Found address of \"pScriptArtMessageParams\" and \"nMessageName\" at 0x%llX!", (unsigned long long)adr);
	}
	else
	{
		WriteLog("Error", "Could not find address of \"pScriptArtMessageParams\" and \"nMessageName\"!");
		foundNMFunctions = false;
	}

	// ART::MessageParamsBase::addInt(ART::MessageParamsBase *this, const char *key, int val)
	adr = enhanced
		? FindPattern("7D ?? 45 89 C6 48 89 D7")
		: FindPattern("48 89 5C 24 ?? 57 48 83 EC 20 48 8B D9 48 63 49 0C 41 8B F8");

	if (adr)
	{
		if (enhanced) adr -= 0x14;
		nUnsafe::SetNMMessageInt = reinterpret_cast<bool(*)(ULONG_PTR, const char*, int)>(adr);
		WriteLog("Operation", "Found address of \"SetNmParameterInt\" at 0x%p!", (void*)nUnsafe::SetNMMessageInt);
	}
	else
	{
		WriteLog("Error", "Could not find address of \"SetNmParameterInt\"!");
		foundNMFunctions = false;
	}

	// ART::MessageParamsBase::addBool(ART::MessageParamsBase *this, const char *key, bool val)
	adr = enhanced
		? FindPattern("7D ?? 45 89 C6 48 89 D3")
		: FindPattern("48 89 5C 24 ?? 57 48 83 EC 20 48 8B D9 48 63 49 0C 41 8A F8");

	if (adr)
	{
		if (enhanced) adr -= 0x14;
		nUnsafe::SetNMMessageBool = reinterpret_cast<bool(*)(ULONG_PTR, const char*, bool)>(adr);
		WriteLog("Operation", "Found address of \"SetNmParameterBool\" at 0x%p!", (void*)nUnsafe::SetNMMessageBool);
	}
	else
	{
		WriteLog("Error", "Could not find address of \"SetNmParameterBool\"!");
		foundNMFunctions = false;
	}

	// ART::MessageParamsBase::addFloat(ART::MessageParamsBase *this, const char *key, float val)
	adr = enhanced
		? FindPattern("41 56 56 57 53 48 83 EC ?? 0F 29 74 24 20 48 89 CE 48 63 79")
		: FindPattern("40 53 48 83 EC 30 48 8B D9 48 63 49 0C");

	if (adr)
	{
		nUnsafe::SetNMMessageFloat = reinterpret_cast<bool(*)(ULONG_PTR, const char*, float)>(adr);
		WriteLog("Operation", "Found address of \"SetNmParameterFloat\" at 0x%p!", (void*)nUnsafe::SetNMMessageFloat);
	}
	else
	{
		WriteLog("Error", "Could not find address of \"SetNmParameterFloat\"!");
		foundNMFunctions = false;
	}

	// ART::MessageParamsBase::addString(ART::MessageParamsBase *this, const char *key, const char *val)
	adr = enhanced
		? FindPattern("41 56 56 57 53 48 83 EC ?? 48 89 CE 48 63 79")
		: FindPattern("57 48 83 EC 20 48 8B D9 48 63 49 0C 49 8B E8");

	if (adr)
	{
		if (!enhanced) adr -= 0x0F;
		nUnsafe::SetNMMessageString = reinterpret_cast<bool(*)(ULONG_PTR, const char*, const char*)>(adr);
		WriteLog("Operation", "Found address of \"SetNmParameterString\" at 0x%p!", (void*)nUnsafe::SetNMMessageString);
	}
	else
	{
		WriteLog("Error", "Could not find address of \"SetNmParameterString\"!");
		foundNMFunctions = false;
	}

	// ART::MessageParamsBase::addVector3(ART::MessageParamsBase* this, const char* key, float x, float y, float z)
	adr = enhanced
		? FindPattern("0F 29 7C 24 30 0F 29 74 24 20 48 89 CE 48 63 79")
		: FindPattern("40 53 48 83 EC 40 48 8B D9 48 63 49 0C");

	if (adr)
	{
		if (enhanced) adr -= 0x0F;
		nUnsafe::SetNMMessageVec3 = reinterpret_cast<bool(*)(ULONG_PTR, const char*, float, float, float)>(adr);
		WriteLog("Operation", "Found address of \"SetNmParameterVector\" at 0x%p!", (void*)nUnsafe::SetNMMessageVec3);
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
	ULONG_PTR GetScriptEntity(const Entity entity)
	{
		if (!nUnsafe::GetScriptEntity)
		{
			static bool s_logged = false;
			if (!s_logged)
			{
				WriteLog("Error", "Script tried to access invalid function \"GetScriptEntity\"!");
				s_logged = true;
			}
			return 0;
		}
		return nUnsafe::GetScriptEntity(entity);
	}

	ULONG_PTR CreateNmMessage()
	{
		if (!nUnsafe::pScriptArtMessageParams || !nUnsafe::nMessageName)
		{
			static bool s_logged = false;
			if (!s_logged)
			{
				WriteLog("Error", "Script tried to access invalid function \"CreateNmMessage\"!");
				s_logged = true;
			}
			return 0;
		}

		CREATE_NM_MESSAGE(false, 0);
		return *nUnsafe::pScriptArtMessageParams;
	}

	void GivePedNMMessage(ULONG_PTR msgPtr, const Ped ped, eNMStr message)
	{
		if (!msgPtr)
			return;

		if (!nUnsafe::pScriptArtMessageParams || !nUnsafe::nMessageName)
		{
			static bool s_logged = false;
			if (!s_logged)
			{
				WriteLog("Error", "Script tried to access invalid function \"GivePedNMMessage\"!");
				s_logged = true;
			}
			return;
		}

		*nUnsafe::nMessageName = message;
		GIVE_PED_NM_MESSAGE(ped);
		return;
	}

	void SetNMMessageParam(ULONG_PTR msgPtr, const char* msgParam, int i)
	{
		if (!msgPtr)
			return;

		if (!nUnsafe::SetNMMessageInt)
		{
			static bool s_logged = false;
			if (!s_logged)
			{
				WriteLog("Error", "Script tried to access invalid function \"SetNMMessageInt\"!");
				s_logged = true;
			}
			return;
		}

		nUnsafe::SetNMMessageInt(msgPtr, msgParam, i);
		return;
	}

	void SetNMMessageParam(ULONG_PTR msgPtr, const char* msgParam, bool b)
	{
		if (!msgPtr)
			return;

		if (!nUnsafe::SetNMMessageBool)
		{
			static bool s_logged = false;
			if (!s_logged)
			{
				WriteLog("Error", "Script tried to access invalid function \"SetNMMessageBool\"!");
				s_logged = true;
			}
			return;
		}

		nUnsafe::SetNMMessageBool(msgPtr, msgParam, b);
		return;
	}

	void SetNMMessageParam(ULONG_PTR msgPtr, const char* msgParam, float f)
	{
		if (!msgPtr)
			return;

		if (!nUnsafe::SetNMMessageFloat)
		{
			static bool s_logged = false;
			if (!s_logged)
			{
				WriteLog("Error", "Script tried to access invalid function \"SetNMMessageFloat\"!");
				s_logged = true;
			}
			return;
		}

		nUnsafe::SetNMMessageFloat(msgPtr, msgParam, f);
		return;
	}

	void SetNMMessageParam(ULONG_PTR msgPtr, const char* msgParam, const char* str)
	{
		if (!msgPtr)
			return;

		if (!nUnsafe::SetNMMessageString)
		{
			static bool s_logged = false;
			if (!s_logged)
			{
				WriteLog("Error", "Script tried to access invalid function \"SetNMMessageString\"!");
				s_logged = true;
			}
			return;
		}

		nUnsafe::SetNMMessageString(msgPtr, msgParam, str);
		return;
	}

	void SetNMMessageParam(ULONG_PTR msgPtr, const char* msgParam, float x, float y, float z)
	{
		if (!msgPtr)
			return;

		if (!nUnsafe::SetNMMessageVec3)
		{
			static bool s_logged = false;
			if (!s_logged)
			{
				WriteLog("Error", "Script tried to access invalid function \"SetNMMessageVec3\"!");
				s_logged = true;
			}
			return;
		}

		nUnsafe::SetNMMessageVec3(msgPtr, msgParam, x, y, z);
		return;
	}
}
#pragma endregion

#pragma region Memory Hooking
typedef void(__fastcall* DoDisableInput_t)(void*, uint32_t, const void*, bool);
DoDisableInput_t TrampolineDoDisableInput = nullptr;
void __fastcall DetourDoDisableInput(void* _this, uint32_t input, const void* options, bool disableRelatedInputs)
{
	if (isPlayerInsideSafehouse)
	{
		switch (input)
		{
		case INPUT_SPRINT: case INPUT_JUMP: case INPUT_AIM: case INPUT_SELECT_WEAPON:
		case INPUT_PICKUP: case INPUT_TALK: case INPUT_DETONATE: case INPUT_VEH_AIM:
		case INPUT_VEH_ATTACK: case INPUT_VEH_ATTACK2: case INPUT_VEH_PASSENGER_ATTACK:
		case INPUT_VEH_SELECT_NEXT_WEAPON: case INPUT_VEH_SELECT_PREV_WEAPON:
		case INPUT_VEH_FLY_SELECT_NEXT_WEAPON: case INPUT_MELEE_ATTACK_LIGHT:
		case INPUT_MELEE_ATTACK_HEAVY: case INPUT_MELEE_ATTACK_ALTERNATE: case INPUT_MELEE_BLOCK:
			return;
		default:
			break;
		}
	}
	if (TrampolineDoDisableInput)
		TrampolineDoDisableInput(_this, input, options, disableRelatedInputs);
	return;
}

typedef bool(__fastcall* EquipWeapon_t)(void*, uint32_t, uint32_t, bool, bool, uint32_t);
EquipWeapon_t TrampolineEquipWeapon = nullptr;
bool __fastcall DetourEquipWeapon(void* _this, uint32_t uWeaponNameHash, uint32_t iVehicleIndex,
	bool bCreateWeaponWhenLoaded, bool bProcessWeaponInstructions, uint32_t attach)
{

	if (_this && isPlayerInsideSafehouse && uWeaponNameHash == WEAPON_UNARMED && isPlayerArmed)
	{
		const uintptr_t pedPtr = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(_this) + 0x10);
		if (pedPtr && pedPtr == playerPedAddress)
			return false;
	}

	if (TrampolineEquipWeapon)
		return TrampolineEquipWeapon(_this, uWeaponNameHash, iVehicleIndex,
			bCreateWeaponWhenLoaded, bProcessWeaponInstructions, attach);

	return false;
}

void AllowWeaponsInsideSafeHouse()
{
	// WriteLog("Info", "---------------------- AllowWeaponsInsideSafeHouse ----------------------");

	const bool enhanced = GetIsEnhancedVersion();
	ULONG_PTR target = NULL;

	// Hook DoDisableInput() and EquipWeapon() called from game scripts
	target = enhanced
		? FindPattern("48 89 C1 BA 1A 00 00 00")
		: FindPattern("44 8A CD BA 1A 00 00 00");

	if (target)
	{
		target = ResolveCall(target + 11);
		WriteLog("Operation", "Found address of \"DoDisableInput\" at 0x%p!", (void*)target);
		MH_STATUS st = MH_CreateHook(reinterpret_cast<LPVOID>(target),
			reinterpret_cast<LPVOID>(DetourDoDisableInput),
			reinterpret_cast<LPVOID*>(&TrampolineDoDisableInput));
		if (st != MH_OK)
			WriteLog("Error", "MH_CreateHook for \"DoDisableInput\" failed with error [%d]!", st);
	}
	else
		WriteLog("Error", "Could not find address of \"DoDisableInput\"!");

	target = enhanced
		? FindPattern("48 89 E9 89 DA 45 31 C9 E8 ?? ?? ?? ?? 8B")
		: FindPattern("4C 8B 55 10 49 8B 82 D0 00 00 00 48 85 C0 74");

	if (target)
	{
		if (enhanced)
			target = ResolveCall(target + 8);
		else
			target -= 59;

		WriteLog("Operation", "Found address of \"EquipWeapon\" at 0x%p!", (void*)target);
		MH_STATUS st = MH_CreateHook(reinterpret_cast<LPVOID>(target),
			reinterpret_cast<LPVOID>(DetourEquipWeapon),
			reinterpret_cast<LPVOID*>(&TrampolineEquipWeapon));
		if (st != MH_OK)
			WriteLog("Error", "MH_CreateHook for \"EquipWeapon\" failed with error [%d]!", st);
	}
	else
		WriteLog("Error", "Could not find address of \"EquipWeapon\"!");

	return;
}

bool hasInitializedHooks = false;
void InitHooks()
{
	if (hasInitializedHooks)
		return;

	if (MH_STATUS st = MH_Initialize(); st != MH_OK)
	{
		WriteLog("Error", "Minhook initialization failed. Error: [%d]", st);
		return;
	}

	if (!Ini::HookGameFunctions)
	{
		WriteLog("Info", "Function hooks are disabled.");
		hasInitializedHooks = true;
		return;
	}

	WriteLog("Info", "------------------------- Enable Hooks -------------------------");

	if (Ini::AllowWeaponsInsideSafeHouse)
		AllowWeaponsInsideSafeHouse();

	if (MH_STATUS st = MH_EnableHook(MH_ALL_HOOKS); st != MH_OK && st != MH_ERROR_ENABLED)
		WriteLog("Error", "MH_EnableHook(MH_ALL_HOOKS) failed with error [%d]!", st);

	hasInitializedHooks = true;
	return;
}

void ShutdownHooks()
{
	WriteLog("Info", "------------------------ Disable Hooks -------------------------");
	MH_DisableHook(MH_ALL_HOOKS);
	MH_Uninitialize();
	return;
}
#pragma endregion

#pragma region Game Pools
struct PoolConfig {
	uint32_t increment = 0;
	uint32_t minSize = 0;
	std::string name;
};

static std::unordered_map<uint32_t, PoolConfig> poolConfigs;

bool hasPoolJsonLoaded = false;
bool LoadPoolsJson()
{
	if (hasPoolJsonLoaded)
		return true;

	json j;
	try
	{
		const std::string raw = LoadJSONResource(
			GetDllInstance(),
			GetIsEnhancedVersion() ? "ENHANCEDPOOLS" : "LEGACYPOOLS");
		j = json::parse(raw);
	}
	catch (const std::exception& e)
	{
		WriteLog("Error", "Failed to parse pools JSON: %s", e.what());
		hasPoolJsonLoaded = true;
		return false;
	}

	for (const auto& [pool, _value] : j.items())
	{
		if (!_value.is_string())
			continue;

		const std::string value = _value.get<std::string>();
		if (value.size() < 2)
			continue;

		const char prefix = value[0];
		if (prefix != '+' && prefix != '>')
			continue;

		uint32_t num = 0;
		auto [ptr, ec] = std::from_chars(value.data() + 1, value.data() + value.size(), num);
		if (ec != std::errc())
			continue;

		const uint32_t poolHash = Joaat(pool.c_str());
		auto& cfg = poolConfigs[poolHash];
		cfg.name = pool;

		if (prefix == '+')
			cfg.increment += num;
		else if (prefix == '>')
			cfg.minSize = std::max(cfg.minSize, num);
	}

	hasPoolJsonLoaded = true;
	return true;
}

typedef uint32_t(__fastcall* GetSizeOfPool_t)(void*, uint32_t, uint32_t);
GetSizeOfPool_t TrampolineGetSizeOfPool = nullptr;
uint32_t __fastcall DetourGetSizeOfPool(void* _this, uint32_t poolNameHash, uint32_t defaultSize)
{
	if (!TrampolineGetSizeOfPool)
		return defaultSize;

	const uint32_t size = TrampolineGetSizeOfPool(_this, poolNameHash, defaultSize);
	uint32_t add = 0, min = 0;
	const char* name = nullptr;

	if (auto it = poolConfigs.find(poolNameHash); it != poolConfigs.end())
	{
		add = it->second.increment;
		min = it->second.minSize;
		name = it->second.name.c_str();
	}

	const uint32_t newSize = std::max(size + add, min);
	if (newSize != size)
	{
		WriteLog("Operation", "Pool \"%s\" extended to %u (was %u)",
			name ? name : "Unknown", newSize, size);
	}

	return newSize;
}

bool IsFunctionHooked(LPVOID funcPtr) {
	if (!funcPtr)
		return false;

	const uint8_t* bytes = reinterpret_cast<const uint8_t*>(funcPtr);

	// Relative JMP (E9 xx xx xx xx)
	if (bytes[0] == 0xE9)
		return true;

	// Absolute JMP via MOV+JMP, 64-bit hooks
	// FF 25 00 00 00 00 — JMP [RIP+0]
	if (bytes[0] == 0xFF && bytes[1] == 0x25)
		return true;

	// PUSH+RET (68 xx xx xx xx C3), 32-bit absolute jump
	if (bytes[0] == 0x68 && bytes[5] == 0xC3)
		return true;

	return false;
}

inline void MH_CreateEnableHook_Safe(LPVOID address, LPVOID detour, LPVOID* trampoline)
{
	if (address && !IsFunctionHooked(address))
	{
		MH_CreateHook(reinterpret_cast<LPVOID>(address), detour, trampoline);
		MH_EnableHook(reinterpret_cast<LPVOID>(address));
	}
}

bool hasExtendedGamePools = false;
void ExtendGamePools()
{
	if (!Ini::ExtendGamePools)
	{
		WriteLog("Info", "Extended pools are disabled.");
		hasExtendedGamePools = true;
		return;
	}

	if (!LoadPoolsJson())
	{
		WriteLog("Error", "ExtendGamePools: pool JSON failed to load, skipping.");
		hasExtendedGamePools = true;
		return;
	}

	WriteLog("Info", "------------------------- Extend Pools -------------------------");
	WriteLog("Operation", "Finding \"GetSizeOfPool\" address...");

	// Get address by CRoadBlock::InitPool and not directly since it doesn't seem to work
	// Pattern is the same for Legacy and Enhanced
	ULONG_PTR address = FindPattern("BA 01 C7 2C F7 41 ?? 01 00 00 00 E8");
	//address = FindPatternGlobal("45 33 DB 44 8B D2 66 44 39 59 ?? 74 ?? 44 0F B7 49 ?? 33 D2 41 8B C2 41 F7 F1 48 8B 41 ?? 48 8B 0C D0 EB ?? 44 3B 11 74 ?? 48 8B 49");

	if (address)
	{
		address = ResolveCall(address + 11);
		WriteLog("Operation", "Found address of \"GetSizeOfPool\" at: 0x%llX", (unsigned long long)address);
		MH_STATUS st = MH_CreateHook(reinterpret_cast<LPVOID>(address),
			reinterpret_cast<LPVOID>(DetourGetSizeOfPool),
			reinterpret_cast<LPVOID*>(&TrampolineGetSizeOfPool));
		if (st == MH_OK)
			MH_EnableHook(reinterpret_cast<LPVOID>(address));
		else
			WriteLog("Error", "MH_CreateHook for \"GetSizeOfPool\" failed with error [%d]!", st);
	}
	else
		WriteLog("Error", "Could not find address of \"GetSizeOfPool\"!");

	hasExtendedGamePools = true;
	return;
}
#pragma endregion

#pragma region Memory Patching
constexpr const char* DefaultFindAddressErr = "Could not find address(es)!";
constexpr const char* DefaultDoneMsg = "Done!";
constexpr const char* DefaultPatchErr = "Failed to apply memory patch!";

// Credits Chiheb-Bacha: https://github.com/Chiheb-Bacha/StraightToStoryMode/blob/master/Game.cpp
void DisableIntroScreens()
{
	WriteLog("Info", "-------------------- Disable Intro Screens ---------------------");
	WriteLog("Operation", "Finding intro screens addresses...");

	if (GetIsEnhancedVersion())
	{
		// Splash Screen
		ULONG_PTR address = FindPattern("0F 85 A9 00 00 00 48 8D 54");
		if (address)
		{
			address -= 13;
			WriteLog("Operation", "Found address 1 at 0x%llX! Patching 6 bytes...", (unsigned long long)address);
			if (!SafePatch(address, { 0xE9, 0x86, 0x01, 0x00, 0x00, 0x90 }))
				WriteLog("Error", DefaultPatchErr);

			// Legal Warnings
			address = FindPattern("00 E9 ?? 05 00 00 E8 ?? 07");
			if (address)
			{
				address -= 15;
				constexpr int nBytes = 6;
				WriteLog("Operation", "Found address 2 at 0x%llX! Patching %d bytes...", (unsigned long long)address, nBytes);
				if (SafeMemset(address, 0x90, nBytes))
					WriteLog("Operation", DefaultDoneMsg);
				else
					WriteLog("Error", DefaultPatchErr);
			}
			else
				WriteLog("Error", DefaultFindAddressErr);
		}
		else
			WriteLog("Error", DefaultFindAddressErr);

		return;
	}

	// Splash Screen - CLoadingScreens::InitUpdateIntroMovie
	ULONG_PTR address = FindPattern("48 8D 55 ?? E8 ?? ?? ?? ?? 84 C0 74 ?? 48 8B 05");
	if (address)
	{
		address += 9;
		WriteLog("Operation", "Found address 1 at 0x%llX! Patching 3 bytes...", (unsigned long long)address);
		if (!SafePatch(address, { 0x90, 0x90, 0xEB }))
			WriteLog("Error", DefaultPatchErr);

		// Legal Warnings - CLoadingScreens::InitUpdateLegalMain
		address = FindPattern("83 EC ?? 84 C9 74 ?? 83 25 ?? ?? ?? ?? 00 E8");
		if (address)
		{
			constexpr int nBytes = 1;
			WriteLog("Operation", "Found address 2 at 0x%llX! Patching %d byte...", (unsigned long long)address, nBytes);
			bool ok = SafeMemset(address, 0xC3, 1);
			
			// CLoadingScreens::ms_LegalPage
			/* and dword ptr [rip + 0x????????], 0 --- 83 25 ?? ?? ?? ?? 00 */
			const ULONG_PTR ms_LegalPageAddr = ResolveRip(address + 7, 2, 7);
			ok = ok && SafeWrite<int32_t>(ms_LegalPageAddr, 0x02);

			if (ok)
				WriteLog("Operation", DefaultDoneMsg);
			else
				WriteLog("Error", DefaultPatchErr);
		}
		else
			WriteLog("Error", DefaultFindAddressErr);
	}
	else
		WriteLog("Error", DefaultFindAddressErr);

	return;
}

void DisableEnhancedLandingPage()
{
	WriteLog("Info", "--------------------- Disable Landing Page ---------------------");

	if (GetIsEnhancedVersion())
	{
		WriteLog("Operation", "Finding landing page address...");
		constexpr int nBytes = 3;
		ULONG_PTR address = FindPattern("E8 ?? ?? ?? ?? 8B 0D ?? ?? ?? ?? 83 C1 ?? 84 C0");
		if (address)
		{
			address = ResolveCall(address);
			WriteLog("Operation", "Found address at 0x%llX! Patching %d bytes...", (unsigned long long)address, nBytes);
			if (SafePatch(address, { 0x31, 0xC0, 0xC3 }))
				WriteLog("Operation", DefaultDoneMsg);
			else
				WriteLog("Error", DefaultPatchErr);
		}
		else
			WriteLog("Error", DefaultFindAddressErr);

		return;
	}

	WriteLog("Info", "Patch skipped: game is not enhanced version");
	return;
}

// Credits FiveM: https://github.com/citizenfx/fivem/blob/master/code/components/gta-streaming-five/src/UnkStuff.cpp
void LowPriorityPropsPatch()
{
	if (GetModuleHandleW(L"vfs.dll") != nullptr)
	{
		WriteLog("Info", "Lenny's Mod Loader detected, low priority props patch disabled to prevent crashes.");
		return;
	}

	WriteLog("Info", "---------------------- Low Priority Props ----------------------");

	if (!Ini::ExtendGamePools)
	{
		WriteLog("Warning", "LowPriorityPropsPatch is enabled while ExtendGamePools is disabled. An external pool extender mod is required or the game will crash due to pool limits!");
	}

	if (GetIsEnhancedVersion())
	{
		WriteLog("Operation", "Finding prop priority address...");

		ULONG_PTR address = FindPattern("C7 05 ?? ?? ?? ?? 02 00 00 00 B8 02 00 00 00 89 05");
		if (address)
		{
			WriteLog("Operation", "Found address at 0x%llX! Patching 2 bytes...", (unsigned long long)address);
			ScopedPageWrite guard(reinterpret_cast<void*>(address + 6), 6);
			if (guard.ok())
			{
				*reinterpret_cast<uint8_t*>(address + 6) = 0x03;
				*reinterpret_cast<uint8_t*>(address + 11) = 0x03;
				FlushInstructionCache(GetCurrentProcess(), reinterpret_cast<void*>(address + 6), 6);
				WriteLog("Operation", DefaultDoneMsg);
			}
			else
				WriteLog("Error", DefaultPatchErr);
		}
		else
			WriteLog("Error", DefaultFindAddressErr);

		// Patch "rage::fwMapDataContents::Entities_Create", should be updated to the same method as Legacy... (setting rage::fwMapData::ms_entityLevelCap)
		//C7 05 0C ?? ?? ?? ?? 00 00 00 B8 02 00 00 00 89 05
		/*
		const ULONG_PTR address = FindPattern("8B 47 5C 3B 05 ?? ?? ?? ?? 7E");
		if (address)
		{
			WriteLog("Operation", "Found address at 0x%X! Patching 6 byte...", address);
			constexpr int priOptionLowSize = 6;
			constexpr char priOptionLow[priOptionLowSize] = { '\x90', '\x90', '\x90', '\x83', '\xF8', '\x03' };
			memmove(reinterpret_cast<void*>(address + 3), &priOptionLow, priOptionLowSize);
			WriteLog("Operation", DefaultDoneMsg);
		}
		else
			WriteLog("Error", DefaultFindAddressErr);
		*/

		return;
	}

	WriteLog("Operation", "Finding prop priority addresses...");
	ULONG_PTR address = FindPattern("BB 02 00 00 00 39 1D");
	if (address)
	{
		WriteLog("Operation", "Found address 1 at 0x%llX! Patching 1 byte...", (unsigned long long)address);

		// rage::fwEntityDef.m_priorityLevel
		// Make GTA default rage::fwMapData::ms_entityLevelCap to PRI_OPTIONAL_LOW, not PRI_OPTIONAL_MEDIUM (RAGE suite defaults)
		if (!SafeWrite<uint8_t>(address + 1, 0x03))
			WriteLog("Error", DefaultPatchErr);

		address = FindPattern("0F 2F 47 24 0F 93 05");
		if (address)
		{
			constexpr int nBytes2 = 7;
			WriteLog("Operation", "Found address 2 at 0x%llX! Patching %d bytes...", (unsigned long long)address, nBytes2);
			// Don't disable low-priority objects when LOD distance is <20%
			// CInstancePriority::ms_bForceLowestPriority = this->m_settings.m_graphics.m_LodScale <= 0.2;
			if (SafeMemset(address + 4, 0x90, nBytes2))
				WriteLog("Operation", DefaultDoneMsg);
			else
				WriteLog("Error", DefaultPatchErr);
		}
		else
			WriteLog("Error", DefaultFindAddressErr);
	}
	else
		WriteLog("Error", DefaultFindAddressErr);

	return;
}

// Credits aint-no-other-option: https://github.com/aint-no-other-option/CenterSteeringPatch/
void CenterSteeringPatch()
{
	WriteLog("Info", "----------------------- Center Steering ------------------------");
	WriteLog("Operation", "Finding steering addresses...");

	if (GetIsEnhancedVersion())
	{
		constexpr int nBytes = 10; // same for both enhanced patterns

		// mov dword ptr [rsi+9DCh], 0  (exit-vehicle path)
		ULONG_PTR address = FindPattern("C7 86 DC 09 00 00 00 00 00 00 E9");
		if (address)
		{
			WriteLog("Operation", "Found address 1 at 0x%llX! Patching %d bytes...", (unsigned long long)address, nBytes);
			if (!SafeMemset(address, 0x90, nBytes))
				WriteLog("Error", DefaultPatchErr);

			// mov dword ptr [rsi+9DCh], 0  (dive-out path)
			address = FindPattern("C7 86 DC 09 00 00 00 00 00 00 31 C0");
			if (address)
			{
				WriteLog("Operation", "Found address 2 at 0x%llX! Patching %d bytes...", (unsigned long long)address, nBytes);
				if (SafeMemset(address, 0x90, nBytes))
					WriteLog("Operation", DefaultDoneMsg);
				else
					WriteLog("Error", DefaultPatchErr);
			}
			else
				WriteLog("Error", DefaultFindAddressErr);
		}
		else
			WriteLog("Error", DefaultFindAddressErr);

		return;
	}

	/* Address of centering when getting out of car normally */
	ULONG_PTR address = FindPattern("72 ?? 84 C0 75 ?? 44 89");
	if (address)
	{
		constexpr int nBytes1 = 7;
		WriteLog("Operation", "Found address 1 at 0x%llX! Patching %d bytes...", (unsigned long long)address, nBytes1);
		if (!SafeMemset(address + 6, 0x90, nBytes1))
			WriteLog("Error", DefaultPatchErr);

		/* Address of centering when diving out */
		address = FindPattern("89 82 ?? ?? ?? ?? 38 81");
		if (address)
		{
			constexpr int nBytes2 = 6;
			WriteLog("Operation", "Found address 2 at 0x%llX! Patching %d bytes...", (unsigned long long)address, nBytes2);
			if (SafeMemset(address, 0x90, nBytes2))
				WriteLog("Operation", DefaultDoneMsg);
			else
				WriteLog("Error", DefaultPatchErr);
		}
		else
			WriteLog("Error", DefaultFindAddressErr);
	}
	else
		WriteLog("Error", DefaultFindAddressErr);

	return;
}

// Credits aint-no-other-option: https://github.com/aint-no-other-option/CopBumpSteeringPatch
void CopBumpSteeringPatch()
{
	WriteLog("Info", "--------------------------- Cop Bump ---------------------------");
	WriteLog("Operation", "Finding cop bump addresses...");

	if (GetIsEnhancedVersion())
	{
		ULONG_PTR address = FindPattern("F3 41 0F 11 8D 38 1A 00 00 80");
		if (address)
		{
			constexpr int nBytes1 = 9;
			WriteLog("Operation", "Found address 1 at 0x%llX! Patching %d bytes...", (unsigned long long)address, nBytes1);
			if (!SafeMemset(address, 0x90, nBytes1))
				WriteLog("Error", DefaultPatchErr);

			address = FindPattern("F3 41 0F 11 B5 3C 1A 00 00 F3");
			if (address)
			{
				constexpr int nBytes2 = 9;
				WriteLog("Operation", "Found address 2 at 0x%llX! Patching %d bytes...", (unsigned long long)address, nBytes2);
				if (SafeMemset(address, 0x90, nBytes2))
					WriteLog("Operation", DefaultDoneMsg);
				else
					WriteLog("Error", DefaultPatchErr);
			}
			else
				WriteLog("Error", DefaultFindAddressErr);
		}
		else
			WriteLog("Error", DefaultFindAddressErr);

		return;
	}

	ULONG_PTR address = FindPattern("F3 0F 11 ?? ?? ?? 00 00 45 ?? ?? 74 ?? E8 ?? ?? ?? ?? 84");
	if (address)
	{
		constexpr int nBytes1 = 8;
		WriteLog("Operation", "Found address 1 at 0x%llX! Patching %d bytes...", (unsigned long long)address, nBytes1);
		if (!SafeMemset(address, 0x90, nBytes1))
			WriteLog("Error", DefaultPatchErr);

		address = FindPattern("EB 08 F3 0F 59 35 ?? ?? ?? 00 F3 0F 11");
		if (address)
		{
			constexpr int nBytes2 = 8;
			WriteLog("Operation", "Found address 2 at 0x%llX! Patching %d bytes...", (unsigned long long)address, nBytes2);
			if (SafeMemset(address + 10, 0x90, nBytes2))
				WriteLog("Operation", DefaultDoneMsg);
			else
				WriteLog("Error", DefaultPatchErr);
		}
		else
			WriteLog("Error", DefaultFindAddressErr);
	}
	else
		WriteLog("Error", DefaultFindAddressErr);

	/*
	constexpr int nBytes = 8;
	const ULONG_PTR address = FindPattern("33 F6 F3 0F 11 87 ?? ?? ?? ?? 45 84 ED 74 25");
	if (address)
	{
		WriteLog("Operation", "Finding cop bump address...");
		WriteLog("Operation", "Found address at 0x%X! Patching %d bytes...", address, nBytes);
		memset(reinterpret_cast<void*>(address + 2), 0x90, nBytes);
		WriteLog("Operation", DefaultDoneMsg);
	}
	else
		WriteLog("Error", DefaultFindAddressErr);
	*/

	return;
}

// Credits CamxxCore: https://github.com/CamxxCore/GTAVWeaponWheelMod
void HUDWheelSlowdownPatch()
{
	WriteLog("Info", "---------------------- HUD Wheel Slowdown ----------------------");
	WriteLog("Operation", "Finding HUD wheel slowdown addresses...");

	if (GetIsEnhancedVersion())
	{
		ULONG_PTR address = FindPattern("80 ?? 64 01 74");
		if (address)
		{
			WriteLog("Operation", "Found address 1 at 0x%llX!", (unsigned long long)address);

			bool ok = true;

			// Remove vignetting: patch CSelectionWheel::TriggerFadeOutEffect to ret immediately
			const ULONG_PTR TriggerFadeOutEffectAdr = ResolveCall(address + 33);
			ok = ok && SafeWrite<uint8_t>(TriggerFadeOutEffectAdr, 0xC3); // ret

			// Remove vignetting: patch CSelectionWheel::TriggerFadeInEffect to ret immediately
			const ULONG_PTR TriggerFadeInEffectAdr = ResolveCall(address + 13);
			ok = ok && SafeWrite<uint8_t>(TriggerFadeInEffectAdr, 0xC3); // ret

			// Timescale override: replace CTimeWarper::SetTargetTimeWarp call with
			// "xor edi, edi" (zero out the timescale argument) + NOP sled
			ok = ok && SafeMemset(address - 11, 0x90, 11);
			ok = ok && SafePatch(address - 11, { 0x31, 0xFF }); // xor edi, edi

			/* Weapon Wheel Audio SlowMo Address (audNorthAudioEngine::ActivateSlowMoMode & audFrontendAudioEntity::StartWeaponWheel) */
			address = FindPattern("82 90 D2 F1");  // Joaat("SLOWMO_WEAPON")
			if (address)
			{
				constexpr int nBytes2 = 10;
				WriteLog("Operation", "Found address 2 at 0x%llX! Patching %d bytes...", (unsigned long long)address, nBytes2 + 1);
				ok = ok && SafeMemset(address - 1, 0x90, nBytes2);
				ok = ok && SafeWrite<uint8_t>(address - 11, 0x00);
			}
			else
			{
				WriteLog("Error", DefaultFindAddressErr);
				ok = false;
			}

			/*
			address = FindPattern("84 DB 74 ?? E8 ?? ?? ?? ?? 80 3D");
			if (address)
			{
				constexpr int nBytes2 = 5;
				WriteLog("Operation", "Found address 3 at 0x%X! Patching %d bytes...", address, nBytes2);
				memset(reinterpret_cast<void*>(address + 4), 0x90, nBytes2);
			}
			else
				WriteLog("Error", DefaultFindAddressErr);
			*/

			if (ok)
				WriteLog("Operation", DefaultDoneMsg);
			else
				WriteLog("Error", DefaultPatchErr);
		}
		else
			WriteLog("Error", DefaultFindAddressErr);

		return;
	}

	ULONG_PTR address = FindPattern("38 51 64 74 19");
	if (address)
	{
		WriteLog("Operation", "Found address 1 at 0x%llX!", (unsigned long long)address);

		bool ok = true;

		// Remove vignetting: CSelectionWheel::TriggerFadeOutEffect → ret
		const ULONG_PTR TriggerFadeOutEffectAdr = ResolveCall(address + 25);
		ok = ok && SafeWrite<uint8_t>(TriggerFadeOutEffectAdr, 0xC3);

		// Remove vignetting: CSelectionWheel::TriggerFadeInEffect → ret
		const ULONG_PTR TriggerFadeInEffectAdr = ResolveCall(address + 8);
		ok = ok && SafeWrite<uint8_t>(TriggerFadeInEffectAdr, 0xC3);

		// Timescale override: "xor edx, edx" (zero timescale arg, Legacy uses rdx)
		ok = ok && SafePatch(address + 34, { 0x31, 0xD2 });

		// Weapon Wheel Audio SlowMo — Joaat("SLOWMO_WEAPON") = 0xF1D29082
		address = FindPattern("82 90 D2 F1");
		if (address)
		{
			constexpr int nBytes2 = 10;
			WriteLog("Operation", "Found address 2 at 0x%llX! Patching %d bytes...", (unsigned long long)address, nBytes2 + 1);
			ok = ok && SafeMemset(address - 1, 0x90, nBytes2);
			ok = ok && SafeWrite<uint8_t>(address - 4, 0x00);
		}
		else
		{
			WriteLog("Error", DefaultFindAddressErr);
			ok = false;
		}

		if (ok)
			WriteLog("Operation", DefaultDoneMsg);
		else
			WriteLog("Error", DefaultPatchErr);
	}
	else
		WriteLog("Error", DefaultFindAddressErr);

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
		return;
	}

	if (Ini::DisableIntroScreens) { DisableIntroScreens(); }
	if (Ini::DisableEnhancedLandingPage) { DisableEnhancedLandingPage(); }
	if (Ini::LowPriorityPropsPatch) { LowPriorityPropsPatch(); }
	if (Ini::CenterSteeringPatch) { CenterSteeringPatch(); }
	if (Ini::CopBumpSteeringPatch) { CopBumpSteeringPatch(); }
	if (Ini::HUDWheelSlowdownPatch) { HUDWheelSlowdownPatch(); }

	hasAppliedExePatches = true;
	return;
}
#pragma endregion