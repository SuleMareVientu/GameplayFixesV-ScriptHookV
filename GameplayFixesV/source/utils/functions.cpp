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
	
	return -1;
}

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
}

void RawLog(const std::string& szInfo, const std::string& szData)
{
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
}

bool hasWrittenToLog = false;
constexpr int logBufferSize = 2048;
void WriteLog(const char* szInfo, const char* szFormat, ...)
{
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
		std::string jsonText = LoadJSONResource(GetDllInstance(), IDR_WEAPONINFOJSON);
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

			if (GetGameVersion() >= VER_1_0_1103_2_STEAM)
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
	const Vector3 off = GET_PED_BONE_COORDS(GetPlayerPed(), BONETAG_PH_R_HAND, 0.0f, 0.0f, 0.0f);
	SET_LOCAL_PLAYER_PERMITTED_TO_COLLECT_PICKUPS_WITH_MODEL(GET_WEAPONTYPE_MODEL(weaponHash), false);

	if (GetGameVersion() >= VER_1_0_1290_1_STEAM)
		wp.PickupIndex = CREATE_PICKUP(GET_PICKUP_TYPE_FROM_WEAPON_HASH(weaponHash), off.x, off.y, off.z, PLACEMENT_FLAG_LOCAL_ONLY, -1, false, NULL);
	else
		wp.PickupIndex = CREATE_PICKUP(GetPickupTypeFromWeaponModel(GET_WEAPONTYPE_MODEL(weaponHash)), off.x, off.y, off.z, PLACEMENT_FLAG_LOCAL_ONLY, -1, false, NULL);
	
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
			// FORCE_ACTIVATE_PHYSICS_ON_UNFIXED_PICKUP(po, true); // DO NOT ENABLE, requires b1180
			SET_ENTITY_DYNAMIC(po, true);
			// SET_PICKUP_COLLIDES_WITH_PROJECTILES(po, true);	// DO NOT ENABLE, requires b678

			if (droppedWeapons[i].TintIndex > -1)
				SET_WEAPON_OBJECT_TINT_INDEX(po, droppedWeapons[i].TintIndex);

			if (GetGameVersion() >= VER_1_0_1103_2_STEAM)
			{
				if (droppedWeapons[i].CamoIndex > -1)
					SET_WEAPON_OBJECT_CAMO_INDEX(po, droppedWeapons[i].CamoIndex);
			}

			if (!droppedWeapons[i].Components.empty())
			{
				LOOP(j, droppedWeapons[i].Components.size())
				{
					if (droppedWeapons[i].Components[j] == Joaat("UNK"))
						continue;

					GIVE_WEAPON_COMPONENT_TO_WEAPON_OBJECT(po, droppedWeapons[i].Components[j]);
				}
			}

			if (!droppedWeapons[i].Liveries.empty() && GetGameVersion() >= VER_1_0_1103_2_STEAM)
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

		if (!droppedWeapons[i].Liveries.empty() && GetGameVersion() >= VER_1_0_1103_2_STEAM)
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
int minimapScaleformIndex = NULL;
int RequestMinimapScaleform()
{
	if (!HAS_SCALEFORM_MOVIE_LOADED(minimapScaleformIndex))
	{
		minimapScaleformIndex = REQUEST_SCALEFORM_MOVIE("MINIMAP");
		CALL_SCALEFORM_MOVIE_METHOD(minimapScaleformIndex, "INITIALISE");
		return NULL;
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
bool IsPlayerAiming()
{
	if (IS_PLAYER_FREE_AIMING(GET_PLAYER_INDEX()) || IS_PED_AIMING_FROM_COVER(GetPlayerPed() ||
		GET_PED_CONFIG_FLAG(GetPlayerPed(), PCF_IsAimingGun, false) || GET_PED_RESET_FLAG(GetPlayerPed(), PRF_IsAiming)))
		return true;

	return false;
}

bool IsPlayerInsideSafehouse()
{
	const Vector3 tmpCoords = GetPlayerCoords();
	constexpr int arrSize = 5;
	int safehouses[arrSize] = {
		GET_INTERIOR_AT_COORDS_WITH_TYPE(tmpCoords.x, tmpCoords.y, tmpCoords.z, "v_franklins"),
		GET_INTERIOR_AT_COORDS_WITH_TYPE(tmpCoords.x, tmpCoords.y, tmpCoords.z, "v_franklinshouse"),
		GET_INTERIOR_AT_COORDS_WITH_TYPE(tmpCoords.x, tmpCoords.y, tmpCoords.z, "v_michael"),
		GET_INTERIOR_AT_COORDS_WITH_TYPE(tmpCoords.x, tmpCoords.y, tmpCoords.z, "v_trailer"),
		GET_INTERIOR_AT_COORDS_WITH_TYPE(tmpCoords.x, tmpCoords.y, tmpCoords.z, "v_trevors")
	};

	int playerInterior = GET_INTERIOR_FROM_ENTITY(GetPlayerPed());
	LOOP(i, arrSize)
	{
		if (safehouses[i] == playerInterior)
			return true;
	}

	//Special check for Trevor's office inside the strip club
	if (GET_ROOM_KEY_FROM_ENTITY(GetPlayerPed()) == strp3off)	//room key for "strp3off"
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

bool isFakeWanted = false;
bool GetFakeWanted() { return isFakeWanted; }
void SetFakeWanted(Player player, bool toggle)
{
	Vector3 fakeCoords{ 7000.0f, 7000.0f, 0.0f};
	switch (toggle)
	{
	case true:
		SET_MAX_WANTED_LEVEL(5);	//Fix for Menyoo never wanted
		SET_PLAYER_WANTED_LEVEL_NO_DROP(GetPlayer(), 1, true);
		SET_PLAYER_WANTED_LEVEL_NOW(GetPlayer(), true);

		//Various ignore player commands
		SET_PLAYER_WANTED_CENTRE_POSITION(GetPlayer(), &fakeCoords);
		SET_POLICE_IGNORE_PLAYER(GetPlayer(), true);
		SetDispatchServices(false);
		SET_DISPATCH_COPS_FOR_PLAYER(GetPlayer(), false);
		SET_IGNORE_LOW_PRIORITY_SHOCKING_EVENTS(GetPlayer(), true);
		SET_WANTED_LEVEL_DIFFICULTY(GetPlayer(), 0.001f);
		SET_WANTED_LEVEL_MULTIPLIER(0.0f);
		SET_AUDIO_FLAG("PoliceScannerDisabled", true);
		SET_BLOCK_WANTED_FLASH(true);
		FORCE_OFF_WANTED_STAR_FLASH(true);
		HIDE_HUD_COMPONENT_THIS_FRAME(HUD_WANTED_STARS);
		isFakeWanted = true;
		break;
	case false:
		if (GET_PLAYER_WANTED_LEVEL(GetPlayer()) != 0)
			CLEAR_PLAYER_WANTED_LEVEL(GetPlayer());

		//Reset ignore player commands
		SET_POLICE_IGNORE_PLAYER(GetPlayer(), false);
		SetDispatchServices(true);
		SET_DISPATCH_COPS_FOR_PLAYER(GetPlayer(), true);
		SET_IGNORE_LOW_PRIORITY_SHOCKING_EVENTS(GetPlayer(), false);
		RESET_WANTED_LEVEL_DIFFICULTY(GetPlayer());
		SET_WANTED_LEVEL_MULTIPLIER(1.0f);
		SET_AUDIO_FLAG("PoliceScannerDisabled", false);
		SET_BLOCK_WANTED_FLASH(false);
		FORCE_OFF_WANTED_STAR_FLASH(false);
		HIDE_HUD_COMPONENT_THIS_FRAME(HUD_WANTED_STARS);
		isFakeWanted = false;
		break;
	}
	return;
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
#pragma endregion

void UpdatePlayerVars()
{
	retrievedWeaponThisFrame = false;
	return;
}
