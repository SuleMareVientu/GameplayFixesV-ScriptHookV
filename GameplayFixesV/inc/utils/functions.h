#pragma once
#include <shv\natives.h>
#include <globals.h>
#include <script.h>
#include "utils\nm.h"

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
void SplitString(const char* charStr, std::string arr[], const int arrSize, const bool toUpper = false);
int GetRandomIntInRange(int minValue = 0, int maxValue = 65535, bool useRd = false); // 0-65535 is the max range for natives, useRd has no such limit
bool GetWeightedBool(int chance, bool useRd = false);
Vector3 Normalize(Vector3 v);

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
void ClearEntityLastDamageEntity(Entity entity);
void ClearPedLastDamageBone(Ped ped);
void ClearEntityLastWeaponDamage(Entity entity);
void ClearLastDamages();

bool HasEntityBeenDamagedByWeaponThisFrame(Ped ped, Hash weaponHash, int weaponType = 0);
//bool HasEntityBeenDamagedByAnyPedThisFrame(Ped ped);
bool CanDisarmPed(Ped ped, bool includeLeftHand);
int GetWeaponBlipSprite(const Hash weaponHash);
bool ShouldWeaponSpawnPickupWhenDropped(const Hash weaponHash, const bool checkWeaponType);
void DropPlayerWeapon(Hash weaponHash, const bool shouldCurse, Vector3 wpRot = { 0.0f, 0.0f, 0.0f });
void RestorePlayerRetrievedWeapon(bool autoEquip);

void TaskNMShot(Ped ped, Hash wpHash, int partIndex, Vector3 hitLoc, Vector3 impulseNorm, bool isAiming = false, bool isCrouched = false);
void TaskNMElectrocute(Ped ped);

// Weapon Model Hash -> Pickup Hash
static const std::unordered_map<unsigned int, unsigned int> wpPickupMap = {
	{0xA0BD351E, 0x6E4E65C2},	{0xE3C5D4DF, 0x741C684A},	{0xF55C8CD1, 0x6C5B941A},
	{0x1053C3FD, 0xF33C83B0},	{0x3D2E1AE8, 0xDF711959},	{0x9A385232, 0xB2B5325E},
	{0x856E5E8E, 0x85CAA9B1},	{0xD3EDBC71, 0xB2930A14},	{0x14A5B1EB, 0xFE2A352C},
	{0xD37A33C0, 0x693583AD},	{0xC103D44A, 0x1D9588D3},	{0xE231B874, 0x3A4C2AD2},
	{0xF2F47DA7, 0x4D36C349},	{0x19314199, 0x2F36B434},	{0x291CEA47, 0xA9355DCD},
	{0xD7B77A96, 0x96B412A3},	{0x4AD4095A, 0x9299C95B},	{0x1152354B, 0x5E0683A1},
	{0xCB82F7CD, 0x2DD30479},	{0x5EDD1FDA, 0x1CD604C7},	{0xBDD3A2FF, 0x7C119D58},
	{0x5778A9B1, 0xF9AFB48F},	{0x1807703D, 0x8967B4F3},	{0x35FDE08C, 0x3B662889},
	{0xDBD6BF92, 0x2E764125},	{0x5FECD5DB, 0xFD16169E},	{0x0E727AA0, 0xC69DE3FF},
	{0x89D650BF, 0x278D8734},	{0x9E8C3644, 0x5EA16D74},	{0x03D22723, 0x295691A9},
	{0x01F242A3, 0x81EE601E},	{0xDD6AE86A, 0x88EAACA7},	{0x6EFFF508, 0x872DC888},
	{0x44973FE6, 0xFA51ABF5},	{0x1443689A, 0xC5B72713},	{0x72E1C281, 0x9CF13918},
	{0x97F39713, 0x0968339D},	{0xB332242B, 0x815D66E8},	{0xD6678401, 0xE342F8F0},
	{0x167C5572, 0x74C4BDE2},	{0xA991DAE8, 0x3BA8D4DF},	{0x54628D86, 0x8C0FCB13},
	{0x7A3DFC6A, 0x3B0F70A7},	{0x913C962E, 0x9F55D149},	{0xBE66C593, 0x5DB6C18A},
	{0xF24DB7E1, 0x6D60976C},	{0xFBA55721, 0x3DE942BD},	{0x8DD7AC21, 0x05A26FE0},
	{0x458AA8A3, 0xB692F043},	{0x8340605C, 0x4FB4C410},	{0xF8C6DF84, 0xF4F897B3},
	{0x6FD84B0A, 0x624F7213},	{0x715C7E1F, 0xC01EB678},	{0x4D5603F0, 0x46E43EBC},
	{0x223BDDC4, 0x5307A4EC},	{0x23DD6B9D, 0xBFEE6C3B},	{0xBD006A3C, 0xEBF89D5F},
	{0x1D4575B8, 0x22B15640},	{0x6277C21A, 0x763F7121},	{0x62954071, 0x4E301CD0},
	{0x9026C985, 0xE46E11B4},	{0xB7E2DDAF, 0xBED46EC5},	{0x9A006B02, 0x079284A9},
	{0xA1E9339B, 0x7BABC7FF},	{0xBD73D886, 0x77D8B593},	{0x25E0D719, 0x34B4EED0},
	{0xE9E94EA9, 0x5F787310},	{0x2CE227AC, 0x79E54E5D},	{0x832A1A76, 0xCA2E3CA5},
	{0x50685513, 0xBD4DE242},	{0xACF847EC, 0x789576E2},	{0xB32BE614, 0xFD9CAEDE},
	{0xF9D04ADB, 0x8ADDEC75},	{0x731A7664, 0x0FE73AB5},	{0x0D42D39D, 0xF9E2DF1F},
	{0x99D81D79, 0xD8257ABF},	{0xEC3D031B, 0xF5C5DADC},	{0x87CEDC90, 0xBDB6FFA5},
	{0x3683EE4B, 0x614BFCAC},	{0xC68D1A60, 0xDDE4181A},	{0x524A1B1A, 0xBCC5C1F2},
	{0xCB09B7F2, 0x0977C0F2},	{0xA5306B35, 0xF0EA0639},	{0xC603E5F5, 0xD3722A5B},
	{0xF5A94D45, 0xAF692CA9},	{0x215844F3, 0x093EBB26},	{0xBBBBBD0F, 0xE5121369},
	{0x6911A7A9, 0x8187206F},	{0x5AA545FF, 0xBDD874BC},	{0xB10406B1, 0xA91FDC8B},
	{0x24F01D7F, 0xFF0A8297},	{0x3B4FA26F, 0x499A096A},	{0x97D698A7, 0xEF2B7390},
	{0xCA13B425, 0xCC90A373},	{0x12A7F642, 0x84BC86BB},	{0x6378759D, 0x8A161D60},
	{0xEAFC7C95, 0xA82571E4},	{0x255CF054, 0xFF5C260B},	{0x5907ED46, 0x58E67768},
	{0x0E727AA0, 0xC3805DF7},	{0xFAEB2D36, 0xDD11C54F},	{0x5FECD5DB, 0xB4583E02},
	{0x477B68E5, 0x88A66E6D},	{0x22B52501, 0xC4695016},	{0x1496BAC3, 0xF8C55D2B},
	{0xD814E756, 0xB095C646},	{0x57156C93, 0x1EEAD374},	{0x8602C536, 0xE1F8A128}
};
Hash GetPickupTypeFromWeaponModel(const Hash wpModel);
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
bool IsPedMainProtagonist(const Ped ped);
bool IsPedACop(const Ped ped);
bool IsPlayerAiming();
bool IsPlayerInsideSafehouse();
void SetDispatchServices(bool toggle);
bool GetFakeWanted();
void SetFakeWanted(Player player, bool toggle);
inline int GetNumberOfScriptInstances(const char* name) { return (GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH(Joaat(name))); }

enum eDamageZone {
	DZ_HEAD = 0,
	DZ_UPPER_BODY = 1,
	DZ_LOWER_BODY = 2,
	DZ_ARMOR = 3
};

int GetGeneralDamageFromBoneTag(const int boneTag);
int GetNMPartIndexFromBoneTag(const int boneTag);
int GetBoneTagFromNMPartIndex(const int partIndex);
#pragma endregion

void UpdatePlayerVars();