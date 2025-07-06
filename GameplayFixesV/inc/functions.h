#pragma once
#include <Windows.h>
#include <natives.h>
#include <string>
#include <filesystem>
#include <script.h>
#include "nm.h"
#include <globals.h>

class Timer {
	int gameTimer = 0;
public:
	void Set(int value) { gameTimer = GET_GAME_TIMER() - value; return; };
	void Reset() { gameTimer = GET_GAME_TIMER(); return; };
	int Get() const { return (GET_GAME_TIMER() - gameTimer); }
	Timer(const int startVal = 0) { gameTimer = (startVal * -1); }
};

inline Player GetPlayer() { return PLAYER_ID(); }
inline Ped GetPlayerPed() { return PLAYER_PED_ID(); }
inline Vector3 GetPlayerCoords() { return GET_ENTITY_COORDS(PLAYER_PED_ID(), false); }

#pragma region Generic
inline int Abs(const int n) { return n * ((n > 0) - (n < 0)); }
inline float Abs(const float n) { return n * ((n > 0.0f) - (n < 0.0f)); }

template <typename T>
inline bool ArrayContains(const T value, const T a[], const T n)
{
	unsigned int i = 0;
	while (i < n && a[i] != value) ++i;
	return i == n ? false : true;
}

template <typename T>
inline bool Between(const T val, const T min, const T max)
{
	if (val >= min && val <= max)
		return true;

	return false;
}

template <typename T>
inline bool BetweenExclude(const T val, const T min, const T max)
{
	if (val > min && val < max)
		return true;

	return false;
}

std::filesystem::path AbsoluteModulePath(HINSTANCE module);
ULONG_PTR FindPattern(std::string signature);
ULONG_PTR FindPatternGlobal(std::string signature);
void SplitString(const char* charStr, std::string arr[], const int arrSize, const bool toUpper = false);
int GetRandomIntInRange(int startRange = 0, int endRange = 65535);
bool GetWeightedBool(int chance);

static const std::unordered_map<std::string, int> mapPadControls = {
	{"PAD_UP", INPUT_FRONTEND_UP},
	{"PAD_DOWN", INPUT_FRONTEND_DOWN},
	{"PAD_RIGHT", INPUT_FRONTEND_RIGHT},
	{"PAD_LEFT", INPUT_FRONTEND_LEFT},
	{"B_UP", INPUT_FRONTEND_Y},			// Y	
	{"B_DOWN", INPUT_FRONTEND_ACCEPT},	// A	
	{"B_RIGHT", INPUT_FRONTEND_CANCEL},	// B
	{"B_LEFT", INPUT_FRONTEND_X},		// X
	{"RT", INPUT_FRONTEND_RT},
	{"LT", INPUT_FRONTEND_LT},
	{"RB", INPUT_FRONTEND_RB},
	{"LB", INPUT_FRONTEND_LB},
	{"RSB", INPUT_FRONTEND_RS},
	{"LSB", INPUT_FRONTEND_LS},
	{"START", INPUT_FRONTEND_PAUSE},
	{"BACK", INPUT_FRONTEND_SELECT}
};
int GetPadControlFromString(const std::string& str);

static const std::unordered_map<std::string, int> mapVKs = {
	{"LBUTTON", 0x01}, {"RBUTTON", 0x02}, {"CANCEL", 0x03}, {"MBUTTON", 0x04}, {"XBUTTON1", 0x05},
	{"XBUTTON2", 0x06}, {"BACK", 0x08}, {"TAB", 0x09}, {"CLEAR", 0x0C}, {"RETURN", 0x0D}, {"SHIFT", 0x10},
	{"CONTROL", 0x11}, {"MENU", 0x12}, {"PAUSE", 0x13}, {"CAPITAL", 0x14}, {"HANGUL", 0x15}, {"JUNJA", 0x17},
	{"FINAL", 0x18}, {"HANJA", 0x19}, {"ESCAPE", 0x1B}, {"CONVERT", 0x1C}, {"NONCONVERT", 0x1D}, {"ACCEPT", 0x1E},
	{"MODECHANGE", 0x1F}, {"SPACE", 0x20}, {"PRIOR", 0x21}, {"NEXT", 0x22}, {"END", 0x23}, {"HOME", 0x24},
	{"LEFT", 0x25}, {"UP", 0x26}, {"RIGHT", 0x27}, {"DOWN", 0x28}, {"SELECT", 0x29}, {"PRINT", 0x2A},
	{"EXECUTE", 0x2B}, {"SNAPSHOT", 0x2C}, {"INSERT", 0x2D}, {"DELETE", 0x2E}, {"HELP", 0x2F}, {"0", 0x30},
	{"1", 0x31}, {"2", 0x32}, {"3", 0x33}, {"4", 0x34}, {"5", 0x35}, {"6", 0x36},
	{"7", 0x37}, {"8", 0x38}, {"9", 0x39}, {"A", 0x41}, {"B", 0x42}, {"C", 0x43},
	{"D", 0x44}, {"E", 0x45}, {"F", 0x46}, {"G", 0x47}, {"H", 0x48}, {"I", 0x49},
	{"J", 0x4A}, {"K", 0x4B}, {"L", 0x4C}, {"M", 0x4D}, {"N", 0x4E}, {"O", 0x4F},
	{"P", 0x50}, {"Q", 0x51}, {"R", 0x52}, {"S", 0x53}, {"T", 0x54}, {"U", 0x55},
	{"V", 0x56}, {"W", 0x57}, {"X", 0x58}, {"Y", 0x59}, {"Z", 0x5A}, {"LWIN", 0x5B},
	{"RWIN", 0x5C}, {"APPS", 0x5D}, {"SLEEP", 0x5F}, {"NUMPAD0", 0x60}, {"NUMPAD1", 0x61}, {"NUMPAD2", 0x62},
	{"NUMPAD3", 0x63}, {"NUMPAD4", 0x64}, {"NUMPAD5", 0x65}, {"NUMPAD6", 0x66}, {"NUMPAD7", 0x67}, {"NUMPAD8", 0x68},
	{"NUMPAD9", 0x69}, {"MULTIPLY", 0x6A}, {"ADD", 0x6B}, {"SEPARATOR", 0x6C}, {"SUBTRACT", 0x6D}, {"DECIMAL", 0x6E},
	{"DIVIDE", 0x6F}, {"F1", 0x70}, {"F2", 0x71}, {"F3", 0x72}, {"F4", 0x73}, {"F5", 0x74},
	{"F6", 0x75}, {"F7", 0x76}, {"F8", 0x77}, {"F9", 0x78}, {"F10", 0x79}, {"F11", 0x7A},
	{"F12", 0x7B}, {"F13", 0x7C}, {"F14", 0x7D}, {"F15", 0x7E}, {"F16", 0x7F}, {"F17", 0x80},
	{"F18", 0x81}, {"F19", 0x82}, {"F20", 0x83}, {"F21", 0x84}, {"F22", 0x85}, {"F23", 0x86},
	{"F24", 0x87}, {"NAVIGATIONVIEW", 0x88}, {"NAVIGATIONMENU", 0x89}, {"NAVIGATIONUP", 0x8A}, {"NAVIGATIONDOWN", 0x8B},
	{"NAVIGATIONLEFT", 0x8C}, {"NAVIGATIONRIGHT", 0x8D}, {"NAVIGATIONACCEPT", 0x8E}, {"NAVIGATIONCANCEL", 0x8F},
	{"NUMLOCK", 0x90}, {"SCROLL", 0x91}, {"LSHIFT", 0xA0}, {"RSHIFT", 0xA1}, {"LCONTROL", 0xA2}, {"RCONTROL", 0xA3},
	{"LMENU", 0xA4}, {"RMENU", 0xA5}, {"BROWSER_BACK", 0xA6}, {"BROWSER_FORWARD", 0xA7}, {"BROWSER_REFRESH", 0xA8},
	{"BROWSER_STOP", 0xA9}, {"BROWSER_SEARCH", 0xAA}, {"BROWSER_FAVORITES", 0xAB},
	{"BROWSER_HOME", 0xAC}, {"VOLUME_MUTE", 0xAD}, {"VOLUME_DOWN", 0xAE}, {"VOLUME_UP", 0xAF},
	{"MEDIA_NEXT_TRACK", 0xB0}, {"MEDIA_PREV_TRACK", 0xB1}, {"MEDIA_STOP", 0xB2}, {"MEDIA_PLAY_PAUSE", 0xB3},
	{"LAUNCH_MAIL", 0xB4}, {"LAUNCH_MEDIA_SELECT", 0xB5}, {"LAUNCH_APP1", 0xB6}, {"LAUNCH_APP2", 0xB7},
	{"OEM_1", 0xBA}, {"OEM_PLUS", 0xBB}, {"OEM_COMMA", 0xBC}, {"OEM_MINUS", 0xBD}, {"OEM_PERIOD", 0xBE},
	{"OEM_2", 0xBF}, {"OEM_3", 0xC0}, {"GAMEPAD_A", 0xC3}, {"GAMEPAD_B", 0xC4}, {"GAMEPAD_X", 0xC5}, {"GAMEPAD_Y", 0xC6},
	{"GAMEPADRIGHTBUMPER", 0xC7}, {"GAMEPADLEFTBUMPER", 0xC8}, {"GAMEPADLEFTTRIGGER", 0xC9}, {"GAMEPADRIGHTTRIGGER", 0xCA},
	{"GAMEPADDPADUP", 0xCB}, {"GAMEPADDPADDOWN", 0xCC}, {"GAMEPADDPADLEFT", 0xCD}, {"GAMEPADDPADRIGHT", 0xCE},
	{"GAMEPADMENU", 0xCF}, {"GAMEPADVIEW", 0xD0}, {"GAMEPADLEFTSTICKBTN", 0xD1}, {"GAMEPADRIGHTSTICKBTN", 0xD2},
	{"GAMEPADLEFTSTICKUP", 0xD3}, {"GAMEPADLEFTSTICKDOWN", 0xD4}, {"GAMEPADLEFTSTICKRIGHT", 0xD5}, {"GAMEPADLEFTSTICKLEFT", 0xD6},
	{"GAMEPADRIGHTSTICKUP", 0xD7}, {"GAMEPADRIGHTSTICKDOWN", 0xD8}, {"GAMEPADRIGHTSTICKRIGHT", 0xD9}, {"GAMEPADRIGHTSTICKLEFT", 0xDA},
	{"OEM_4", 0xDB}, {"OEM_5", 0xDC}, {"OEM_6", 0xDD}, {"OEM_7", 0xDE}, {"OEM_8", 0xDF},	 {"OEM_102", 0xE2},
	{"PROCESSKEY", 0xE5}, {"PACKET", 0xE7}, {"ATTN", 0xF6}, {"CRSEL", 0xF7}, {"EXSEL", 0xF8}, {"EREOF", 0xF9},
	{"PLAY", 0xFA}, {"ZOOM", 0xFB}, {"NONAME", 0xFC}, {"PA1", 0xFD}, {"OEM_CLEAR", 0xFE}
};
int GetVKFromString(const std::string& str);
#pragma endregion

#pragma region Log
void ClearLog();
void RawLog(const std::string& szInfo, const std::string& szData);
void WriteLog(const char* szInfo, const char* szFormat, ...);
#pragma endregion

#pragma region JSON
std::string LoadJSONResource(HINSTANCE hInstance, int resourceID);
bool LoadWeaponJson();
#pragma endregion

#pragma region Print
void Print(char* string, int ms = 0);
void Print(std::string string, int ms = 0);
void PrintInt(int value, int ms = 0);
void PrintFloat(float value, int ms = 0);
void PrintHelp(char* string, bool playSound = false, int overrideDuration = -1);
int ShowNotification(const char* str, bool flash = false);
#pragma endregion

#pragma region Assets Request
bool RequestModel(Hash model);
bool RequestAnimDict(char* animDict);
bool RequestClipSet(char* animDict);
//bool RequestScaleform(const char* name, int* handle);
#pragma endregion

#pragma region Ped Flags
void EnablePedConfigFlag(Ped ped, int flag);
void DisablePedConfigFlag(Ped ped, int flag);
void EnablePedResetFlag(Ped ped, int flag);
void DisablePedResetFlag(Ped ped, int flag);
#pragma endregion

#pragma region Weapons
bool HasEntityBeenDamagedByWeaponThisFrame(Ped ped, Hash weaponHash, int weaponType = 0);
//bool HasEntityBeenDamagedByAnyPedThisFrame(Ped ped);
bool CanDisarmPed(Ped ped, bool includeLeftHand);
int GetWeaponBlipSprite(const Hash weaponHash);
bool WillWeaponSpawnPickupWhenDropped(const Hash weaponHash, const bool checkWeaponType);
void DropPlayerWeapon(Hash weaponHash, const bool shouldCurse);
void RestorePlayerRetrievedWeapon();
#pragma endregion

#pragma region Vehicle
inline Vehicle GetVehiclePedIsUsing(const Ped ped) { return GET_VEHICLE_PED_IS_USING(ped); }
Vehicle GetVehiclePedIsIn(const Ped ped, const bool includeEntering = true, const bool includeExiting = false);
Vehicle GetVehiclePedIsEntering(const Ped ped);
Vehicle GetVehiclePedIsExiting(const Ped ped);
Vehicle GetVehiclePedIsEnteringOrExiting(const Ped ped);
bool DoesVehicleHaveAbility(const Vehicle veh);
#pragma endregion

#pragma region HUD
int RequestMinimapScaleform();
void SetTextStyle(TextStyle Style = defaultTextStyle, bool bDrawBeforeFade = false);
int GetHudComponentFromString(const char* str);
void SetHealthHudDisplayValues(int healthPercentage, int armourPercentage, bool showDamage = true);
#pragma endregion

#pragma region Misc
bool IsPlayerAiming();
bool IsPlayerInsideSafehouse();
void SetDispatchServices(bool toggle);
bool GetFakeWanted();
void SetFakeWanted(Player player, bool toggle);
inline int GetNumberOfScriptInstances(const char* name) { return (GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH(Joaat(name))); }
#pragma endregion

#pragma region Game Functions
namespace nGame
{
ULONG_PTR GetScriptEntity(Entity entity);
int GetFragInstNmGtaOffset();
ULONG_PTR CreateNmMessage();
void GivePedNMMessage(ULONG_PTR msgMemPtr, const Ped ped, const char* message);
void SetNMMessageParam(ULONG_PTR msgMemPtr, const char* msgParam, int i);
void SetNMMessageParam(ULONG_PTR msgMemPtr, const char* msgParam, bool b);
void SetNMMessageParam(ULONG_PTR msgMemPtr, const char* msgParam, float f);
void SetNMMessageParam(ULONG_PTR msgMemPtr, const char* msgParam, const char* str);
void SetNMMessageParam(ULONG_PTR msgMemPtr, const char* msgParam, float x, float y, float z);
inline void GivePedNMMessage(ULONG_PTR msgMemPtr, const Ped ped, eNMStr message) { GivePedNMMessage(msgMemPtr, ped, NMStrings[message]); return; }
inline void SetNMMessageParam(ULONG_PTR msgMemPtr, eNMStr msgParam, int i) { SetNMMessageParam(msgMemPtr, NMStrings[msgParam], i); return; }
inline void SetNMMessageParam(ULONG_PTR msgMemPtr, eNMStr msgParam, bool b) { SetNMMessageParam(msgMemPtr, NMStrings[msgParam], b); return; }
inline void SetNMMessageParam(ULONG_PTR msgMemPtr, eNMStr msgParam, float f) { SetNMMessageParam(msgMemPtr, NMStrings[msgParam], f); return; }
inline void SetNMMessageParam(ULONG_PTR msgMemPtr, eNMStr msgParam, const char* str) { SetNMMessageParam(msgMemPtr, NMStrings[msgParam], str); return; }
inline void SetNMMessageParam(ULONG_PTR msgMemPtr, eNMStr msgParam, float x, float y, float z) { SetNMMessageParam(msgMemPtr, NMStrings[msgParam], x, y, z); return; }
/*
void SetNMMessageInt(ULONG_PTR msgMemPtr, const char* msgParam, int i);
void SetNMMessageBool(ULONG_PTR msgMemPtr, const char* msgParam, bool b);
void SetNMMessageFloat(ULONG_PTR msgMemPtr, const char* msgParam, float f);
void SetNMMessageString(ULONG_PTR msgMemPtr, const char* msgParam, const char* str);
void SetNMMessageVec3(ULONG_PTR msgMemPtr, const char* msgParam, float x, float y, float z);
inline void GivePedNMMessage(ULONG_PTR msgMemPtr, const Ped ped, eNMStr message) { GivePedNMMessage(msgMemPtr, ped, NMStrings[message]); return; }
inline void SetNMMessageInt(ULONG_PTR msgMemPtr, eNMStr msgParam, int i) { SetNMMessageInt(msgMemPtr, NMStrings[msgParam], i); return; }
inline void SetNMMessageBool(ULONG_PTR msgMemPtr, eNMStr msgParam, bool b) { SetNMMessageBool(msgMemPtr, NMStrings[msgParam], b); return; }
inline void SetNMMessageFloat(ULONG_PTR msgMemPtr, eNMStr msgParam, float f) { SetNMMessageFloat(msgMemPtr, NMStrings[msgParam], f); return; }
inline void SetNMMessageString(ULONG_PTR msgMemPtr, eNMStr msgParam, const char* str) { SetNMMessageString(msgMemPtr, NMStrings[msgParam], str); return; }
inline void SetNMMessageVec3(ULONG_PTR msgMemPtr, eNMStr msgParam, float x, float y, float z) { SetNMMessageVec3(msgMemPtr, NMStrings[msgParam], x, y, z); return; }
*/
}
#pragma endregion

void GetGameFunctionsAddresses();
void ApplyExePatches();
void UpdatePlayerVars();