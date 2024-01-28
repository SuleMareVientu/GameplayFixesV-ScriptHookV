//ScriptHook
#include <natives.h>
//Custom
#include "functions.h"
#include "..\globals.h"
#include "timers.h"

static bool isFakeWanted = false;
static Weapon lastPlayerWeapon = NULL;
static Vehicle lastVeh = NULL;
static Vehicle tmpVeh = NULL;
static bool wasSetAsMissionEntity = false;

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

static bool IsPlayerInsideSafehouse(Ped playerPed, Vector3 playerLoc)
{
	const int arrSize = 5;
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
static void AllowWeaponsInsideSafeHouse()
{
	//Force selected player weapon for 1000ms upon exit/enter of safehouse - USES TIMERB
	if (TIMERB() < 1000 && lastPlayerWeapon != WEAPON_UNARMED)
		SET_CURRENT_PED_WEAPON(playerPed, lastPlayerWeapon, true);

	//Checks if player is inside valid interior
	int playerInterior = GET_INTERIOR_FROM_ENTITY(playerPed);
	if (!IS_VALID_INTERIOR(playerInterior) ||
		//reset fakewanted also if player is inside the strip club but not inside the office
		(GET_INTERIOR_AT_COORDS(113.53f, -1287.61f, 28.64f)  == playerInterior /*v_strip3*/ && GET_ROOM_KEY_FROM_ENTITY(playerPed) != strp3off))
	{
		if (isFakeWanted)
		{
			SetFakeWanted(player, false);
			SETTIMERB(0);
		}
		lastPlayerWeapon = GET_SELECTED_PED_WEAPON(playerPed);
		return;
	}

	//Check if player is inside a safehouse, and not just in a random interior and return if player was already wanted before entering the interior
	if (!IsPlayerInsideSafehouse(playerPed, playerLoc) || (!isFakeWanted && GET_PLAYER_WANTED_LEVEL(player) != 0))
		return;

	//Reset weapon timer
	if (!isFakeWanted)
		SETTIMERB(0);
	else
		lastPlayerWeapon = GET_SELECTED_PED_WEAPON(playerPed);

	//If all cheks pass, set fake wanted level
	SetFakeWanted(player, true);
	return;
}

static void DisableCarMidAirAndRollControl()
{
	Vehicle veh = GET_VEHICLE_PED_IS_IN(playerPed, true);

	if (!DOES_ENTITY_EXIST(veh))
		return;

	//Clean up old veh stuck checks
	if (lastVeh != veh && DOES_ENTITY_EXIST(lastVeh) && DOES_VEHICLE_HAVE_STUCK_VEHICLE_CHECK(veh))
		REMOVE_VEHICLE_STUCK_CHECK(veh);

	//Check if veh is car and is driveable then proceed
	const int time = 100;
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
			DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_VEH_MOVE_UP_ONLY, false);
			DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_VEH_MOVE_DOWN_ONLY, false);
			DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_VEH_MOVE_LEFT_ONLY, false);
			DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_VEH_MOVE_RIGHT_ONLY, false);
			DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_VEH_MOVE_LEFT, false);
			DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_VEH_MOVE_RIGHT, false);
			DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_VEH_MOVE_UP, false);
			DISABLE_CONTROL_ACTION(PLAYER_CONTROL, INPUT_VEH_MOVE_DOWN, false);
		}
	}
	return;
}

static void DisableForcedCarExplosionOnImpact()
{
	Vehicle veh = GET_VEHICLE_PED_IS_IN(playerPed, true);
	if (!DOES_ENTITY_EXIST(veh))
		return;

	//Check if vehicle is boat/car and if it's in the air, else set it as not a mission entity
	Hash vehModel = GET_ENTITY_MODEL(veh);
	if (!IS_ENTITY_IN_AIR(veh) || GET_ENTITY_SPEED(veh) < 10.0f || (!IS_THIS_MODEL_A_CAR(vehModel) && !IS_THIS_MODEL_A_BOAT(vehModel)))
	{
		//Wait 500ms before setting vehicle as no longer needed
		if (TIMERD() > 500 && IS_ENTITY_A_MISSION_ENTITY(veh) && wasSetAsMissionEntity)
		{
			wasSetAsMissionEntity = false;
			SET_VEHICLE_AS_NO_LONGER_NEEDED(&veh);
			SET_ALLOW_VEHICLE_EXPLODES_ON_CONTACT(veh, true);
		}
		return;
	}

	SETTIMERD(0);
	if (!IS_ENTITY_A_MISSION_ENTITY(veh))
	{
		wasSetAsMissionEntity = true;
		SET_ENTITY_AS_MISSION_ENTITY(veh, true, false);
		SET_ALLOW_VEHICLE_EXPLODES_ON_CONTACT(veh, false);
	}
	return;
}

static void DisableRagdollOnVehicleRoof()
{
	//Velocity Unit -> Km/h
	if (iniMaxVehicleSpeed > 0.0f)
	{
		float speed = GET_ENTITY_SPEED(playerPed) * 3.6f;
		if (speed > iniMaxVehicleSpeed)
			DisablePedResetFlag(playerPed, PRF_BlockRagdollFromVehicleFallOff);
		else
			EnablePedResetFlag(playerPed, PRF_BlockRagdollFromVehicleFallOff);
	}
	else
		EnablePedResetFlag(playerPed, PRF_BlockRagdollFromVehicleFallOff);
	return;
}

static void LeaveEngineOnWhenExitingVehicles()
{
	Vehicle veh = GET_VEHICLE_PED_IS_USING(playerPed);
	if (!DOES_ENTITY_EXIST(veh))
		return;

	const int timeToEnableAction = 200;
	EnablePedConfigFlag(playerPed, PCF_LeaveEngineOnWhenExitingVehicles);
	if (IS_DISABLED_CONTROL_JUST_PRESSED(PLAYER_CONTROL, INPUT_VEH_EXIT) && IS_PED_IN_ANY_VEHICLE(playerPed, false))
		SETTIMERC(0);
	else if (IS_DISABLED_CONTROL_JUST_RELEASED(PLAYER_CONTROL, INPUT_VEH_EXIT) && TIMERC() < timeToEnableAction)
	{
		SETTIMERC(0);
		SET_VEHICLE_ENGINE_ON(veh, false, true, false);
	}
	return;
}

//Very very bad method of doing this, but it works!
//When a vehicle is physically attached to another vehicle it retains the same steering on exit (thanks to RAGE constraints)
//Only problem is that the vehicle now inherits most veh flags from parent. 
//Don't know if this could cause issues, but better then patching memory
static void DisableWheelsAutoCenterOnCarExit()
{
	if (!DOES_ENTITY_EXIST(tmpVeh))
	{
		const Hash caddy2 = -537896628;	//hash for "caddy2", smallest vehicle that did the job. DO NOT USE NON-CAR VEHICLES
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
		SETTIMERE(0);
		ATTACH_ENTITY_TO_ENTITY_PHYSICALLY(veh, tmpVeh, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, true, 2);
		return;
	}

	//Detach veh after 1500ms 
	if (IS_ENTITY_ATTACHED_TO_ENTITY(veh, tmpVeh) && TIMERE() > 1500)
	{
		DETACH_ENTITY(veh, false, false);
		DETACH_ENTITY(tmpVeh, false, false);
		return;
	}
	return;
}

void SetPlayerFlags()
{
	if (iniFriendlyFire)
		PED::SET_CAN_ATTACK_FRIENDLY(playerPed, true, true);

	if (iniPlayerCanJackFriendlyPeds)
		EnablePedConfigFlag(playerPed, PCF_PlayerCanJackFriendlyPlayers);

	if (iniSprintInsideInteriors)
		EnablePedConfigFlag(playerPed, PCF_IgnoreInteriorCheckForSprinting);

	if (iniAllowWeaponsInsideSafeHouse)
		AllowWeaponsInsideSafeHouse();

	if (iniDisableCarMidAirAndRollControl)
		DisableCarMidAirAndRollControl();

	if (iniDisableForcedCarExplosionOnImpact)
		DisableForcedCarExplosionOnImpact();

	if (iniLeaveEngineOnWhenExitingVehicles)
		LeaveEngineOnWhenExitingVehicles();

	if (iniDisableWheelsAutoCenterOnCarExit)
		DisableWheelsAutoCenterOnCarExit();

	if (iniDisableRagdollOnVehicleRoof)
		DisableRagdollOnVehicleRoof();

	if (iniDisableShallowWaterBikeJumpOut)
	{
		if (GET_ENTITY_SUBMERGED_LEVEL(playerPed) < 0.9f)
			EnablePedResetFlag(playerPed, PRF_DisableShallowWaterBikeJumpOutThisFrame);
	}
	return;
}