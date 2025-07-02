//ScriptHook
#include <natives.h>
// #include <types.h> //Already included in globals.h

//std
#include <sstream>
#include <math.h>
#include <algorithm>
#include <random>
#include <numeric>
#include <set>
#include <fstream>

#include <Psapi.h>

//Custom
#include "functions.h"
#include "keyboard.h"
#include "ini.h"
#include "peds.h"

#pragma region Generic
std::filesystem::path AbsoluteModulePath(HINSTANCE module)
{
	wchar_t path[FILENAME_MAX] = { 0 };
	GetModuleFileNameW(module, path, FILENAME_MAX);
	return std::filesystem::path(path);
}

void SplitString(const char* charStr, std::string arr[], const int arrSize, const bool toUpper)
{
	constexpr char space = 0x20; constexpr char tab = 0x09; constexpr char comma = 0x2C;
	std::string str; str = charStr;
	str.erase(std::remove(str.begin(), str.end(), space), str.end());
	str.erase(std::remove(str.begin(), str.end(), tab), str.end());

	int i = 0;
	std::istringstream ss(str);
	std::string token;
	if (toUpper)
	{
		while (std::getline(ss, token, comma) && i < arrSize)
		{
			std::transform(token.begin(), token.end(), token.begin(), ::toupper);
			arr[i] = token;
			i++;
		}
	}
	else
	{
		while (std::getline(ss, token, comma) && i < arrSize)
		{
			arr[i] = token;
			i++;
		}
	}
	return;
}

int GetRandomIntInRange(int startRange, int endRange)
{
	endRange += 1;
	SET_RANDOM_SEED(GET_GAME_TIMER());
	return GET_RANDOM_INT_IN_RANGE(startRange, endRange);
}

bool GetWeightedBool(int chance)	//Chance out of 100
{
	if (chance <= 0) return false;
	if (chance >= 100) return true;
	const double n = chance * 0.01;
	std::default_random_engine gen(GET_GAME_TIMER());
	std::bernoulli_distribution dist(n);
	return dist(gen);
}

int GetPadControlFromString(const std::string& str)
{
	std::string tmpStr = str;
	std::transform(tmpStr.begin(), tmpStr.end(), tmpStr.begin(), ::toupper);
	auto it = mapPadControls.find(tmpStr);
	if (it != mapPadControls.end())
		return it->second;
	else
		return -1;
}

int GetVKFromString(const std::string& str)
{
	std::string tmpStr = str;
	std::transform(tmpStr.begin(), tmpStr.end(), tmpStr.begin(), ::toupper);
	auto it = mapVKs.find(tmpStr);
	if (it != mapVKs.end())
		return it->second;
	else
		return -1;
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

std::string LoadJSONResource(HINSTANCE hInstance, int resourceID)
{
	HRSRC hRes = FindResource(hInstance, MAKEINTRESOURCE(resourceID), "JSON");
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
	if (!hasWeaponJsonLoaded)
	{
		std::string jsonText = LoadJSONResource(g_hInstance, IDR_WEAPONINFOJSON);
		json j = json::parse(jsonText);
		weaponInfo = j.get<std::vector<WeaponJson>>();
		hasWeaponJsonLoaded = true;
	}
	return hasWeaponJsonLoaded;
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

void Print(std::string string, int ms)
{
	std::string strCpy = string;
	BEGIN_TEXT_COMMAND_PRINT("STRING");
	ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(const_cast<char*>(strCpy.c_str()));
	END_TEXT_COMMAND_PRINT(ms, 1);
	return;
}

void PrintInt(int value, int ms)
{
	BEGIN_TEXT_COMMAND_PRINT("NUMBER");
	ADD_TEXT_COMPONENT_INTEGER(value);
	END_TEXT_COMMAND_PRINT(ms, 1);
	return;
}

void PrintFloat(float value, int ms)
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
bool HasEntityBeenDamagedByWeaponThisFrame(Ped ped, Hash weaponHash, int weaponType)
{
	const bool res = HAS_ENTITY_BEEN_DAMAGED_BY_WEAPON(ped, weaponHash, weaponType);
	if (res)
		CLEAR_ENTITY_LAST_WEAPON_DAMAGE(ped);

	return res;
}

/*
bool HasEntityBeenDamagedByAnyPedThisFrame(Ped ped)
{
	const bool res = HAS_ENTITY_BEEN_DAMAGED_BY_ANY_PED(ped);
	if (res) { CLEAR_ENTITY_LAST_DAMAGE_ENTITY(ped); }
	return res;
}
*/

//Inspired by jedijosh920 implementation of "Disarm"
bool CanDisarmPed(Ped ped, bool includeLeftHand)
{
	int bone = NULL;
	if (!GET_PED_LAST_DAMAGE_BONE(ped, &bone) || bone == NULL)
		return false;

	if (bone == BONETAG_PH_R_HAND || bone == BONETAG_R_HAND)
	{
		CLEAR_PED_LAST_DAMAGE_BONE(ped);
		return true;
	}
	else if (includeLeftHand && (bone == BONETAG_PH_L_HAND || bone == BONETAG_L_HAND))
	{
		CLEAR_PED_LAST_DAMAGE_BONE(ped);
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

bool WillWeaponSpawnPickupWhenDropped(const Hash weaponHash, const bool checkWeaponType)
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
void DropPlayerWeapon(Hash weaponHash, const bool shouldCurse)
{
	while (!LoadWeaponJson()) {}

	if (shouldCurse && !IS_AMBIENT_SPEECH_PLAYING(GetPlayerPed()))
		PLAY_PED_AMBIENT_SPEECH_NATIVE(GetPlayerPed(), "GENERIC_CURSE_MED", "SPEECH_PARAMS_FORCE", false);

	WeaponPickup wp;

	const auto it = std::find(weaponInfo.begin(), weaponInfo.end(), weaponHash);
	if (it != weaponInfo.end())
	{
		if (!it->Components.empty())
		{
			wp.WpHash = weaponHash;
			wp.TintIndex = GET_PED_WEAPON_TINT_INDEX(GetPlayerPed(), weaponHash);
			wp.CamoIndex = GET_PED_WEAPON_CAMO_INDEX(GetPlayerPed(), weaponHash);
			LOOP(i, it->Components.size())
			{
				const Hash component = Joaat(it->Components[i].Name.c_str());
				if (component == Joaat("UNK"))
					continue;

				if (HAS_PED_GOT_WEAPON_COMPONENT(GetPlayerPed(), weaponHash, component))
					wp.Components.push_back(component);
			}
		}

		if (!it->Liveries.empty())
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
	const Vector3 off = GET_PED_BONE_COORDS(GetPlayerPed(), BONETAG_PH_R_HAND, 0.0f, 0.0f, 0.0f);
	SET_LOCAL_PLAYER_PERMITTED_TO_COLLECT_PICKUPS_WITH_MODEL(GET_WEAPONTYPE_MODEL(weaponHash), false);
	wp.PickupIndex = CREATE_PICKUP(GET_PICKUP_TYPE_FROM_WEAPON_HASH(weaponHash), off.x, off.y, off.z, PLACEMENT_FLAG_LOCAL_ONLY, -1, false, NULL);
	wp.PickupBlip = ADD_BLIP_FOR_PICKUP(wp.PickupIndex);
	SET_BLIP_SPRITE(wp.PickupBlip, GetWeaponBlipSprite(weaponHash));
	SET_BLIP_SCALE(wp.PickupBlip, 0.7f);
	droppedWeapons.push_back(wp);
	return;
}

bool retrievedWeaponThisFrame = false;
void RestorePlayerRetrievedWeapon()
{
	while (!LoadWeaponJson()) {}

	if (retrievedWeaponThisFrame || droppedWeapons.empty())
		return;

	//EnablePedConfigFlag(GetPlayerPed(), PCF_BlockAutoSwapOnWeaponPickups);

	LOOP(i, droppedWeapons.size())
	{
		if (!DOES_PICKUP_EXIST(droppedWeapons[i].PickupIndex))
		{
			droppedWeapons.erase(droppedWeapons.begin() + i);
			continue;
		}

		const Vector3 loc1 = GetPlayerCoords(); const Vector3 loc2 = GET_PICKUP_COORDS(droppedWeapons[i].PickupIndex);
		if (VDIST2(loc1.x, loc1.y, loc1.z, loc2.x, loc2.y, loc2.z) > (256.0f * 256.0f))
		{
			REMOVE_PICKUP(droppedWeapons[i].PickupIndex);
			droppedWeapons.erase(droppedWeapons.begin() + i);
			continue;
		}

		// Restore original model appearance
		if (DOES_PICKUP_OBJECT_EXIST(droppedWeapons[i].PickupIndex))
		{
			const Object po = GET_PICKUP_OBJECT(droppedWeapons[i].PickupIndex);
			SET_ACTIVATE_OBJECT_PHYSICS_AS_SOON_AS_IT_IS_UNFROZEN(po, true);
			ACTIVATE_PHYSICS(po);
			FORCE_ACTIVATE_PHYSICS_ON_UNFIXED_PICKUP(po, true);
			SET_ENTITY_DYNAMIC(po, true);
			SET_PICKUP_COLLIDES_WITH_PROJECTILES(po, true);

			if (droppedWeapons[i].TintIndex > -1)
				SET_WEAPON_OBJECT_TINT_INDEX(po, droppedWeapons[i].TintIndex);

			if (droppedWeapons[i].CamoIndex > -1)
				SET_WEAPON_OBJECT_CAMO_INDEX(po, droppedWeapons[i].CamoIndex);

			if (!droppedWeapons[i].Components.empty())
			{
				LOOP(j, droppedWeapons[i].Components.size())
				{
					if (droppedWeapons[i].Components[j] == Joaat("UNK"))
						continue;

					GIVE_WEAPON_COMPONENT_TO_WEAPON_OBJECT(po, droppedWeapons[i].Components[j]);
				}
			}

			if (!droppedWeapons[i].Liveries.empty())
			{
				LOOP(j, droppedWeapons[i].Liveries.size())
				{
					if (droppedWeapons[i].Liveries[j].LiveryHash == Joaat("UNK") ||
						droppedWeapons[i].Liveries[j].TintIndex < 0)
						continue;

					GIVE_WEAPON_COMPONENT_TO_WEAPON_OBJECT(po, droppedWeapons[i].Liveries[j].LiveryHash);
					SET_WEAPON_OBJECT_COMPONENT_TINT_INDEX(po, droppedWeapons[i].Liveries[j].LiveryHash,
						droppedWeapons[i].Liveries[j].TintIndex);
				}
			}
		}

		// Restore ability to pickup weapons after ragdoll has ended
		if (!IS_PED_RAGDOLL(GetPlayerPed()))
			SET_LOCAL_PLAYER_PERMITTED_TO_COLLECT_PICKUPS_WITH_MODEL(GET_WEAPONTYPE_MODEL(droppedWeapons[i].WpHash), true);

		if (!HAS_PICKUP_BEEN_COLLECTED(droppedWeapons[i].PickupIndex))
			continue;

		if (droppedWeapons[i].TintIndex > -1)
			SET_PED_WEAPON_TINT_INDEX(GetPlayerPed(), droppedWeapons[i].WpHash, droppedWeapons[i].TintIndex);

		if (!droppedWeapons[i].Components.empty())
		{
			LOOP(j, droppedWeapons[i].Components.size())
			{
				if (droppedWeapons[i].Components[j] == Joaat("UNK"))
					continue;

				GIVE_WEAPON_COMPONENT_TO_PED(GetPlayerPed(), droppedWeapons[i].WpHash, droppedWeapons[i].Components[j]);
			}
		}

		if (!droppedWeapons[i].Liveries.empty())
		{
			LOOP(j, droppedWeapons[i].Liveries.size())
			{
				if (droppedWeapons[i].Liveries[j].LiveryHash == Joaat("UNK") ||
					droppedWeapons[i].Liveries[j].TintIndex < 0)
					continue;

				GIVE_WEAPON_COMPONENT_TO_PED(GetPlayerPed(), droppedWeapons[i].WpHash, droppedWeapons[i].Liveries[j].LiveryHash);
				SET_PED_WEAPON_COMPONENT_TINT_INDEX(GetPlayerPed(), droppedWeapons[i].WpHash,
					droppedWeapons[i].Liveries[j].LiveryHash,
					droppedWeapons[i].Liveries[j].TintIndex);
			}
		}

		retrievedWeaponThisFrame = true;
		REMOVE_PICKUP(droppedWeapons[i].PickupIndex);
		droppedWeapons.erase(droppedWeapons.begin() + i);
		break;
	}
	return;
}
#pragma endregion

#pragma region Vehicle
Vehicle GetVehiclePedIsIn(Ped ped, bool includeEntering, bool includeExiting)
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

Vehicle GetVehiclePedIsEntering(Ped ped)
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

Vehicle GetVehiclePedIsExiting(Ped ped)
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

Vehicle GetVehiclePedIsEnteringOrExiting(Ped ped)
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
#pragma endregion

#pragma region Misc
bool IsPlayerAiming()
{
	if (IS_PLAYER_FREE_AIMING(GET_PLAYER_INDEX()) || IS_PED_AIMING_FROM_COVER(GetPlayerPed() ||
		GET_PED_CONFIG_FLAG(GetPlayerPed(), PCF_IsAimingGun, false) || GET_PED_RESET_FLAG(GetPlayerPed(), PRF_IsAiming)))
		return true;

	return false;
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
#pragma endregion

#pragma region Memory Patching

#define BUFFER_SIZE 2048

void ClearLog()
{
	std::ofstream ofFile(g_hInstanceLogName, std::ofstream::out | std::ofstream::trunc);
	ofFile.close();
}

void RawLog(const std::string& szInfo, const std::string& szData)
{
	std::ofstream ofFile(g_hInstanceLogName, std::ios_base::out | std::ios_base::app);

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
}

void WriteLog(const char* szInfo, const char* szFormat, ...)
{
	char szBuf[BUFFER_SIZE];
	va_list args;
	va_start(args, szFormat);
	vsnprintf(szBuf, BUFFER_SIZE, szFormat, args);
	va_end(args);
	RawLog(std::string(szInfo), std::string(szBuf));
}

ULONG_PTR FindPattern(const char* szPattern, const char* szMask)
{
	MODULEINFO stInfo;

	if (GetModuleInformation(GetCurrentProcess(),
		GetModuleHandle(NULL), &stInfo, sizeof(MODULEINFO)))
	{
		SIZE_T index = 0;
		ULONG_PTR startAddr = (ULONG_PTR)stInfo.lpBaseOfDll;
		for (SIZE_T i = 0; i < stInfo.SizeOfImage; i++)
		{
			if (*(PBYTE)(startAddr + i) == (BYTE)szPattern[index] ||
				szMask[index] == '?')
			{
				if (szMask[index + 1] == NULL)
					return (ULONG_PTR)((startAddr + i) - (strlen(szMask) - 1));
				index++;
			}
			else
			{
				index = 0;
			}
		}
	}
	else
		WriteLog("Error", "GetModuleInformation() failed! [%d]", GetLastError());

	return 0;
}

// Credits aint-no-other-option: https://github.com/aint-no-other-option/CopBumpSteeringPatch
void CopBumpSteeringPatch(bool& bError)
{
	constexpr int nBytes = 8;

	WriteLog("Operation", "Finding cop bump address...");

	ULONG_PTR address = FindPattern("\x33\xf6\xf3\x0f\x11\x87\x00\x00\x00\x00\x45\x84\xed\x74\x25", "xxxxxx????xxxxx");
	if (address)
	{
		WriteLog("Operation", "Found address! Patching %d bytes...", nBytes);
		memset((void*)(address + 2), 0x90, nBytes);
		WriteLog("Operation", "Done!");
		WriteLog("Info", "If a crash occurs then wrong address got patched. Try again or wait for an update.");
	}
	else
	{
		WriteLog("Error", "Could not find address! Either try again or the game has been updated and the address changed.");
		bError = true;
	}

	WriteLog("Info", "----------------------------------------------------------------");
	return;
}

// Credits aint-no-other-option: https://github.com/aint-no-other-option/CenterSteeringPatch/
void CenterSteeringPatch(bool& bError)
{
	constexpr int nBytes1 = 7;
	constexpr int nBytes2 = 6;

	/* Address of centering when getting out of car normally */
	WriteLog("Operation", "Finding steering address 1...");
	ULONG_PTR address = FindPattern("\x44\x89\xbb\x00\x00\x00\x00\x8b\x0d", "xxx????xx");
	if (address)
	{
		WriteLog("Operation", "Found address! Patching %d bytes...", nBytes1);
		memset((void*)address, 0x90, nBytes1);

		/* Address of centering when diving out */
		WriteLog("Operation", "Finding steering address 2...");
		address = FindPattern("\x89\x82\x00\x00\x00\x00\x38\x81", "xx????xx");
		if (address)
		{
			WriteLog("Operation", "Found address! Patching %d bytes...", nBytes2);
			memset((void*)address, 0x90, nBytes2);
			WriteLog("Operation", "Done!");
			WriteLog("Info", "If a crash occurs then wrong address got patched. Try again or wait for mod update.");
		}
		else
		{
			WriteLog("Error", "Could not find address! Either try again or the game has been updated and the address changed.");
			bError = true;
		}
	}
	else
	{
		WriteLog("Error", "Could not find address! Either try again or the game has been updated and the address changed.");
		bError = true;
	}

	WriteLog("Info", "----------------------------------------------------------------");
	return;
}

bool hasAppliedMemPatches = false;
void ApplyMemPatches()
{
	if (hasAppliedMemPatches)
		return;

	bool bError = false;
	ClearLog();
	WriteLog("Started", "%s v%d.%d", g_hInstanceName.c_str(), VER_MAX, VER_MIN);
	WriteLog("Info", "----------------------------------------------------------------");

	CopBumpSteeringPatch(bError);
	CenterSteeringPatch(bError);

	if (bError)
		WriteLog("Finished", "Error patching addresses.");
	else
		WriteLog("Finished", "Patching complete.");

	hasAppliedMemPatches = true;
	return;
}
#pragma endregion

void UpdatePlayerVars()
{
	retrievedWeaponThisFrame = false;
	return;
}