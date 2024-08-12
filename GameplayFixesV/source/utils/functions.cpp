//ScriptHook
#include <natives.h>
// #include <types.h> //Already included in globals.h
//Custom
#include "functions.h"
#include <random>
#include "ini.h"
#include "..\globals.h"

#pragma region Ped Flags
void EnablePedConfigFlag(Ped ped, int flag)
{
	if (!PED::GET_PED_CONFIG_FLAG(ped, flag, false))
		PED::SET_PED_CONFIG_FLAG(ped, flag, true);
	return;
}

void DisablePedConfigFlag(Ped ped, int flag)
{
	if (PED::GET_PED_CONFIG_FLAG(ped, flag, false))
		PED::SET_PED_CONFIG_FLAG(ped, flag, false);
	return;
}

void EnablePedResetFlag(Ped ped, int flag)
{
	if (!PED::GET_PED_RESET_FLAG(ped, flag))
		PED::SET_PED_RESET_FLAG(ped, flag, true);
	return;
}

void DisablePedResetFlag(Ped ped, int flag)
{
	if (PED::GET_PED_RESET_FLAG(ped, flag))
		PED::SET_PED_RESET_FLAG(ped, flag, false);
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
#pragma endregion

#pragma region Misc
bool ArrayContains(int value, const int a[], int n)
{
	int i = 0;
	while (i < n && a[i] != value) i++;
	return i == n ? false : true;
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

	if (!DOES_ENTITY_EXIST(veh)) veh = NULL;
	return veh;
}

Vehicle GetVehiclePedIsEntering(Ped ped)
{
	Vehicle veh = NULL;
	if (GET_PED_RESET_FLAG(ped, PRF_IsEnteringVehicle)) veh = GET_VEHICLE_PED_IS_USING(ped);
	if (!DOES_ENTITY_EXIST(veh)) veh = NULL;
	return veh;
}

Vehicle GetVehiclePedIsExiting(Ped ped)
{
	Vehicle veh = NULL;
	if (GET_IS_TASK_ACTIVE(ped, CODE_TASK_EXIT_VEHICLE)) veh = GET_VEHICLE_PED_IS_USING(ped);
	if (!DOES_ENTITY_EXIST(veh)) veh = NULL;
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
	if (res) { CLEAR_ENTITY_LAST_WEAPON_DAMAGE(ped); /*CLEAR_ENTITY_LAST_DAMAGE_ENTITY(ped);*/ }
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
#pragma endregion

/////////////////////////////////////////////Player//////////////////////////////////////////////
namespace nGeneral	
{
void FriendlyFire()
{
	SET_CAN_ATTACK_FRIENDLY(GetPlayerPed(), true, true);	// Sets PCF_CanAttackFriendly, PCF_AllowLockonToFriendlyPlayers

	Ped ped = NULL;
	SET_SCENARIO_PEDS_TO_BE_RETURNED_BY_NEXT_COMMAND(true);
	if (!GET_CLOSEST_PED(GetPlayerCoords().x, GetPlayerCoords().y, GetPlayerCoords().z, 10.0f, true, true, &ped, false, true, -1))
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

inline void DisableActionMode() { EnablePedResetFlag(GetPlayerPed(), PRF_DisableActionMode); return; }

void DisarmPlayerWhenShot()
{
	Hash weapon = NULL;
	GET_CURRENT_PED_WEAPON(GetPlayerPed(), &weapon, false);
	if (weapon == WEAPON_UNARMED)
		return;

	if ((CanDisarmPed(GetPlayerPed(), true) && GetWeightedBool(70)) ||
		HasEntityBeenDamagedByWeaponThisFrame(GetPlayerPed(), WEAPON_STUNGUN, GENERALWEAPON_TYPE_INVALID))
	{
		if (!IS_AMBIENT_SPEECH_PLAYING(GetPlayerPed()))
			PLAY_PED_AMBIENT_SPEECH_NATIVE(GetPlayerPed(), "GENERIC_CURSE_MED", "SPEECH_PARAMS_FORCE", false);

		SET_PED_DROPS_WEAPON(GetPlayerPed());
	}
	return;
}

inline void EnableSprintInsideInteriors() { EnablePedConfigFlag(GetPlayerPed(), PCF_IgnoreInteriorCheckForSprinting); return; }

bool IsPlayerInsideSafehouse()
{
	constexpr int arrSize = 5;
	int safehouses[arrSize] = {
		GET_INTERIOR_AT_COORDS_WITH_TYPE(GetPlayerCoords().x, GetPlayerCoords().y, GetPlayerCoords().z, "v_franklins"),
		GET_INTERIOR_AT_COORDS_WITH_TYPE(GetPlayerCoords().x, GetPlayerCoords().y, GetPlayerCoords().z, "v_franklinshouse"),
		GET_INTERIOR_AT_COORDS_WITH_TYPE(GetPlayerCoords().x, GetPlayerCoords().y, GetPlayerCoords().z, "v_michael"),
		GET_INTERIOR_AT_COORDS_WITH_TYPE(GetPlayerCoords().x, GetPlayerCoords().y, GetPlayerCoords().z, "v_trailer"),
		GET_INTERIOR_AT_COORDS_WITH_TYPE(GetPlayerCoords().x, GetPlayerCoords().y, GetPlayerCoords().z, "v_trevors")
	};

	int playerInterior = GET_INTERIOR_FROM_ENTITY(GetPlayerPed());
	for (int i = 0; i < arrSize; i++) {
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
			timerLastPlayerWeapon.Set(0);
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
		timerCharacterSwitch.Set(0);
	}
	else if (timerCharacterSwitch.Get() > 250 &&
		(IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_CHARACTER_WHEEL) ||
			IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_SELECT_CHARACTER_MICHAEL) ||
			IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_SELECT_CHARACTER_FRANKLIN) ||
			IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_SELECT_CHARACTER_TREVOR)))
	{
		if (isFakeWanted)
			SetFakeWanted(GetPlayer(), false);

		timerLastPlayerWeapon.Set(0);
		return;
	}

	//Reset weapon timer
	if (!isFakeWanted)
		timerLastPlayerWeapon.Set(0);
	else
		lastPlayerWeapon = GET_SELECTED_PED_WEAPON(GetPlayerPed());

	//If all cheks pass, set fake wanted level
	SetFakeWanted(GetPlayer(), true);
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
		timerToggleFPSWalking.Set(0);
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

	if (!IS_PED_IN_ANY_VEHICLE(GetPlayerPed(), false))
		return;

	if (timePressed > 0)
	{
		if (IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_VEH_HANDBRAKE))
			timerVehCamA.Set(0);
		else if (timerVehCamA.Get() > timePressed && IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_VEH_HANDBRAKE))
		{
			SetCamSmoothHeadingLimit();
			SET_THIRD_PERSON_CAM_RELATIVE_HEADING_LIMITS_THIS_UPDATE(handbrakeCamHeadingMin, handbrakeCamHeadingMax);
		}
		else if (timerVehCamA.Get() > timePressed && IS_CONTROL_JUST_RELEASED(PLAYER_CONTROL, INPUT_VEH_HANDBRAKE))
		{
			timerVehCamB.Set(0);
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

	timerCarExplosion.Set(0);
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
	Hash vehModel = GET_ENTITY_MODEL(veh);
	if (!IS_THIS_MODEL_A_CAR(vehModel) && !IS_THIS_MODEL_A_BIKE(vehModel) && !IS_THIS_MODEL_A_QUADBIKE(vehModel))
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

Timer timerEngineControl;
constexpr int TURN_OFF_ENGINE_DURATION = 250;
void LeaveEngineOnWhenExitingVehicles()
{
	Vehicle veh = GetVehiclePedIsIn(GetPlayerPed(), true, true);
	if (!DOES_ENTITY_EXIST(veh))
		return;

	SET_VEHICLE_KEEP_ENGINE_ON_WHEN_ABANDONED(veh, true);	//Same thing as setting PCF_LeaveEngineOnWhenExitingVehicles
	if (IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_VEH_EXIT))
		timerEngineControl.Set(0);
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

	const Vehicle veh = GetVehiclePedIsIn(GetPlayerPed());
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
		tmpVeh = CREATE_VEHICLE(vehHash, 0.0f, 0.0f, -100.0f, 0.0f, false, false, true);
		SET_ENTITY_AS_MISSION_ENTITY(tmpVeh, false, true);
		SET_ENTITY_VISIBLE(tmpVeh, false, false);
		FREEZE_ENTITY_POSITION(tmpVeh, true);
		return;
	}

	if (GET_IS_TASK_ACTIVE(GetPlayerPed(), CODE_TASK_EXIT_VEHICLE) || timerWheelsAutoCenter.Get() > 2000 || GET_ENTITY_SPEED(veh) > maxVehSpeedDetach)	//CTaskExitVehicle
		shouldAttachVeh = false;

	if (IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_VEH_EXIT) && IS_PED_IN_ANY_VEHICLE(GetPlayerPed(), false)) //&& !IS_VEHICLE_ATTACHED_TO_TRAILER(veh))
		shouldAttachVeh = true;

	if (shouldAttachVeh && !IS_ENTITY_ATTACHED_TO_ENTITY(veh, tmpVeh) && GET_ENTITY_SPEED(veh) < maxVehSpeedDetach)
	{
		timerWheelsAutoCenter.Set(0);

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
		hydraulicsTimer.Set(0);
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

inline void DisableFlyThroughWindscreen() { DisablePedConfigFlag(GetPlayerPed(), PCF_WillFlyThroughWindscreen); return; }
inline void DisableBikeKnockOff() { SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(GetPlayerPed(), KNOCKOFFVEHICLE_NEVER); return; }
inline void DisableDragOutCar() { SET_PED_CAN_BE_DRAGGED_OUT(GetPlayerPed(), false); return; }	//Same thing as setting CPED_CONFIG_FLAG_DontDragMeOutCar

void DisableShallowWaterBikeJumpOut()
{
	if (GET_ENTITY_SUBMERGED_LEVEL(GetPlayerPed()) < 0.7f)
		EnablePedResetFlag(GetPlayerPed(), PRF_DisableShallowWaterBikeJumpOutThisFrame);
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
		timerBigMap.Set(0);
	else if (IS_CONTROL_JUST_RELEASED(FRONTEND_CONTROL, INPUT_HUD_SPECIAL) && timerBigMap.Get() < 200)
	{
		isBigMapActive ^= true;
		SET_BIGMAP_ACTIVE(isBigMapActive, false);
	}
	return;
}

inline void DisableMinimapTilt() { DONT_TILT_MINIMAP_THIS_FRAME(); return; }

inline void HideMinimapFog() { SET_MINIMAP_HIDE_FOW(true); return; }

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
	int staminaPercentage = ROUND(100.0f - GET_PLAYER_SPRINT_STAMINA_REMAINING(GetPlayer()));		//GET_PLAYER_SPRINT_STAMINA_REMAINING goes from 0 to 100 and then healt depletes
	if (INI::MergeHealthAndArmour)
	{
		int health = GET_ENTITY_HEALTH(GetPlayerPed()) - 100 + GET_PED_ARMOUR(GetPlayerPed());			//We need to subtract 100 because the player fatal healt is 100 not 0
		int maxHealth = GET_ENTITY_MAX_HEALTH(GetPlayerPed()) - 100 + GET_PLAYER_MAX_ARMOUR(GetPlayer());
		int newHealthPercentage = ROUND(health * 100.0f / maxHealth);							//Always ensure a 100 offset to fix hud ratio
		int realHealthPercentage = ROUND((GET_ENTITY_HEALTH(GetPlayerPed()) - 100.0f) * 100.0f / (GET_ENTITY_MAX_HEALTH(GetPlayerPed()) - 100.0f));

		//Flash health bar every 400ms if health is below 25%
		if (realHealthPercentage > 25 || timerFlashHealth.Get() > flashHealthInterval)
		{
			if (timerFlashHealth.Get() > (flashHealthInterval * 2))
				timerFlashHealth.Set(0);

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

inline void HideSatNav() { CALL_SCALEFORM_MOVIE_METHOD(RequestMinimapScaleform(), "HIDE_SATNAV"); return; }
inline void HideMinimapDepth() { CALL_SCALEFORM_MOVIE_METHOD(RequestMinimapScaleform(), "HIDE_DEPTH"); return; }
}	//END nHUD

///////////////////////////////////////////////Audio///////////////////////////////////////////////
namespace nAudio
{
inline void DisableWantedMusic() { SET_AUDIO_FLAG("WantedMusicDisabled", true); return; }
inline void DisablePoliceScanner() { SET_AUDIO_FLAG("PoliceScannerDisabled", true); return; }
inline void DisableFlightMusic() { SET_AUDIO_FLAG("DisableFlightMusic", true); return; }

Timer timerRadioMusic;
void SetRadiosMusicOnly()
{
	if (timerRadioMusic.Get() > 10000)
	{
		LOOP(i, RadioStationsSize)
		{ SET_RADIO_STATION_MUSIC_ONLY(RadioStations[i], true); }

		SET_RADIO_STATION_MUSIC_ONLY(GET_PLAYER_RADIO_STATION_NAME(), true);
		timerRadioMusic.Set(0);
	}
	return;
}

Vehicle lastVehRadioOff = NULL;
void DefaultVehicleRadioOff()
{
	Vehicle tmpVeh = GET_VEHICLE_PED_IS_USING(GetPlayerPed());
	if (DOES_ENTITY_EXIST(tmpVeh) && tmpVeh != lastVehRadioOff)
	{
		lastVehRadioOff = tmpVeh;
		if (!GET_IS_VEHICLE_ENGINE_RUNNING(tmpVeh) && GET_PED_RESET_FLAG(GetPlayerPed(), PRF_IsEnteringVehicle))
			SET_VEH_RADIO_STATION(lastVehRadioOff, "OFF");
	}
	return;
}
}	//END nAudio

void UpdatePlayerOptions()
{
	if (INI::FriendlyFire) { nGeneral::FriendlyFire(); }
	if (INI::DisableActionMode) { nGeneral::DisableActionMode(); }
	if (INI::DisarmPlayerWhenShot) { nGeneral::DisarmPlayerWhenShot(); }
	if (INI::SprintInsideInteriors) { nGeneral::EnableSprintInsideInteriors(); }
	if (INI::AllowWeaponsInsideSafeHouse) { nGeneral::AllowWeaponsInsideSafeHouse(); }

	//////////////////////////////////////Player Controls//////////////////////////////////
	if (INI::ToggleFPSWalking) { nControls::ToggleFPSWalking(); }
	if (INI::CamFollowVehicleDuringHandbrake) { nControls::CamFollowVehicleDuringHandbrake(); }
	if (INI::DisableAssistedMovement) { nControls::DisableAssistedMovement(); }
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
	if (INI::DisableRagdollOnVehicleRoof) { nVehicle::DisableRagdollOnVehicleRoof(); }
	if (INI::DisableFlyThroughWindscreen) { nVehicle::DisableFlyThroughWindscreen(); }
	if (INI::DisableBikeKnockOff) { nVehicle::DisableBikeKnockOff(); }
	if (INI::DisableDragOutCar) { nVehicle::DisableDragOutCar(); }
	if (INI::DisableShallowWaterBikeJumpOut) { nVehicle::DisableShallowWaterBikeJumpOut(); }
	if (INI::DisableStuntJumps) { nVehicle::DisableStuntJumps(); }

	///////////////////////////////////////////HUD/////////////////////////////////////////
	if (INI::AllowGameExecutionOnPauseMenu) { nHUD::AllowGameExecutionOnPauseMenu(); }
	if (INI::DisableHUDPostFX) { nHUD::DisableHUDPostFX(); }
	if (INI::DisablePauseMenuPostFX) { nHUD::DisablePauseMenuPostFX(); }
	if (INI::DisableSpecialAbilityPostFX) { nHUD::DisableSpecialAbilityPostFX(); }
	if (INI::EnableBigMapToggle) { nHUD::EnableBigMapToggle(); }
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
	//////////////////////////////////////////Audio////////////////////////////////////////
	if (INI::DisableWantedMusic) { nAudio::DisableWantedMusic(); }
	if (INI::DisablePoliceScanner) { nAudio::DisablePoliceScanner(); }
	if (INI::DisableFlyingMusic) { nAudio::DisableFlightMusic(); }
	if (INI::DisableRadioInterruptions) { nAudio::SetRadiosMusicOnly(); }
	if (INI::DefaultVehicleRadioOff) { nAudio::DefaultVehicleRadioOff(); }
	return;
}