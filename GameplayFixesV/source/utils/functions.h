#pragma once
#include <natives.h>
#include <string>
#include <unordered_map>
#include "..\globals.h"

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

void EnablePedConfigFlag(Ped ped, int flag);
void DisablePedConfigFlag(Ped ped, int flag);
void EnablePedResetFlag(Ped ped, int flag);
void DisablePedResetFlag(Ped ped, int flag);

void Print(char* string, int ms = 1);
void PrintInt(int value, int ms = 1);
void PrintFloat(float value, int ms = 1);
void PrintHelp(char* string, bool playSound = false, int overrideDuration = -1);
int ShowNotification(const char* str, bool flash = false);

void from_json(const nlohmann::json& j, WpComponentJson& c);
void from_json(const nlohmann::json& j, WeaponJson& w);
std::string LoadJSONResource(HINSTANCE hInstance, int resourceID);
bool LoadWeaponJson();
Hash DropPlayerWeapon(const bool checkWeaponType, const bool shouldCurse);
void RestorePlayerRetrievedWeapon();

bool ArrayContains(int value, const int a[], int n);
void SplitString(const char* charStr, std::string arr[], const int arrSize, const bool toUpper = false);
int GetRandomIntInRange(int startRange = 0, int endRange = 65535);
bool GetWeightedBool(int chance);
Vehicle GetVehiclePedIsIn(Ped ped, bool includeEntering = true, bool includeExiting = false);
Vehicle GetVehiclePedIsEntering(Ped ped);
Vehicle GetVehiclePedIsExiting(Ped ped);
Vehicle GetVehiclePedIsEnteringOrExiting(Ped ped);
bool HasEntityBeenDamagedByWeaponThisFrame(Ped ped, Hash weaponHash, int weaponType = 0);
bool CanDisarmPed(Ped ped, bool includeLeftHand);
void SetTextStyle(TextStyle Style = defaultTextStyle, bool bDrawBeforeFade = false);

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
int GetPadControlFromString(const std::string &str);

static const std::unordered_map<std::string, int> mapVKs = {
	{"LBUTTON", 0X01}, {"RBUTTON", 0X02}, {"CANCEL", 0X03}, {"MBUTTON", 0X04}, {"XBUTTON1", 0X05},
	{"XBUTTON2", 0X06}, {"BACK", 0X08}, {"TAB", 0X09}, {"CLEAR", 0X0C}, {"RETURN", 0X0D}, {"SHIFT", 0X10},
	{"CONTROL", 0X11}, {"MENU", 0X12}, {"PAUSE", 0X13}, {"CAPITAL", 0X14}, {"HANGUL", 0X15}, {"JUNJA", 0X17},
	{"FINAL", 0X18}, {"HANJA", 0X19}, {"ESCAPE", 0X1B}, {"CONVERT", 0X1C}, {"NONCONVERT", 0X1D}, {"ACCEPT", 0X1E},
	{"MODECHANGE", 0X1F}, {"SPACE", 0X20}, {"PRIOR", 0X21}, {"NEXT", 0X22}, {"END", 0X23}, {"HOME", 0X24},
	{"LEFT", 0X25}, {"UP", 0X26}, {"RIGHT", 0X27}, {"DOWN", 0X28}, {"SELECT", 0X29}, {"PRINT", 0X2A},
	{"EXECUTE", 0X2B}, {"SNAPSHOT", 0X2C}, {"INSERT", 0X2D}, {"DELETE", 0X2E}, {"HELP", 0X2F}, {"0", 0X30},
	{"1", 0X31}, {"2", 0X32}, {"3", 0X33}, {"4", 0X34}, {"5", 0X35}, {"6", 0X36},
	{"7", 0X37}, {"8", 0X38}, {"9", 0X39}, {"A", 0X41}, {"B", 0X42}, {"C", 0X43},
	{"D", 0X44}, {"E", 0X45}, {"F", 0X46}, {"G", 0X47}, {"H", 0X48}, {"I", 0X49},
	{"J", 0X4A}, {"K", 0X4B}, {"L", 0X4C}, {"M", 0X4D}, {"N", 0X4E}, {"O", 0X4F},
	{"P", 0X50}, {"Q", 0X51}, {"R", 0X52}, {"S", 0X53}, {"T", 0X54}, {"U", 0X55},
	{"V", 0X56}, {"W", 0X57}, {"X", 0X58}, {"Y", 0X59}, {"Z", 0X5A}, {"LWIN", 0X5B},
	{"RWIN", 0X5C}, {"APPS", 0X5D}, {"SLEEP", 0X5F}, {"NUMPAD0", 0X60}, {"NUMPAD1", 0X61}, {"NUMPAD2", 0X62},
	{"NUMPAD3", 0X63}, {"NUMPAD4", 0X64}, {"NUMPAD5", 0X65}, {"NUMPAD6", 0X66}, {"NUMPAD7", 0X67}, {"NUMPAD8", 0X68},
	{"NUMPAD9", 0X69}, {"MULTIPLY", 0X6A}, {"ADD", 0X6B}, {"SEPARATOR", 0X6C}, {"SUBTRACT", 0X6D}, {"DECIMAL", 0X6E},
	{"DIVIDE", 0X6F}, {"F1", 0X70}, {"F2", 0X71}, {"F3", 0X72}, {"F4", 0X73}, {"F5", 0X74},
	{"F6", 0X75}, {"F7", 0X76}, {"F8", 0X77}, {"F9", 0X78}, {"F10", 0X79}, {"F11", 0X7A},
	{"F12", 0X7B}, {"F13", 0X7C}, {"F14", 0X7D}, {"F15", 0X7E}, {"F16", 0X7F}, {"F17", 0X80},
	{"F18", 0X81}, {"F19", 0X82}, {"F20", 0X83}, {"F21", 0X84}, {"F22", 0X85}, {"F23", 0X86},
	{"F24", 0X87}, {"NAVIGATIONVIEW", 0X88}, {"NAVIGATIONMENU", 0X89}, {"NAVIGATIONUP", 0X8A}, {"NAVIGATIONDOWN", 0X8B},
	{"NAVIGATIONLEFT", 0X8C}, {"NAVIGATIONRIGHT", 0X8D}, {"NAVIGATIONACCEPT", 0X8E}, {"NAVIGATIONCANCEL", 0X8F},
	{"NUMLOCK", 0X90}, {"SCROLL", 0X91}, {"LSHIFT", 0XA0}, {"RSHIFT", 0XA1}, {"LCONTROL", 0XA2}, {"RCONTROL", 0XA3},
	{"LMENU", 0XA4}, {"RMENU", 0XA5}, {"BROWSER_BACK", 0XA6}, {"BROWSER_FORWARD", 0XA7}, {"BROWSER_REFRESH", 0XA8},
	{"BROWSER_STOP", 0XA9}, {"BROWSER_SEARCH", 0XAA}, {"BROWSER_FAVORITES", 0XAB},
	{"BROWSER_HOME", 0XAC}, {"VOLUME_MUTE", 0XAD}, {"VOLUME_DOWN", 0XAE}, {"VOLUME_UP", 0XAF},
	{"MEDIA_NEXT_TRACK", 0XB0}, {"MEDIA_PREV_TRACK", 0XB1}, {"MEDIA_STOP", 0XB2}, {"MEDIA_PLAY_PAUSE", 0XB3},
	{"LAUNCH_MAIL", 0XB4}, {"LAUNCH_MEDIA_SELECT", 0XB5}, {"LAUNCH_APP1", 0XB6}, {"LAUNCH_APP2", 0XB7},
	{"OEM_1", 0XBA}, {"OEM_PLUS", 0XBB}, {"OEM_COMMA", 0XBC}, {"OEM_MINUS", 0XBD}, {"OEM_PERIOD", 0XBE},
	{"OEM_2", 0XBF}, {"OEM_3", 0XC0}, {"GAMEPAD_A", 0XC3}, {"GAMEPAD_B", 0XC4}, {"GAMEPAD_X", 0XC5}, {"GAMEPAD_Y", 0XC6},
	{"GAMEPADRIGHTBUMPER", 0XC7}, {"GAMEPADLEFTBUMPER", 0XC8}, {"GAMEPADLEFTTRIGGER", 0XC9}, {"GAMEPADRIGHTTRIGGER", 0XCA},
	{"GAMEPADDPADUP", 0XCB}, {"GAMEPADDPADDOWN", 0XCC}, {"GAMEPADDPADLEFT", 0XCD}, {"GAMEPADDPADRIGHT", 0XCE},
	{"GAMEPADMENU", 0XCF}, {"GAMEPADVIEW", 0XD0}, {"GAMEPADLEFTSTICKBTN", 0XD1}, {"GAMEPADRIGHTSTICKBTN", 0XD2},
	{"GAMEPADLEFTSTICKUP", 0XD3}, {"GAMEPADLEFTSTICKDOWN", 0XD4}, {"GAMEPADLEFTSTICKRIGHT", 0XD5}, {"GAMEPADLEFTSTICKLEFT", 0XD6},
	{"GAMEPADRIGHTSTICKUP", 0XD7}, {"GAMEPADRIGHTSTICKDOWN", 0XD8}, {"GAMEPADRIGHTSTICKRIGHT", 0XD9}, {"GAMEPADRIGHTSTICKLEFT", 0XDA},
	{"OEM_4", 0XDB}, {"OEM_5", 0XDC}, {"OEM_6", 0XDD}, {"OEM_7", 0XDE}, {"OEM_8", 0XDF},	 {"OEM_102", 0XE2},
	{"PROCESSKEY", 0XE5}, {"PACKET", 0XE7}, {"ATTN", 0XF6}, {"CRSEL", 0XF7}, {"EXSEL", 0XF8}, {"EREOF", 0XF9},
	{"PLAY", 0XFA}, {"ZOOM", 0XFB}, {"NONAME", 0XFC}, {"PA1", 0XFD}, {"OEM_CLEAR", 0XFE}
};
int GetVKFromString(const std::string &str);

void RefreshIni();
void UpdatePlayerOptions();