//ScriptHook
#include <natives.h>
// #include <types.h> //Already included in globals.h
//Custom
#include "functions.h"
#include <random>
#include <string>
#include <sstream>
#include <algorithm>
#include <math.h>
#include <numeric>
#include "keyboard.h"
#include "ini.h"
#include "peds.h"

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

#pragma region Print
void Print(char* string, int ms)
{
	BEGIN_TEXT_COMMAND_PRINT("STRING");
	ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(string);
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

#pragma endregion

#pragma region Weapons
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

	DWORD size = SizeofResource(hInstance, hRes);
	const char* data = static_cast<const char*>(LockResource(hData));
	if (!data) throw std::runtime_error("Failed to lock resource");

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
	while (!LoadWeaponJson()){}

	if (shouldCurse && !IS_AMBIENT_SPEECH_PLAYING(GetPlayerPed()))
		PLAY_PED_AMBIENT_SPEECH_NATIVE(GetPlayerPed(), "GENERIC_CURSE_MED", "SPEECH_PARAMS_FORCE", false);

	WeaponPickup wp;
	LOOP(i, weaponInfo.size())
	{
		if (Joaat(weaponInfo[i].Name.c_str()) != weaponHash)
			continue;

		if (!weaponInfo[i].Components.empty())
		{
			wp.WpHash = weaponHash;
			wp.TintIndex = GET_PED_WEAPON_TINT_INDEX(GetPlayerPed(), weaponHash);
			wp.CamoIndex = GET_PED_WEAPON_CAMO_INDEX(GetPlayerPed(), weaponHash);
			LOOP(j, weaponInfo[i].Components.size())
			{
				const Hash component = Joaat(weaponInfo[i].Components[j].Name.c_str());
				if (component == Joaat("UNK"))
					continue;

				if (HAS_PED_GOT_WEAPON_COMPONENT(GetPlayerPed(), weaponHash, component))
					wp.Components.push_back(component);
			}
		}

		if (!weaponInfo[i].Liveries.empty())
		{
			LOOP(j, weaponInfo[i].Liveries.size())
			{
				const Hash livery = Joaat(weaponInfo[i].Liveries[j].Name.c_str());
				if (livery == Joaat("UNK"))
					continue;
				
				const int liveryTint = GET_PED_WEAPON_COMPONENT_TINT_INDEX(GetPlayerPed(), weaponHash, livery);
				if (liveryTint > -1)
					wp.Liveries.push_back(WeaponPickupLivery{ livery, liveryTint });
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

#pragma region Misc
inline int Abs(int n) { return n * ((n > 0) - (n < 0)); }
inline float Abs(float n) { return n * ((n > 0.0f) - (n < 0.0f)); }

bool ArrayContains(int value, const int a[], int n)
{
	int i = 0;
	while (i < n && a[i] != value) ++i;
	return i == n ? false : true;
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

Vehicle GetVehiclePedIsIn(Ped ped, bool includeEntering, bool includeExiting)
{
	Vehicle veh = GET_VEHICLE_PED_IS_USING(ped);
	if ((!includeEntering && GET_PED_RESET_FLAG(ped, PRF_IsEnteringVehicle)) ||
		(!includeExiting && GET_IS_TASK_ACTIVE(ped, CODE_TASK_EXIT_VEHICLE)))
	{ veh = NULL; }

	if (!DOES_ENTITY_EXIST(veh))
		veh = NULL;

	return veh;
}

Vehicle GetVehiclePedIsEntering(Ped ped)
{
	Vehicle veh = NULL;
	if (GET_PED_RESET_FLAG(ped, PRF_IsEnteringVehicle)) veh = GET_VEHICLE_PED_IS_USING(ped);
	if (!DOES_ENTITY_EXIST(veh))
		veh = NULL;

	return veh;
}

Vehicle GetVehiclePedIsExiting(Ped ped)
{
	Vehicle veh = NULL;
	if (GET_IS_TASK_ACTIVE(ped, CODE_TASK_EXIT_VEHICLE)) veh = GET_VEHICLE_PED_IS_USING(ped);
	if (!DOES_ENTITY_EXIST(veh))
		veh = NULL;

	return veh;
}

Vehicle GetVehiclePedIsEnteringOrExiting(Ped ped)
{
	Vehicle veh = NULL;
	if (GET_PED_RESET_FLAG(ped, PRF_IsEnteringVehicle) || GET_IS_TASK_ACTIVE(ped, CODE_TASK_EXIT_VEHICLE))
		veh = GET_VEHICLE_PED_IS_USING(ped);
	
	if (!DOES_ENTITY_EXIST(veh)) veh = NULL;
	return veh;
}

bool IsPlayerAiming()
{
	if (IS_PLAYER_FREE_AIMING(GET_PLAYER_INDEX()) || IS_PED_AIMING_FROM_COVER(GetPlayerPed() ||
		GET_PED_CONFIG_FLAG(GetPlayerPed(), PCF_IsAimingGun, false) || GET_PED_RESET_FLAG(GetPlayerPed(), PRF_IsAiming)))
		return true;

	return false;
}

/*
bool HasEntityBeenDamagedByAnyPedThisFrame(Ped ped)
{
	const bool res = HAS_ENTITY_BEEN_DAMAGED_BY_ANY_PED(ped);
	if (res) { CLEAR_ENTITY_LAST_DAMAGE_ENTITY(ped); }
	return res;
}
*/

bool HasEntityBeenDamagedByWeaponThisFrame(Ped ped, Hash weaponHash, int weaponType)
{
	const bool res = HAS_ENTITY_BEEN_DAMAGED_BY_WEAPON(ped, weaponHash, weaponType);
	if (res)
		CLEAR_ENTITY_LAST_WEAPON_DAMAGE(ped);

	return res;
}

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

/*
static bool RequestScaleform(const char* name, int* handle)
{
	if (HAS_SCALEFORM_MOVIE_LOADED(*handle))
		return true;

	*handle = REQUEST_SCALEFORM_MOVIE(name);
	return false;
}
*/

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

int GetPadControlFromString(const std::string &str)
{
	std::string tmpStr = str;
	std::transform(tmpStr.begin(), tmpStr.end(), tmpStr.begin(), ::toupper);
	auto it = mapPadControls.find(tmpStr);
	if (it != mapPadControls.end())
		return it->second;
	else
		return -1;
}

int GetVKFromString(const std::string &str)
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

/////////////////////////////////////////////Player//////////////////////////////////////////////
namespace nGeneral
{
bool CanCrouch(Ped ped)
{
	if (!DOES_ENTITY_EXIST(ped) || IS_ENTITY_DEAD(ped, false) || IS_PED_DEAD_OR_DYING(ped, true) ||
		IS_PED_INJURED(ped) || IS_PED_USING_ANY_SCENARIO(ped) || IS_PED_RAGDOLL(ped) ||
		IS_PED_GETTING_UP(ped) || IS_PED_FALLING(ped) || IS_PED_JUMPING(ped) ||
		IS_PED_DIVING(ped) || IS_PED_SWIMMING(ped) || IS_PED_GOING_INTO_COVER(ped) ||
		IS_PED_CLIMBING(ped) || IS_PED_VAULTING(ped) || IS_PED_HANGING_ON_TO_VEHICLE(ped) ||
		IS_PED_IN_ANY_VEHICLE(ped, true) || IS_PED_IN_COVER(ped, false) || !IS_PED_ON_FOOT(ped) ||
		IS_PED_TAKING_OFF_HELMET(ped) || GET_ENTITY_SUBMERGED_LEVEL(ped) >= 0.7f || IS_PED_PERFORMING_MELEE_ACTION(ped))
		return false;

	return true;
}

// Ensure that movement clipsets are loaded before applying them
constexpr char* crouchedMovementClipSet = "move_ped_crouched";
constexpr char* crouchedStrafingClipSet = "move_ped_crouched_strafing";
constexpr float blendSpeedCrouched = 0.55f;
bool isCrouched = false;
void SetCrouch(Ped ped, bool state)
{
	if (state)
	{
		isCrouched = true;
		SET_PED_STEALTH_MOVEMENT(ped, false, NULL);
		SET_PED_MOVEMENT_CLIPSET(ped, crouchedMovementClipSet, blendSpeedCrouched);
		SET_PED_STRAFE_CLIPSET(ped, crouchedStrafingClipSet);
		//SET_WEAPON_ANIMATION_OVERRIDE(ped, "Ballistic");
		DisablePedConfigFlag(ped, PCF_OpenDoorArmIK);
		EnablePedConfigFlag(ped, PCF_PhoneDisableTextingAnimations);
		EnablePedConfigFlag(ped, PCF_PhoneDisableTalkingAnimations);
		EnablePedConfigFlag(ped, PCF_PhoneDisableCameraAnimations);
		SET_PED_CAN_PLAY_GESTURE_ANIMS(ped, false);
		SET_PED_CAN_PLAY_AMBIENT_ANIMS(ped, false);
		SET_PED_CAN_PLAY_AMBIENT_BASE_ANIMS(ped, false);
		SET_PED_CAN_PLAY_AMBIENT_IDLES(ped, true, true);
	}
	else
	{
		isCrouched = false;
		SET_PED_STEALTH_MOVEMENT(ped, false, NULL);
		RESET_PED_MOVEMENT_CLIPSET(ped, blendSpeedCrouched);
		RESET_PED_STRAFE_CLIPSET(ped);
		//RESET_PED_WEAPON_MOVEMENT_CLIPSET(ped);
		//SET_WEAPON_ANIMATION_OVERRIDE(ped, "DEFAULT");
		SET_PED_MAX_MOVE_BLEND_RATIO(ped, PEDMOVEBLENDRATIO_SPRINT);
		EnablePedConfigFlag(ped, PCF_OpenDoorArmIK);
		DisablePedConfigFlag(ped, PCF_PhoneDisableTextingAnimations);
		DisablePedConfigFlag(ped, PCF_PhoneDisableTalkingAnimations);
		DisablePedConfigFlag(ped, PCF_PhoneDisableCameraAnimations);
		SET_PED_CAN_PLAY_GESTURE_ANIMS(ped, true);
		SET_PED_CAN_PLAY_AMBIENT_ANIMS(ped, true);
		SET_PED_CAN_PLAY_AMBIENT_BASE_ANIMS(ped, true);
		SET_PED_CAN_PLAY_AMBIENT_IDLES(ped, false, false);
	}
	return;
}

Timer timerCrouch;
constexpr int crouchHold = 250;
void EnableCrouching()
{
	if (!RequestClipSet(crouchedMovementClipSet) || !RequestClipSet(crouchedStrafingClipSet))
		return;

	if (IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_DUCK))
	{
		if (isCrouched)
			SetCrouch(GetPlayerPed(), false);
		else
			timerCrouch.Reset();
	}
	else if (IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_DUCK) && timerCrouch.Get() > crouchHold)
	{
		timerCrouch.Set(INT_MIN);
		if (!isCrouched && CanCrouch(GetPlayerPed()))
			SetCrouch(GetPlayerPed(), true);
	}

	if (isCrouched)
	{
		if (!CanCrouch(GetPlayerPed()))
			SetCrouch(GetPlayerPed(), false);
		else
		{
			EnablePedResetFlag(GetPlayerPed(), PRF_DisableActionMode);
			EnablePedResetFlag(GetPlayerPed(), PRF_DontUseSprintEnergy);
			DISABLE_ON_FOOT_FIRST_PERSON_VIEW_THIS_UPDATE();

			if (IsPlayerAiming())
				SET_PED_MAX_MOVE_BLEND_RATIO(GetPlayerPed(), 0.25f);
			else
				SET_PED_MAX_MOVE_BLEND_RATIO(GetPlayerPed(), PEDMOVEBLENDRATIO_RUN);
		}
	}
	return;
}

void FriendlyFire()
{
	SET_CAN_ATTACK_FRIENDLY(GetPlayerPed(), true, true);	// Sets PCF_CanAttackFriendly, PCF_AllowLockonToFriendlyPlayers

	Ped ped = NULL;
	SET_SCENARIO_PEDS_TO_BE_RETURNED_BY_NEXT_COMMAND(true);
	const Vector3 tmpCoords = GetPlayerCoords();
	if (!GET_CLOSEST_PED(tmpCoords.x, tmpCoords.y, tmpCoords.z, 10.0f, true, true, &ped, false, true, -1))
		return;

	int relationship = GET_RELATIONSHIP_BETWEEN_PEDS(GetPlayerPed(), ped);
	if (relationship == ACQUAINTANCE_TYPE_PED_RESPECT || relationship == ACQUAINTANCE_TYPE_PED_LIKE)
	{
		SET_PED_CAN_BE_TARGETTED(ped, true);
		SET_PED_CAN_BE_TARGETTED_BY_PLAYER(ped, GetPlayer(), true);
		SET_PED_CAN_BE_TARGETED_WHEN_INJURED(ped, true);
		SET_ALLOW_LOCKON_TO_PED_IF_FRIENDLY(ped, true);
		SET_PED_CAN_RAGDOLL(ped, true);
		CLEAR_RAGDOLL_BLOCKING_FLAGS(ped, RBF_MELEE);
		CLEAR_RAGDOLL_BLOCKING_FLAGS(ped, RBF_PLAYER_IMPACT);
		//CLEAR_RAGDOLL_BLOCKING_FLAGS(ped, RBF_BULLET_IMPACT);
		SET_PED_DIES_WHEN_INJURED(ped, true);
		SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, false);
		DisablePedConfigFlag(ped, PCF_PreventAllMeleeTaunts);
		DisablePedConfigFlag(ped, PCF_NeverEverTargetThisPed);
		EnablePedConfigFlag(ped, PCF_AllowPlayerLockOnIfFriendly);
	}
	return;
}

void EnableStealthForAllPeds()
{
	if (!IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_DUCK))
		return;

	switch (GET_ENTITY_MODEL(GetPlayerPed()))
	{
	case FranklinPed:
	case MichaelPed:
	case TrevorPed:
		return;
	default:
		if (IS_PED_ON_FOOT(GetPlayerPed()) && !IS_PED_RAGDOLL(GetPlayerPed()) && !IS_PED_CLIMBING(GetPlayerPed()) &&
			!IS_PED_DIVING(GetPlayerPed()) && !IS_PED_SWIMMING(GetPlayerPed()) &&
			GET_PED_STEALTH_MOVEMENT(GetPlayerPed())) // Not an error, GET_PED_STEALTH_MOVEMENT needs to return true here
		{
			SET_PED_STEALTH_MOVEMENT(GetPlayerPed(), true, "DEFAULT_ACTION");
		}
		return;
	}
	return;
}

inline void DisableActionMode() { EnablePedResetFlag(GetPlayerPed(), PRF_DisableActionMode); return; }

void DisarmPlayerWhenShot()
{
	RestorePlayerRetrievedWeapon();

	if ((CanDisarmPed(GetPlayerPed(), true) && GetWeightedBool(70)) ||
		HasEntityBeenDamagedByWeaponThisFrame(GetPlayerPed(), WEAPON_STUNGUN, GENERALWEAPON_TYPE_INVALID))
	{
		Hash wp = NULL; GET_CURRENT_PED_WEAPON(GetPlayerPed(), &wp, false);
		if (WillWeaponSpawnPickupWhenDropped(wp, true))
			DropPlayerWeapon(wp, true);
	}
	return;
}

Hash lastRagdollWp = NULL;
void DropPlayerWeaponWhenRagdolling()
{
	RestorePlayerRetrievedWeapon();

	Hash tmp = NULL;
	const bool res = GET_CURRENT_PED_WEAPON(GetPlayerPed(), &tmp, false);

	if (tmp == lastRagdollWp)
	{
		if (IS_PED_RAGDOLL(GetPlayerPed()) && WillWeaponSpawnPickupWhenDropped(lastRagdollWp, true))
			DropPlayerWeapon(lastRagdollWp, false);
	}

	// GET_CURRENT_PED_WEAPON returns true when a weapon is usable (i.e. in their hand). we do this to check if
	// the player was holding the gun correctly
	if (res)
		lastRagdollWp = tmp;
	else
		lastRagdollWp = NULL;

	return;
}

constexpr int timeClearDirtDecal = 120000;	//2min
Timer timerDirtDecal(timeClearDirtDecal);
int oldHealthWounds = 0;
void DynamicallyCleanWoundsAndDirt()
{
	if (IS_PLAYER_SWITCH_IN_PROGRESS())
	{
		timerDirtDecal.Set(120000);
		return;
	}	

	const float subLevel = GET_ENTITY_SUBMERGED_LEVEL(GetPlayerPed());
	if (subLevel >= 0.3f)
	{
		CLEAR_PED_BLOOD_DAMAGE_BY_ZONE(GetPlayerPed(), PDZ_RIGHT_LEG);
		CLEAR_PED_BLOOD_DAMAGE_BY_ZONE(GetPlayerPed(), PDZ_LEFT_LEG);
	}

	if (subLevel >= 0.7f || IS_PED_SWIMMING(GetPlayerPed()))
	{
		timerDirtDecal.Reset();
		CLEAR_PED_BLOOD_DAMAGE_BY_ZONE(GetPlayerPed(), PDZ_TORSO);
		CLEAR_PED_BLOOD_DAMAGE_BY_ZONE(GetPlayerPed(), PDZ_RIGHT_ARM);
		CLEAR_PED_BLOOD_DAMAGE_BY_ZONE(GetPlayerPed(), PDZ_LEFT_ARM);
		CLEAR_PED_BLOOD_DAMAGE_BY_ZONE(GetPlayerPed(), PDZ_MEDALS);
	}

	if (subLevel > 0.9f)
		CLEAR_PED_BLOOD_DAMAGE_BY_ZONE(GetPlayerPed(), PDZ_HEAD);

	if (subLevel == 1.0f || IS_PED_SWIMMING_UNDER_WATER(GetPlayerPed()))
		CLEAR_PED_BLOOD_DAMAGE(GetPlayerPed());

	// Also clean when player picks up a medkit
	const int health = GET_ENTITY_HEALTH(GetPlayerPed());
	if (health > oldHealthWounds && health == GET_ENTITY_MAX_HEALTH(GetPlayerPed()))
	{
		timerDirtDecal.Reset();
		CLEAR_PED_BLOOD_DAMAGE(GetPlayerPed());
	}

	// Dirt ad oldHealth should always be at the bottom
	oldHealthWounds = health;
	if (timerDirtDecal.Get() <= timeClearDirtDecal)
	{
		if (timerDirtDecal.Get() <= (timeClearDirtDecal / 2))
			SET_PED_SWEAT(GetPlayerPed(), 0.0f);

		CLEAR_PED_ENV_DIRT(GetPlayerPed());	//Must be called every frame
		//CLEAR_PED_DAMAGE_DECAL_BY_ZONE(GetPlayerPed(), PDZ_TORSO, "basic_dirt_cloth");
		//CLEAR_PED_DAMAGE_DECAL_BY_ZONE(GetPlayerPed(), PDZ_TORSO, "basic_dirt_skin");
	}
	return;
}

inline void EnableSprintInsideInteriors() { EnablePedConfigFlag(GetPlayerPed(), PCF_IgnoreInteriorCheckForSprinting); return; }

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
void SetFakeWanted(Player player, bool toggle)
{
	Vector3 fakeCoords{ 7000.0f, NULL, 7000.0f, NULL, 0.0f, NULL };
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

//This approach isn't great, and could cause a lot of issues with the game's story... Too bad
//MUST BE CALLED EVERY FRAME
//A different and more aggressive approach would be to terminate these scripts, which are responsible for the family scenes
//inside the safehouses and disable the player's control: family_scene_f0, family_scene_f1, family_scene_m, family_scene_t0, family_scene_t1
Weapon lastPlayerWeapon = NULL;
Timer timerLastPlayerWeapon;
Timer timerCharacterSwitch;
void AllowWeaponsInsideSafeHouse()
{
	//Force selected player weapon for 1000ms upon exit/enter of safehouse
	if (timerLastPlayerWeapon.Get() < 1000 && lastPlayerWeapon != WEAPON_UNARMED)
		SET_CURRENT_PED_WEAPON(GetPlayerPed(), lastPlayerWeapon, true);

	//Checks if player is inside valid interior
	int playerInterior = GET_INTERIOR_FROM_ENTITY(GetPlayerPed());
	if (!IS_VALID_INTERIOR(playerInterior) ||
		//reset fakewanted also if player is inside the strip club but not inside the office
		(GET_INTERIOR_AT_COORDS(113.53f, -1287.61f, 28.64f) == playerInterior /*v_strip3*/ && GET_ROOM_KEY_FROM_ENTITY(GetPlayerPed()) != strp3off))
	{
		if (isFakeWanted)
		{
			timerLastPlayerWeapon.Reset();
			SetFakeWanted(GetPlayer(), false);
		}
		lastPlayerWeapon = GET_SELECTED_PED_WEAPON(GetPlayerPed());
		return;
	}

	//Check if player is inside a safehouse, and not just in a random interior and return if player was already wanted before entering the interior
	if (!IsPlayerInsideSafehouse() || (!isFakeWanted && GET_PLAYER_WANTED_LEVEL(GetPlayer()) != 0))
		return;

	//Allow player to switch characters while inside the safehouse
	if (IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_CHARACTER_WHEEL) ||
		IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_SELECT_CHARACTER_MICHAEL) ||
		IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_SELECT_CHARACTER_FRANKLIN) ||
		IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_SELECT_CHARACTER_TREVOR))
	{
		timerCharacterSwitch.Reset();
	}
	else if (timerCharacterSwitch.Get() > 250 &&
		(IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_CHARACTER_WHEEL) ||
			IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_SELECT_CHARACTER_MICHAEL) ||
			IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_SELECT_CHARACTER_FRANKLIN) ||
			IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_SELECT_CHARACTER_TREVOR)))
	{
		if (isFakeWanted)
			SetFakeWanted(GetPlayer(), false);

		timerLastPlayerWeapon.Reset();
		return;
	}

	//Reset weapon timer
	if (!isFakeWanted)
		timerLastPlayerWeapon.Reset();
	else
		lastPlayerWeapon = GET_SELECTED_PED_WEAPON(GetPlayerPed());

	//If all checks pass, set fake wanted level
	SetFakeWanted(GetPlayer(), true);
	return;
}

void SilentWanted()
{
	SET_POLICE_RADAR_BLIPS(false);
	SET_BLOCK_WANTED_FLASH(true);
	FORCE_OFF_WANTED_STAR_FLASH(true);
	HIDE_HUD_COMPONENT_THIS_FRAME(HUD_WANTED_STARS);

	SET_AUDIO_FLAG("WantedMusicDisabled", true);
	SET_AUDIO_FLAG("WantedMusicOnMission", false);
	IS_PED_IN_ANY_POLICE_VEHICLE(GetPlayerPed()) ? SET_AUDIO_FLAG("PoliceScannerDisabled", false) : SET_AUDIO_FLAG("PoliceScannerDisabled", true);
	return;
}
}	//END nGeneral

/////////////////////////////////////////Player Controls/////////////////////////////////////////
namespace nControls
{
inline void DisableAssistedMovement() { ASSISTED_MOVEMENT_OVERRIDE_LOAD_DISTANCE_THIS_FRAME(NULL); return; }

bool isWalking = false;
float playerLastMoveBlend = 0.0f;
Timer timerToggleFPSWalking;
void ToggleFPSWalking()
{
	if (!IS_PED_ON_FOOT(GetPlayerPed()) || !IS_CONTROL_ENABLED(PLAYER_CONTROL, INPUT_SPRINT) ||
		GET_FOLLOW_PED_CAM_VIEW_MODE() != CAM_VIEW_MODE_FIRST_PERSON)
	{
		if (isWalking)
		{
			isWalking = false;
			SET_PED_MAX_MOVE_BLEND_RATIO(GetPlayerPed(), PEDMOVEBLENDRATIO_SPRINT);
		}
		return;
	}

	//Walk Toggle
	if (IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_SPRINT))
	{
		timerToggleFPSWalking.Reset();
		playerLastMoveBlend = GET_PED_DESIRED_MOVE_BLEND_RATIO(GetPlayerPed());
		SET_PED_MAX_MOVE_BLEND_RATIO(GetPlayerPed(), playerLastMoveBlend);
	}
	else if (timerToggleFPSWalking.Get() < 250)
	{
		if (IS_CONTROL_JUST_RELEASED(PLAYER_CONTROL, INPUT_SPRINT))
		{
			if (isWalking)
			{
				SET_PED_MAX_MOVE_BLEND_RATIO(GetPlayerPed(), PEDMOVEBLENDRATIO_SPRINT);
				isWalking = false;
			}
			else
			{
				SET_PED_MAX_MOVE_BLEND_RATIO(GetPlayerPed(), PEDMOVEBLENDRATIO_WALK);
				isWalking = true;
			}
		}
		else
			SET_PED_MAX_MOVE_BLEND_RATIO(GetPlayerPed(), playerLastMoveBlend);
	}
	else if (IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_SPRINT))
	{
		isWalking = false;
		SET_PED_MAX_MOVE_BLEND_RATIO(GetPlayerPed(), PEDMOVEBLENDRATIO_SPRINT);
	}
	else if (isWalking)
		SET_PED_MAX_MOVE_BLEND_RATIO(GetPlayerPed(), PEDMOVEBLENDRATIO_WALK);
	return;
}

void DisableCameraAutoCenter()
{
	switch (INI::DisableCameraAutoCenter)
	{
	case 1:
		if (IS_PED_ON_FOOT(GetPlayerPed()))
			SET_FOLLOW_CAM_IGNORE_ATTACH_PARENT_MOVEMENT_THIS_UPDATE();
		break;
	case 2:
		if (GetVehiclePedIsIn(GetPlayerPed(), true, true) != NULL)
			SET_FOLLOW_CAM_IGNORE_ATTACH_PARENT_MOVEMENT_THIS_UPDATE();
		break;
	case 3:
		SET_FOLLOW_CAM_IGNORE_ATTACH_PARENT_MOVEMENT_THIS_UPDATE();
		break;
	}
	return;
}

float handbrakeCamHeadingMin = 0.0f;
float handbrakeCamHeadingMax = 0.0f;
void SetCamSmoothHeadingLimit()
{
	const float frametimeMultiplier = TIMESTEP() * 275.0f;
	const float heading = GET_GAMEPLAY_CAM_RELATIVE_HEADING();
	if (heading > 0.0f)
	{
		handbrakeCamHeadingMax = heading - frametimeMultiplier;
		if (handbrakeCamHeadingMax < 0.0f)
			handbrakeCamHeadingMax = 0.0f;

		handbrakeCamHeadingMin = 0.0f;
	}
	else
	{
		handbrakeCamHeadingMin = heading + frametimeMultiplier;
		if (handbrakeCamHeadingMin > 0.0f)
			handbrakeCamHeadingMin = 0.0f;

		handbrakeCamHeadingMax = 0.0f;
	}
	return;
}

Timer timerVehCamA;
Timer timerVehCamB;
void CamFollowVehicleDuringHandbrake()
{
	const int timePressed = INI::CamFollowVehDelay;
	constexpr int delay = 300;

	const Vehicle veh = GET_VEHICLE_PED_IS_USING(GetPlayerPed());
	if (!DOES_ENTITY_EXIST(veh))
		return;

	Hash vehModel = GET_ENTITY_MODEL(veh);
	if (!IS_THIS_MODEL_A_CAR(vehModel) && !(IS_THIS_MODEL_A_BIKE(vehModel) && !IS_THIS_MODEL_A_BICYCLE(vehModel))
		&& !IS_THIS_MODEL_A_QUADBIKE(vehModel))
		return;

	if (timePressed > 0)
	{
		if (IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_VEH_HANDBRAKE))
			timerVehCamA.Reset();
		else if (timerVehCamA.Get() > timePressed && IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_VEH_HANDBRAKE))
		{
			SetCamSmoothHeadingLimit();
			SET_THIRD_PERSON_CAM_RELATIVE_HEADING_LIMITS_THIS_UPDATE(handbrakeCamHeadingMin, handbrakeCamHeadingMax);
		}
		else if (timerVehCamA.Get() > timePressed && IS_CONTROL_JUST_RELEASED(PLAYER_CONTROL, INPUT_VEH_HANDBRAKE))
		{
			timerVehCamB.Reset();
			SetCamSmoothHeadingLimit();
			SET_THIRD_PERSON_CAM_RELATIVE_HEADING_LIMITS_THIS_UPDATE(handbrakeCamHeadingMin, handbrakeCamHeadingMax);
		}
		else if (timerVehCamB.Get() < delay)
		{
			SetCamSmoothHeadingLimit();
			SET_THIRD_PERSON_CAM_RELATIVE_HEADING_LIMITS_THIS_UPDATE(handbrakeCamHeadingMin, handbrakeCamHeadingMax);
		}
	}
	else if (IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_VEH_HANDBRAKE))
	{
		SetCamSmoothHeadingLimit();
		SET_THIRD_PERSON_CAM_RELATIVE_HEADING_LIMITS_THIS_UPDATE(handbrakeCamHeadingMin, handbrakeCamHeadingMax);
	}
	return;
}

bool wasUsingHoodCam = false;
void DisableFirstPersonView()
{
	if (GET_FOLLOW_PED_CAM_VIEW_MODE() == CAM_VIEW_MODE_FIRST_PERSON)
		SET_FOLLOW_PED_CAM_VIEW_MODE(CAM_VIEW_MODE_THIRD_PERSON_NEAR);

	if (IS_FOLLOW_VEHICLE_CAM_ACTIVE())
	{
		const bool isUsingHoodCam = GET_IS_USING_HOOD_CAMERA();	//Requires b372
		const int vehModel = GET_ENTITY_MODEL(GET_VEHICLE_PED_IS_USING(GetPlayerPed()));
		if (IS_THIS_MODEL_A_CAR(vehModel) || (IS_THIS_MODEL_A_BOAT(vehModel) && !IS_THIS_MODEL_A_JETSKI(vehModel)) ||
			IS_THIS_MODEL_A_PLANE(vehModel) || IS_THIS_MODEL_A_HELI(vehModel))
		{
			if (wasUsingHoodCam && isUsingHoodCam)
				SET_FOLLOW_VEHICLE_CAM_VIEW_MODE(CAM_VIEW_MODE_FIRST_PERSON);

			if (IS_CONTROL_JUST_RELEASED(PLAYER_CONTROL, INPUT_NEXT_CAMERA))
			{
				if (GET_FOLLOW_VEHICLE_CAM_VIEW_MODE() == CAM_VIEW_MODE_THIRD_PERSON_FAR)
					wasUsingHoodCam = true;
				else
					wasUsingHoodCam = false;
			}

			if (isUsingHoodCam && !GET_IS_TASK_ACTIVE(GetPlayerPed(), CODE_TASK_EXIT_VEHICLE))
				return;
		}

		if (GET_FOLLOW_VEHICLE_CAM_VIEW_MODE() == CAM_VIEW_MODE_FIRST_PERSON && !GET_IS_TASK_ACTIVE(GetPlayerPed(), CODE_TASK_EXIT_VEHICLE))
		{
			wasUsingHoodCam = false;
			SET_FOLLOW_VEHICLE_CAM_VIEW_MODE(CAM_VIEW_MODE_THIRD_PERSON_NEAR);
		}
	}

	DISABLE_ON_FOOT_FIRST_PERSON_VIEW_THIS_UPDATE(); //Also works in vehicles
	return;
}

void DisableIdleCamera()
{
	INVALIDATE_CINEMATIC_VEHICLE_IDLE_MODE();
	INVALIDATE_IDLE_CAM();
	return;
}

void DisableRecording()
{
	DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_REPLAY_START_STOP_RECORDING, false);
	DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_REPLAY_START_STOP_RECORDING_SECONDARY, false);
	REPLAY_PREVENT_RECORDING_THIS_FRAME();
	return;
}

void DisableMobilePhone()
{
	if (DOES_SCRIPT_EXIST("cellphone_controller"))
		TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("cellphone_controller");

	if (IS_MOBILE_PHONE_CALL_ONGOING()) { STOP_SCRIPTED_CONVERSATION(false); }
	if (IS_PED_RINGTONE_PLAYING(GetPlayerPed())) { STOP_PED_RINGTONE(GetPlayerPed()); }
	DESTROY_MOBILE_PHONE();

	EnablePedConfigFlag(GetPlayerPed(), PCF_PhoneDisableTextingAnimations);
	EnablePedConfigFlag(GetPlayerPed(), PCF_PhoneDisableTalkingAnimations);
	EnablePedConfigFlag(GetPlayerPed(), PCF_PhoneDisableCameraAnimations);

	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_PHONE, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_UP, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_DOWN, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_LEFT, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_RIGHT, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_SELECT, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_CANCEL, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_OPTION, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_EXTRA_OPTION, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_SCROLL_FORWARD, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_SCROLL_BACKWARD, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_CAMERA_FOCUS_LOCK, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_CAMERA_GRID, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_CAMERA_SELFIE, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_CAMERA_DOF, false);
	DISABLE_CONTROL_ACTION(FRONTEND_CONTROL, INPUT_CELLPHONE_CAMERA_EXPRESSION, false);
	return;
}
}	//END nControls

//////////////////////////////////////////Player Vehicle/////////////////////////////////////////
namespace nVehicle
{
Vehicle lastVeh = NULL;
void DisableCarMidAirAndRollControl()
{
	Vehicle veh = GetVehiclePedIsIn(GetPlayerPed());
	if (!DOES_ENTITY_EXIST(veh))
		return;

	//Clean up old veh stuck checks, since the game is limited to ~16 of them
	if (lastVeh != veh && DOES_ENTITY_EXIST(lastVeh) && DOES_VEHICLE_HAVE_STUCK_VEHICLE_CHECK(veh))
		REMOVE_VEHICLE_STUCK_CHECK(veh);

	//Check if veh is car and is driveable then proceed
	constexpr int time = 100;
	Hash vehModel = GET_ENTITY_MODEL(veh);
	if (IS_THIS_MODEL_A_CAR(vehModel) && vehModel != 0x586765FB /*deluxo*/)
	{
		if (!DOES_VEHICLE_HAVE_STUCK_VEHICLE_CHECK(veh))
			ADD_VEHICLE_STUCK_CHECK_WITH_WARP(veh, 0.1f, time, false, false, false, -1);

		if (IS_ENTITY_IN_AIR(veh) ||
			IS_VEHICLE_STUCK_TIMER_UP(veh, VEH_STUCK_HUNG_UP, time) ||
			IS_VEHICLE_STUCK_TIMER_UP(veh, VEH_STUCK_ON_ROOF, time) ||
			IS_VEHICLE_STUCK_TIMER_UP(veh, VEH_STUCK_ON_SIDE, time))
		{
			DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_VEH_MOVE_LR, false);
			DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_VEH_MOVE_UD, false);
			DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_VEH_MOVE_UP_ONLY, false);
			DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_VEH_MOVE_DOWN_ONLY, false);
			DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_VEH_MOVE_LEFT_ONLY, false);
			DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_VEH_MOVE_RIGHT_ONLY, false);
			DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_VEH_MOVE_LEFT, false);
			DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_VEH_MOVE_RIGHT, false);
			DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_VEH_MOVE_UP, false);
			DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_VEH_MOVE_DOWN, false);

			//Still enable player to change steering angle  - Control normal must be inverted
			SET_VEHICLE_STEER_BIAS(veh, -0.5f * GET_DISABLED_CONTROL_UNBOUND_NORMAL(PLAYER_CONTROL, INPUT_VEH_MOVE_LR));
		}
	}
	return;
}

bool wasSetAsMissionEntity = false;
Timer timerCarExplosion;
void DisableForcedCarExplosionOnImpact()
{
	Vehicle veh = GetVehiclePedIsIn(GetPlayerPed());
	if (!DOES_ENTITY_EXIST(veh))
		return;

	//Check if vehicle is boat/car
	Hash vehModel = GET_ENTITY_MODEL(veh);
	if (!IS_THIS_MODEL_A_CAR(vehModel) && !IS_THIS_MODEL_A_BOAT(vehModel))
		return;

	//If not in the air, set it as no longer needed as a mission entity
	if (!IS_ENTITY_IN_AIR(veh) || GET_ENTITY_SPEED(veh) < 10.0f)
	{
		//Wait 500ms before setting vehicle as no longer needed
		if (timerCarExplosion.Get() > 500 && IS_ENTITY_A_MISSION_ENTITY(veh) && wasSetAsMissionEntity)
		{
			wasSetAsMissionEntity = false;
			SET_ENTITY_AS_NO_LONGER_NEEDED(&veh);
		}
		return;
	}

	timerCarExplosion.Reset();
	if (!IS_ENTITY_A_MISSION_ENTITY(veh))
	{
		wasSetAsMissionEntity = true;
		SET_ENTITY_AS_MISSION_ENTITY(veh, true, false);
	}
	return;
}

void DisableEngineSmoke()
{
	Vehicle veh = GetVehiclePedIsIn(GetPlayerPed());
	if (veh == NULL)
		return;

	constexpr float limit = ENGINE_DAMAGE_RADBURST + 50.0f;
	if (GET_VEHICLE_ENGINE_HEALTH(veh) <= limit)
		SET_VEHICLE_ENGINE_HEALTH(veh, limit);

	return;
}

Vehicle lastVehEngine = NULL;
void DisableEngineFire()
{
	Vehicle veh = GetVehiclePedIsIn(GetPlayerPed());
	if (veh == NULL)
		return;

	//Check if vehicle is a car/bike/quad and adjust engine health limit
	const int vehModel = GET_ENTITY_MODEL(veh);
	if (!IS_THIS_MODEL_A_CAR(vehModel) && !(IS_THIS_MODEL_A_BIKE(vehModel) && !IS_THIS_MODEL_A_BICYCLE(vehModel))
		&& !IS_THIS_MODEL_A_QUADBIKE(vehModel))
		return;

	//Engine health safeguard
	constexpr float limit = ENGINE_DAMAGE_ONFIRE + 50.0f;
	if (GET_VEHICLE_ENGINE_HEALTH(veh) <= limit)
		SET_VEHICLE_ENGINE_HEALTH(veh, limit);

	//Check if we have already disabled engine fire on this vehicle)
	if (veh == lastVehEngine)
		return;

	lastVehEngine = veh;
	if (!IS_ENTITY_A_MISSION_ENTITY(veh))
	{
		SET_ENTITY_AS_MISSION_ENTITY(veh, true, false);
		SET_DISABLE_VEHICLE_ENGINE_FIRES(veh, true);
		SET_ENTITY_AS_NO_LONGER_NEEDED(&veh);
	}
	else
		SET_DISABLE_VEHICLE_ENGINE_FIRES(veh, true);

	return;
}

Timer timerEngineControl;
constexpr int TURN_OFF_ENGINE_DURATION = 250;
void LeaveEngineOnWhenExitingVehicles()
{
	Vehicle veh = GetVehiclePedIsIn(GetPlayerPed(), true, true);
	if (!DOES_ENTITY_EXIST(veh))
		return;

	SET_VEHICLE_KEEP_ENGINE_ON_WHEN_ABANDONED(veh, true);	//Same thing as setting PCF_LeaveEngineOnWhenExitingVehicles
	if (IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_VEH_EXIT))
		timerEngineControl.Reset();
	else if (IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_VEH_EXIT) && 
		timerEngineControl.Get() > TURN_OFF_ENGINE_DURATION &&
		GET_IS_TASK_ACTIVE(GetPlayerPed(), CODE_TASK_EXIT_VEHICLE))
	{ SET_VEHICLE_ENGINE_ON(veh, false, true, false); }
	return;
}

//Very very bad method of doing this, but it works!
//When a vehicle is physically attached to another vehicle it retains the same steering on exit (thanks to RAGE constraints)
//Only problem is that the vehicle now inherits most veh flags from parent. 
//Don't know if this could cause issues, but better then patching memory
Timer timerWheelsAutoCenter;
Vehicle tmpVeh = NULL;
bool shouldAttachVeh = false;
void DisableWheelsAutoCenterOnCarExit()
{
	constexpr float maxVehSpeedDetach = 10.0f;

	const Vehicle veh = GET_VEHICLE_PED_IS_USING(GetPlayerPed());
	if (!DOES_ENTITY_EXIST(veh))
		return;

	const int vehHash = GET_ENTITY_MODEL(veh);	//Disable for bikes and bicycles, can cause issues
	if (!IS_THIS_MODEL_A_CAR(vehHash) || !IS_VEHICLE_DRIVEABLE(veh, false) || GET_ENTITY_SUBMERGED_LEVEL(GetPlayerPed()) > 0.7f)
		return;

	const int tmpVehHash = GET_ENTITY_MODEL(tmpVeh);
	if (tmpVehHash != vehHash)
	{
		if (DOES_ENTITY_EXIST(tmpVeh))
		{
			if (IS_ENTITY_ATTACHED_TO_ENTITY(veh, tmpVeh))
				DETACH_ENTITY(veh, false, false);

			SET_ENTITY_AS_MISSION_ENTITY(tmpVeh, false, true);
			DELETE_ENTITY(&tmpVeh);
			SET_MODEL_AS_NO_LONGER_NEEDED(tmpVehHash);
			tmpVeh = NULL;
		}

		if (!HAS_MODEL_LOADED(vehHash))
		{
			REQUEST_MODEL(vehHash);
			return;
		}
		tmpVeh = CREATE_VEHICLE(vehHash, 0.0f, 0.0f, -90.0f, 0.0f, false, false, true);
		SET_ENTITY_AS_MISSION_ENTITY(tmpVeh, false, true);
		SET_ENTITY_VISIBLE(tmpVeh, false, false);
		FREEZE_ENTITY_POSITION(tmpVeh, true);
		return;
	}

	if (GET_IS_TASK_ACTIVE(GetPlayerPed(), CODE_TASK_EXIT_VEHICLE) || timerWheelsAutoCenter.Get() > 2000 || GET_ENTITY_SPEED(veh) > maxVehSpeedDetach)
		shouldAttachVeh = false;

	if (IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_VEH_EXIT) && GetVehiclePedIsIn(GetPlayerPed(), false, false)) //&& !IS_VEHICLE_ATTACHED_TO_TRAILER(veh))
		shouldAttachVeh = true;

	if (shouldAttachVeh && !IS_ENTITY_ATTACHED_TO_ENTITY(veh, tmpVeh) && GET_ENTITY_SPEED(veh) < maxVehSpeedDetach)
	{
		timerWheelsAutoCenter.Reset();

		float yOffset = 100.0f;
		if (GET_ENTITY_SPEED_VECTOR(veh, true).y < 0.0f)
			yOffset *= -1.0f;

		const Vector3 tmpOffset = GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, 0.0f, yOffset, 0.0f);
		SET_ENTITY_COORDS_NO_OFFSET(tmpVeh, tmpOffset.x, tmpOffset.y, GET_ENTITY_COORDS(tmpVeh, false).z, true, true, false);

		ATTACH_ENTITY_TO_ENTITY_PHYSICALLY(veh, tmpVeh, NULL, NULL, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, true, 2);
		return;
	}

	//Detach veh 
	if (IS_ENTITY_ATTACHED_TO_ENTITY(veh, tmpVeh) && !shouldAttachVeh)
	{
		DETACH_ENTITY(veh, false, false);
		return;
	}
	return;
}

Timer hydraulicsTimer;
Vehicle lastHydraulicsVeh = NULL;
Vehicle lastUsedHydraulicsVeh = NULL;
float hydraulicsState[MAX_WHEELS] = { 0, 0, 0, 0, 0, 0, 0, 0 };
void KeepCarHydraulicsPosition()
{
	if (IS_PED_IN_ANY_VEHICLE(GetPlayerPed(), false) && !GET_PED_RESET_FLAG(GetPlayerPed(), PRF_IsEnteringOrExitingVehicle))
	{
		const Vehicle veh = GetVehiclePedIsIn(GetPlayerPed());
		if (!DOES_ENTITY_EXIST(veh) || !IS_THIS_MODEL_A_CAR(GET_ENTITY_MODEL(veh)) || !IS_VEHICLE_DRIVEABLE(veh, false))
			return;

		if (IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_VEH_EXIT))
		{
			LOOP(i, MAX_WHEELS) { hydraulicsState[i] = GET_HYDRAULIC_SUSPENSION_RAISE_FACTOR(lastHydraulicsVeh, i); }
		}

		lastHydraulicsVeh = veh;
		if (veh != lastUsedHydraulicsVeh) lastUsedHydraulicsVeh = NULL;
	}

	if (GET_IS_TASK_ACTIVE(GetPlayerPed(), CODE_TASK_EXIT_VEHICLE))
	{
		hydraulicsTimer.Reset();
		LOOP(i, MAX_WHEELS) { SET_HYDRAULIC_SUSPENSION_RAISE_FACTOR(lastHydraulicsVeh, i, hydraulicsState[i]); }
		lastUsedHydraulicsVeh = lastHydraulicsVeh;
	}
	else if (hydraulicsTimer.Get() < 500)
	{
		LOOP(i, MAX_WHEELS) { SET_HYDRAULIC_SUSPENSION_RAISE_FACTOR(lastHydraulicsVeh, i, hydraulicsState[i]); }
	}

	if (DOES_ENTITY_EXIST(lastUsedHydraulicsVeh) && GetVehiclePedIsEntering(GetPlayerPed()) == lastUsedHydraulicsVeh)
	{
		LOOP(i, MAX_WHEELS) { SET_HYDRAULIC_SUSPENSION_RAISE_FACTOR(lastHydraulicsVeh, i, hydraulicsState[i]); }
	}
	return;
}

void EnableHeliWaterPhysics()
{
	Vehicle heli = GET_VEHICLE_PED_IS_USING(GetPlayerPed());
	if (!DOES_ENTITY_EXIST(heli) || !IS_THIS_MODEL_A_HELI(GET_ENTITY_MODEL(heli)) ||
		!GET_IS_VEHICLE_ENGINE_RUNNING(heli))
		return;

	constexpr int arrSize = 4;
	const int rotors[arrSize] = {
		GET_ENTITY_BONE_INDEX_BY_NAME(heli, "ROTOR_MAIN"),
		GET_ENTITY_BONE_INDEX_BY_NAME(heli, "ROTOR_MAIN_2"),
		GET_ENTITY_BONE_INDEX_BY_NAME(heli, "ROTOR_REAR"),
		GET_ENTITY_BONE_INDEX_BY_NAME(heli, "ROTOR_REAR_2")
	};

	Vector3 heliCoords = GET_ENTITY_COORDS(heli, false);
	Vector3 min = nullVec; Vector3 max = nullVec;
	GET_MODEL_DIMENSIONS(GET_ENTITY_MODEL(heli), &min, &max);
	heliCoords.z += min.z;

	float z = NULL;
	if (!GET_GROUND_Z_FOR_3D_COORD(heliCoords.x, heliCoords.y, heliCoords.z, &z, true, true))
		return;

	constexpr float maxHeight = 15.0f;
	const float distGround = heliCoords.z - z;
	if (distGround > maxHeight)
		return;

	const float mult = 1.0f - (distGround / maxHeight);
	const float speed = -2.5f * mult;
	constexpr float rate = 0.9f;
	LOOP(i, arrSize)
	{
		if (rotors[i] < 0)
			continue;

		// Apply only for main rotors first, so that if the helicopter has a tail it doesn't apply the effect.
		// Eg.: Cargobobs have both a front and rear motor, but don't have a tail and thus the effect should be applied to both motors,
		// an helicopter will instead have a rear motors at it's tail that should NOT produce the effect
		if (i <= 1)
		{
			const Vector3 tmpCoords = GET_WORLD_POSITION_OF_ENTITY_BONE(heli, rotors[i]);
			MODIFY_WATER(tmpCoords.x, tmpCoords.y, speed, rate);
		}
		else
		{
			if (GET_ENTITY_BONE_INDEX_BY_NAME(heli, "TAIL") < 0)
			{
				const Vector3 tmpCoords = GET_WORLD_POSITION_OF_ENTITY_BONE(heli, rotors[i]);
				MODIFY_WATER(tmpCoords.x, tmpCoords.y, speed, rate);
			}
		}
	}
	return;
}

void DisableRagdollOnVehicleRoof()
{
	//Velocity Unit -> Km/h
	if (INI::MaxVehicleSpeed > 0.0f)
	{
		const float speed = GET_ENTITY_SPEED(GetPlayerPed()) * 3.6f;	// m\s to Km\h
		if (speed > INI::MaxVehicleSpeed)
			return;
	}

	EnablePedResetFlag(GetPlayerPed(), PRF_BlockRagdollFromVehicleFallOff);
	return;
}

inline void DisableDragOutCar() { SET_PED_CAN_BE_DRAGGED_OUT(GetPlayerPed(), false); return; }	//Same thing as setting CPED_CONFIG_FLAG_DontDragMeOutCar
inline void DisableFlyThroughWindscreen() { DisablePedConfigFlag(GetPlayerPed(), PCF_WillFlyThroughWindscreen); return; }
inline void DisableBikeKnockOff() { SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(GetPlayerPed(), KNOCKOFFVEHICLE_NEVER); return; }

void DisableShallowWaterBikeJumpOut()
{
	const Vehicle veh = GetVehiclePedIsIn(GetPlayerPed(), false, false);
	if (DOES_ENTITY_EXIST(veh))
	{
		if (GET_ENTITY_SUBMERGED_LEVEL(veh) > 0.9f || (IS_ENTITY_IN_WATER(veh) && !IS_VEHICLE_DRIVEABLE(veh, false)))
			return;
	}

	EnablePedResetFlag(GetPlayerPed(), PRF_DisableShallowWaterBikeJumpOutThisFrame);
	return;
}

inline void DisableVehicleJitter() { SET_CAR_HIGH_SPEED_BUMP_SEVERITY_MULTIPLIER(0.0f); return; }

void DisableAirVehicleTurbulence()
{
	const Vehicle veh = GetVehiclePedIsIn(GetPlayerPed(), true, true);
	const Hash vehHash = GET_ENTITY_MODEL(veh);
	if (IS_THIS_MODEL_A_PLANE(vehHash))
		SET_PLANE_TURBULENCE_MULTIPLIER(veh, 0.0f);
	else if (IS_THIS_MODEL_A_HELI(vehHash))
		SET_HELI_TURBULENCE_SCALAR(veh, 0.0f);

	return;
}

void DisableAutoEquipHelmets()
{
	if (INI::DisableAutoEquipHelmets == 1)
		EnablePedConfigFlag(GetPlayerPed(), PCF_DisableAutoEquipHelmetsInBikes);
	else if (INI::DisableAutoEquipHelmets == 2)
		EnablePedConfigFlag(GetPlayerPed(), PCF_DisableAutoEquipHelmetsInAicraft);
	else
	{
		EnablePedConfigFlag(GetPlayerPed(), PCF_DisableAutoEquipHelmetsInBikes);
		EnablePedConfigFlag(GetPlayerPed(), PCF_DisableAutoEquipHelmetsInAicraft);
	}

	return;
}

inline void DisableStuntJumps() { SET_STUNT_JUMPS_CAN_TRIGGER(false); return; }
}	//END nVehicle

///////////////////////////////////////////////HUD///////////////////////////////////////////////
namespace nHUD
{
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

bool HasPlayerVehicleAbility()
{
	Vehicle veh = GetVehiclePedIsIn(GetPlayerPed());
	if (DOES_ENTITY_EXIST(veh) && (GET_VEHICLE_HAS_KERS(veh) || GET_HAS_ROCKET_BOOST(veh) || GET_CAR_HAS_JUMP(veh)))
		return true;

	return false;
}

void SetHealthHudDisplayValues(int healthPercentage, int armourPercentage, bool showDamage = true)
{
	SET_HEALTH_HUD_DISPLAY_VALUES(healthPercentage + 100, armourPercentage, showDamage);
	SET_MAX_HEALTH_HUD_DISPLAY(200);
	SET_MAX_ARMOUR_HUD_DISPLAY(100);
	return;
}

void AllowGameExecutionOnPauseMenu()
{
	if (!IS_CONTROL_PRESSED(FRONTEND_CONTROL, INPUT_FRONTEND_PAUSE_ALTERNATE))
		SET_PAUSE_MENU_ACTIVE(false);

	if (IS_CONTROL_JUST_PRESSED(FRONTEND_CONTROL, INPUT_FRONTEND_PAUSE) &&
		GET_CURRENT_FRONTEND_MENU_VERSION() != FE_MENU_VERSION_SP_PAUSE)
	{ ACTIVATE_FRONTEND_MENU(FE_MENU_VERSION_SP_PAUSE, false, -1); }

	return;
}

void DisablePauseMenuPostFX()
{
	CLEAR_ALL_TCMODIFIER_OVERRIDES("hud_def_blur");
	CLEAR_ALL_TCMODIFIER_OVERRIDES("bloommid");
	CLEAR_ALL_TCMODIFIER_OVERRIDES("hud_def_Franklin");
	CLEAR_ALL_TCMODIFIER_OVERRIDES("hud_def_Michael");
	CLEAR_ALL_TCMODIFIER_OVERRIDES("hud_def_Trevor");
	CLEAR_ALL_TCMODIFIER_OVERRIDES("hud_def_desat_Neutral");

	ADD_TCMODIFIER_OVERRIDE("hud_def_blur", "default");
	ADD_TCMODIFIER_OVERRIDE("bloommid", "default");
	switch (GET_ENTITY_MODEL(GetPlayerPed()))
	{
	case FranklinPed:
		ADD_TCMODIFIER_OVERRIDE("hud_def_Franklin", "default"); break;
	case MichaelPed:
		ADD_TCMODIFIER_OVERRIDE("hud_def_Michael", "default"); break;
	case TrevorPed:
		ADD_TCMODIFIER_OVERRIDE("hud_def_Trevor", "default"); break;
	default:
		ADD_TCMODIFIER_OVERRIDE("hud_def_desat_Neutral", "default"); break;
	}
	return;
}

void DisableHUDPostFX()
{
	LOOP(i, AnimPostFXSize)
	{ ANIMPOSTFX_STOP_AND_FLUSH_REQUESTS(AnimPostFX[i]); }
	return;
}

void DisableSpecialAbilityPostFX()
{
	LOOP(i, AbilityPostFXSize)
	{ ANIMPOSTFX_STOP_AND_FLUSH_REQUESTS(AbilityPostFX[i]); }
	return;
}

Timer timerBigMap;
bool isBigMapActive = false;
void EnableBigMapToggle()
{
	if (IS_CONTROL_JUST_PRESSED(FRONTEND_CONTROL, INPUT_HUD_SPECIAL))
		timerBigMap.Reset();
	else if (IS_CONTROL_JUST_RELEASED(FRONTEND_CONTROL, INPUT_HUD_SPECIAL) && timerBigMap.Get() < 200)
	{
		isBigMapActive ^= true;
		SET_BIGMAP_ACTIVE(isBigMapActive, false);
	}
	return;
}

float speed = 0.0f;
Timer speedTimer;
void MinimapSpeedometer()
{
	if (!IS_MINIMAP_RENDERING() || IS_RADAR_HIDDEN() || !GetVehiclePedIsIn(GetPlayerPed(), false, false))
		return;

	if (speedTimer.Get() > 100)
	{
		speed = GET_ENTITY_SPEED(GetPlayerPed());
		speedTimer.Reset();
	}

	std::string text = "";
	if (SHOULD_USE_METRIC_MEASUREMENTS())
	{
		text = std::to_string(speed * 3.6f);
		text = text.substr(0, (text.find(".") + 2)) + "km/h";
	}
	else
	{
		text = std::to_string(speed * 2.236936f);
		text = text.substr(0, (text.find(".") + 2)) + "mph";
	}

	float txtX = 0.0f, txtY = 0.0f;
	float n = (16.0f / 9.0f) / GET_SCREEN_ASPECT_RATIO();	// Scale X axis if ratio differs from 16/9
	SET_SCRIPT_GFX_ALIGN(UI_ALIGN_LEFT, UI_ALIGN_BOTTOM);
	if (isBigMapActive)
		GET_SCRIPT_GFX_ALIGN_POSITION(0.18f * n, -0.047f, &txtX, &txtY);
	else
		GET_SCRIPT_GFX_ALIGN_POSITION(0.1f * n, -0.047f, &txtX, &txtY);
	
	RESET_SCRIPT_GFX_ALIGN();

	SetTextStyle(TextStyle{ FONT_CONDENSED, 0.44f, 0.44f, RGBA{250, 250, 250, 180}, DROPSTYLE_DROPSHADOWONLY, false, 0.0f, 1.0f });

	BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
	ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text.c_str());
	END_TEXT_COMMAND_DISPLAY_TEXT(txtX, txtY, false);
	return;
}

void SetRadarZoom()
{
	if (INI::SetRadarZoom < 0.0f)
		return;

	if (INI::SetRadarZoom >= 100.0f)
		SET_RADAR_ZOOM_PRECISE(100.0f);
	else
		SET_RADAR_ZOOM_PRECISE(INI::SetRadarZoom);

	DONT_ZOOM_MINIMAP_WHEN_RUNNING_THIS_FRAME();
	DONT_ZOOM_MINIMAP_WHEN_SNIPING_THIS_FRAME();
	return;
}

inline void DisableMinimapTilt() { DONT_TILT_MINIMAP_THIS_FRAME(); return; }
inline void HideMinimapFog() { SET_MINIMAP_HIDE_FOW(true); return; }
inline void HideSatNav() { CALL_SCALEFORM_MOVIE_METHOD(RequestMinimapScaleform(), "HIDE_SATNAV"); return; }
inline void HideMinimapDepth() { CALL_SCALEFORM_MOVIE_METHOD(RequestMinimapScaleform(), "HIDE_DEPTH"); return; }

void HideMinimapBars()
{
	if (BEGIN_SCALEFORM_MOVIE_METHOD(RequestMinimapScaleform(), "SETUP_HEALTH_ARMOUR"))
	{
		SCALEFORM_MOVIE_METHOD_ADD_PARAM_INT(3);
		END_SCALEFORM_MOVIE_METHOD();
	}
	return;
}

void AlwaysHideAbilityBar()
{
	if (BEGIN_SCALEFORM_MOVIE_METHOD(RequestMinimapScaleform(), "SET_ABILITY_BAR_VISIBILITY_IN_MULTIPLAYER"))
	{
		SCALEFORM_MOVIE_METHOD_ADD_PARAM_BOOL(false);
		END_SCALEFORM_MOVIE_METHOD();
	}
	return;
}

void HideAbilityBarForNonMainCharacters()
{
	if (IS_SPECIAL_ABILITY_ENABLED(GetPlayer(), 0) || HasPlayerVehicleAbility())
	{
		if (BEGIN_SCALEFORM_MOVIE_METHOD(RequestMinimapScaleform(), "MULTIPLAYER_IS_ACTIVE"))
		{
			SCALEFORM_MOVIE_METHOD_ADD_PARAM_BOOL(false);
			SCALEFORM_MOVIE_METHOD_ADD_PARAM_BOOL(false);
			END_SCALEFORM_MOVIE_METHOD();
		}
	}
	else if (BEGIN_SCALEFORM_MOVIE_METHOD(RequestMinimapScaleform(), "SET_ABILITY_BAR_VISIBILITY_IN_MULTIPLAYER"))
	{
		SCALEFORM_MOVIE_METHOD_ADD_PARAM_BOOL(false);
		END_SCALEFORM_MOVIE_METHOD();
	}
	return;
}

Timer timerFlashHealth;
constexpr int flashHealthInterval = 400;
void ReplaceArmourBarWithStamina()
{
	int staminaPercentage = ROUND(100.0f - GET_PLAYER_SPRINT_STAMINA_REMAINING(GetPlayer()));				//GET_PLAYER_SPRINT_STAMINA_REMAINING goes from 0 to 100 and then health depletes
	if (INI::MergeHealthAndArmour)
	{
		int health = GET_ENTITY_HEALTH(GetPlayerPed()) - 100 + GET_PED_ARMOUR(GetPlayerPed());				//We need to subtract 100 because the player fatal health is 100 not 0
		int maxHealth = GET_ENTITY_MAX_HEALTH(GetPlayerPed()) - 100 + GET_PLAYER_MAX_ARMOUR(GetPlayer());
		int newHealthPercentage = ROUND(health * 100.0f / maxHealth);										//Always ensure a 100 offset to fix hud ratio
		int realHealthPercentage = ROUND((GET_ENTITY_HEALTH(GetPlayerPed()) - 100.0f) * 100.0f / (GET_ENTITY_MAX_HEALTH(GetPlayerPed()) - 100.0f));

		//Flash health bar every 400ms if health is below 25%
		if (realHealthPercentage > 25 || timerFlashHealth.Get() > flashHealthInterval)
		{
			if (timerFlashHealth.Get() > (flashHealthInterval * 2))
				timerFlashHealth.Reset();

			SetHealthHudDisplayValues(newHealthPercentage, staminaPercentage);
		}
		else if (realHealthPercentage <= 25 && GET_PED_ARMOUR(GetPlayerPed()) > 10)
		{
			if (timerFlashHealth.Get() <= flashHealthInterval)
				SetHealthHudDisplayValues(realHealthPercentage, staminaPercentage);
			else
				SetHealthHudDisplayValues(newHealthPercentage, staminaPercentage);
		}
	}
	else
	{
		int health = GET_ENTITY_HEALTH(GetPlayerPed()) - 100;
		int maxHealth = GET_ENTITY_MAX_HEALTH(GetPlayerPed()) - 100;
		int healthPercentage = ROUND(health * 100.0f / maxHealth);
		SetHealthHudDisplayValues(healthPercentage, staminaPercentage);
	}
	return;
}

int GetHudComponentFromString(std::string str)
{
	switch (Joaat(str.c_str()))
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

bool InitializedHideHudComponents = false;
std::string hudComponentsStrArr[MAX_HUD_COMPONENTS]{};
unsigned int hudComponentsArr[MAX_HUD_COMPONENTS]{};
void HideHudComponents()
{
	if (!InitializedHideHudComponents)
	{
		SplitString(INI::HudComponents, hudComponentsStrArr, MAX_HUD_COMPONENTS, true);
		LOOP(i, MAX_HUD_COMPONENTS)
		{
			if (!hudComponentsStrArr[i].empty())
				hudComponentsArr[i] = GetHudComponentFromString(hudComponentsStrArr[i]);
		}
		InitializedHideHudComponents = true;
	}

	LOOP(i, MAX_HUD_COMPONENTS)
	{
		if (hudComponentsArr[i] > 0)
			HIDE_HUD_COMPONENT_THIS_FRAME(hudComponentsArr[i]);
	}
	return;
}

void HideWeaponReticle()
{
	Hash playerWeapon = NULL;
	GET_CURRENT_PED_WEAPON(GetPlayerPed(), &playerWeapon, false);
	if (GET_WEAPONTYPE_GROUP(playerWeapon) == WEAPONGROUP_SNIPER && GET_PED_CONFIG_FLAG(GetPlayerPed(), PCF_IsAimingGun, false))
		return;

	HIDE_HUD_COMPONENT_THIS_FRAME(HUD_RETICLE);
	return;
}

inline void HideEnemiesBlips() { SET_POLICE_RADAR_BLIPS(false); return; }
}	//END nHUD

///////////////////////////////////////////////Audio///////////////////////////////////////////////
namespace nAudio
{
inline void DisableWantedMusic() { SET_AUDIO_FLAG("WantedMusicDisabled", true); return; }
inline void DisablePoliceScanner() { SET_AUDIO_FLAG("PoliceScannerDisabled", true); return; }
inline void DisableFlightMusic() { SET_AUDIO_FLAG("DisableFlightMusic", true); return; }

int DisableRadiosCount = 0;
void SetRadiosMusicOnly()
{
	LOOP(i, 5)
	{
		SET_RADIO_STATION_MUSIC_ONLY(RadioStations[DisableRadiosCount], true);

		DisableRadiosCount++;
		if (DisableRadiosCount >= RadioStationsSize)
			DisableRadiosCount = 0;
	}

	SET_RADIO_STATION_MUSIC_ONLY(GET_PLAYER_RADIO_STATION_NAME(), true);
	return;
}

Vehicle lastVehRadioOff = NULL;
void DefaultVehicleRadioOff()
{
	const Vehicle tmpVeh = GET_VEHICLE_PED_IS_USING(GetPlayerPed());
	if (DOES_ENTITY_EXIST(tmpVeh) && tmpVeh != lastVehRadioOff)
	{
		lastVehRadioOff = tmpVeh;
		if (INI::DefaultVehicleRadioOff == 2)
			SET_VEH_RADIO_STATION(lastVehRadioOff, "OFF");
		else
		{
			if (!GET_IS_VEHICLE_ENGINE_RUNNING(tmpVeh) && GET_PED_RESET_FLAG(GetPlayerPed(), PRF_IsEnteringVehicle))
				SET_VEH_RADIO_STATION(lastVehRadioOff, "OFF");
		}
	}
	return;
}

bool InitializedMuteSounds = false;
constexpr int maxMutedSounds = 10;
std::string mutedSoundsArr[maxMutedSounds]{};
void MuteSounds()
{
	if (!InitializedMuteSounds)
	{
		SplitString(INI::Sounds, mutedSoundsArr, maxMutedSounds, true);
		LOOP(i, maxMutedSounds)
		{
			if (mutedSoundsArr[i].empty())
				continue;

			switch (Joaat(mutedSoundsArr[i].c_str()))
			{
			case Joaat("AMBIENCE"): mutedSoundsArr[i] = "MUTES_AMBIENCE_SCENE"; break;
			case Joaat("CUTSCENE"): mutedSoundsArr[i] = "MUTES_CUTSCENE_SCENE"; break;
			case Joaat("FRONTEND"): mutedSoundsArr[i] = "MUTES_FRONTEND_SCENE"; break;
			// Mutes all music emitters, including radios (cars, static ones like the stripclub etc.)
			case Joaat("MUSIC"): mutedSoundsArr[i] = "MUTES_MUSIC_SCENE"; break;
			case Joaat("RADIO"): mutedSoundsArr[i] = "MUTES_RADIO_SCENE"; break;
			case Joaat("SFX"): mutedSoundsArr[i] = "MUTES_SFX_SCENE"; break;
			case Joaat("SPEECH"): mutedSoundsArr[i] = "MUTES_SPEECH_SCENE"; break;
			case Joaat("GUNS"): mutedSoundsArr[i] = "MUTES_GUNS_SCENE"; break;
			case Joaat("VEHICLES"): mutedSoundsArr[i] = "MUTES_VEHICLES_SCENE"; break;
			case Joaat("FOOTSTEPS"): mutedSoundsArr[i] = "MUTES_FOOTSTEPS_SCENE"; break;
			default: mutedSoundsArr[i] = ""; break;
			}
		}
		STOP_AUDIO_SCENES();
		InitializedMuteSounds = true;
	}

	LOOP(i, maxMutedSounds)
	{
		if (mutedSoundsArr[i].empty() || IS_AUDIO_SCENE_ACTIVE(mutedSoundsArr[i].c_str()))
			continue;

		START_AUDIO_SCENE(mutedSoundsArr[i].c_str());
	}
	return;
}

void MuteArtificialAmbientSounds()
{
	// Affects: ambience_general, ambience_music, positioned_radio, ambience_oneshot_vehicles, ambience_industrial, ambience_birds, ambience_speech
	if (!IS_AUDIO_SCENE_ACTIVE("CREATOR_SCENES_AMBIENCE"))
		START_AUDIO_SCENE("CREATOR_SCENES_AMBIENCE");

	return;
}

inline void DisablePlayerPainAudio() { DISABLE_PED_PAIN_AUDIO(GetPlayerPed(), true); return; }
}	//END nAudio

namespace nPeds
{
int DisableScenariosCount = 0;
int DisableScenarioGroupsCount = 0;
void DisableScenarios()
{
	LOOP(i, 10)
	{
		if (IS_SCENARIO_TYPE_ENABLED(Scenarios[DisableScenariosCount]))
			SET_SCENARIO_TYPE_ENABLED(Scenarios[DisableScenariosCount], false);

		DisableScenariosCount++;
		if (DisableScenariosCount >= ScenariosSize)
			DisableScenariosCount = 0;

		if (DOES_SCENARIO_GROUP_EXIST(ScenarioGroups[DisableScenarioGroupsCount]) &&
			IS_SCENARIO_GROUP_ENABLED(ScenarioGroups[DisableScenarioGroupsCount]))
			SET_SCENARIO_GROUP_ENABLED(ScenarioGroups[DisableScenarioGroupsCount], false);

		DisableScenarioGroupsCount++;
		if (DisableScenarioGroupsCount >= ScenarioGroupsSize)
			DisableScenarioGroupsCount = 0;
	}
	return;
}

void DisableWorldPopulation()
{
	SET_PED_POPULATION_BUDGET(0);
	SET_REDUCE_PED_MODEL_BUDGET(0);
	SET_PED_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
	SET_SCENARIO_PED_DENSITY_MULTIPLIER_THIS_FRAME(0.0f, 0.0f);

	SET_VEHICLE_POPULATION_BUDGET(0);
	SET_REDUCE_VEHICLE_MODEL_BUDGET(0);
	SET_RANDOM_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
	SET_PARKED_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
	//SET_AMBIENT_VEHICLE_RANGE_MULTIPLIER_THIS_FRAME(0.0f);

	nGeneral::SetDispatchServices(false);
	SET_RANDOM_BOATS(false);
	SET_RANDOM_TRAINS(false);
	SET_DISABLE_RANDOM_TRAINS_THIS_FRAME(true);
	return;
}
}

Timer timerRefreshIni; //Allow reload every 5s
void RefreshIni()
{
	if (IsKeyJustUp(INI::ReloadIniKey) && timerRefreshIni.Get() > 5000)
	{
		timerRefreshIni.Reset();
		ShowNotification("GameplayFixesV:~n~Reloaded INI");
		ReadINI();
		SetupPedFunctions();
		nHUD::InitializedHideHudComponents = false;
		nAudio::InitializedMuteSounds = false;
	}
	return;
}

void UpdatePlayerOptions()
{
	//Vars that need to be set every frame
	retrievedWeaponThisFrame = false;

	if (INI::EnableCrouching) { nGeneral::EnableCrouching(); }
	if (INI::FriendlyFire) { nGeneral::FriendlyFire(); }
	if (INI::EnableStealthForAllPeds) { nGeneral::EnableStealthForAllPeds(); }
	if (INI::DisableActionMode) { nGeneral::DisableActionMode(); }
	if (INI::DisarmPlayerWhenShot) { nGeneral::DisarmPlayerWhenShot(); }
	if (INI::DropPlayerWeaponWhenRagdolling) { nGeneral::DropPlayerWeaponWhenRagdolling(); }
	if (INI::DynamicallyCleanWoundsAndDirt) { nGeneral::DynamicallyCleanWoundsAndDirt(); }
	if (INI::SprintInsideInteriors) { nGeneral::EnableSprintInsideInteriors(); }
	if (INI::AllowWeaponsInsideSafeHouse) { nGeneral::AllowWeaponsInsideSafeHouse(); }
	if (INI::SilentWanted) { nGeneral::SilentWanted(); }
	
	//////////////////////////////////////Player Controls//////////////////////////////////
	if (INI::DisableAssistedMovement) { nControls::DisableAssistedMovement(); }
	if (INI::ToggleFPSWalking) { nControls::ToggleFPSWalking(); }
	if (INI::DisableCameraAutoCenter > 0) { nControls::DisableCameraAutoCenter(); }
	if (INI::CamFollowVehicleDuringHandbrake) { nControls::CamFollowVehicleDuringHandbrake(); }
	if (INI::DisableFirstPersonView) { nControls::DisableFirstPersonView(); }
	if (INI::DisableIdleCamera) { nControls::DisableIdleCamera(); }
	if (INI::DisableRecording) { nControls::DisableRecording(); }
	if (INI::DisableMobilePhone) { nControls::DisableMobilePhone(); }

	//////////////////////////////////////Player Vehicle///////////////////////////////////
	if (INI::DisableCarMidAirAndRollControl) { nVehicle::DisableCarMidAirAndRollControl(); }
	if (INI::DisableForcedCarExplosionOnImpact) { nVehicle::DisableForcedCarExplosionOnImpact(); }
	if (INI::DisableEngineSmoke) { nVehicle::DisableEngineSmoke(); }
	if (INI::DisableEngineFire) { nVehicle::DisableEngineFire(); }
	if (INI::LeaveEngineOnWhenExitingVehicles) { nVehicle::LeaveEngineOnWhenExitingVehicles(); }
	if (INI::DisableWheelsAutoCenterOnCarExit) { nVehicle::DisableWheelsAutoCenterOnCarExit(); }
	if (INI::KeepCarHydraulicsPosition) { nVehicle::KeepCarHydraulicsPosition(); }
	if (INI::EnableHeliWaterPhysics) { nVehicle::EnableHeliWaterPhysics(); }
	if (INI::DisableRagdollOnVehicleRoof) { nVehicle::DisableRagdollOnVehicleRoof(); }
	if (INI::DisableDragOutCar) { nVehicle::DisableDragOutCar(); }
	if (INI::DisableFlyThroughWindscreen) { nVehicle::DisableFlyThroughWindscreen(); }
	if (INI::DisableBikeKnockOff) { nVehicle::DisableBikeKnockOff(); }
	if (INI::DisableShallowWaterBikeJumpOut) { nVehicle::DisableShallowWaterBikeJumpOut(); }
	if (INI::DisableVehicleJitter) { nVehicle::DisableVehicleJitter(); }
	if (INI::DisableAirVehicleTurbulence) { nVehicle::DisableAirVehicleTurbulence(); }
	if (INI::DisableAutoEquipHelmets > 0) { nVehicle::DisableAutoEquipHelmets(); }
	if (INI::DisableStuntJumps) { nVehicle::DisableStuntJumps(); }

	///////////////////////////////////////////HUD/////////////////////////////////////////
	if (INI::AllowGameExecutionOnPauseMenu) { nHUD::AllowGameExecutionOnPauseMenu(); }
	if (INI::DisableHUDPostFX) { nHUD::DisableHUDPostFX(); }
	if (INI::DisablePauseMenuPostFX) { nHUD::DisablePauseMenuPostFX(); }
	if (INI::DisableSpecialAbilityPostFX) { nHUD::DisableSpecialAbilityPostFX(); }
	if (INI::EnableBigMapToggle) { nHUD::EnableBigMapToggle(); }
	if (INI::MinimapSpeedometer) { nHUD::MinimapSpeedometer(); }
	if (INI::SetRadarZoom >= 0.0f) { nHUD::SetRadarZoom(); }
	if (INI::DisableMinimapTilt) { nHUD::DisableMinimapTilt(); }
	if (INI::HideMinimapFog) { nHUD::HideMinimapFog(); }
	if (INI::HideMinimapSatNav) { nHUD::HideSatNav(); }
	if (INI::HideMinimapDepth) { nHUD::HideMinimapDepth(); }

	if (INI::HideMinimapBars) nHUD::HideMinimapBars();
	else
	{
		if (INI::AlwaysHideAbilityBar) { nHUD::AlwaysHideAbilityBar(); }
		else if (INI::HideAbilityBarForNonMainCharacters) nHUD::HideAbilityBarForNonMainCharacters();
		if (INI::ReplaceArmourBarWithStamina) { nHUD::ReplaceArmourBarWithStamina(); }
	}

	if (INI::HideHudComponents) { nHUD::HideHudComponents(); }
	if (INI::HideWeaponReticle) { nHUD::HideWeaponReticle(); }
	if (INI::HideEnemiesBlips) { nHUD::HideEnemiesBlips(); }

	//////////////////////////////////////////Audio////////////////////////////////////////
	if (INI::DisableWantedMusic) { nAudio::DisableWantedMusic(); }
	if (INI::DisablePoliceScanner) { nAudio::DisablePoliceScanner(); }
	if (INI::DisableFlyingMusic) { nAudio::DisableFlightMusic(); }
	if (INI::DisableRadioInterruptions) { nAudio::SetRadiosMusicOnly(); }
	if (INI::DefaultVehicleRadioOff > 0) { nAudio::DefaultVehicleRadioOff(); }
	if (INI::MuteSounds) { nAudio::MuteSounds(); }
	if (INI::DisablePlayerPainAudio) { nAudio::DisablePlayerPainAudio(); }
	if (INI::MuteArtificialAmbientSounds) { nAudio::MuteArtificialAmbientSounds(); }
	
	//////////////////////////////////////////Peds/////////////////////////////////////////
	if (INI::DisableScenarios) { nPeds::DisableScenarios(); }
	if (INI::DisableWorldPopulation) { nPeds::DisableWorldPopulation(); }
	return;
}