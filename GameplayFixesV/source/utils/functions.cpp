//ScriptHook
#include <natives.h>
//Custom
#include "functions.h"
#include "..\globals.h"
#include "timers.h"

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

bool GetWeightedBool(int chance, int startRange, int endRange)
{
	SET_RANDOM_SEED(GET_GAME_TIMER());
	bool rand = (chance >= GET_RANDOM_INT_IN_RANGE(startRange, endRange));
	return rand;
}

static void FriendlyFire()
{
	PED::SET_CAN_ATTACK_FRIENDLY(playerPed, true, true);

	Ped ped = NULL;
	SET_SCENARIO_PEDS_TO_BE_RETURNED_BY_NEXT_COMMAND(true);
	if (!GET_CLOSEST_PED(playerLoc.x, playerLoc.y, playerLoc.z, 10.0f, true, true, &ped, false, true, -1))
		return;

	int relationship = GET_RELATIONSHIP_BETWEEN_PEDS(playerPed, ped);
	if (relationship == 0 || relationship == 1)
	{
		SET_PED_CAN_RAGDOLL(ped, true);
		SET_PED_CAN_BE_TARGETTED(ped, true);
		SET_PED_CAN_BE_TARGETTED_BY_PLAYER(ped, player, true);
		SET_PED_CAN_BE_TARGETED_WHEN_INJURED(ped, true);
		DisablePedConfigFlag(ped, PCF_NeverEverTargetThisPed);
		CAN_PED_IN_COMBAT_SEE_TARGET(playerPed, ped);
	}
	return;
}

static bool CanDisarmPlayer()
{
	int bone = NULL;
	GET_PED_LAST_DAMAGE_BONE(playerPed, &bone);

	if (bone == NULL)
	{
		CLEAR_PED_LAST_DAMAGE_BONE(playerPed);
		return false;
	}

	switch (bone)
	{
	case SkelRightHand:		case PHRightHand:		case IKRightHand:		case SkelRightFinger00:	case SkelRightFinger01:
	case SkelRightFinger02:	case SkelRightFinger10:	case SkelRightFinger11:	case SkelRightFinger12:	case SkelRightFinger20:
	case SkelRightFinger21:	case SkelRightFinger22:	case SkelRightFinger30:	case SkelRightFinger31:	case SkelRightFinger32:
	case SkelRightFinger40:	case SkelRightFinger41:	case SkelRightFinger42:
	case SkelLeftHand:		case PHLeftHand:		case IKLeftHand:		case SkelLeftFinger00:	case SkelLeftFinger01:
	case SkelLeftFinger02:	case SkelLeftFinger10:	case SkelLeftFinger11:	case SkelLeftFinger12:	case SkelLeftFinger20:
	case SkelLeftFinger21:	case SkelLeftFinger22:	case SkelLeftFinger30:	case SkelLeftFinger31:	case SkelLeftFinger32:
	case SkelLeftFinger40:	case SkelLeftFinger41:	case SkelLeftFinger42:
		CLEAR_PED_LAST_DAMAGE_BONE(playerPed);
		return true;

	default:
		CLEAR_PED_LAST_DAMAGE_BONE(playerPed);
		return false;
	}
}

static void DisarmPlayerWhenShot()
{
	Hash weapon = NULL;
	GET_CURRENT_PED_WEAPON(playerPed, &weapon, false);
	if (weapon == WEAPON_UNARMED)
		return;

	if (HAS_ENTITY_BEEN_DAMAGED_BY_WEAPON(playerPed, WEAPON_STUNGUN, 0) || CanDisarmPlayer())
	{
		if (!IS_AMBIENT_SPEECH_PLAYING(playerPed))
			PLAY_PED_AMBIENT_SPEECH_NATIVE(playerPed, "GENERIC_CURSE_MED", "SPEECH_PARAMS_FORCE", false);

		SET_PED_DROPS_WEAPON(playerPed);
	}
	CLEAR_ENTITY_LAST_WEAPON_DAMAGE(playerPed);
	return;
}

static bool IsPlayerInsideSafehouse(Ped playerPed, Vector3 playerLoc)
{
	constexpr int arrSize = 5;
	int safehouses[arrSize] = {
		GET_INTERIOR_AT_COORDS_WITH_TYPE(playerLoc.x, playerLoc.y, playerLoc.z, "v_franklins"),
		GET_INTERIOR_AT_COORDS_WITH_TYPE(playerLoc.x, playerLoc.y, playerLoc.z, "v_franklinshouse"),
		GET_INTERIOR_AT_COORDS_WITH_TYPE(playerLoc.x, playerLoc.y, playerLoc.z, "v_michael"),
		GET_INTERIOR_AT_COORDS_WITH_TYPE(playerLoc.x, playerLoc.y, playerLoc.z, "v_trailer"),
		GET_INTERIOR_AT_COORDS_WITH_TYPE(playerLoc.x, playerLoc.y, playerLoc.z, "v_trevors")
	};

	int playerInterior = GET_INTERIOR_FROM_ENTITY(playerPed);
	for (int i = 0; i < arrSize; i++) {
		if (safehouses[i] == playerInterior)
			return true;
	}

	//Special check for Trevor's office inside the strip club
	if (GET_ROOM_KEY_FROM_ENTITY(playerPed) == strp3off)	//room key for "strp3off"
		return true;

	return false;
}

static void SetDispatchServices(bool toggle)
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

static bool isFakeWanted = false;
static void SetFakeWanted(Player player, bool toggle)
{
	Vector3 fakeCoords{ 7000.0f, NULL, 7000.0f, NULL, 0.0f, NULL };
	switch (toggle)
	{
	case true:
		SET_MAX_WANTED_LEVEL(5);	//Fix for Menyoo never wanted
		SET_PLAYER_WANTED_LEVEL_NO_DROP(player, 1, true);
		SET_PLAYER_WANTED_LEVEL_NOW(player, true);

		//Various ignore player commands
		SET_PLAYER_WANTED_CENTRE_POSITION(player, &fakeCoords);
		SET_POLICE_IGNORE_PLAYER(player, true);
		SetDispatchServices(false);
		SET_DISPATCH_COPS_FOR_PLAYER(player, false);
		SET_IGNORE_LOW_PRIORITY_SHOCKING_EVENTS(player, true);
		SET_WANTED_LEVEL_DIFFICULTY(player, 0.001f);
		SET_WANTED_LEVEL_MULTIPLIER(0.0f);
		SET_AUDIO_FLAG("PoliceScannerDisabled", true);
		SET_BLOCK_WANTED_FLASH(true);
		FORCE_OFF_WANTED_STAR_FLASH(true);
		HIDE_HUD_COMPONENT_THIS_FRAME(HUD_WANTED_STARS);
		isFakeWanted = true;
		break;
	case false:
		if (GET_PLAYER_WANTED_LEVEL(player) != 0)
			CLEAR_PLAYER_WANTED_LEVEL(player);

		//Reset ignore player commands
		SET_POLICE_IGNORE_PLAYER(player, false);
		SetDispatchServices(true);
		SET_DISPATCH_COPS_FOR_PLAYER(player, true);
		SET_IGNORE_LOW_PRIORITY_SHOCKING_EVENTS(player, false);
		RESET_WANTED_LEVEL_DIFFICULTY(player);
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
static Weapon lastPlayerWeapon = NULL;
static void AllowWeaponsInsideSafeHouse()
{
	//Force selected player weapon for 1000ms upon exit/enter of safehouse - USES TIMERB
	if (TimerB.Get() < 1000 && lastPlayerWeapon != WEAPON_UNARMED)
		SET_CURRENT_PED_WEAPON(playerPed, lastPlayerWeapon, true);

	//Checks if player is inside valid interior
	int playerInterior = GET_INTERIOR_FROM_ENTITY(playerPed);
	if (!IS_VALID_INTERIOR(playerInterior) ||
		//reset fakewanted also if player is inside the strip club but not inside the office
		(GET_INTERIOR_AT_COORDS(113.53f, -1287.61f, 28.64f)  == playerInterior /*v_strip3*/ && GET_ROOM_KEY_FROM_ENTITY(playerPed) != strp3off))
	{
		if (isFakeWanted)
		{
			TimerB.Set(0);
			SetFakeWanted(player, false);
		}
		lastPlayerWeapon = GET_SELECTED_PED_WEAPON(playerPed);
		return;
	}

	//Check if player is inside a safehouse, and not just in a random interior and return if player was already wanted before entering the interior
	if (!IsPlayerInsideSafehouse(playerPed, playerLoc) || (!isFakeWanted && GET_PLAYER_WANTED_LEVEL(player) != 0))
		return;

	//Allow player to switch characters while inside the safehouse
	if (IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_CHARACTER_WHEEL)				||
		IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_SELECT_CHARACTER_MICHAEL)		||
		IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_SELECT_CHARACTER_FRANKLIN)	||
		IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_SELECT_CHARACTER_TREVOR))
	{
		TimerF.Set(0);
	}
	else if (TimerF.Get() > 250 &&
		(IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_CHARACTER_WHEEL)				||
			IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_SELECT_CHARACTER_MICHAEL)	||
			IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_SELECT_CHARACTER_FRANKLIN)	||
			IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_SELECT_CHARACTER_TREVOR)))
	{
		if (isFakeWanted)
			SetFakeWanted(player, false);

		TimerB.Set(0);
		return;
	}

	//Reset weapon timer
	if (!isFakeWanted)
		TimerB.Set(0);
	else
		lastPlayerWeapon = GET_SELECTED_PED_WEAPON(playerPed);

	//If all cheks pass, set fake wanted level
	SetFakeWanted(player, true);
	return;
}

static bool isWalking = false;
static float playerLastMoveBlend = 0.0f;
static void ToggleFPSWalking()
{
	if (!IS_PED_ON_FOOT(playerPed) || !IS_CONTROL_ENABLED(PLAYER_CONTROL, INPUT_SPRINT) ||
		GET_FOLLOW_PED_CAM_VIEW_MODE() != CAM_VIEW_MODE_FIRST_PERSON)
	{
		if (isWalking)
		{
			isWalking = false;
			SET_PED_MAX_MOVE_BLEND_RATIO(playerPed, PEDMOVEBLENDRATIO_SPRINT);
		}
		return;
	}

	//Walk Toggle
	if (IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_SPRINT))
	{
		TimerA.Set(0);
		playerLastMoveBlend = GET_PED_DESIRED_MOVE_BLEND_RATIO(playerPed);
		SET_PED_MAX_MOVE_BLEND_RATIO(playerPed, playerLastMoveBlend);
	}
	else if (TimerA.Get() < 250)
	{
		if (IS_CONTROL_JUST_RELEASED(PLAYER_CONTROL, INPUT_SPRINT))
		{
			if (isWalking)
			{
				SET_PED_MAX_MOVE_BLEND_RATIO(playerPed, PEDMOVEBLENDRATIO_SPRINT);
				isWalking = false;
			}
			else
			{
				SET_PED_MAX_MOVE_BLEND_RATIO(playerPed, PEDMOVEBLENDRATIO_WALK);
				isWalking = true;
			}
		}
		else
			SET_PED_MAX_MOVE_BLEND_RATIO(playerPed, playerLastMoveBlend);
	}
	else if (IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_SPRINT))
	{
		isWalking = false;
		SET_PED_MAX_MOVE_BLEND_RATIO(playerPed, PEDMOVEBLENDRATIO_SPRINT);
	}
	else if (isWalking)
		SET_PED_MAX_MOVE_BLEND_RATIO(playerPed, PEDMOVEBLENDRATIO_WALK);
	return;
}

static float handbrakeCamHeadingMin = 0.0f;
static float handbrakeCamHeadingMax = 0.0f;
static void SetCamSmoothHeadingLimit()
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

static void CamFollowVehicleDuringHandbrake()
{
	const int timePressed = iniCamFollowVehDelay;
	constexpr int delay = 300;

	if (!IS_PED_IN_ANY_VEHICLE(playerPed, false))
		return;

	if (timePressed > 0)
	{
		if (IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_VEH_HANDBRAKE))
			TimerG.Set(0);
		else if (TimerG.Get() > timePressed && IS_CONTROL_PRESSED(PLAYER_CONTROL, INPUT_VEH_HANDBRAKE))
		{
			SetCamSmoothHeadingLimit();
			SET_THIRD_PERSON_CAM_RELATIVE_HEADING_LIMITS_THIS_UPDATE(handbrakeCamHeadingMin, handbrakeCamHeadingMax);
		}
		else if (TimerG.Get() > timePressed && IS_CONTROL_JUST_RELEASED(PLAYER_CONTROL, INPUT_VEH_HANDBRAKE))
		{
			TimerH.Set(0);
			SetCamSmoothHeadingLimit();
			SET_THIRD_PERSON_CAM_RELATIVE_HEADING_LIMITS_THIS_UPDATE(handbrakeCamHeadingMin, handbrakeCamHeadingMax);
		}
		else if (TimerH.Get() < delay)
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

static void DisableRecording()
{
	DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_REPLAY_START_STOP_RECORDING, false);
	DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_REPLAY_START_STOP_RECORDING_SECONDARY, false);
	REPLAY_PREVENT_RECORDING_THIS_FRAME();
	return;
}

static Vehicle lastVeh = NULL;
static void DisableCarMidAirAndRollControl()
{
	Vehicle veh = GET_VEHICLE_PED_IS_IN(playerPed, true);
	if (!DOES_ENTITY_EXIST(veh))
		return;

	//Clean up old veh stuck checks, since the game is limited to ~16 of them
	if (lastVeh != veh && DOES_ENTITY_EXIST(lastVeh) && DOES_VEHICLE_HAVE_STUCK_VEHICLE_CHECK(veh))
		REMOVE_VEHICLE_STUCK_CHECK(veh);

	//Check if veh is car and is driveable then proceed
	constexpr int time = 100;
	Hash vehModel = GET_ENTITY_MODEL(veh);
	if (IS_THIS_MODEL_A_CAR(vehModel) && vehModel != 1483171323 /*deluxo*/)
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

static bool wasSetAsMissionEntity = false;
static void DisableForcedCarExplosionOnImpact()
{
	Vehicle veh = GET_VEHICLE_PED_IS_IN(playerPed, true);
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
		if (TimerD.Get() > 500 && IS_ENTITY_A_MISSION_ENTITY(veh) && wasSetAsMissionEntity)
		{
			wasSetAsMissionEntity = false;
			SET_ENTITY_AS_NO_LONGER_NEEDED(&veh);
		}
		return;
	}

	TimerD.Set(0);
	if (!IS_ENTITY_A_MISSION_ENTITY(veh))
	{
		wasSetAsMissionEntity = true;
		SET_ENTITY_AS_MISSION_ENTITY(veh, true, false);
	}
	return;
}

static void DisableEngineSmoke()
{
	Vehicle veh = GET_VEHICLE_PED_IS_USING(playerPed);
	if (veh == NULL)
		return;

	constexpr float limit = ENGINE_DAMAGE_RADBURST + 50.0f;
	if (GET_VEHICLE_ENGINE_HEALTH(veh) <= limit)
		SET_VEHICLE_ENGINE_HEALTH(veh, limit);

	return;
}

static Vehicle lastVehEngine = NULL;
static void DisableEngineFire()
{
	Vehicle veh = GET_VEHICLE_PED_IS_USING(playerPed);
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

static void DisableRagdollOnVehicleRoof()
{
	//Velocity Unit -> Km/h
	if (iniMaxVehicleSpeed > 0.0f)
	{
		const float speed = GET_ENTITY_SPEED(playerPed) * 3.6f;	// m\s to Km\h
		if (speed > iniMaxVehicleSpeed)
			return;
	}

	EnablePedResetFlag(playerPed, PRF_BlockRagdollFromVehicleFallOff);
	return;
}

static void LeaveEngineOnWhenExitingVehicles()
{
	Vehicle veh = GET_VEHICLE_PED_IS_IN(playerPed, true);
	if (veh == NULL)
		return;

	constexpr int TURN_OFF_ENGINE_DURATION = 250;
	SET_VEHICLE_KEEP_ENGINE_ON_WHEN_ABANDONED(veh, true);
	//EnablePedConfigFlag(playerPed, PCF_LeaveEngineOnWhenExitingVehicles);
	if (IS_DISABLED_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_VEH_EXIT))
		TimerC.Set(0);
	else if (IS_DISABLED_CONTROL_JUST_RELEASED(PLAYER_CONTROL, INPUT_VEH_EXIT) && TimerC.Get() > TURN_OFF_ENGINE_DURATION)
		SET_VEHICLE_ENGINE_ON(veh, false, true, false);
	 
	return;
}

//Very very bad method of doing this, but it works!
//When a vehicle is physically attached to another vehicle it retains the same steering on exit (thanks to RAGE constraints)
//Only problem is that the vehicle now inherits most veh flags from parent. 
//Don't know if this could cause issues, but better then patching memory
static Vehicle tmpVeh = NULL;
static void DisableWheelsAutoCenterOnCarExit()
{
	if (!DOES_ENTITY_EXIST(tmpVeh))
	{
		constexpr Hash caddy2 = -537896628;	//hash for "caddy2", smallest vehicle that did the job. DO NOT USE NON-CAR VEHICLES
		if (!HAS_MODEL_LOADED(caddy2))
		{
			REQUEST_MODEL(caddy2);
			return;
		}
		tmpVeh = CREATE_VEHICLE(caddy2, 0.0f, 0.0f, 0.0f, 0.0f, false, false, true);
		SET_ENTITY_AS_MISSION_ENTITY(tmpVeh, true, true);
		SET_ENTITY_VISIBLE(tmpVeh, false, false);
		FREEZE_ENTITY_POSITION(tmpVeh, true);
		return;
	}

	Vehicle veh = GET_VEHICLE_PED_IS_USING(playerPed);
	if (!DOES_ENTITY_EXIST(veh))
		return;

	//Disable for bikes and bicycles, can cause issues
	Hash vehModel = GET_ENTITY_MODEL(veh);
	if (!IS_THIS_MODEL_A_CAR(vehModel) && !IS_THIS_MODEL_A_QUADBIKE(vehModel))
		return;

	if (IS_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_VEH_EXIT) && IS_PED_IN_ANY_VEHICLE(playerPed, false) && !IS_VEHICLE_ATTACHED_TO_TRAILER(veh) && GET_ENTITY_SPEED(veh) < 7.0f)
	{
		TimerE.Set(0);
		ATTACH_ENTITY_TO_ENTITY_PHYSICALLY(veh, tmpVeh, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, true, 2);
		return;
	}

	//Detach veh after 1500ms 
	if (IS_ENTITY_ATTACHED_TO_ENTITY(veh, tmpVeh) && TimerE.Get() > 1500)
	{
		DETACH_ENTITY(veh, false, false);
		DETACH_ENTITY(tmpVeh, false, false);
		return;
	}
	return;
}

static void DisableShallowWaterBikeJumpOut()
{
	if (GET_ENTITY_SUBMERGED_LEVEL(playerPed) < 0.9f)
		EnablePedResetFlag(playerPed, PRF_DisableShallowWaterBikeJumpOutThisFrame);
	return;
}

void SetPlayerFlags()
{
	if (iniFriendlyFire)
		FriendlyFire();

	if (iniPlayerCanJackFriendlyPeds)
		EnablePedConfigFlag(playerPed, PCF_PlayerCanJackFriendlyPlayers);

	if (iniDisarmPlayerWhenShot)
		DisarmPlayerWhenShot();

	if (iniSprintInsideInteriors)
		EnablePedConfigFlag(playerPed, PCF_IgnoreInteriorCheckForSprinting);

	if (iniAllowWeaponsInsideSafeHouse)
		AllowWeaponsInsideSafeHouse();

	//////////////////////////////////////Player Controls//////////////////////////////////
	if (iniToggleFPSWalking)
		ToggleFPSWalking();

	if (iniCamFollowVehicleDuringHandbrake)
		CamFollowVehicleDuringHandbrake();

	if (iniDisableRecording)
		DisableRecording();

	if (iniDisableMobilePhone)
		DESTROY_MOBILE_PHONE();

	//////////////////////////////////////Player Vehicle///////////////////////////////////
	if (iniDisableCarMidAirAndRollControl)
		DisableCarMidAirAndRollControl();

	if (iniDisableForcedCarExplosionOnImpact)
		DisableForcedCarExplosionOnImpact();

	if (iniDisableEngineSmoke)
		DisableEngineSmoke();

	if (iniDisableEngineFire)
		DisableEngineFire();

	if (iniLeaveEngineOnWhenExitingVehicles)
		LeaveEngineOnWhenExitingVehicles();

	if (iniDisableWheelsAutoCenterOnCarExit)
		DisableWheelsAutoCenterOnCarExit();

	if (iniDisableRagdollOnVehicleRoof)
		DisableRagdollOnVehicleRoof();

	if (iniDisableShallowWaterBikeJumpOut)
		DisableShallowWaterBikeJumpOut();

	if (iniDisableStuntJumps)
		SET_STUNT_JUMPS_CAN_TRIGGER(false);

	return;
}