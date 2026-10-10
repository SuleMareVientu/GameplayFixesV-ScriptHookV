//ScriptHook
#include <shv\natives.h>
#include <shv\types.h>

//std
#include <sstream>
#include <math.h>
#include <algorithm>
#include <random>
#include <numeric>
#include <set>
#include <fstream>

//Custom
#include "utils\functions.h"
#include "utils\keyboard.h"
#include "utils\ini.h"
#include "utils\peds.h"

#pragma region Generic
std::filesystem::path AbsoluteModulePath(HINSTANCE module)
{
	wchar_t path[FILENAME_MAX] = { 0 };
	GetModuleFileNameW(module, path, FILENAME_MAX);
	return std::filesystem::path(path);
}

void SplitString(const char* charStr, std::string arr[], const int arrSize, const bool toUpper)
{
	if (!charStr || arrSize <= 0)
		return;

	int i = 0;
	std::string token;
	std::istringstream ss(charStr);
	while (i < arrSize && std::getline(ss, token, ','))
	{
		// Trim whitespace
		const auto first = token.find_first_not_of(" \t\r\n");
		if (first == std::string::npos)
		{
			token.clear();
		}
		else
		{
			const auto last = token.find_last_not_of(" \t\r\n");
			token = token.substr(first, (last - first + 1));
		}

		if (toUpper)
			std::transform(token.begin(), token.end(), token.begin(), ::toupper);

		arr[i++] = std::move(token);
	}

	while (i < arrSize)
	{
		arr[i++].clear();
	}
	return;
}

/*
//    The range [minValue, maxValue] is inclusive.
int GetRandomIntInRange(int minValue, int maxValue, bool useRd)
{
	if (useRd)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> distrib(minValue, maxValue);
		return distrib(gen);
	}

	maxValue += 1;
	SET_RANDOM_SEED(rand());
	return GET_RANDOM_INT_IN_RANGE(minValue, maxValue);
}

//Chance out of 100
bool GetWeightedBool(int chance, bool useRd)
{
	if (chance <= 0) return false;
	if (chance >= 100) return true;
	
	const double n = chance * 0.01;
	
	if (useRd)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::bernoulli_distribution dist(n);
		return dist(gen);
	}

	std::default_random_engine gen(rand());
	std::bernoulli_distribution dist(n);
	return dist(gen);
}


Vector3 Normalize(Vector3 v)
{
	const float w = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	v.x /= w;
	v.y /= w;
	v.z /= w;
	return v;
}
*/

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

int GetPadControlFromString(const std::string& str)
{
	std::string tmpStr = str;
	std::transform(tmpStr.begin(), tmpStr.end(), tmpStr.begin(), ::toupper);
	auto it = mapPadControls.find(tmpStr);
	if (it != mapPadControls.end())
		return it->second;
	
	return -1;
}

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

int GetVKFromString(const std::string& str)
{
	std::string tmpStr = str;
	std::transform(tmpStr.begin(), tmpStr.end(), tmpStr.begin(), ::toupper);
	auto it = mapVKs.find(tmpStr);
	if (it != mapVKs.end())
		return it->second;
	
	return -1;
}
#pragma endregion

#pragma region Log
void ClearLog()
{
	std::ofstream ofFile(GetDllInstanceLogName(), std::ofstream::out | std::ofstream::trunc);
	ofFile.close();
	return;
}

void RawLog(const std::string& szInfo, const std::string& szData)
{
	if (!Ini::EnableLogging && !Ini::EnableDebugProfiler)
		return;

	std::ofstream ofFile(GetDllInstanceLogName(), std::ios_base::out | std::ios_base::app);

	if (ofFile.is_open())
	{
		SYSTEMTIME stCurrTime;
		GetLocalTime(&stCurrTime);

		ofFile << "|" <<
			std::setw(2) << std::setfill('0') << stCurrTime.wHour << ":" <<
			std::setw(2) << std::setfill('0') << stCurrTime.wMinute << ":" <<
			std::setw(2) << std::setfill('0') << stCurrTime.wSecond << "." <<
			std::setw(3) << std::setfill('0') << stCurrTime.wMilliseconds <<
			"|[" << szInfo << "] -- " << szData << "\n";

		ofFile.close();
	}
	return;
}

bool hasWrittenToLog = false;
constexpr int logBufferSize = 2048;
void WriteLog(const char* szInfo, const char* szFormat, ...)
{
	if (!Ini::EnableLogging && !Ini::EnableDebugProfiler)
		return;

	if (!hasWrittenToLog)
	{
		ClearLog();

		char szBuf[logBufferSize];
		snprintf(szBuf, logBufferSize, "%s v%d.%d", "GameplayFixesV", VER_MAX, VER_MIN);
		RawLog(std::string("Started"), std::string(szBuf));

		hasWrittenToLog = true;
	}

	char szBuf[logBufferSize];
	va_list args;
	va_start(args, szFormat);
	vsnprintf(szBuf, logBufferSize, szFormat, args);
	va_end(args);
	RawLog(std::string(szInfo), std::string(szBuf));
	return;
}
#pragma endregion

#pragma region JSON
void from_json(const nlohmann::json& j, WpTintJson& c) {
	if (j.contains("Index") && j["Index"].is_number_integer())
		c.Index = j.value("Index", -1);
	else
		c.Index = -1;
}

void from_json(const nlohmann::json& j, WpComponentJson& c) {
	if (j.contains("Name") && j["Name"].is_string())
		c.Name = j.value("Name", "");
	else
		c.Name = "UNK";

	if (j.contains("AttachBone") && j["AttachBone"].is_string())
		c.Bone = Joaat(j.value("AttachBone", "").c_str());
	else
		c.Bone = Joaat("UNK");
}

void from_json(const nlohmann::json& j, WpLiveryJson& c) {
	if (j.contains("Name") && j["Name"].is_string())
		c.Name = j.value("Name", "");
	else
		c.Name = "UNK";
}

void from_json(const nlohmann::json& j, WeaponJson& w) {

	if (j.contains("Name") && j["Name"].is_string())
		w.Name = j.value("Name", "");
	else
		w.Name = "UNK";

	if (j.contains("Category") && j["Category"].is_string())
		w.Category = j.value("Category", "");
	else
		w.Category = "UNK";

	if (j.contains("AmmoType") && j["AmmoType"].is_string())
		w.AmmoType = j.value("AmmoType", "");
	else
		w.AmmoType = "UNK";

	if (j.contains("DamageType") && j["DamageType"].is_string())
		w.DamageType = j.value("DamageType", "");
	else
		w.DamageType = "UNK";

	if (j.contains("Tints") && j["Tints"].is_array())
		w.Tints = j.value("Tints", std::vector<WpTintJson>{});
	else
		w.Tints = std::vector<WpTintJson>{};

	if (j.contains("Components") && j["Components"].is_array())
		w.Components = j.value("Components", std::vector<WpComponentJson>{});
	else
		w.Components = std::vector<WpComponentJson>{};

	if (j.contains("Liveries") && j["Liveries"].is_array())
		w.Liveries = j.value("Liveries", std::vector<WpLiveryJson>{});
	else
		w.Liveries = std::vector<WpLiveryJson>{};
}

std::string LoadJSONResource(HINSTANCE hInstance, const char* resource)
{
	HRSRC hRes = FindResource(hInstance, resource, "JSON");
	if (!hRes) throw std::runtime_error("Resource not found");

	HGLOBAL hData = LoadResource(hInstance, hRes);
	if (!hData) throw std::runtime_error("Failed to load resource");

	const char* data = static_cast<const char*>(LockResource(hData));
	if (!data) throw std::runtime_error("Failed to lock resource");

	DWORD size = SizeofResource(hInstance, hRes);
	return std::string(data, size);
}

bool hasWeaponJsonLoaded = false;
std::vector<WeaponJson> weaponInfo;
bool LoadWeaponJson()
{
	if (hasWeaponJsonLoaded)
		return true;

	json j;
	try
	{
		const std::string raw = LoadJSONResource(GetDllInstance(), "WEAPONINFO");
		j = json::parse(raw);
		weaponInfo = j.get<std::vector<WeaponJson>>();
	}
	catch (const std::exception& e)
	{
		WriteLog("Error", "Failed to parse weapon info JSON: %s", e.what());
		hasWeaponJsonLoaded = true;
		return false;
	}

	hasWeaponJsonLoaded = true;
	return true;
}
#pragma endregion

#pragma region Print
void Print(char* string, int ms)
{
	BEGIN_TEXT_COMMAND_PRINT("STRING");
	ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(string);
	END_TEXT_COMMAND_PRINT(ms, 1);
	return;
}

void Print(const std::string& string, int ms)
{
	BEGIN_TEXT_COMMAND_PRINT("STRING");
	ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(const_cast<char*>(string.c_str()));
	END_TEXT_COMMAND_PRINT(ms, 1);
	return;
}

void Print(const int value, int ms)
{
	BEGIN_TEXT_COMMAND_PRINT("NUMBER");
	ADD_TEXT_COMPONENT_INTEGER(value);
	END_TEXT_COMMAND_PRINT(ms, 1);
	return;
}

void Print(const float value, int ms)
{
	BEGIN_TEXT_COMMAND_PRINT("NUMBER");
	ADD_TEXT_COMPONENT_FLOAT(value, 4);
	END_TEXT_COMMAND_PRINT(ms, 1);
	return;
}

void PrintHelp(char* string, bool playSound, int overrideDuration)
{
	BEGIN_TEXT_COMMAND_DISPLAY_HELP("STRING");
	ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(string);
	END_TEXT_COMMAND_DISPLAY_HELP(NULL, false, playSound, overrideDuration);
	return;
}

int ShowNotification(const char* str, bool flash)
{
	BEGIN_TEXT_COMMAND_THEFEED_POST("STRING");
	ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(str);
	return END_TEXT_COMMAND_THEFEED_POST_TICKER(flash, false);
}
#pragma endregion

#pragma region Assets Request
bool RequestModel(Hash model)
{
	if (!IS_MODEL_VALID(model))
		return false;

	if (!HAS_MODEL_LOADED(model))
	{
		REQUEST_MODEL(model);
		return false;
	}
	return true;
}

bool RequestAnimDict(char* animDict)
{
	if (!DOES_ANIM_DICT_EXIST(animDict))
		return false;

	if (!HAS_ANIM_DICT_LOADED(animDict))
	{
		REQUEST_ANIM_DICT(animDict);
		return false;
	}
	return true;
}

// REQUEST_ANIM_SET is deprecated, use REQUEST_CLIP_SET
bool RequestClipSet(char* animDict)
{
	if (!HAS_CLIP_SET_LOADED(animDict))
	{
		REQUEST_CLIP_SET(animDict);
		return false;
	}
	return true;
}

/*
bool RequestScaleform(const char* name, int* handle)
{
	if (HAS_SCALEFORM_MOVIE_LOADED(*handle))
		return true;

	*handle = REQUEST_SCALEFORM_MOVIE(name);
	return false;
}
*/

Object CreateObject(Hash model, float locX, float locY, float locZ, float rotX, float rotY, float rotZ)
{
	const Object obj = CREATE_OBJECT_NO_OFFSET(model, locX, locY, locZ, false, true, false);
	if (rotX != 0.0f || rotY != 0.0f || rotZ != 0.0f)
		SET_ENTITY_ROTATION(obj, rotX, rotY, rotZ, EULER_YXZ, false);

	return obj;
}

void DeleteEntity(Entity* obj)
{
	if (obj && DOES_ENTITY_EXIST(*obj))
	{
		if (IS_ENTITY_ATTACHED(*obj))
			DETACH_ENTITY(*obj, false, false);

		SET_ENTITY_AS_MISSION_ENTITY(*obj, false, true);
		DELETE_ENTITY(obj);
	}
	return;
}
#pragma endregion

#pragma region Ped Flags
void EnablePedConfigFlag(Ped ped, int flag)
{
	if (!GET_PED_CONFIG_FLAG(ped, flag, false))
		SET_PED_CONFIG_FLAG(ped, flag, true);
	return;
}

void DisablePedConfigFlag(Ped ped, int flag)
{
	if (GET_PED_CONFIG_FLAG(ped, flag, false))
		SET_PED_CONFIG_FLAG(ped, flag, false);
	return;
}

void EnablePedResetFlag(Ped ped, int flag)
{
	if (!GET_PED_RESET_FLAG(ped, flag))
		SET_PED_RESET_FLAG(ped, flag, true);
	return;
}

void DisablePedResetFlag(Ped ped, int flag)
{
	if (GET_PED_RESET_FLAG(ped, flag))
		SET_PED_RESET_FLAG(ped, flag, false);
	return;
}
#pragma endregion

#pragma region Weapons
static std::set<Entity> lastDamageEntity;
static std::set<Ped> lastDamageBone;
static std::set<Entity> lastWeaponDamage;

void ClearEntityLastDamageEntity(Entity entity)
{
	lastDamageEntity.emplace(entity);
	return;
}

void ClearPedLastDamageBone(Ped ped)
{
	lastDamageBone.emplace(ped);
	return;
}

void ClearEntityLastWeaponDamage(Entity entity)
{
	lastWeaponDamage.emplace(entity);
	return;
}

// This function ensures that the damage isn't cleared until the next frame,
// allowing multiple functions to check the last damage
void ClearLastDamages()
{
	if (!lastDamageEntity.empty())
	{
		for (const Entity& ped : lastDamageEntity) {
			CLEAR_ENTITY_LAST_DAMAGE_ENTITY(ped);
		}
		lastDamageEntity.clear();
	}

	if (!lastDamageBone.empty())
	{
		for (const Ped& ped : lastDamageBone) {
			CLEAR_PED_LAST_DAMAGE_BONE(ped);
		}
		lastDamageBone.clear();
	}

	if (!lastWeaponDamage.empty())
	{
		for (const Entity& entity : lastWeaponDamage) {
			CLEAR_ENTITY_LAST_WEAPON_DAMAGE(entity);
		}
		lastWeaponDamage.clear();
	}
	return;
}

void ResetLastDamages()
{
	lastDamageEntity.clear();
	lastDamageBone.clear();
	lastWeaponDamage.clear();
}

bool HasEntityBeenDamagedByWeaponThisFrame(Ped ped, Hash weaponHash, int weaponType)
{
	const bool res = HAS_ENTITY_BEEN_DAMAGED_BY_WEAPON(ped, weaponHash, weaponType);
	if (res)
		ClearEntityLastWeaponDamage(ped);

	return res;
}

/*
bool HasEntityBeenDamagedByAnyPedThisFrame(Ped ped)
{
	if (HAS_ENTITY_BEEN_DAMAGED_BY_ANY_PED(ped))
	{
		ClearEntityLastDamageEntity(ped);
		return true;
	}

	return false;
}
*/

bool DoesPedWeaponHaveComponentType(const Ped ped, const Hash weaponHash, const Hash attachPart, const bool findAll)
{
	if (!LoadWeaponJson())
		return false;

	const auto it = std::find(weaponInfo.begin(), weaponInfo.end(), weaponHash);
	if (it == weaponInfo.end() || it->Components.empty())
		return false;

	LOOP(i, it->Components.size())
	{
		const Hash componentHash = Joaat(it->Components[i].Name.c_str());
		if (componentHash == Joaat("UNK"))
			continue;

		const Hash bone = it->Components[i].Bone;
		if (bone == Joaat("UNK"))
			continue;

		bool found = false;
		if (findAll)
		{
			switch (attachPart)
			{
			case WAP_Clip:
				switch (bone)
				{
				case WAP_Clip:
				case WAP_Clip_2:
					found = true; break;
				}
				break;
			case WAP_Flsh:
				switch (bone)
				{
				case WAP_Flsh:
				case WAP_Flsh_2:
					found = true; break;
				}
				break;
			case WAP_FlshLasr:
				switch (bone)
				{
				case WAP_FlshLasr:
				case WAP_FlshLasr_2:
				case WAP_FlshLasr_3:
					found = true; break;
				}
				break;
			case WAP_Supp:
				switch (bone)
				{
				case WAP_Supp:
				case WAP_Supp_2:
				case WAP_Supp_3:
					found = true; break;
				}
				break;
			case WAP_Grip:
				switch (bone)
				{
				case WAP_Grip:
				case WAP_Grip_2:
				case WAP_Grip_3:
					found = true; break;
				}
				break;
			case WAP_Scop:
				switch (bone)
				{
				case WAP_Scop:
				case WAP_Scop_2:
				case WAP_Scop_3:
					found = true; break;
				}
				break;
			default:
				if (attachPart == bone)
					found = true;
				break;
			}
		}
		else if (attachPart == bone)
			found = true;

		if (found && HAS_PED_GOT_WEAPON_COMPONENT(ped, weaponHash, componentHash))
			return true;
	}

	return false;
}

//Inspired by jedijosh920 implementation of "Disarm"
bool CanDisarmPed(Ped ped, bool includeLeftHand)
{
	int bone = NULL;
	if (!GET_PED_LAST_DAMAGE_BONE(ped, &bone) || bone == NULL)
		return false;

	if (bone == BONETAG_PH_R_HAND || bone == BONETAG_R_HAND)
	{
		ClearPedLastDamageBone(ped);
		return true;
	}
	else if (includeLeftHand && (bone == BONETAG_PH_L_HAND || bone == BONETAG_L_HAND))
	{
		ClearPedLastDamageBone(ped);
		return true;
	}

	return false;
}

int GetWeaponBlipSprite(const Hash weaponHash)
{
	switch (weaponHash)
	{
	case Joaat("WEAPON_MINIGUN"):
		return 173; break;
	case Joaat("WEAPON_KNIFE"):
	case Joaat("WEAPON_DAGGER"):
	case Joaat("WEAPON_HATCHET"):
	case Joaat("WEAPON_MACHETE"):
	case Joaat("WEAPON_SWITCHBLADE"):
	case Joaat("WEAPON_BATTLEAXE"):
	case Joaat("WEAPON_STONE_HATCHET"):
		return 154; break;
	case Joaat("WEAPON_BOTTLE"):
		return 155; break;
	case Joaat("WEAPON_PETROLCAN"):
		return 415; break;
	}

	const int group = GET_WEAPONTYPE_GROUP(weaponHash);
	switch (group)
	{
	case WEAPONGROUP_MELEE:
		return 151; break;
	case WEAPONGROUP_PISTOL:
		return 156; break;
	case WEAPONGROUP_SMG:
		return 159; break;
	case WEAPONGROUP_RIFLE:
	case WEAPONGROUP_MG:
		return 150; break;
	case WEAPONGROUP_SHOTGUN:
		return 158; break;
	case WEAPONGROUP_SNIPER:
		return 160; break;
	case WEAPONGROUP_HEAVY:
		return 157; break;
	case WEAPONGROUP_PETROLCAN:
		return 415; break;
	default:
		return 408; break;
	}
}

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

// DO NOT USE GET_PICKUP_TYPE_FROM_WEAPON_HASH, it requires b1290
Hash GetPickupTypeFromWeaponModel(const Hash wpModel)
{
	auto it = wpPickupMap.find(wpModel);
	if (it != wpPickupMap.end())
		return it->second;

	return 0;
}

bool ShouldWeaponSpawnPickupWhenDropped(const Hash weaponHash, const bool checkWeaponType)
{
	if (weaponHash == WEAPON_UNARMED || weaponHash == 0)
		return false;

	if (checkWeaponType)
	{
		const int wpGroup = GET_WEAPONTYPE_GROUP(weaponHash);
		if (wpGroup == WEAPONGROUP_MELEE || wpGroup == WEAPONGROUP_THROWN ||
			wpGroup == WEAPONGROUP_FIREEXTINGUISHER || wpGroup == WEAPONGROUP_PETROLCAN ||
			wpGroup == WEAPONGROUP_LOUDHAILER || wpGroup == WEAPONGROUP_DIGISCANNER ||
			wpGroup == WEAPONGROUP_NIGHTVISION || wpGroup == WEAPONGROUP_PARACHUTE ||
			wpGroup == WEAPONGROUP_JETPACK || wpGroup == WEAPONGROUP_METALDETECTOR)
			return false;
	}

	return true;
}

std::vector<WeaponPickup> droppedWeapons;

class WeaponDropManager
{
public:
	static void Drop(Hash weaponHash, const bool shouldCurse, Vector3 wpRot)
	{
		if (!LoadWeaponJson())
			return;

		if (shouldCurse && !IS_AMBIENT_SPEECH_PLAYING(GetPlayerPed()))
			PLAY_PED_AMBIENT_SPEECH_NATIVE(GetPlayerPed(), "GENERIC_CURSE_MED", "SPEECH_PARAMS_FORCE", false);

		WeaponPickup wp;
		wp.WpHash = weaponHash;
		wp.TintIndex = GET_PED_WEAPON_TINT_INDEX(GetPlayerPed(), weaponHash);

		if (GetGameVersion() >= VER_1_0_1103_2_STEAM)
			wp.CamoIndex = GET_PED_WEAPON_CAMO_INDEX(GetPlayerPed(), weaponHash);

		const auto it = std::find(weaponInfo.begin(), weaponInfo.end(), weaponHash);
		if (it != weaponInfo.end())
		{
			if (!it->Components.empty())
			{
				LOOP(i, it->Components.size())
				{
					const Hash component = Joaat(it->Components[i].Name.c_str());
					if (component == Joaat("UNK"))
						continue;

					if (HAS_PED_GOT_WEAPON_COMPONENT(GetPlayerPed(), weaponHash, component))
						wp.Components.push_back(component);
				}
			}

			if (!it->Liveries.empty() && GetGameVersion() >= VER_1_0_1103_2_STEAM)
			{
				LOOP(i, it->Liveries.size())
				{
					const Hash livery = Joaat(it->Liveries[i].Name.c_str());
					if (livery == Joaat("UNK"))
						continue;

					const int liveryTint = GET_PED_WEAPON_COMPONENT_TINT_INDEX(GetPlayerPed(), weaponHash, livery);
					if (liveryTint > -1)
						wp.Liveries.push_back(WpPickupLivery{ livery, liveryTint });
				}
			}
		}

		SET_CURRENT_PED_WEAPON(GetPlayerPed(), WEAPON_UNARMED, true);
		REMOVE_WEAPON_FROM_PED(GetPlayerPed(), weaponHash);

		// Pickup Section
		const Vector3 loc = GET_PED_BONE_COORDS(GetPlayerPed(), BONETAG_PH_R_HAND, 0.0f, 0.0f, 0.0f);
		SET_LOCAL_PLAYER_PERMITTED_TO_COLLECT_PICKUPS_WITH_MODEL(GET_WEAPONTYPE_MODEL(weaponHash), false);

		if (wpRot != Vector3())
		{
			if (GetGameVersion() >= VER_1_0_1290_1_STEAM)
			{
				wp.PickupIndex = CREATE_PICKUP_ROTATE(GET_PICKUP_TYPE_FROM_WEAPON_HASH(weaponHash),
					loc.x, loc.y, loc.z, wpRot.x, wpRot.y, wpRot.z, PLACEMENT_FLAG_LOCAL_ONLY, -1, EULER_YXZ, false, NULL);
			}
			else
			{
				wp.PickupIndex = CREATE_PICKUP_ROTATE(GetPickupTypeFromWeaponModel(GET_WEAPONTYPE_MODEL(weaponHash)),
					loc.x, loc.y, loc.z, wpRot.x, wpRot.y, wpRot.z, PLACEMENT_FLAG_LOCAL_ONLY, -1, EULER_YXZ, false, NULL);
			}
		}
		else
		{
			if (GetGameVersion() >= VER_1_0_1290_1_STEAM)
				wp.PickupIndex = CREATE_PICKUP(GET_PICKUP_TYPE_FROM_WEAPON_HASH(weaponHash), loc.x, loc.y, loc.z, PLACEMENT_FLAG_LOCAL_ONLY, -1, false, NULL);
			else
				wp.PickupIndex = CREATE_PICKUP(GetPickupTypeFromWeaponModel(GET_WEAPONTYPE_MODEL(weaponHash)), loc.x, loc.y, loc.z, PLACEMENT_FLAG_LOCAL_ONLY, -1, false, NULL);
		}

		if (Ini::DroppedWeaponsBlip)
		{
			wp.PickupBlip = ADD_BLIP_FOR_PICKUP(wp.PickupIndex);
			SET_BLIP_SPRITE(wp.PickupBlip, GetWeaponBlipSprite(weaponHash));
			SET_BLIP_SCALE(wp.PickupBlip, 0.7f);
		}

		droppedWeapons.push_back(wp);
	}

	static void Restore(bool autoEquip)
	{
		if (!LoadWeaponJson() || s_retrievedWeaponThisFrame || droppedWeapons.empty())
			return;

		const float maxDist = Ini::DroppedWeaponsMaxDistance;
		const Vector3 playerCoords = GetPlayerCoords();

		for (auto it = droppedWeapons.begin(); it != droppedWeapons.end();)
		{
			if (!DOES_PICKUP_EXIST(it->PickupIndex))
			{
				it = droppedWeapons.erase(it);
				continue;
			}

			if (maxDist > 0.0f)
			{
				const Vector3 pickupCoords = GET_PICKUP_COORDS(it->PickupIndex);
				if ((playerCoords - pickupCoords).LengthSq() > (maxDist * maxDist))
				{
					REMOVE_PICKUP(it->PickupIndex);
					it = droppedWeapons.erase(it);
					continue;
				}
			}

			// Restore original model appearance
			if (!it->Initialized && DOES_PICKUP_OBJECT_EXIST(it->PickupIndex))
			{
				const Object po = GET_PICKUP_OBJECT(it->PickupIndex);
				SET_ACTIVATE_OBJECT_PHYSICS_AS_SOON_AS_IT_IS_UNFROZEN(po, true);
				ACTIVATE_PHYSICS(po);

				if (GetGameVersion() >= VER_1_0_1180_2_STEAM)
					FORCE_ACTIVATE_PHYSICS_ON_UNFIXED_PICKUP(po, true);

				SET_ENTITY_DYNAMIC(po, true);
				SET_ENTITY_REQUIRES_MORE_EXPENSIVE_RIVER_CHECK(po, true);

				if (GetGameVersion() >= VER_1_0_678_1_STEAM)
					SET_PICKUP_COLLIDES_WITH_PROJECTILES(po, true);

				if (it->TintIndex > -1)
					SET_WEAPON_OBJECT_TINT_INDEX(po, it->TintIndex);

				if (GetGameVersion() >= VER_1_0_1103_2_STEAM)
				{
					if (it->CamoIndex > -1)
						SET_WEAPON_OBJECT_CAMO_INDEX(po, it->CamoIndex);
				}

				if (!it->Components.empty())
				{
					LOOP(j, it->Components.size())
					{
						if (it->Components[j] == Joaat("UNK"))
							continue;

						GIVE_WEAPON_COMPONENT_TO_WEAPON_OBJECT(po, it->Components[j]);
					}
				}

				if (!it->Liveries.empty() && GetGameVersion() >= VER_1_0_1103_2_STEAM)
				{
					LOOP(j, it->Liveries.size())
					{
						if (it->Liveries[j].LiveryHash == Joaat("UNK") ||
							it->Liveries[j].TintIndex < 0)
							continue;

						GIVE_WEAPON_COMPONENT_TO_WEAPON_OBJECT(po, it->Liveries[j].LiveryHash);
						SET_WEAPON_OBJECT_COMPONENT_TINT_INDEX(po, it->Liveries[j].LiveryHash,
							it->Liveries[j].TintIndex);
					}
				}

				it->Initialized = true;
			}

			// Restore ability to pickup weapons after ragdoll has ended
			if (!IS_PED_RAGDOLL(GetPlayerPed()))
				SET_LOCAL_PLAYER_PERMITTED_TO_COLLECT_PICKUPS_WITH_MODEL(GET_WEAPONTYPE_MODEL(it->WpHash), true);

			if (!HAS_PICKUP_BEEN_COLLECTED(it->PickupIndex))
			{
				++it;
				continue;
			}

			if (it->TintIndex > -1)
				SET_PED_WEAPON_TINT_INDEX(GetPlayerPed(), it->WpHash, it->TintIndex);

			if (!it->Components.empty())
			{
				LOOP(j, it->Components.size())
				{
					if (it->Components[j] == Joaat("UNK"))
						continue;

					GIVE_WEAPON_COMPONENT_TO_PED(GetPlayerPed(), it->WpHash, it->Components[j]);
				}
			}

			if (!it->Liveries.empty() && GetGameVersion() >= VER_1_0_1103_2_STEAM)
			{
				LOOP(j, it->Liveries.size())
				{
					if (it->Liveries[j].LiveryHash == Joaat("UNK") ||
						it->Liveries[j].TintIndex < 0)
						continue;

					GIVE_WEAPON_COMPONENT_TO_PED(GetPlayerPed(), it->WpHash, it->Liveries[j].LiveryHash);
					SET_PED_WEAPON_COMPONENT_TINT_INDEX(GetPlayerPed(), it->WpHash,
						it->Liveries[j].LiveryHash,
						it->Liveries[j].TintIndex);
				}
			}

			Hash weapon = NULL;
			GET_CURRENT_PED_WEAPON(GetPlayerPed(), &weapon, false);
			if (autoEquip && weapon == WEAPON_UNARMED)
				SET_CURRENT_PED_WEAPON(GetPlayerPed(), it->WpHash, false);

			s_retrievedWeaponThisFrame = true;
			REMOVE_PICKUP(it->PickupIndex);
			it = droppedWeapons.erase(it);
			break;
		}
	}

	static void ResetFrame()
	{
		s_retrievedWeaponThisFrame = false;
	}

	static void Clear()
	{
		s_retrievedWeaponThisFrame = false;
		if (!GET_IS_LOADING_SCREEN_ACTIVE())
		{
			for (auto& wp : droppedWeapons)
			{
				if (wp.PickupBlip && DOES_BLIP_EXIST(wp.PickupBlip))
					REMOVE_BLIP(&wp.PickupBlip);

				if (wp.PickupIndex && DOES_PICKUP_EXIST(wp.PickupIndex))
					REMOVE_PICKUP(wp.PickupIndex);
			}
		}
		droppedWeapons.clear();
	}

private:
	static inline bool s_retrievedWeaponThisFrame = false;
};

void ResetWeaponDrops()
{
	WeaponDropManager::Clear();
}

void DropPlayerWeapon(Hash weaponHash, const bool shouldCurse, Vector3 wpRot)
{
	WeaponDropManager::Drop(weaponHash, shouldCurse, wpRot);
}

void RestorePlayerRetrievedWeapon(bool autoEquip)
{
	WeaponDropManager::Restore(autoEquip);
}

template <typename TMsg>
inline void SendNMMessage(Ped ped, eNMStr messageId, TMsg&& msg, const char* paramName = "start", bool paramVal = true)
{
	ULONG_PTR msgPtr = nGame::CreateNmMessage();
	msg(msgPtr);
	if (paramName)
		nGame::SetNMMessageParam(msgPtr, paramName, paramVal);
	nGame::GivePedNMMessage(msgPtr, ped, messageId);
}

void TaskNMShot(Ped ped, Hash wpHash, int partIndex, Vector3 hitLoc, Vector3 impulseNorm, bool isAiming, bool isCrouched)
{
	bool bulletProofVest = false;
	if (ped == GetPlayerPed())
		bulletProofVest = (GET_PED_ARMOUR(ped) / GET_PLAYER_MAX_ARMOUR(GetPlayer())) >= 0.25f;
	else
		bulletProofVest = GET_PED_ARMOUR(ped) >= 25;

	bool crouching = false;
	if ((IS_PED_IN_COVER(ped, false) && !IS_PED_IN_HIGH_COVER(ped)) || isCrouched)
		crouching = true;

	constexpr float pistolImpulseMult = 30.0f;
	constexpr float rifleImpulseMult = pistolImpulseMult * 2.0f;
	constexpr float shotgunImpulseMult = rifleImpulseMult * 2.0f;
	constexpr float sniperImpulseMult = shotgunImpulseMult;
	constexpr float heavyImpulseMult = shotgunImpulseMult * 1.5f;

	bool pointGun = false;
	float impulseMult = pistolImpulseMult;
	const Hash wpGroup = GET_WEAPONTYPE_GROUP(wpHash);
	switch (wpGroup)
	{
	case WEAPONGROUP_PISTOL:
	case WEAPONGROUP_SMG:
		impulseMult = pistolImpulseMult;
		pointGun = true;
		break;
	case WEAPONGROUP_RIFLE:
	case WEAPONGROUP_MG:
		impulseMult = rifleImpulseMult;
		pointGun = true;
		break;
	case WEAPONGROUP_SHOTGUN:
		impulseMult = shotgunImpulseMult;
		break;
	case WEAPONGROUP_SNIPER:
		impulseMult = sniperImpulseMult;
		break;
	case WEAPONGROUP_HEAVY:
		impulseMult = heavyImpulseMult;
		break;
	case WEAPONGROUP_RUBBERGUN:
		impulseMult = pistolImpulseMult;
		pointGun = true;
		break;
	}

	if (!isAiming)
		pointGun = false;

	NMMessageShot shot;
	shot.initialNeckDamping = 0.5f;
	shot.minArmsLooseness = 0.0f;
	shot.angVelScale = 0.5f;
	shot.timeBeforeReachForWound = 0.0f;
	shot.cpainSmooth2Time = 0.0f;
	shot.cpainMag = 0.0f;
	shot.cpainTwistMag = 0.0f;
	shot.cpainSmooth2Zero = 0.5f;
	shot.crouching = crouching;
	shot.bulletProofVest = bulletProofVest;
	shot.reachForWound = !bulletProofVest;
	shot.allowInjuredLeg = true;
	shot.allowInjuredThighReach = true;
	shot.fallingReaction = 0;
	shot.initialWeaknessZeroDuration = 0.2f;
	shot.initialWeaknessRampDuration = 0.2f;
	shot.cStrUpperMin = 1.0f;
	shot.cStrLowerMin = 1.0f;
	SendNMMessage(ped, NM_SHOT_MSG, shot);

	SendNMMessage(ped, NM_SHOTSHOCKSPIN_MSG, NMMessageShotShockSpin{});
	SendNMMessage(ped, NM_SHOTINGUTS_MSG, NMMessageShotInGuts{});

	NMMessageShotConfigureArms shotConfigureArms;
	shotConfigureArms.pointGun = pointGun;
	shotConfigureArms.useArmsWindmill = GetWeightedBool(50) && !bulletProofVest && !pointGun;
	SendNMMessage(ped, NM_SHOTCONFIGUREARMS_MSG, shotConfigureArms);

	SendNMMessage(ped, NM_BALANCE_MSG, NMMessageBodyBalance{});
	SendNMMessage(ped, NM_STAYUPRIGHT_MSG, NMMessageStayUpright{});
	SendNMMessage(ped, NM_SET_FALLING_REACTION_MSG, NMMessageSetFallingReaction{});

	NMMessageShotNewBullet shotNewBullet;
	shotNewBullet.normal = -(impulseNorm);
	shotNewBullet.bodyPart = partIndex;
	shotNewBullet.hitPoint = hitLoc;
	shotNewBullet.localHitPointInfo = false;
	SendNMMessage(ped, NM_SHOTNEWBULLET_MSG, shotNewBullet);

	NMMessageApplyBulletImpulse applyBulletImpulse;
	applyBulletImpulse.equalizeAmount = 0.5f;
	applyBulletImpulse.partIndex = partIndex;
	applyBulletImpulse.impulse = impulseNorm * impulseMult;
	applyBulletImpulse.hitPoint = hitLoc;
	applyBulletImpulse.localHitPointInfo = false;
	applyBulletImpulse.extraShare = 0.25f;
	SendNMMessage(ped, NM_APPLYBULLETIMP_MSG, applyBulletImpulse, "localImpulseInfo", false);

	NMMessageConfigureBullets configureBullets;
	configureBullets.doCounterImpulse = GetWeightedBool(50);
	SendNMMessage(ped, NM_CONFIGUREBULLETS_MSG, configureBullets);
	return;
}

void TaskNMElectrocute(Ped ped)
{
	SendNMMessage(ped, NM_ELECTROCUTE_MSG, NMMessageElectrocute{});
	SendNMMessage(ped, NM_BALANCE_MSG, NMMessageBodyBalance{});

	NMMessageConfigureBalance configureBalance;
	configureBalance.maxSteps = 15;
	SendNMMessage(ped, NM_CONFIGURE_BALANCE_MSG, configureBalance);

	SendNMMessage(ped, NM_STAGGERFALL_MSG, NMMessageStaggerFall{});
	return;
}
#pragma endregion

#pragma region Vehicle
Vehicle GetVehiclePedIsIn(const Ped ped, const bool includeEntering, const bool includeExiting)
{
	Vehicle veh = GET_VEHICLE_PED_IS_USING(ped);
	if ((!includeEntering && GET_PED_RESET_FLAG(ped, PRF_IsEnteringVehicle)) ||
		(!includeExiting && GET_IS_TASK_ACTIVE(ped, CODE_TASK_EXIT_VEHICLE)))
	{
		veh = NULL;
	}

	if (!DOES_ENTITY_EXIST(veh))
		veh = NULL;

	return veh;
}

Vehicle GetVehiclePedIsEntering(const Ped ped)
{
	Vehicle veh = NULL;
	if (GET_PED_RESET_FLAG(ped, PRF_IsEnteringVehicle))
	{
		veh = GET_VEHICLE_PED_IS_USING(ped);
		if (!DOES_ENTITY_EXIST(veh))
			veh = NULL;
	}

	return veh;
}

Vehicle GetVehiclePedIsExiting(const Ped ped)
{
	Vehicle veh = NULL;
	if (GET_IS_TASK_ACTIVE(ped, CODE_TASK_EXIT_VEHICLE))
	{
		veh = GET_VEHICLE_PED_IS_USING(ped);
		if (!DOES_ENTITY_EXIST(veh))
			veh = NULL;
	}

	return veh;
}

Vehicle GetVehiclePedIsEnteringOrExiting(const Ped ped)
{
	Vehicle veh = NULL;
	if (GET_PED_RESET_FLAG(ped, PRF_IsEnteringVehicle) ||
		GET_IS_TASK_ACTIVE(ped, CODE_TASK_EXIT_VEHICLE) ||
		GET_PED_RESET_FLAG(ped, PRF_IsEnteringOrExitingVehicle))
	{
		veh = GET_VEHICLE_PED_IS_USING(ped);
		if (!DOES_ENTITY_EXIST(veh))
			veh = NULL;
	}

	return veh;
}

// Requires b944
bool DoesVehicleHaveAbility(const Vehicle veh)
{
	if (DOES_ENTITY_EXIST(veh) && (GET_VEHICLE_HAS_KERS(veh) || GET_HAS_ROCKET_BOOST(veh) || GET_CAR_HAS_JUMP(veh)))
		return true;

	return false;
}
#pragma endregion

#pragma region HUD
bool initializedMinimap = false;
int minimapScaleformIndex = -1;
int RequestMinimapScaleform()
{
	if (!HAS_SCALEFORM_MOVIE_LOADED(minimapScaleformIndex))
	{
		initializedMinimap = false;
		minimapScaleformIndex = REQUEST_SCALEFORM_MOVIE("MINIMAP");
		return -1;
	}
	else if (!initializedMinimap)
	{
		if (BEGIN_SCALEFORM_MOVIE_METHOD(minimapScaleformIndex, "MINIMAP"))
		{
			initializedMinimap = true;
			SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(NULL);
			END_SCALEFORM_MOVIE_METHOD();
		}
	}
	return minimapScaleformIndex;
}

void SetTextStyle(TextStyle Style, bool bDrawBeforeFade)
{
	// choose font
	SET_TEXT_FONT(Style.aFont);
	if (Style.WrapStartX != 0.0f || Style.WrapEndX != 0.0f)
		SET_TEXT_WRAP(Style.WrapStartX, Style.WrapEndX);

	SET_TEXT_SCALE(Style.XScale, Style.YScale);
	SET_TEXT_COLOUR(Style.colour.R, Style.colour.G, Style.colour.B, Style.colour.A);

	switch (Style.drop)
	{
	case DROPSTYLE_NONE:
		break;
	case DROPSTYLE_ALL:
		SET_TEXT_OUTLINE();
		SET_TEXT_DROP_SHADOW();
		break;
	case DROPSTYLE_DROPSHADOWONLY:
		SET_TEXT_DROP_SHADOW();
		break;
	case DROPSTYLE_OUTLINEONLY:
		SET_TEXT_OUTLINE();
		break;
	}

	if (bDrawBeforeFade)
		SET_SCRIPT_GFX_DRAW_ORDER(GFX_ORDER_AFTER_HUD);

	//Other
	SET_TEXT_CENTRE(Style.centre);
	//SET_TEXT_DROPSHADOW(); // RGB parameters are unused, it's the same as SET_TEXT_DROP_SHADOW();
	//SET_TEXT_EDGE(0, 0, 0, 0, 0); // nullsub
	return;
}

int GetHudComponentFromString(const char* str)
{
	switch (Joaat(str))
	{
	case Joaat("HUD_WANTED_STARS"): return HUD_WANTED_STARS;
	case Joaat("HUD_WEAPON_ICON"): return HUD_WEAPON_ICON;
	case Joaat("HUD_CASH"): return HUD_CASH;
	case Joaat("HUD_MP_CASH"): return HUD_MP_CASH;
	case Joaat("HUD_MP_MESSAGE"): return HUD_MP_MESSAGE;
	case Joaat("HUD_VEHICLE_NAME"): return HUD_VEHICLE_NAME;
	case Joaat("HUD_AREA_NAME"): return HUD_AREA_NAME;
	case Joaat("HUD_UNUSED"): return HUD_UNUSED;
	case Joaat("HUD_STREET_NAME"): return HUD_STREET_NAME;
	case Joaat("HUD_HELP_TEXT"): return HUD_HELP_TEXT;
	case Joaat("HUD_FLOATING_HELP_TEXT_1"): return HUD_FLOATING_HELP_TEXT_1;
	case Joaat("HUD_FLOATING_HELP_TEXT_2"): return HUD_FLOATING_HELP_TEXT_2;
	case Joaat("HUD_CASH_CHANGE"): return HUD_CASH_CHANGE;
	case Joaat("HUD_RETICLE"): return HUD_RETICLE;
	case Joaat("HUD_SUBTITLE_TEXT"): return HUD_SUBTITLE_TEXT;
	case Joaat("HUD_RADIO_STATIONS"): return HUD_RADIO_STATIONS;
	case Joaat("HUD_SAVING_GAME"): return HUD_SAVING_GAME;
	case Joaat("HUD_GAME_STREAM"): return HUD_GAME_STREAM;
	case Joaat("HUD_WEAPON_WHEEL"): return HUD_WEAPON_WHEEL;
	case Joaat("HUD_WEAPON_WHEEL_STATS"): return HUD_WEAPON_WHEEL_STATS;
	}
	return -1;
}

void SetHealthHudDisplayValues(int healthPercentage, int armourPercentage, bool showDamage)
{
	SET_HEALTH_HUD_DISPLAY_VALUES(healthPercentage + 100, armourPercentage, showDamage);
	SET_MAX_HEALTH_HUD_DISPLAY(200);
	SET_MAX_ARMOUR_HUD_DISPLAY(100);
	return;
}
#pragma endregion

#pragma region Misc
void PlayScriptedAnim(const Ped ped, const char* dictionary0, const char* anim0, const float phase0, const float rate0, const float weight0, const int type, const int filter, const float blendInDelta, const float blendOutDelta, const int timeToPlay, const int flags, const int ikFlags)
{
	AnimData animData{}, nullAnimData{};
	SetAnimData(animData,
		dictionary0, anim0, phase0, rate0, weight0,
		type, filter, blendInDelta, blendOutDelta, timeToPlay, flags, ikFlags);
	TASK_SCRIPTED_ANIMATION(
		ped,
		reinterpret_cast<int*>(&animData),
		reinterpret_cast<int*>(&nullAnimData),
		reinterpret_cast<int*>(&nullAnimData),
		blendInDelta,
		blendOutDelta
	);
	return;
}

bool IsPedMainProtagonist(const Ped ped)
{
	switch (GET_PED_TYPE(ped))
	{
	case PEDTYPE_PLAYER1:			// Michael
	case PEDTYPE_PLAYER2:			// Franklin
	case PEDTYPE_PLAYER_UNUSED:		// Trevor
		return true;
	}
	return false;
}

bool IsPedMissionOrCompanion(const Ped ped)
{
	if (IS_ENTITY_A_MISSION_ENTITY(ped) || GET_PED_TYPE(ped) == PEDTYPE_MISSION || IsPedMainProtagonist(ped))
		return true;

	const Ped playerPed = GetPlayerPed();
	if (DOES_ENTITY_EXIST(playerPed))
	{
		const int playerGroup = GET_PLAYER_GROUP(GetPlayer());
		if (IS_PED_IN_GROUP(ped) && IS_PED_GROUP_MEMBER(ped, playerGroup))
			return true;

		const Hash relGroup = GET_PED_RELATIONSHIP_GROUP_HASH(ped);
		if (relGroup == RELGROUPHASH_PLAYER || relGroup == Joaat("PLAYER"))
			return true;

		const int rel = GET_RELATIONSHIP_BETWEEN_PEDS(ped, playerPed);
		if (rel == ACQUAINTANCE_TYPE_PED_RESPECT || rel == ACQUAINTANCE_TYPE_PED_LIKE)
			return true;

		const Vehicle playerVeh = GetVehiclePedIsUsing(playerPed);
		if (DOES_ENTITY_EXIST(playerVeh) && IS_PED_IN_ANY_VEHICLE(ped, false) && GetVehiclePedIsUsing(ped) == playerVeh)
			return true;
	}

	return false;
}

bool IsPedACop(const Ped ped)
{
	/*
	const Vector3 loc = GET_ENTITY_COORDS(ped, true);
	const Hash model = GET_ENTITY_MODEL(ped);

	if (!IS_MODEL_VALID(model))
		return;

	Vector3 min{ 0.0f, 0.0f, 0.0f }; Vector3 max{ 0.0f, 0.0f, 0.0f };
	GET_MODEL_DIMENSIONS(model, &min, &max);
	min += loc; max += loc;
	return IS_COP_PED_IN_AREA_3D(min.x, min.y, min.z, max.x, max.y, max.z);
	*/

	const int type = GET_PED_TYPE(ped);
	return (type == PEDTYPE_COP || type == PEDTYPE_SWAT || type == PEDTYPE_ARMY);
}

bool IsFirstPersonActive()
{
	if ((!IS_FOLLOW_PED_CAM_ACTIVE() && !IS_FOLLOW_VEHICLE_CAM_ACTIVE()) &&
		(GET_FOLLOW_PED_CAM_VIEW_MODE() == CAM_VIEW_MODE_FIRST_PERSON || GET_FOLLOW_VEHICLE_CAM_VIEW_MODE() == CAM_VIEW_MODE_FIRST_PERSON))
		return true;

	return false;
}

bool IsPlayerAiming(bool includeAimGunTask, bool includeShooting)
{
	if (IS_PLAYER_FREE_AIMING(GET_PLAYER_INDEX()) || IS_PED_AIMING_FROM_COVER(GetPlayerPed()) ||
		GET_PED_CONFIG_FLAG(GetPlayerPed(), PCF_IsAimingGun, false) || GET_PED_RESET_FLAG(GetPlayerPed(), PRF_IsAiming))
		return true;
	else if (includeAimGunTask && GET_PED_RESET_FLAG(GetPlayerPed(), PRF_HasGunTaskWithAimingState))	// When shooting without aiming
		return true;
	else if (includeShooting && IS_PED_SHOOTING(GetPlayerPed()))
		return true;

	return false;
}

bool isPlayerInsideSafehouse = false;
bool isPlayerArmed = false;

bool IsPlayerInsideSafehouse()
{
	const Ped playerPed = GetPlayerPed();
	if (!DOES_ENTITY_EXIST(playerPed) || GET_INTERIOR_FROM_ENTITY(playerPed) == 0)
		return false;

	const Vector3 tmpCoords = GetPlayerCoords();
	constexpr const char* safehouses[] = {
		"v_franklins",
		"v_franklinshouse",
		"v_michael",
		"v_trailer",
		"v_trevors"
	};

	for (const char* sh : safehouses)
	{
		if (GET_INTERIOR_AT_COORDS_WITH_TYPE(tmpCoords.x, tmpCoords.y, tmpCoords.z, sh))
			return true;
	}

	//Special check for Trevor's office inside the strip club
	if (GET_ROOM_KEY_FROM_ENTITY(playerPed) == strp3off)	//room key for "strp3off"
		return true;

	return false;
}

void SetDispatchServices(bool toggle)
{
	ENABLE_DISPATCH_SERVICE(DT_POLICE_AUTOMOBILE, toggle);
	ENABLE_DISPATCH_SERVICE(DT_POLICE_HELICOPTER, toggle);
	ENABLE_DISPATCH_SERVICE(DT_SWAT_AUTOMOBILE, toggle);
	ENABLE_DISPATCH_SERVICE(DT_POLICE_RIDERS, toggle);
	ENABLE_DISPATCH_SERVICE(DT_POLICE_VEHICLE_REQUEST, toggle);
	ENABLE_DISPATCH_SERVICE(DT_POLICE_ROAD_BLOCK, toggle);
	ENABLE_DISPATCH_SERVICE(DT_POLICE_AUTOMOBILE_WAIT_PULLED_OVER, toggle);
	ENABLE_DISPATCH_SERVICE(DT_POLICE_AUTOMOBILE_WAIT_CRUISING, toggle);
	ENABLE_DISPATCH_SERVICE(DT_SWAT_HELICOPTER, toggle);
	ENABLE_DISPATCH_SERVICE(DT_POLICE_BOAT, toggle);
	ENABLE_DISPATCH_SERVICE(DT_ARMY_VEHICLE, toggle);
	return;
}

Hash GetCharacterStatHash(const char* statName)
{
	if (statName == nullptr || *statName == '\0')
		return NULL;

	char statBuf[64];
	const char* prefix = "SP1_";
	switch (GET_PED_TYPE(GetPlayerPed()))
	{
	case PEDTYPE_PLAYER1:			// Michael
		prefix = "SP0_";
		break;
	case PEDTYPE_PLAYER2:			// Franklin
		prefix = "SP1_";
		break;
	case PEDTYPE_PLAYER_UNUSED:		// Trevor
		prefix = "SP2_";
		break;
	default:						// return Franklin as default
		prefix = "SP1_";
		break;
	}

	snprintf(statBuf, sizeof(statBuf), "%s%s", prefix, statName);
	return Joaat(statBuf);
}

// head -> 0, upper body -> 1, lower body -> 2, armor -> 3
int GetGeneralDamageFromBoneTag(const int boneTag)
{
	int zone = -1;
	if (boneTag < 0)
		return zone;

	switch (boneTag)
	{
	case BONETAG_ROOT:
	case BONETAG_PELVIS:
	case BONETAG_SPINE:
	case BONETAG_SPINE1:
	case BONETAG_SPINE2:
	case BONETAG_SPINE3:
	case BONETAG_R_CLAVICLE:
	case BONETAG_L_CLAVICLE:
		zone = DZ_ARMOR; break;

	case BONETAG_NECK:
	case BONETAG_HEAD:
		zone = DZ_HEAD; break;

	case BONETAG_R_UPPERARM:
	case BONETAG_R_FOREARM:
	case BONETAG_L_UPPERARM:
	case BONETAG_L_FOREARM:
	case BONETAG_R_HAND: case BONETAG_PH_R_HAND:
	case BONETAG_L_HAND: case BONETAG_PH_L_HAND:
	case BONETAG_R_FINGER0: case BONETAG_R_FINGER01: case BONETAG_R_FINGER02:
	case BONETAG_R_FINGER1: case BONETAG_R_FINGER11: case BONETAG_R_FINGER12:
	case BONETAG_R_FINGER2: case BONETAG_R_FINGER21: case BONETAG_R_FINGER22:
	case BONETAG_R_FINGER3: case BONETAG_R_FINGER31: case BONETAG_R_FINGER32:
	case BONETAG_R_FINGER4: case BONETAG_R_FINGER41: case BONETAG_R_FINGER42:
	case BONETAG_L_FINGER0: case BONETAG_L_FINGER01: case BONETAG_L_FINGER02:
	case BONETAG_L_FINGER1: case BONETAG_L_FINGER11: case BONETAG_L_FINGER12:
	case BONETAG_L_FINGER2: case BONETAG_L_FINGER21: case BONETAG_L_FINGER22:
	case BONETAG_L_FINGER3: case BONETAG_L_FINGER31: case BONETAG_L_FINGER32:
	case BONETAG_L_FINGER4: case BONETAG_L_FINGER41: case BONETAG_L_FINGER42:
		zone = DZ_UPPER_BODY; break;

	case BONETAG_L_THIGH:
	case BONETAG_L_CALF:
	case BONETAG_L_FOOT:
	case BONETAG_L_TOE:
	case BONETAG_R_THIGH:
	case BONETAG_R_CALF:
	case BONETAG_R_FOOT:
	case BONETAG_R_TOE:
		zone = DZ_LOWER_BODY; break;

	default:
		zone = DZ_ARMOR; break;
	}

	return zone;
}

int GetNMPartIndexFromBoneTag(const int boneTag)
{
	int partIndex = -1;
	if (boneTag < 0)
		return partIndex;

	switch (boneTag)
	{
	case BONETAG_ROOT:
	case BONETAG_PELVIS:
		partIndex = RAGDOLL_PELVIS; break;
	case BONETAG_SPINE:
		partIndex = RAGDOLL_SPINE; break;
	case BONETAG_SPINE1:
		partIndex = RAGDOLL_SPINE1; break;
	case BONETAG_SPINE2:
		partIndex = RAGDOLL_SPINE2; break;
	case BONETAG_SPINE3:
		partIndex = RAGDOLL_SPINE3; break;
	case BONETAG_NECK:
		partIndex = RAGDOLL_NECK; break;
	case BONETAG_HEAD:
		partIndex = RAGDOLL_HEAD; break;

	case BONETAG_R_CLAVICLE:
		partIndex = RAGDOLL_CLAVICLE_R; break;
	case BONETAG_R_UPPERARM:
		partIndex = RAGDOLL_UPPERARM_R; break;
	case BONETAG_R_FOREARM:
		partIndex = RAGDOLL_LOWERARM_R; break;

	case BONETAG_R_HAND: case BONETAG_PH_R_HAND:
	case BONETAG_R_FINGER0: case BONETAG_R_FINGER01: case BONETAG_R_FINGER02:
	case BONETAG_R_FINGER1: case BONETAG_R_FINGER11: case BONETAG_R_FINGER12:
	case BONETAG_R_FINGER2: case BONETAG_R_FINGER21: case BONETAG_R_FINGER22:
	case BONETAG_R_FINGER3: case BONETAG_R_FINGER31: case BONETAG_R_FINGER32:
	case BONETAG_R_FINGER4: case BONETAG_R_FINGER41: case BONETAG_R_FINGER42:
		partIndex = RAGDOLL_HAND_R; break;

	case BONETAG_L_CLAVICLE:
		partIndex = RAGDOLL_CLAVICLE_L; break;
	case BONETAG_L_UPPERARM:
		partIndex = RAGDOLL_UPPERARM_L; break;
	case BONETAG_L_FOREARM:
		partIndex = RAGDOLL_LOWERARM_L; break;

	case BONETAG_L_HAND: case BONETAG_PH_L_HAND:
	case BONETAG_L_FINGER0: case BONETAG_L_FINGER01: case BONETAG_L_FINGER02:
	case BONETAG_L_FINGER1: case BONETAG_L_FINGER11: case BONETAG_L_FINGER12:
	case BONETAG_L_FINGER2: case BONETAG_L_FINGER21: case BONETAG_L_FINGER22:
	case BONETAG_L_FINGER3: case BONETAG_L_FINGER31: case BONETAG_L_FINGER32:
	case BONETAG_L_FINGER4: case BONETAG_L_FINGER41: case BONETAG_L_FINGER42:
		partIndex = RAGDOLL_HAND_L; break;

	case BONETAG_L_THIGH:
		partIndex = RAGDOLL_THIGH_L; break;
	case BONETAG_L_CALF:
		partIndex = RAGDOLL_CALF_L; break;
	case BONETAG_L_FOOT:
	case BONETAG_L_TOE:
		partIndex = RAGDOLL_FOOT_L; break;

	case BONETAG_R_THIGH:
		partIndex = RAGDOLL_THIGH_R; break;
	case BONETAG_R_CALF:
		partIndex = RAGDOLL_CALF_R; break;
	case BONETAG_R_FOOT:
	case BONETAG_R_TOE:
		partIndex = RAGDOLL_FOOT_R; break;

	default:
		partIndex = RAGDOLL_PELVIS; break;
	}

	return partIndex;
}

int GetBoneTagFromNMPartIndex(const int partIndex)
{
	int boneTag = -1;
	if (partIndex < 0)
		return boneTag;

	switch (partIndex)
	{
	case RAGDOLL_PELVIS:
		boneTag = BONETAG_PELVIS; break;
	case RAGDOLL_THIGH_L:
		boneTag = BONETAG_L_THIGH; break;
	case RAGDOLL_CALF_L:
		boneTag = BONETAG_L_CALF; break;
	case RAGDOLL_FOOT_L:
		boneTag = BONETAG_L_FOOT; break;
	case RAGDOLL_THIGH_R:
		boneTag = BONETAG_R_THIGH; break;
	case RAGDOLL_CALF_R:
		boneTag = BONETAG_R_CALF; break;
	case RAGDOLL_FOOT_R:
		boneTag = BONETAG_R_FOOT; break;
	case RAGDOLL_SPINE:
		boneTag = BONETAG_SPINE; break;
	case RAGDOLL_SPINE1:
		boneTag = BONETAG_SPINE1; break;
	case RAGDOLL_SPINE2:
		boneTag = BONETAG_SPINE2; break;
	case RAGDOLL_SPINE3:
		boneTag = BONETAG_SPINE3; break;
	case RAGDOLL_CLAVICLE_L:
		boneTag = BONETAG_L_CLAVICLE; break;
	case RAGDOLL_UPPERARM_L:
		boneTag = BONETAG_L_UPPERARM; break;
	case RAGDOLL_LOWERARM_L:
		boneTag = BONETAG_L_FOREARM; break;
	case RAGDOLL_HAND_L:
		boneTag = BONETAG_L_HAND; break;
	case RAGDOLL_CLAVICLE_R:
		boneTag = BONETAG_R_CLAVICLE; break;
	case RAGDOLL_UPPERARM_R:
		boneTag = BONETAG_R_UPPERARM; break;
	case RAGDOLL_LOWERARM_R:
		boneTag = BONETAG_R_FOREARM; break;
	case RAGDOLL_HAND_R:
		boneTag = BONETAG_R_HAND; break;
	case RAGDOLL_NECK:
		boneTag = BONETAG_NECK; break;
	case RAGDOLL_HEAD:
		boneTag = BONETAG_HEAD; break;
	default:
		boneTag = BONETAG_PELVIS; break;
	}

	return boneTag;
}
#pragma endregion

void UpdatePlayerVars()
{
	WeaponDropManager::ResetFrame();

	if (Ini::AllowWeaponsInsideSafeHouse)
	{
		const Ped playerPed = GetPlayerPed();
		if (DOES_ENTITY_EXIST(playerPed))
		{
			playerPedAddress = reinterpret_cast<uintptr_t>(getScriptHandleBaseAddress(playerPed));
			isPlayerInsideSafehouse = IsPlayerInsideSafehouse();
			isPlayerArmed = (GET_SELECTED_PED_WEAPON(playerPed) != WEAPON_UNARMED);
		}
		else
		{
			playerPedAddress = 0;
			isPlayerInsideSafehouse = false;
			isPlayerArmed = false;
		}
	}
	else
	{
		playerPedAddress = 0;
		isPlayerInsideSafehouse = false;
		isPlayerArmed = false;
	}
	return;
}

void ResetSafehouseState()
{
	isPlayerInsideSafehouse = false;
	isPlayerArmed = false;
	playerPedAddress = 0;
}
