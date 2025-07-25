#pragma once
#include <shv\types.h>
#include <string>
#include <memory>
#include <vector>
#include <nm_globals.h>

//Json
#include <windows.h>
#include "..\res\resource.h"
#include <libs\json.hpp>
using json = nlohmann::json;

#pragma region MACROS

#define VER_MAX 3
#define VER_MIN 0
#define LOOP(i, n) for(int i = 0; i < n; ++i)

#pragma endregion

#pragma region Global Functions
constexpr unsigned int Joaat(const char* str)
{
	if (str == nullptr || *str == '\0')
		return 0;

	unsigned int hash = 0;
	for (unsigned long long i = 0; str[i] != '\0'; ++i)
	{
		unsigned char b = static_cast<unsigned char>(str[i]);
		if (b >= 'A' && b <= 'Z')
			b = b | 1 << 5; // Convert to lowercase
		else if (b == '\\')
			b = '/';

		hash += b;
		hash += hash << 10;
		hash ^= hash >> 6;
	}

	// Finalize hash
	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;
	return hash;
};

HINSTANCE GetDllInstance();
const char* GetDllInstanceName();
const char* GetDllInstanceNameNoExt();
const char* GetDllInstanceIniName();
const char* GetDllInstanceLogName();
int GetGameVersion();
bool GetIsEnhancedVersion();

bool GetFoundNMFunctions();
bool GetPatchedCenterSteering();
bool GetPatchedHUDWheelSlowdown();
bool GetIsPlayerCrouching();
int GetNMReactionTime();
#pragma endregion

#pragma region Custom Types and Structures
union scrValue
{
	int Int;
	unsigned int Uns;
	float Float;
	const char* String;
	scrValue* Reference;
	unsigned long long Any;
	bool operator==(const scrValue &val) { return Int == val.Int; }
};

typedef struct { int R, G, B, A; } RGBA;

typedef std::unique_ptr<unsigned char[]> NmMessage;
typedef unsigned char* NmMessagePtr;

// Json stuff
struct WpTintJson {
	int Index = -1;
};

struct WpComponentJson {
	std::string Name = "UNK";
	Hash Bone = Joaat("UNK");
};

struct WpLiveryJson {
	std::string Name = "UNK";
};

struct WeaponJson {
	std::string Name = "UNK";
	std::string Category = "UNK";
	std::string AmmoType = "UNK";
	std::string DamageType = "UNK";
	std::vector<WpTintJson> Tints = {};
	std::vector<WpComponentJson> Components = {};
	std::vector<WpLiveryJson> Liveries = {};
	bool operator==(const std::string& n) const { return (Name == n); }
	bool operator==(const Hash& h) const { return (Joaat(Name.c_str()) == h); }
};

extern bool hasWeaponJsonLoaded;
extern std::vector<WeaponJson> weaponInfo;

struct WpPickupLivery {
	Hash LiveryHash = 0;
	int TintIndex = -1;
};

struct WeaponPickup {
	Hash WpHash = 0;
	Pickup PickupIndex = -1;
	Blip PickupBlip = 0;
	int TintIndex = -1;
	int CamoIndex = -1;
	std::vector<Hash> Components = {};
	std::vector<WpPickupLivery> Liveries = {};
	bool operator==(const Hash& h) const { return (WpHash == h); }
};

extern std::vector<WeaponPickup> droppedWeapons;
#pragma endregion

#pragma region Constants
constexpr Vector3 nullVec{ NULL, NULL, NULL };

//Hashes
constexpr unsigned int FE_MENU_VERSION_SP_PAUSE = Joaat("FE_MENU_VERSION_SP_PAUSE");
constexpr unsigned int strp3off = Joaat("strp3off");
constexpr unsigned int WEAPON_UNARMED = Joaat("WEAPON_UNARMED");
constexpr unsigned int WEAPON_STUNGUN = Joaat("WEAPON_STUNGUN");

constexpr unsigned int MichaelPed = Joaat("Player_Zero");
constexpr unsigned int FranklinPed = Joaat("Player_One");
constexpr unsigned int TrevorPed = Joaat("Player_Two");

//Ped move blend ratios
constexpr float PEDMOVEBLENDRATIO_STILL = 0.0f;
constexpr float PEDMOVEBLENDRATIO_WALK = 1.0f;
constexpr float PEDMOVEBLENDRATIO_RUN = 2.0f;
constexpr float PEDMOVEBLENDRATIO_SPRINT = 3.0f;

//Vehicle engine values	
constexpr float ENGINE_HEALTH_MAX = 1000.0f;
constexpr float ENGINE_DAMAGE_PLANE_DAMAGE_START = 900.0f;
constexpr float ENGINE_DAMAGE_RADBURST = 400.0f;
constexpr float ENGINE_DAMAGE_OIL_LEAKING = 200.0f;
constexpr float ENGINE_DAMAGE_ONFIRE = 0.0f;
constexpr float ENGINE_DAMAGE_FIRE_FULL = -700.0f;
constexpr float ENGINE_DAMAGE_FX_FADE = -3200.0f;
constexpr float ENGINE_DAMAGE_FIRE_FINISH = -3600.0f;
constexpr float ENGINE_DAMAGE_FINSHED = -4000.0f;
constexpr float ENGINE_DAMAGE_CAP_BY_MELEE = 100.0f;
#pragma endregion

#pragma region Ped Enums
enum ePedFlag {
	//Ped Config Flags
	PCF_NoCriticalHits = 2,							// ped cannot be killed by a single bullet
	PCF_NeverEverTargetThisPed = 9,					// script control over player targeting
	PCF_NotAllowedToCrouch = 22,					// Is this ped allowed to crouch at all?
	PCF_ForceDieIfInjured = 25,						// script command so missions peds die if injured
	PCF_ForceDieInCar = 28,							// don't fall out car if killed
	PCF_WillFlyThroughWindscreen = 32,				// Ped will fly through the vehicle windscreen upon a forward impact at high velocity
	PCF_DieWhenRagdoll = 33,
	PCF_DisablePlayerLockon = 43,
	PCF_AllowLockonToFriendlyPlayers = 45,
	PCF_IsSitting = 68,								// is the ped sitting
	PCF_IsAimingGun = 78,							// Is performing an aim task
	PCF_UsingCrouchedPedCapsule = 99,				// Set to indicate that the ped's bounds are in the crouched configuration
	PCF_OpenDoorArmIK = 104,						// Set if the ped should enable open door arm IK
	PCF_UNUSED_REPLACE_ME = 125,
	PCF_CanAttackFriendly = 140,					// True allows this ped to attack peds theya re friendly with
	PCF_IsInjured = 166,							// When true, the ped will use injured movement anim sets and getup animations.
	PCF_PreventAllMeleeTaunts = 169,				// Disable all melee taunts for this particular ped
	PCF_EnableWeaponBlocking = 186,					// When enabled, Non-player peds can use WeaponBlocking behaviors
	PCF_HasHurtStarted = 187,
	PCF_DisableHurt = 188,							// Will prevent the peds go into hurt combat mode
	PCF_LeaveEngineOnWhenExitingVehicles = 241,		// The player will leave the engine running when leaving vehicles
	PCF_PhoneDisableTextingAnimations = 242,		// tells taskmobile phone to not texting animations.  Currently don't play these in MP
	PCF_PhoneDisableTalkingAnimations = 243,		// tells taskmobile phone to not talking animations.  Currently don't play these in MP
	PCF_PhoneDisableCameraAnimations = 244,			// tells taskmobile phone to not camera animations.  Currently don't play these in MP
	PCF_PlayerCanJackFriendlyPlayers = 252,			// If a friendly player is driving the vehicle, if the player taps to enter, they will enter as a passenger, if they hold, they'll jack the driver
	PCF_AllowPlayerLockOnIfFriendly = 266,			// If this ped is friendly with the player, this will allow the ped to lockon
	PCF_DisableGoToWritheWhenInjured = 281,			// If set, CPed::DAMAGED_GOTOWRITHE will no longer get set.  In particular, tazer hits wil no longer kill this ped in one hit.
	PCF_DisableWritheShootFromGround = 327,			// If set, the ped will no longer shoot while writhing.
	PRF_DontUseSprintEnergy = 353,					// Player does not get tired when sprinting
	PCF_DisableAutoEquipHelmetsInBikes = 380,		// Prevents ped from auto-equipping helmets when entering a bike (includes quadbikes)
	PCF_DisableAutoEquipHelmetsInAicraft = 381,		// Prevents ped from auto-equipping helmets when entering an aircraft
	PCF_BlockAutoSwapOnWeaponPickups = 416,			// Block auto weapon swaps for weapon pickups.
	PCF_IgnoreInteriorCheckForSprinting = 427,		// When set, player will be able to sprint inside interriors even if it is tagged to prevent it.

	//Ped Reset Flags
	PRF_Wandering = 10,
	PRF_IsAiming = 27,								// TASK_GUN or TASK_USE_COVER
	PRF_DisablePlayerLockon = 43,
	PRF_DisablePlayerJumping = 46,					// Disables player jumping if true. Reset in ResetPostPhysics, as it will be set via script
	PRF_DisablePlayerVaulting = 47,					// Disables player vaulting/climbing if true.
	PRF_ScriptDisableSecondaryAnimationTasks = 58,
	PRF_WanderingStoppedForOtherPed = 61,
	PRF_NotAllowedToChangeCrouchState = 68,			// if set the ped will not be allowed to change their crouch state
	PRF_DisableCrouchWhileInCover = 88,				// Force the crouch flag to return true while in cover.
	PRF_DisablePlayerAutoVaulting = 102,
	PRF_DisableDynamicCapsuleRadius = 121,			// Turn off dynamic adjustments to ped capsules
	PRF_HurtThisFrame = 127,						// The ped has entered the hurt state this frame
	PRF_ExpandPedCapsuleFromSkeleton = 129,			// Set the ped capsule radius based on skeleton
	PRF_ShootFromGround = 140,
	PRF_IsEnteringOrExitingVehicle = 152,			// TASK_ENTER_VEHICLE or TASK_EXIT_VEHICLE
	PRF_HasGunTaskWithAimingState = 154,			// Ped is running TASK_GUN and task's state is State_Aim
	PRF_MakeHeadInvisible = 166,					// If set, scale the head of the ped to 0.001
	PRF_DisableDropDowns = 195,						// Disable drop downs for this ped
	PRF_IsExitingVehicle = 199,						// TASK_EXIT_VEHICLE
	PRF_DisableActionMode = 200,					// Disable combat anims for ped.
	PRF_IsEnteringVehicle = 230,					// TASK_ENTER_VEHICLE
	PRF_BlockRagdollFromVehicleFallOff = 274,		// Disables ped from ragdolling while ot top of vehicles
	PRF_DisableDustOffAnims = 334,					// Disable ambient dust off animations
	PRF_IsInVehicleChase = 338,
	PRF_RemoveHelmet = 367,							// Forces a ped to remove its helmet.
	PRF_IsRemovingHelmet = 368,						// Returns true if ped is removing its helmet.
	PRF_NeverRagdoll = 370,							// Disable all attempts by this ped to ragdoll.
	PRF_DisableWallHitAnimation = 371,				// Disable stuck wall hit animation for the ped this frame.
	PRF_SuppressTakedownMeleeActions = 438,			// This will suppress all takedown melee actions (RA_IS_TAKEDOWN or RA_IS_STEALTH_KILL, defined in action_table.meta)
	PRF_DisableShallowWaterBikeJumpOutThisFrame = 445
};

enum ePedMotionState {
	MS_NONE = 0xEE717723,
	MS_ON_FOOT_IDLE = 0x9072A713,			// The standing idle pose for on foot movement
	MS_ON_FOOT_WALK = 0xD827C3DB,			// Walking straight forward in on foot movement
	MS_ON_FOOT_RUN = 0xFFF7E7A4,			// Running straight forward in on foot movement
	MS_ON_FOOT_SPRINT = 0xBD8817DB,			// Sprinting straight forward in on foot movement
	MS_CROUCH_IDLE = 0x43FB099E,			// The standing idle for crouching movement
	MS_CROUCH_WALK = 0x08C31A98,			// walking straight forward whilst crouching
	MS_CROUCH_RUN = 0x3593CF09,				// running straight forward whilst crouching
	MS_DO_NOTHING = 0x0EC17E58,
	MS_ANIMATEDVELOCITY = 0x551AAC43,
	MS_INVEHICLE = 0x94D9D58D,
	MS_AIMING = 0x3F67C6AF,					// aiming
	MS_DIVING_IDLE = 0x4848CDED,			// Idling whilst swimming underwater
	MS_DIVING_SWIM = 0x916E828C,			// swimming forwards whilst swimming underwater
	MS_SWIMMING_TREADWATER = 0xD1BF11C7,
	MS_DEAD = 0x0DBB071C,
	MS_STEALTHMODE_IDLE = 0x422D7A25,
	MS_STEALTHMODE_WALK = 0x042AB6A2,
	MS_STEALTHMODE_RUN = 0xFB0B79E1,
	MS_PARACHUTING = 0xBAC0F10B,			// parachuting
	MS_ACTIONMODE_IDLE = 0xDA40A0DC,
	MS_ACTIONMODE_WALK = 0xD2905EA7,
	MS_ACTIONMODE_RUN = 0x31BADE14,
	MS_JETPACK = 0x535E6A5E
};

enum ePedtype {
	PEDTYPE_INVALID = -1,
	PEDTYPE_PLAYER1 = 0,			// Michael
	PEDTYPE_PLAYER2,            // Franklin
	PEDTYPE_PLAYER_NETWORK,     // Player controlled over the network (not by this machine)
	PEDTYPE_PLAYER_UNUSED,		// Trevor
	PEDTYPE_CIVMALE,
	PEDTYPE_CIVFEMALE,
	PEDTYPE_COP,
	PEDTYPE_GANG1,
	PEDTYPE_GANG2,
	PEDTYPE_GANG3,
	PEDTYPE_GANG4,
	PEDTYPE_GANG5,
	PEDTYPE_GANG6,
	PEDTYPE_GANG7,
	PEDTYPE_GANG8,
	PEDTYPE_GANG9,
	PEDTYPE_GANG10,
	PEDTYPE_GANG_CHINESE_JAPANESE,
	PEDTYPE_GANG_PUERTO_RICAN,
	PEDTYPE_DEALER,
	PEDTYPE_MEDIC,
	PEDTYPE_FIRE,
	PEDTYPE_CRIMINAL,
	PEDTYPE_BUM,
	PEDTYPE_PROSTITUTE,
	PEDTYPE_SPECIAL,
	PEDTYPE_MISSION,
	PEDTYPE_SWAT,
	PEDTYPE_ANIMAL,
	PEDTYPE_ARMY,
	PEDTYPE_LAST_PEDTYPE
};

enum eRelGroupHash {
	RELGROUPHASH_PLAYER = 1862763509,
	RELGROUPHASH_CIVMALE = 45677184,
	RELGROUPHASH_CIVFEMALE = 1191392768,
	RELGROUPHASH_COP = -1533126372,
	RELGROUPHASH_HATES_PLAYER = -2065892691,
	RELGROUPHASH_AMBIENT_GANG_LOST = -1865950624,
	RELGROUPHASH_AMBIENT_GANG_MEXICAN = 296331235,
	RELGROUPHASH_AMBIENT_GANG_FAMILY = 1166638144,
	RELGROUPHASH_AMBIENT_ARMY = -472287501,
	RELGROUPHASH_SECURITY_GUARD = -183807561,
	RELGROUPHASH_AMBIENT_GANG_MARABUNTE = 2037579709,
	RELGROUPHASH_AMBIENT_GANG_CULT = 2017343592,
	RELGROUPHASH_AMBIENT_GANG_SALVA = -1821475077,
	RELGROUPHASH_AMBIENT_GANG_WEICHENG = 1782292358,
	RELGROUPHASH_AMBIENT_GANG_BALLAS = -1033021910,
	RELGROUPHASH_AMBIENT_GANG_HILLBILLY = -1285976420,
	RELGROUPHASH_AGGRESSIVE_INVESTIGATE = -347613984,
	RELGROUPHASH_NO_RELATIONSHIP = -86095805,
	RELGROUPHASH_SHARK = 580191176,
	RELGROUPHASH_PRIVATE_SECURITY = -1467815081,
	RELGROUPHASH_CAT = 1157867945
};

enum eRagdollBlockingFlags
{
	RBF_NONE = 0,
	RBF_BULLET_IMPACT = 1,
	RBF_VEHICLE_IMPACT = 2,
	RBF_FIRE = 4,
	RBF_ELECTROCUTION = 8,
	RBF_PLAYER_IMPACT = 16,
	RBF_EXPLOSION = 32,
	RBF_IMPACT_OBJECT = 64,
	RBF_MELEE = 128,
	RBF_RUBBER_BULLET = 256,
	RBF_FALLING = 512,
	RBF_WATER_JET = 1024,
	RBF_DROWNING = 2048,
	RBF_ALLOW_BLOCK_DEAD_PED = 4096,
	RBF_PLAYER_BUMP = 8192,
	RBF_PLAYER_RAGDOLL_BUMP = 16384,
	RBF_PED_RAGDOLL_BUMP = 32768,
	RBF_VEHICLE_GRAB = 65536
};
constexpr int RAGDOLL_BLOCKING_FLAGS_ALL = (RBF_BULLET_IMPACT | RBF_VEHICLE_IMPACT | RBF_FIRE |
	RBF_ELECTROCUTION | RBF_PLAYER_IMPACT | RBF_EXPLOSION | RBF_IMPACT_OBJECT | RBF_MELEE |
	RBF_RUBBER_BULLET | RBF_FALLING | RBF_WATER_JET | RBF_DROWNING | RBF_ALLOW_BLOCK_DEAD_PED |
	RBF_PLAYER_BUMP | RBF_PLAYER_RAGDOLL_BUMP | RBF_PED_RAGDOLL_BUMP | RBF_VEHICLE_GRAB);

enum eRelationshipType
{
	ACQUAINTANCE_TYPE_PED_NONE = 255,
	ACQUAINTANCE_TYPE_PED_RESPECT = 0,
	ACQUAINTANCE_TYPE_PED_LIKE,
	ACQUAINTANCE_TYPE_PED_IGNORE,
	ACQUAINTANCE_TYPE_PED_DISLIKE,
	ACQUAINTANCE_TYPE_PED_WANTED,
	ACQUAINTANCE_TYPE_PED_HATE,
	ACQUAINTANCE_TYPE_PED_DEAD
};

enum ePedDecorationZone {
	PDZ_TORSO = 0,
	PDZ_HEAD,
	PDZ_LEFT_ARM,
	PDZ_RIGHT_ARM,
	PDZ_LEFT_LEG,
	PDZ_RIGHT_LEG,
	PDZ_MEDALS,
	PDZ_INVALID
};

enum ePedBoneTag {
	BONETAG_NULL = -1,

	BONETAG_ROOT = 0,
	BONETAG_PELVIS = 0x2E28,
	BONETAG_SPINE = 0x5C01,
	BONETAG_SPINE1 = 0x60F0,
	BONETAG_SPINE2 = 0x60F1,
	BONETAG_SPINE3 = 0x60F2,
	BONETAG_NECK = 0x9995,
	BONETAG_HEAD = 0x796E,
	BONETAG_R_CLAVICLE = 0x29D2,
	BONETAG_R_UPPERARM = 0x9D4D,
	BONETAG_R_FOREARM = 0x6E5C,
	BONETAG_R_HAND = 0xDEAD,
	BONETAG_R_FINGER0 = 0xE5F2,
	BONETAG_R_FINGER01 = 0xFA10,
	BONETAG_R_FINGER02 = 0xFA11,
	BONETAG_R_FINGER1 = 0xE5F3,
	BONETAG_R_FINGER11 = 0xFA60,
	BONETAG_R_FINGER12 = 0xFA61,
	BONETAG_R_FINGER2 = 0xE5F4,
	BONETAG_R_FINGER21 = 0xFA70,
	BONETAG_R_FINGER22 = 0xFA71,
	BONETAG_R_FINGER3 = 0xE5F5,
	BONETAG_R_FINGER31 = 0xFA40,
	BONETAG_R_FINGER32 = 0xFA41,
	BONETAG_R_FINGER4 = 0xE5F6,
	BONETAG_R_FINGER41 = 0xFA50,
	BONETAG_R_FINGER42 = 0xFA51,

	BONETAG_L_CLAVICLE = 0xFCD9,
	BONETAG_L_UPPERARM = 0xB1C5,
	BONETAG_L_FOREARM = 0xEEEB,
	BONETAG_L_HAND = 0x49D9,
	BONETAG_L_FINGER0 = 0x67F2,
	BONETAG_L_FINGER01 = 0xFF9,
	BONETAG_L_FINGER02 = 0xFFA,
	BONETAG_L_FINGER1 = 0x67F3,
	BONETAG_L_FINGER11 = 0x1049,
	BONETAG_L_FINGER12 = 0x104A,
	BONETAG_L_FINGER2 = 0x67F4,
	BONETAG_L_FINGER21 = 0x1059,
	BONETAG_L_FINGER22 = 0x105A,
	BONETAG_L_FINGER3 = 0x67F5,
	BONETAG_L_FINGER31 = 0x1029,
	BONETAG_L_FINGER32 = 0x102A,
	BONETAG_L_FINGER4 = 0x67F6,
	BONETAG_L_FINGER41 = 0x1039,
	BONETAG_L_FINGER42 = 0x103A,

	BONETAG_L_THIGH = 0xE39F,
	BONETAG_L_CALF = 0xF9BB,
	BONETAG_L_FOOT = 0x3779,
	BONETAG_L_TOE = 0x83C,
	BONETAG_R_THIGH = 0xCA72,
	BONETAG_R_CALF = 0x9000,
	BONETAG_R_FOOT = 0xCC4D,
	BONETAG_R_TOE = 0x512D,

	BONETAG_PH_L_HAND = 0xEB95,
	BONETAG_PH_R_HAND = 0x6F06
};
#pragma endregion

#pragma region Vehicle Enums
enum eVehicleSeat {
	VS_ANY_PASSENGER = -2,		// Any passenger seat
	VS_DRIVER = -1,				// Drivers seat
	VS_FRONT_RIGHT = 0,			// Front Right seat
	VS_BACK_LEFT,				// Back left 	
	VS_BACK_RIGHT,				// Back right
	VS_EXTRA_LEFT_1,
	VS_EXTRA_RIGHT_1,
	VS_EXTRA_LEFT_2,
	VS_EXTRA_RIGHT_2,
	VS_EXTRA_LEFT_3,
	VS_EXTRA_RIGHT_3
};

enum eVehicleSearchFlag {
	VEHICLE_SEARCH_FLAG_RETURN_LAW_ENFORCER_VEHICLES = 1,						//  Don't mind if the returned vehicle is a law enforcer
	VEHICLE_SEARCH_FLAG_RETURN_MISSION_VEHICLES = 2,							//  Don't mind if the returned vehicle has been created by a script
	VEHICLE_SEARCH_FLAG_RETURN_RANDOM_VEHICLES = 4,								//  Don't mind if the returned vehicle hasn't been created by a script (You will usually want to set this)
	VEHICLE_SEARCH_FLAG_RETURN_VEHICLES_CONTAINING_GROUP_MEMBERS = 8,			//  Don't mind if the returned vehicle contains group members
	VEHICLE_SEARCH_FLAG_RETURN_VEHICLES_CONTAINING_A_PLAYER = 16,				//  Don't mind if the returned vehicle contains a player ped
	VEHICLE_SEARCH_FLAG_RETURN_VEHICLES_CONTAINING_A_DEAD_OR_DYING_PED = 32,	//  Don't mind if the returned vehicle contains a dead or dying ped
	VEHICLE_SEARCH_FLAG_RETURN_VEHICLES_WITH_PEDS_ENTERING_OR_EXITING = 64,		//  Don't mind if peds are in the process of entering or exiting the returned vehicle
	VEHICLE_SEARCH_FLAG_DO_NETWORK_CHECKS = 128,								//  Don't return a network clone or a deletable network object(?)
	VEHICLE_SEARCH_FLAG_CHECK_VEHICLE_OCCUPANTS_STATES = 256,					// Only return the vehicle if all its occupants have a PedState of PED_DRIVING or PED_DEAD (not sure if it matters if this is set or not)
	VEHICLE_SEARCH_FLAG_RETURN_LAW_ENFORCER_VEHICLES_ONLY = 1024,				//  Only return law enforcement vehicles
	VEHICLE_SEARCH_FLAG_ALLOW_VEHICLE_OCCUPANTS_TO_BE_PERFORMING_A_SCRIPTED_TASK = 2048, // Usually cars are only returned if all the occupants are performing their default task  (see also PERFORMING_A_NON_DEFAULT_TASK below. It's less strict)
	VEHICLE_SEARCH_FLAG_RETURN_HELICOPTORS_ONLY = 4096,							// Only return helicoptor (can be combimed with the boats_only and planes_only flags)
	VEHICLE_SEARCH_FLAG_RETURN_BOATS_ONLY = 8192,								// Only return boats (can be combimed with the helicoptor_only and planes_only flags)
	VEHICLE_SEARCH_FLAG_RETURN_PLANES_ONLY = 16384,								// Only return planes (can be combimed with the helicoptor_only and boats_only flags)
	VEHICLE_SEARCH_FLAG_ALLOW_LAW_ENFORCER_VEHICLES_WITH_WANTED_LEVEL = 32768,	// Allow return law enforcement vehs if player has a wanted level
	VEHICLE_SEARCH_FLAG_ALLOW_VEHICLE_OCCUPANTS_TO_BE_PERFORMING_A_NON_DEFAULT_TASK = 65536,	// Usually cars are only returned if all the occupants are performing their default task (see also PERFORMING_A_SCRIPTED_TASK above. It's stricter)
	VEHICLE_SEARCH_FLAG_ALLOW_TRAILERS = 131076,	// allow trailers to be included
	VEHICLE_SEARCH_FLAG_ALLOW_BLIMPS = 262144,		// allow blimps to be included
	VEHICLE_SEARCH_FLAG_ALLOW_SUBMARINES = 524288	// allow submarines to be included
};

constexpr int defaultVSF = (
	VEHICLE_SEARCH_FLAG_RETURN_LAW_ENFORCER_VEHICLES | VEHICLE_SEARCH_FLAG_RETURN_MISSION_VEHICLES |
	VEHICLE_SEARCH_FLAG_RETURN_RANDOM_VEHICLES | VEHICLE_SEARCH_FLAG_RETURN_VEHICLES_CONTAINING_GROUP_MEMBERS |
	VEHICLE_SEARCH_FLAG_RETURN_VEHICLES_CONTAINING_A_PLAYER | VEHICLE_SEARCH_FLAG_RETURN_VEHICLES_CONTAINING_A_DEAD_OR_DYING_PED |
	VEHICLE_SEARCH_FLAG_RETURN_VEHICLES_WITH_PEDS_ENTERING_OR_EXITING |
	VEHICLE_SEARCH_FLAG_ALLOW_VEHICLE_OCCUPANTS_TO_BE_PERFORMING_A_SCRIPTED_TASK |
	VEHICLE_SEARCH_FLAG_ALLOW_VEHICLE_OCCUPANTS_TO_BE_PERFORMING_A_NON_DEFAULT_TASK | VEHICLE_SEARCH_FLAG_ALLOW_BLIMPS |
	VEHICLE_SEARCH_FLAG_ALLOW_SUBMARINES);

enum eVehStuckTypes {
	VEH_STUCK_ON_ROOF = 0,
	VEH_STUCK_ON_SIDE,
	VEH_STUCK_HUNG_UP,
	VEH_STUCK_JAMMED,
	VEH_STUCK_RESET_ALL
};

enum eVehModtype {
	MOD_SPOILER = 0,
	MOD_BUMPER_F,
	MOD_BUMPER_R,
	MOD_SKIRT,
	MOD_EXHAUST,
	MOD_CHASSIS,
	MOD_GRILL,
	MOD_BONNET,
	MOD_WING_L,
	MOD_WING_R,
	MOD_ROOF,

	MOD_ENGINE,
	MOD_BRAKES,
	MOD_GEARBOX,
	MOD_HORN,
	MOD_SUSPENSION,
	MOD_ARMOUR,

	MOD_TOGGLE_NITROUS,
	MOD_TOGGLE_TURBO,
	MOD_TOGGLE_SUBWOOFER,
	MOD_TOGGLE_TYRE_SMOKE,
	MOD_TOGGLE_HYDRAULICS,
	MOD_TOGGLE_XENON_LIGHTS,

	MOD_WHEELS,
	MOD_REAR_WHEELS,

	// Lowrider
	MOD_PLTHOLDER,
	MOD_PLTVANITY,

	MOD_INTERIOR1,
	MOD_INTERIOR2,
	MOD_INTERIOR3,
	MOD_INTERIOR4,
	MOD_INTERIOR5,
	MOD_SEATS,
	MOD_STEERING,
	MOD_KNOB,
	MOD_PLAQUE,
	MOD_ICE,

	MOD_TRUNK,
	MOD_HYDRO,

	MOD_ENGINEBAY1,
	MOD_ENGINEBAY2,
	MOD_ENGINEBAY3,

	MOD_CHASSIS2,
	MOD_CHASSIS3,
	MOD_CHASSIS4,
	MOD_CHASSIS5,

	MOD_DOOR_L,
	MOD_DOOR_R,

	MOD_LIVERY
};

enum eKnockOffVehicle {
	KNOCKOFFVEHICLE_DEFAULT,
	KNOCKOFFVEHICLE_NEVER,
	KNOCKOFFVEHICLE_EASY,
	KNOCKOFFVEHICLE_HARD
};

enum eWheelList {
	WHEEL_CAR_FRONT_LEFT = 0,
	WHEEL_CAR_FRONT_RIGHT,
	WHEEL_CAR_MID_LEFT,
	WHEEL_CAR_MID_RIGHT,
	WHEEL_CAR_REAR_LEFT,
	WHEEL_CAR_REAR_RIGHT,
	WHEEL_BIKE_FRONT,
	WHEEL_BIKE_REAR,
	MAX_WHEELS
};
#pragma endregion

#pragma region Weapon Enums
enum eGeneralWeaponType
{
	GENERALWEAPON_TYPE_INVALID = 0,
	GENERALWEAPON_TYPE_ANYMELEE,
	GENERALWEAPON_TYPE_ANYWEAPON
};

enum eWeaponGroup {
	WEAPONGROUP_INVALID = 0,
	WEAPONGROUP_MELEE = 0xD49321D4,				//GROUP_MELEE
	WEAPONGROUP_PISTOL = 0x18D5FA97,			//GROUP_PISTOL
	WEAPONGROUP_SMG = 0xC6E9A5C5,				//GROUP_SMG
	WEAPONGROUP_RIFLE = 0x39D5C192,				//GROUP_RIFLE
	WEAPONGROUP_MG = 0x451B04BC,				//GROUP_MG
	WEAPONGROUP_SHOTGUN = 0x33431399,			//GROUP_SHOTGUN
	WEAPONGROUP_SNIPER = 0xB7BBD827,			//GROUP_SNIPER
	WEAPONGROUP_HEAVY = 0xA27A4F9F,				//GROUP_HEAVY
	WEAPONGROUP_THROWN = 0x5C4C5883,			//GROUP_THROWN
	WEAPONGROUP_RUBBERGUN = 0x054C7FFC,			//GROUP_RUBBERGUN
	WEAPONGROUP_STUNGUN = 0x29268262,			//GROUP_STUNGUN
	WEAPONGROUP_FIREEXTINGUISHER = 0xFDBF656C,	//GROUP_FIREEXTINGUISHER
	WEAPONGROUP_PETROLCAN = 0x5F1BE07C,			//GROUP_PETROLCAN
	WEAPONGROUP_LOUDHAILER = 0x1E3DEED0,		//GROUP_LOUDHAILER
	WEAPONGROUP_DIGISCANNER = 0xD2F7B56B,		//GROUP_DIGISCANNER
	WEAPONGROUP_NIGHTVISION = 0xD035CE98,		//GROUP_NIGHTVISION
	WEAPONGROUP_PARACHUTE = 0x19B9968F,			//GROUP_PARACHUTE
	WEAPONGROUP_JETPACK = 0x8B16BE36,			//GROUP_JETPACK
	WEAPONGROUP_METALDETECTOR = 0xE0154937		//GROUP_METALDETECTOR
};

enum eAmmoType {
	AMMOTYPE_INVALID = 0,

	AMMOTYPE_PISTOL = Joaat("AMMO_PISTOL"),												//1950175060
	AMMOTYPE_SMG = Joaat("AMMO_SMG"),													//1820140472
	AMMOTYPE_RIFLE = Joaat("AMMO_RIFLE"),												//218444191
	AMMOTYPE_MG = Joaat("AMMO_MG"),														//1788949567
	AMMOTYPE_SHOTGUN = Joaat("AMMO_SHOTGUN"),											//-1878508229
	AMMOTYPE_RUBBERGUN = Joaat("AMMO_RUBBERGUN"),										//-1866678455
	AMMOTYPE_STUNGUN = Joaat("AMMO_STUNGUN"),											//-1339118112
	AMMOTYPE_SNIPER = Joaat("AMMO_SNIPER"),												//1285032059
	AMMOTYPE_REMOTE = Joaat("AMMO_SNIPER_REMOTE"),										//-19235536
	AMMOTYPE_FIRE_EXTINGUISHER = Joaat("AMMO_FIREEXTINGUISHER"),						//1359393852
	AMMOTYPE_PETROL_CAN = Joaat("AMMO_PETROLCAN"),										//-899475295
	AMMOTYPE_MINIGUN = Joaat("AMMO_MINIGUN"),											//-1614428030
	AMMOTYPE_GRENADE_LAUNCHER = Joaat("AMMO_GRENADELAUNCHER"),							//1003267566
	AMMOTYPE_GRENADE_LAUNCHER_SMOKE = Joaat("AMMO_GRENADELAUNCHER_SMOKE"),				//826266432
	AMMOTYPE_RPG = Joaat("AMMO_RPG"),													//1742569970
	AMMOTYPE_GRENADE = Joaat("AMMO_GRENADE"),											//1003688881
	AMMOTYPE_STICKY_BOMB = Joaat("AMMO_STICKYBOMB"),									//1411692055
	AMMOTYPE_SMOKE_GRENADE = Joaat("AMMO_SMOKEGRENADE"),								//-435287898
	AMMOTYPE_BZ_GAS = Joaat("AMMO_BZGAS"),												//-1686864220
	AMMOTYPE_FLARE = Joaat("AMMO_FLARE"),												//1808594799
	AMMOTYPE_MOLOTOV = Joaat("AMMO_MOLOTOV"),											//1446246869
	AMMOTYPE_TANK = Joaat("AMMO_TANK"),													//-1474608608
	AMMOTYPE_SPACE_ROCKET = Joaat("AMMO_SPACE_ROCKET"),									//527765612
	AMMOTYPE_PLAYER_LASER = Joaat("AMMO_PLAYER_LASER"),									//-165357558
	AMMOTYPE_ENEMY_LASER = Joaat("AMMO_ENEMY_LASER"),									//-1372674932

	AMMOTYPE_DLC_PROGRAMMABLE_AR = Joaat("AMMO_PROGRAMMABLEAR"),						//-873900901
	AMMOTYPE_DLC_HARPOON = Joaat("AMMO_HARPOON"),										//-1366951173
	AMMOTYPE_DLC_FLAREGUN = Joaat("AMMO_FLAREGUN"),										//1173416293
	AMMOTYPE_DLC_FIREWORK = Joaat("AMMO_FIREWORK"),										//-1356599793
	AMMOTYPE_DLC_AMRIFLE = Joaat("AMMO_AMRIFLE"),										//572875586
	AMMOTYPE_DLC_CROSSBOW = Joaat("AMMO_CROSSBOW"),										//-1628563450
	AMMOTYPE_DLC_HOMINGLAUNCHER = Joaat("AMMO_HOMINGLAUNCHER"),							//-1726673363
	AMMOTYPE_DLC_PROXMINE = Joaat("AMMO_PROXMINE"),										//-1356724057
	AMMOTYPE_DLC_SNOWBALL = Joaat("AMMO_SNOWBALL"),										//-2112339603

	//NG-Only Ammo Types
	AMMOTYPE_DLC_RAILGUN = Joaat("AMMO_RAILGUN"),										//2034517757
	AMMOTYPE_DLC_PIPEBOMB = Joaat("AMMO_PIPEBOMB"),										//357983224
	AMMOTYPE_DLC_BZGAS_MK2 = Joaat("AMMO_BZGAS_MK2"),									//-190476753
	AMMOTYPE_DLC_BEANBAG = Joaat("AMMO_BEANBAG"),										//1528506929
	AMMOTYPE_DLC_FLASHGRENADE = Joaat("AMMO_FLASHGRENADE"),								//-1713098100
	AMMOTYPE_DLC_STUNGRENADE = Joaat("AMMO_STUNGRENADE"),								//1715622329

	//Special Ammo Types
	AMMOTYPE_DLC_MG_ARMORPIERCING = Joaat("AMMO_MG_ARMORPIERCING"),						//784861712
	AMMOTYPE_DLC_MG_FMJ = Joaat("AMMO_MG_FMJ"),											//234717365
	AMMOTYPE_DLC_MG_INCENDIARY = Joaat("AMMO_MG_INCENDIARY"),							//1461941360
	AMMOTYPE_DLC_MG_TRACER = Joaat("AMMO_MG_TRACER"),									//1226421483
	AMMOTYPE_DLC_PISTOL_FMJ = Joaat("AMMO_PISTOL_FMJ"),									//-1132792829
	AMMOTYPE_DLC_PISTOL_HOLLOWPOINT = Joaat("AMMO_PISTOL_HOLLOWPOINT"),					//-836519658
	AMMOTYPE_DLC_PISTOL_INCENDIARY = Joaat("AMMO_PISTOL_INCENDIARY"),					//-1416716039
	AMMOTYPE_DLC_PISTOL_TRACER = Joaat("AMMO_PISTOL_TRACER"),							//-1193480661
	AMMOTYPE_DLC_RIFLE_ARMORPIERCING = Joaat("AMMO_RIFLE_ARMORPIERCING"),				//423744068
	AMMOTYPE_DLC_RIFLE_FMJ = Joaat("AMMO_RIFLE_FMJ"),									//1586900444
	AMMOTYPE_DLC_RIFLE_INCENDIARY = Joaat("AMMO_RIFLE_INCENDIARY"),						//-1829688883
	AMMOTYPE_DLC_RIFLE_TRACER = Joaat("AMMO_RIFLE_TRACER"),								//-1340502689
	AMMOTYPE_DLC_SMG_FMJ = Joaat("AMMO_SMG_FMJ"),										//758230489
	AMMOTYPE_DLC_SMG_HOLLOWPOINT = Joaat("AMMO_SMG_HOLLOWPOINT"),						//670318226
	AMMOTYPE_DLC_SMG_INCENDIARY = Joaat("AMMO_SMG_INCENDIARY"),							//-332892697
	AMMOTYPE_DLC_SMG_TRACER = Joaat("AMMO_SMG_TRACER"),									//1569785553
	AMMOTYPE_DLC_SNIPER_ARMORPIERCING = Joaat("AMMO_SNIPER_ARMORPIERCING"),				//-1497580119
	AMMOTYPE_DLC_SNIPER_EXPLOSIVE = Joaat("AMMO_SNIPER_EXPLOSIVE"),						//-1378784071
	AMMOTYPE_DLC_SNIPER_FMJ = Joaat("AMMO_SNIPER_FMJ"),									//-168704490
	AMMOTYPE_DLC_SNIPER_INCENDIARY = Joaat("AMMO_SNIPER_INCENDIARY"),					//796697766
	AMMOTYPE_DLC_SNIPER_TRACER = Joaat("AMMO_SNIPER_TRACER"),							//1184011213
	AMMOTYPE_DLC_SHOTGUN_ARMORPIERCING = Joaat("AMMO_SHOTGUN_ARMORPIERCING"),			//1923327840
	AMMOTYPE_DLC_SHOTGUN_EXPLOSIVE = Joaat("AMMO_SHOTGUN_EXPLOSIVE"),					//-309302955
	AMMOTYPE_DLC_SHOTGUN_HOLLOWPOINT = Joaat("AMMO_SHOTGUN_HOLLOWPOINT"),				//2089185906
	AMMOTYPE_DLC_SHOTGUN_INCENDIARY = Joaat("AMMO_SHOTGUN_INCENDIARY"),					//-609429612

	AMMOTYPE_DLC_RAYPISTOL = Joaat("AMMO_RAYPISTOL"),									//-1526023308
	AMMOTYPE_DLC_HAZARDCAN = Joaat("AMMO_HAZARDCAN"),									//1618528319
	AMMOTYPE_DLC_STUNGUNCNC = Joaat("AMMO_STUNGUNCNC"),									//-1408480069
	AMMOTYPE_DLC_SMG_RUBBER = Joaat("AMMO_SMG_RUBBER"),									//-986319330

	AMMOTYPE_DLC_EMPLAUNCHER = Joaat("AMMO_EMPLAUNCHER"),								//-237025091
	AMMOTYPE_DLC_FERTILIZERCAN = Joaat("AMMO_FERTILIZERCAN"),							//1963932634

	AMMOTYPE_ALL = Joaat("AMMOTYPE_ALL")												//-32260324
};

enum eWeaponAttachPart {
	WAP_Root = Joaat("gun_root"),
	WAP_Gripr = Joaat("gun_gripr"),	// MK2
	WAP_Clip = Joaat("WAPClip"),
	WAP_Clip_2 = Joaat("WAPClip_2"),
	WAP_Flsh = Joaat("WAPFlsh"),
	WAP_Flsh_2 = Joaat("WAPFlsh_2"),
	WAP_FlshLasr = Joaat("WAPFlshLasr"),
	WAP_FlshLasr_2 = Joaat("WAPFlshLasr_2"),
	WAP_FlshLasr_3 = Joaat("WAPFlshLasr_3"),
	WAP_Supp = Joaat("WAPSupp"),
	WAP_Supp_2 = Joaat("WAPSupp_2"),
	WAP_Supp_3 = Joaat("WAPSupp_3"),
	WAP_Grip = Joaat("WAPGrip"),
	WAP_Grip_2 = Joaat("WAPGrip_2"),
	WAP_Grip_3 = Joaat("WAPGrip_3"),
	WAP_Scop = Joaat("WAPScop"),
	WAP_Scop_2 = Joaat("WAPScop_2"),
	WAP_Scop_3 = Joaat("WAPScop_3"),
	WAP_Barrel = Joaat("WAPBarrel"),
	WAP_Torch_Bulb = Joaat("Torch_Bulb"),
};

enum eWeaponCheckFlags {
	WF_INCLUDE_MELEE = 1,
	WF_INCLUDE_PROJECTILE = 2,
	WF_INCLUDE_GUN = 4
};

enum eCombatAttribute {
	CA_INVALID = -1,
	CA_USE_COVER = 0,
	CA_USE_VEHICLE = 1,
	CA_DO_DRIVEBYS = 2,
	CA_LEAVE_VEHICLES = 3,
	CA_CAN_USE_DYNAMIC_STRAFE_DECISIONS = 4,
	CA_ALWAYS_FIGHT = 5,
	CA_FLEE_WHILST_IN_VEHICLE = 6,
	CA_JUST_FOLLOW_VEHICLE = 7,
	CA_PLAY_REACTION_ANIMS = 8,
	CA_WILL_SCAN_FOR_DEAD_PEDS = 9,
	CA_IS_A_GUARD = 10,
	CA_JUST_SEEK_COVER = 11,
	CA_BLIND_FIRE_IN_COVER = 12,
	CA_AGGRESSIVE = 13,
	CA_CAN_INVESTIGATE = 14,
	CA_CAN_USE_RADIO = 15,
	CA_CAN_CAPTURE_ENEMY_PEDS = 16,
	CA_ALWAYS_FLEE = 17,
	CA_CAN_TAUNT_IN_VEHICLE = 20,
	CA_CAN_CHASE_TARGET_ON_FOOT = 21,
	CA_WILL_DRAG_INJURED_PEDS_TO_SAFETY = 22,
	CA_REQUIRES_LOS_TO_SHOOT = 23,
	CA_USE_PROXIMITY_FIRING_RATE = 24,
	CA_DISABLE_SECONDARY_TARGET = 25,
	CA_DISABLE_ENTRY_REACTIONS = 26,
	CA_PERFECT_ACCURACY = 27,
	CA_CAN_USE_FRUSTRATED_ADVANCE = 28,
	CA_MOVE_TO_LOCATION_BEFORE_COVER_SEARCH = 29,
	CA_CAN_SHOOT_WITHOUT_LOS = 30,
	CA_MAINTAIN_MIN_DISTANCE_TO_TARGET = 31,
	CA_CAN_USE_PEEKING_VARIATIONS = 34,
	CA_DISABLE_PINNED_DOWN = 35,
	CA_DISABLE_PIN_DOWN_OTHERS = 36,
	CA_OPEN_COMBAT_WHEN_DEFENSIVE_AREA_IS_REACHED = 37,
	CA_DISABLE_BULLET_REACTIONS = 38,
	CA_CAN_BUST = 39,
	CA_IGNORED_BY_OTHER_PEDS_WHEN_WANTED = 40,
	CA_CAN_COMMANDEER_VEHICLES = 41,
	CA_CAN_FLANK = 42,
	CA_SWITCH_TO_ADVANCE_IF_CANT_FIND_COVER = 43,
	CA_SWITCH_TO_DEFENSIVE_IF_IN_COVER = 44,
	CA_CLEAR_PRIMARY_DEFENSIVE_AREA_WHEN_REACHED = 45,
	CA_CAN_FIGHT_ARMED_PEDS_WHEN_NOT_ARMED = 46,
	CA_ENABLE_TACTICAL_POINTS_WHEN_DEFENSIVE = 47,
	CA_DISABLE_COVER_ARC_ADJUSTMENTS = 48,
	CA_USE_ENEMY_ACCURACY_SCALING = 49,
	CA_CAN_CHARGE = 50,
	CA_REMOVE_AREA_SET_WILL_ADVANCE_WHEN_DEFENSIVE_AREA_REACHED = 51,
	CA_USE_VEHICLE_ATTACK = 52,
	CA_USE_VEHICLE_ATTACK_IF_VEHICLE_HAS_MOUNTED_GUNS = 53,
	CA_ALWAYS_EQUIP_BEST_WEAPON = 54,
	CA_CAN_SEE_UNDERWATER_PEDS = 55,
	CA_DISABLE_AIM_AT_AI_TARGETS_IN_HELIS = 56,
	CA_DISABLE_SEEK_DUE_TO_LINE_OF_SIGHT = 57,
	CA_DISABLE_FLEE_FROM_COMBAT = 58,
	CA_DISABLE_TARGET_CHANGES_DURING_VEHICLE_PURSUIT = 59,
	CA_CAN_THROW_SMOKE_GRENADE = 60,
	CA_CLEAR_AREA_SET_DEFENSIVE_IF_DEFENSIVE_CANNOT_BE_REACHED = 62,
	CA_DISABLE_BLOCK_FROM_PURSUE_DURING_VEHICLE_CHASE = 64,
	CA_DISABLE_SPIN_OUT_DURING_VEHICLE_CHASE = 65,
	CA_DISABLE_CRUISE_IN_FRONT_DURING_BLOCK_DURING_VEHICLE_CHASE = 66,
	CA_CAN_IGNORE_BLOCKED_LOS_WEIGHTING = 67,
	CA_DISABLE_REACT_TO_BUDDY_SHOT = 68,
	CA_PREFER_NAVMESH_DURING_VEHICLE_CHASE = 69,
	CA_ALLOWED_TO_AVOID_OFFROAD_DURING_VEHICLE_CHASE = 70,
	CA_PERMIT_CHARGE_BEYOND_DEFENSIVE_AREA = 71,
	CA_USE_ROCKETS_AGAINST_VEHICLES_ONLY = 72,
	CA_DISABLE_TACTICAL_POINTS_WITHOUT_CLEAR_LOS = 73,
	CA_DISABLE_PULL_ALONGSIDE_DURING_VEHICLE_CHASE = 74,
	CA_DISABLE_ALL_RANDOMS_FLEE = 78,
	CA_WILL_GENERATE_DEAD_PED_SEEN_SCRIPT_EVENTS = 79,
	CA_USE_MAX_SENSE_RANGE_WHEN_RECEIVING_EVENTS = 80,
	CA_RESTRICT_IN_VEHICLE_AIMING_TO_CURRENT_SIDE = 81,
	CA_USE_DEFAULT_BLOCKED_LOS_POSITION_AND_DIRECTION = 82,
	CA_REQUIRES_LOS_TO_AIM = 83,
	CA_CAN_CRUISE_AND_BLOCK_IN_VEHICLE = 84,
	CA_PREFER_AIR_COMBAT_WHEN_IN_AIRCRAFT = 85,
	CA_ALLOW_DOG_FIGHTING = 86,
	CA_PREFER_NON_AIRCRAFT_TARGETS = 87,
	CA_PREFER_KNOWN_TARGETS_WHEN_COMBAT_CLOSEST_TARGET = 88,
	CA_FORCE_CHECK_ATTACK_ANGLE_FOR_MOUNTED_GUNS = 89,
	CA_BLOCK_FIRE_FOR_VEHICLE_PASSENGER_MOUNTED_GUNS = 90
};

enum eFiringPatternHash {
	FIRING_PATTERN_BURST_FIRE = -687903391,
	FIRING_PATTERN_BURST_FIRE_DRIVEBY = -753768974,
	FIRING_PATTERN_FULL_AUTO = -957453492,
	FIRING_PATTERN_SINGLE_SHOT = 1566631136,
	FIRING_PATTERN_DELAY_FIRE_BY_ONE_SEC = 2055493265,
	FIRING_PATTERN_BURST_FIRE_HELI = -1857128337,
	FIRING_PATTERN_SHORT_BURSTS = 445831135,
	FIRING_PATTERN_BURST_FIRE_MICRO = 1122960381,
	FIRING_PATTERN_SLOW_FIRE_TANK = -490063247,
	FIRING_PATTERN_TAMPA_MORTAR = -1842093953
};

enum eFiringType {
	FIRING_TYPE_DEFAULT = 0,								// Some of these don't translate to firing patterns so we'll just not use those ones
	FIRING_TYPE_1_BURST = FIRING_TYPE_DEFAULT,     			// Uses FIRING_TYPE_DEFAULT as it's no longer a valid type
	FIRING_TYPE_1_THEN_AIM = FIRING_PATTERN_SINGLE_SHOT,    // Fire 1 bullet then aim for the duration
	FIRING_TYPE_RANDOM_BURSTS = FIRING_PATTERN_BURST_FIRE,  // Fire random bursts for the time, can change the frequency using SET_PED_SHOOT_RATE
	FIRING_TYPE_CLIP = FIRING_TYPE_DEFAULT,        			// Uses FIRING_TYPE_DEFAULT as it's no longer a valid type
	FIRING_TYPE_CONTINUOUS = FIRING_PATTERN_FULL_AUTO       // Fires at the maximum rate for the duration, reloading if the clips empty
};

/*
static const std::unordered_map<unsigned int, unsigned int> wpPickupMap = {
	{Joaat("W_SG_BULLPUPSHOTGUN"), Joaat("PICKUP_WEAPON_BULLPUPSHOTGUN")},
	{Joaat("W_SB_ASSAULTSMG"), Joaat("PICKUP_WEAPON_ASSAULTSMG")},
	{Joaat("W_PI_PISTOL50"), Joaat("PICKUP_WEAPON_PISTOL50")},
	{Joaat("W_AR_ASSAULTRIFLE"), Joaat("PICKUP_WEAPON_ASSAULTRIFLE")},
	{Joaat("W_AR_CARBINERIFLE"), Joaat("PICKUP_WEAPON_CARBINERIFLE")},
	{Joaat("W_AR_ADVANCEDRIFLE"), Joaat("PICKUP_WEAPON_ADVANCEDRIFLE")},
	{Joaat("W_MG_MG"), Joaat("PICKUP_WEAPON_MG")},
	{Joaat("W_MG_COMBATMG"), Joaat("PICKUP_WEAPON_COMBATMG")},
	{Joaat("W_SR_SNIPERRIFLE"), Joaat("PICKUP_WEAPON_SNIPERRIFLE")},
	{Joaat("W_SR_HEAVYSNIPER"), Joaat("PICKUP_WEAPON_HEAVYSNIPER")},
	{Joaat("W_SB_MICROSMG"), Joaat("PICKUP_WEAPON_MICROSMG")},
	{Joaat("W_SB_SMG"), Joaat("PICKUP_WEAPON_SMG")},
	{Joaat("W_LR_RPG"), Joaat("PICKUP_WEAPON_RPG")},
	{Joaat("W_MG_Minigun"), Joaat("PICKUP_WEAPON_MINIGUN")},
	{Joaat("W_SG_PUMPSHOTGUN"), Joaat("PICKUP_WEAPON_PUMPSHOTGUN")},
	{Joaat("W_SG_SawnOff"), Joaat("PICKUP_WEAPON_SAWNOFFSHOTGUN")},
	{Joaat("W_SG_ASSAULTSHOTGUN"), Joaat("PICKUP_WEAPON_ASSAULTSHOTGUN")},
	{Joaat("W_EX_GRENADEFRAG"), Joaat("PICKUP_WEAPON_GRENADE")},
	{Joaat("W_EX_MOLOTOV"), Joaat("PICKUP_WEAPON_MOLOTOV")},
	{Joaat("W_EX_GRENADESMOKE"), Joaat("PICKUP_WEAPON_SMOKEGRENADE")},
	{Joaat("W_EX_PE"), Joaat("PICKUP_WEAPON_STICKYBOMB")},
	{Joaat("W_PI_PISTOL"), Joaat("PICKUP_WEAPON_PISTOL")},
	{Joaat("W_PI_COMBATPISTOL"), Joaat("PICKUP_WEAPON_COMBATPISTOL")},
	{Joaat("W_PI_APPISTOL"), Joaat("PICKUP_WEAPON_APPISTOL")},
	{Joaat("W_LR_GRENADELAUNCHER"), Joaat("PICKUP_WEAPON_GRENADELAUNCHER")},
	{Joaat("W_PI_STUNGUN"), Joaat("PICKUP_WEAPON_STUNGUN")},
	{Joaat("W_AM_Jerrycan"), Joaat("PICKUP_WEAPON_PETROLCAN")},
	{Joaat("W_ME_knife_01"), Joaat("PICKUP_WEAPON_KNIFE")},
	{Joaat("W_ME_Nightstick"), Joaat("PICKUP_WEAPON_NIGHTSTICK")},
	{Joaat("W_ME_Hammer"), Joaat("PICKUP_WEAPON_HAMMER")},
	{Joaat("W_ME_Bat"), Joaat("PICKUP_WEAPON_BAT")},
	{Joaat("W_ME_GClub"), Joaat("PICKUP_WEAPON_GolfClub")},
	{Joaat("w_me_crowbar"), Joaat("PICKUP_WEAPON_CROWBAR")},
	{Joaat("w_me_bottle"), Joaat("PICKUP_WEAPON_BOTTLE")},
	{Joaat("w_pi_sns_pistol"), Joaat("PICKUP_WEAPON_SNSPISTOL")},
	{Joaat("w_pi_heavypistol"), Joaat("PICKUP_WEAPON_HEAVYPISTOL")},
	{Joaat("w_ar_specialcarbine"), Joaat("PICKUP_WEAPON_SPECIALCARBINE")},
	{Joaat("w_ar_bullpuprifle"), Joaat("PICKUP_WEAPON_BULLPUPRIFLE")},
	{Joaat("W_PI_RAYGUN"), Joaat("PICKUP_WEAPON_RAYPISTOL")},
	{Joaat("W_AR_SRIFLE"), Joaat("PICKUP_WEAPON_RAYCARBINE")},
	{Joaat("W_MG_SMINIGUN"), Joaat("PICKUP_WEAPON_RAYMINIGUN")},
	{Joaat("W_AR_BullpupRifleMK2"), Joaat("PICKUP_WEAPON_BULLPUPRIFLE_MK2")},
	{Joaat("W_PI_Wep1_Gun"), Joaat("PICKUP_WEAPON_DOUBLEACTION")},
	{Joaat("w_sr_marksmanriflemk2"), Joaat("PICKUP_WEAPON_MARKSMANRIFLE_MK2")},
	{Joaat("w_sg_pumpshotgunmk2"), Joaat("PICKUP_WEAPON_PUMPSHOTGUN_MK2")},
	{Joaat("w_pi_revolvermk2"), Joaat("PICKUP_WEAPON_REVOLVER_MK2")},
	{Joaat("W_PI_SNS_PistolMK2"), Joaat("PICKUP_WEAPON_SNSPISTOL_MK2")},
	{Joaat("w_ar_specialcarbinemk2"), Joaat("PICKUP_WEAPON_SPECIALCARBINE_MK2")},
	{Joaat("W_PI_Pistol_XM3"), Joaat("PICKUP_WEAPON_PISTOLXM3")},
	{Joaat("W_ME_Candy_XM3"), Joaat("PICKUP_WEAPON_CANDYCANE")},
	{Joaat("W_AR_RailGun_XM3"), Joaat("PICKUP_WEAPON_RAILGUNXM3")},
	{Joaat("w_ex_apmine"), Joaat("PICKUP_WEAPON_PROXMINE")},
	{Joaat("w_lr_homing"), Joaat("PICKUP_WEAPON_HOMINGLAUNCHER")},
	{Joaat("w_ex_snowball"), Joaat("PICKUP_WEAPON_SNOWBALL")},
	{Joaat("w_sb_gusenberg"), Joaat("PICKUP_WEAPON_GUSENBERG")},
	{Joaat("w_me_dagger"), Joaat("PICKUP_WEAPON_DAGGER")},
	{Joaat("w_pi_vintage_pistol"), Joaat("PICKUP_WEAPON_VINTAGEPISTOL")},
	{Joaat("W_LR_FIREWORK"), Joaat("PICKUP_WEAPON_FIREWORK")},
	{Joaat("w_ar_musket"), Joaat("PICKUP_WEAPON_MUSKET")},
	{Joaat("w_me_hatchet"), Joaat("PICKUP_WEAPON_HATCHET")},
	{Joaat("w_ar_railgun"), Joaat("PICKUP_WEAPON_RAILGUN")},
	{Joaat("w_sg_heavyshotgun"), Joaat("PICKUP_WEAPON_HEAVYSHOTGUN")},
	{Joaat("w_sr_marksmanrifle"), Joaat("PICKUP_WEAPON_MARKSMANRIFLE")},
	{Joaat("w_sg_pumpshotgunh4"), Joaat("PICKUP_WEAPON_COMBATSHOTGUN")},
	{Joaat("w_pi_singleshoth4"), Joaat("PICKUP_WEAPON_GADGETPISTOL")},
	{Joaat("w_ar_bullpuprifleh4"), Joaat("PICKUP_WEAPON_MILITARYRIFLE")},
	{Joaat("w_pi_ceramic_pistol"), Joaat("PICKUP_WEAPON_CERAMICPISTOL")},
	{Joaat("w_ch_jerrycan"), Joaat("PICKUP_WEAPON_HAZARDCAN")},
	{Joaat("w_pi_wep2_gun"), Joaat("PICKUP_WEAPON_NAVYREVOLVER")},
	{Joaat("w_pi_flaregun"), Joaat("PICKUP_WEAPON_FLAREGUN")},
	{Joaat("W_SB_PDW"), Joaat("PICKUP_WEAPON_COMBATPDW")},
	{Joaat("W_ME_Knuckle"), Joaat("PICKUP_WEAPON_KNUCKLE")},
	{Joaat("W_PI_SingleShot"), Joaat("PICKUP_WEAPON_MARKSMANPISTOL")},
	{Joaat("w_ar_assaultrifle_smg"), Joaat("PICKUP_WEAPON_COMPACTRIFLE")},
	{Joaat("w_sg_doublebarrel"), Joaat("PICKUP_WEAPON_DBSHOTGUN")},
	{Joaat("w_me_machette_lr"), Joaat("PICKUP_WEAPON_MACHETE")},
	{Joaat("W_SB_CompactSMG"), Joaat("PICKUP_WEAPON_MACHINEPISTOL")},
	{Joaat("w_me_flashlight"), Joaat("PICKUP_WEAPON_FLASHLIGHT")},
	{Joaat("w_pi_revolver"), Joaat("PICKUP_WEAPON_REVOLVER")},
	{Joaat("w_me_switchblade"), Joaat("PICKUP_WEAPON_SWITCHBLADE")},
	{Joaat("w_sg_sweeper"), Joaat("PICKUP_WEAPON_AUTOSHOTGUN")},
	{Joaat("w_me_battleaxe"), Joaat("PICKUP_WEAPON_BATTLEAXE")},
	{Joaat("w_lr_compactgl"), Joaat("PICKUP_WEAPON_COMPACTLAUNCHER")},
	{Joaat("w_sb_minismg"), Joaat("PICKUP_WEAPON_MINISMG")},
	{Joaat("w_ex_pipebomb"), Joaat("PICKUP_WEAPON_PIPEBOMB")},
	{Joaat("w_me_poolcue"), Joaat("PICKUP_WEAPON_POOLCUE")},
	{Joaat("w_me_wrench"), Joaat("PICKUP_WEAPON_WRENCH")},
	{Joaat("W_AR_ASSAULTRIFLEMK2"), Joaat("PICKUP_WEAPON_ASSAULTRIFLE_MK2")},
	{Joaat("W_AR_CARBINERIFLEMK2"), Joaat("PICKUP_WEAPON_CARBINERIFLE_MK2")},
	{Joaat("W_MG_COMBATMGMK2"), Joaat("PICKUP_WEAPON_COMBATMG_MK2")},
	{Joaat("W_SR_HEAVYSNIPERMK2"), Joaat("PICKUP_WEAPON_HEAVYSNIPER_MK2")},
	{Joaat("W_PI_PISTOLMK2"), Joaat("PICKUP_WEAPON_PISTOL_MK2")},
	{Joaat("W_SB_SMGMK2"), Joaat("PICKUP_WEAPON_SMG_MK2")},
	{Joaat("w_me_Stonehatchet"), Joaat("PICKUP_WEAPON_STONE_HATCHET")},
	{Joaat("W_AM_Digiscanner_REH"), Joaat("PICKUP_WEAPON_METALDETECTOR")},
	{Joaat("W_AR_CarbineRifle_REH"), Joaat("PICKUP_WEAPON_TACTICALRIFLE")},
	{Joaat("W_SR_PrecisionRifle_REH"), Joaat("PICKUP_WEAPON_PRECISIONRIFLE")},
	{Joaat("W_LR_CompactML"), Joaat("PICKUP_WEAPON_EMPLAUNCHER")},
	{Joaat("W_AR_HEAVYRIFLEH"), Joaat("PICKUP_WEAPON_HEAVYRIFLE")},
	{Joaat("W_AM_Jerrycan"), Joaat("PICKUP_WEAPON_PETROLCAN_SMALL_RADIUS")},
	{Joaat("W_AM_Jerrycan_SF"), Joaat("PICKUP_WEAPON_FERTILIZERCAN")},
	{Joaat("W_PI_STUNGUN"), Joaat("PICKUP_WEAPON_STUNGUN_MP")},
	{Joaat("W_PI_PistolSMG_M31"), Joaat("PICKUP_WEAPON_TECPISTOL")},
	{Joaat("W_SL_BattleRifle_M32"), Joaat("PICKUP_WEAPON_BATTLERIFLE")},
	{Joaat("W_LR_CompactSL_M32"), Joaat("PICKUP_WEAPON_SNOWLAUNCHER")},
	{Joaat("W_AM_HackDevice_M32"), Joaat("PICKUP_WEAPON_HACKINGDEVICE")},
	{Joaat("W_ME_Rod_M41"), Joaat("PICKUP_WEAPON_STUNROD")},
	{Joaat("W_AR_BullpupRifleM42"), Joaat("PICKUP_WEAPON_STRICKLER")}
};
*/
#pragma endregion

#pragma region HUD Enums
enum eHudComponents
{
	//HUD = 0,
	HUD_WANTED_STARS = 1,
	HUD_WEAPON_ICON,
	HUD_CASH,
	HUD_MP_CASH,
	HUD_MP_MESSAGE,
	HUD_VEHICLE_NAME,
	HUD_AREA_NAME,
	HUD_UNUSED,
	HUD_STREET_NAME,
	HUD_HELP_TEXT,
	HUD_FLOATING_HELP_TEXT_1,
	HUD_FLOATING_HELP_TEXT_2,
	HUD_CASH_CHANGE,
	HUD_RETICLE,
	HUD_SUBTITLE_TEXT,
	HUD_RADIO_STATIONS,
	HUD_SAVING_GAME,
	HUD_GAME_STREAM,
	HUD_WEAPON_WHEEL,
	HUD_WEAPON_WHEEL_STATS,
	MAX_HUD_COMPONENTS
};

enum eTextFonts {
	FONT_STANDARD = 0,
	FONT_CURSIVE,
	FONT_ROCKSTAR_TAG,
	FONT_LEADERBOARD,
	FONT_CONDENSED,
	FONT_STYLE_FIXED_WIDTH_NUMBERS,
	FONT_CONDENSED_NOT_GAMERNAME,
	FONT_STYLE_PRICEDOWN,
	FONT_STYLE_TAXI
};

enum eDropStyle {
	DROPSTYLE_NONE = 0,
	DROPSTYLE_ALL = 1,
	DROPSTYLE_OUTLINEONLY = 2,
	DROPSTYLE_DROPSHADOWONLY = 3
};

enum eGFXDrawOrder {
	GFX_ORDER_BEFORE_HUD_PRIORITY_LOW = 0,
	GFX_ORDER_BEFORE_HUD,  // standard
	GFX_ORDER_BEFORE_HUD_PRIORITY_HIGH,

	GFX_ORDER_AFTER_HUD_PRIORITY_LOW,
	GFX_ORDER_AFTER_HUD,  // standard
	GFX_ORDER_AFTER_HUD_PRIORITY_HIGH,

	GFX_ORDER_AFTER_FADE_PRIORITY_LOW,
	GFX_ORDER_AFTER_FADE,  // standard
	GFX_ORDER_AFTER_FADE_PRIORITY_HIGH
};

enum eUIAlignment
{
	UI_ALIGN_LEFT = 'L',
	UI_ALIGN_RIGHT = 'R',
	UI_ALIGN_TOP = 'T',
	UI_ALIGN_BOTTOM = 'B',
	UI_ALIGN_IGNORE = 'I'
};

typedef struct {
	eTextFonts aFont;
	float XScale, YScale;
	RGBA colour;
	eDropStyle drop;
	bool centre;
	float WrapStartX, WrapEndX;
} TextStyle;
constexpr TextStyle defaultTextStyle = { FONT_STANDARD, 0.35f, 0.35f, RGBA{255, 255, 255, 255}, DROPSTYLE_NONE, false, 0.0f, 1.0f };
#pragma endregion

#pragma region Shapetest
enum eShapetestStatus {
	SHAPETEST_STATUS_NONEXISTENT = 0,	// Shapetest requests are discarded if they are ignored for a frame or as soon as the results are returned
	SHAPETEST_STATUS_RESULTS_NOTREADY,	// Not ready yet; try again next frame
	SHAPETEST_STATUS_RESULTS_READY		// The result is ready and the results have been returned to you. The shapetest request has also just been destroyed
};

enum eShapetestTypes {
	SHAPETEST_INVALID = 0,
	SHAPETEST_LOSPROBE,
	//SHAPETEST_SPHERE,
	SHAPETEST_BOUND,
	SHAPETEST_BOUNDINGBOX,
	SHAPETEST_BOX,
	SHAPETEST_CAPSULE

};

enum eShapetestFlags {
	SCRIPT_INCLUDE_MOVER = 1,
	SCRIPT_INCLUDE_VEHICLE = 2,
	SCRIPT_INCLUDE_PED = 4,
	SCRIPT_INCLUDE_RAGDOLL = 8,
	SCRIPT_INCLUDE_OBJECT = 16,
	SCRIPT_INCLUDE_PICKUP = 32,
	SCRIPT_INCLUDE_GLASS = 64,
	SCRIPT_INCLUDE_RIVER = 128,
	SCRIPT_INCLUDE_FOLIAGE = 256,
	SCRIPT_INCLUDE_ALL = 511
};

enum eShapetestOptions {
	SCRIPT_SHAPETEST_OPTION_IGNORE_GLASS = 1,
	SCRIPT_SHAPETEST_OPTION_IGNORE_SEE_THROUGH = 2,
	SCRIPT_SHAPETEST_OPTION_IGNORE_NO_COLLISION = 4,
	SCRIPT_SHAPETEST_OPTION_DEFAULT = SCRIPT_SHAPETEST_OPTION_IGNORE_GLASS | SCRIPT_SHAPETEST_OPTION_IGNORE_SEE_THROUGH | SCRIPT_SHAPETEST_OPTION_IGNORE_NO_COLLISION
};
#pragma endregion

#pragma region Misc Enums
enum eEulerRotOrder {
	EULER_XYZ = 0,
	EULER_XZY,
	EULER_YXZ,
	EULER_YZX,
	EULER_ZXY,
	EULER_ZYX,
	EULER_MAX
};

enum eCamViewMode {
	CAM_VIEW_MODE_THIRD_PERSON_NEAR = 0,
	CAM_VIEW_MODE_THIRD_PERSON_MEDIUM,
	CAM_VIEW_MODE_THIRD_PERSON_FAR,
	CAM_VIEW_MODE_CINEMATIC,
	CAM_VIEW_MODE_FIRST_PERSON,
	NUM_CAM_VIEW_MODES,
	CAM_VIEW_MODE_THIRD_PERSON = CAM_VIEW_MODE_THIRD_PERSON_MEDIUM
};

enum ePickupPlacementFlag {
	PLACEMENT_FLAG_MAP = 0,								 // only used in MP. This is used for pickups that are created locally on each machine and only networked when collected.
	PLACEMENT_FLAG_FIXED = 1,							 // sets the pickup as fixed so it cannot move
	PLACEMENT_FLAG_REGENERATES = 2,						 // sets the pickup as regenerating
	PLACEMENT_FLAG_SNAP_TO_GROUND = 3,					 // places the pickup on the ground 
	PLACEMENT_FLAG_ORIENT_TO_GROUND = 4,				 // orientates the pickup correctly on the ground
	PLACEMENT_FLAG_LOCAL_ONLY = 5,						 // creates the pickup non-networked
	PLACEMENT_FLAG_BLIPPED_SIMPLE = 6,					 // gives the pickup a simple blip
	PLACEMENT_FLAG_BLIPPED_COMPLEX = 7,					 // gives the pickup a complex blip
	PLACEMENT_FLAG_UPRIGHT = 8,							 // Some pickups need to be orientated differently to lie on the ground properly. Use this flag if your pickup is not lying correctly.
	PLACEMENT_FLAG_ROTATE = 9,							 // Pickup will rotate 
	PLACEMENT_FLAG_FACEPLAYER = 10,						 // Pickup will always face the player
	PLACEMENT_FLAG_HIDE_IN_PHOTOS = 11,					 // Pickup will be hidden when the player is using the phone camera
	PLACEMENT_FLAG_PLAYER_GIFT = 12,					 // The pickup is being dropped as a gift to another player
	PLACEMENT_FLAG_ON_OBJECT = 13,						 // The pickup is lying on an object and probes for that when snapping or orientating to ground
	PLACEMENT_FLAG_GLOW_IN_TEAM = 14,					 // Set pickups to glow even if pickup can't be picked up because of team checks
	PLACEMENT_CREATION_FLAG_AUTO_EQUIP = 15,			 // if set on a weapon pickup, it will auto equip the picked up weapon. It will ignore autoswap logic
	PLACEMENT_CREATION_FLAG_COLLECTABLE_IN_VEHICLE = 16, // if set the pickup can be collected by a ped in a vehicle
	PLACEMENT_CREATION_FLAG_DISABLE_WEAPON_HD_MODEL = 17,// if set the weapon pickup will render SD model only (HD<->SD model switch will be disabled)
	PLACEMENT_CREATION_FLAG_FORCE_DEFERRED_MODEL = 18    // if set the pickup will render as deferred model (no transparency/alpha blending in this render mode)
};

enum eDispatchType {
	DT_INVALID,
	DT_POLICE_AUTOMOBILE,
	DT_POLICE_HELICOPTER,
	DT_FIRE_DEPARTMENT,
	DT_SWAT_AUTOMOBILE,
	DT_AMBULANCE_DEPARTMENT,
	DT_POLICE_RIDERS,
	DT_POLICE_VEHICLE_REQUEST,
	DT_POLICE_ROAD_BLOCK,
	DT_POLICE_AUTOMOBILE_WAIT_PULLED_OVER,
	DT_POLICE_AUTOMOBILE_WAIT_CRUISING,
	DT_GANGS,
	DT_SWAT_HELICOPTER,
	DT_POLICE_BOAT,
	DT_ARMY_VEHICLE,
	DT_BIKER_BACKUP,
	DT_ASSASSINS
};

enum eApplyForceType
{
	APPLY_TYPE_FORCE = 0,			//continuous force applied every frame, will produce a constant acceleration independent of frame rate.
	APPLY_TYPE_IMPULSE,				//Caution a single impact applied at a point in time, will produce a step change in velocity. If applied continuously will produce an acceleration that IS dependant on frame rate and may also break the physics engine, i.e. please do
	APPLY_TYPE_EXTERNAL_FORCE,		//same as a normal force but applied as if it was an external push to the object, through the breaking system. This means that breakable objects will break apart due to the force you're applying. 	
	APPLY_TYPE_EXTERNAL_IMPULSE, 	//ditto, a nomal impulse plus breaking.			
	APPLY_TYPE_TORQUE,				// Angular equivalent of force, causes continuous angular acceleration independent of framerate
	APPLY_TYPE_ANGULAR_IMPULSE		// Angular equivalent of impulse, causes instantaneous change in angular velocity
};

enum eAudioSpecialEffectMode
{
	AUDIO_SPECIAL_EFFECT_MODE_NORMAL,
	AUDIO_SPECIAL_EFFECT_MODE_UNDERWATER,
	AUDIO_SPECIAL_EFFECT_MODE_STONED,
	AUDIO_SPECIAL_EFFECT_MODE_PAUSE_MENU,
	AUDIO_SPECIAL_EFFECT_MODE_SLOW_MOTION,
	AUDIO_SPECIAL_EFFECT_MODE_DRUNK_STAGE01,
	AUDIO_SPECIAL_EFFECT_MODE_DRUNK_STAGE02,
	AUDIO_SPECIAL_EFFECT_MODE_DRUNK_STAGE03,
	NUM_AUDIO_SPECIAL_EFFECT_MODE
};

// General info about stats (unknown modder)
// Any stat that has characterStat="true" in spstatsetup has SP{0,1,2}_ infront of its name(like SP0_TOTAL_CASH. All other stats are as they appear in the file
// Likewise for mp stats(not that you should be messing with them), but all other with CharacterStat = "true" has MP{ 0,1,2,3,4 }_ infront of them
// SP0 = Michael, SP1 = Franklin, SP2 = Trevor

// Ability Stats:
// Type = int. Min = 0, Max = 100.
// STAMINA			
// STRENGTH
// LUNG_CAPACITY
// WHEELIE_ABILITY
// FLYING_ABILITY
// SHOOTING_ABILITY
// STEALTH_ABILITY
#pragma endregion

#pragma region Const Arrays
constexpr int AnimPostFXSize = 68;
constexpr char* AnimPostFX[AnimPostFXSize] = {
	"DefaultFlash", "DefaultLensDistortion", "DefaultColorGrade", "DefaultMenuFadeIn",
	"DefaultMenuFadeOut"

	"SwitchSceneNeutral", "SwitchSceneNetural", "SwitchSceneFranklin",
	"SwitchSceneMichael", "SwitchSceneTrevor",

	"PauseMenuIn", "PauseMenuOut", "PauseMenuFranklinIn", "PauseMenuFranklinOut",
	"PauseMenuMichaelIn", "PauseMenuMichaelOut", "PauseMenuTrevorIn",
	"PauseMenuTrevorOut"

	"MenuMGIn", "MenuMGOut", "MenuMGFranklinIn", "MenuMGFranklinOut",
	"MenuMGMichaelIn", "MenuMGMichaelOut", "MenuMGTrevorIn", "MenuMGTrevorOut",
	"MenuMGHeistIn", "MenuMGHeistOut", "MenuMGHeistIntro", "MenuMGHeistTint",

	"MenuMGIslandHeistIn", "MenuMGRemixIn", "MenuMGSelectionIn",
	"MenuMGSelectionTint", "MenuMGTournamentIn", "MenuMGTournamentTint",
	"MenuSurvivalAlienIn",

	"SwitchHUDIn", "SwitchHUDOut", "SwitchHUDFranklinIn", "SwitchHUDFranklinOut",
	"SwitchHUDMichaelIn", "SwitchHUDMichaelOut", "SwitchHUDTrevorIn",
	"SwitchHUDTrevorOut",

	"SwitchShortFranklinIn", "SwitchShortFranklinMid", "SwitchShortMichaelIn",
	"SwitchShortMichaelMid", "SwitchShortTrevorIn", "SwitchShortTrevorMid",
	"SwitchShortNeutralIn", "SwitchShortNeutralMid",

	"SwitchOpenNeutral", "SwitchOpenNeutralFIB5", "SwitchOpenNeutralOutHeist",
	"SwitchOpenFranklinIn", "SwitchOpenMichaelIn", "SwitchOpenTrevorIn",
	"SwitchOpenNeutralIn", "SwitchOpenFranklinMid", "SwitchOpenMichaelMid",
	"SwitchOpenTrevorMid", "SwitchOpenNeutralMid", "SwitchOpenFranklinOut",
	"SwitchOpenMichaelOut", "SwitchOpenTrevorOut", "SwitchOpenNeutralOut"
};

constexpr int AbilityPostFXSize = 8;
constexpr char* AbilityPostFX[AbilityPostFXSize] = {
	"DrivingFocus", "DrivingFocusOut", "BulletTime", "BulletTimeOut",
	"REDMIST", "REDMISTOut", "Rampage", "RampageOut"
};

constexpr int RadioStationsSize = 86;
constexpr char* RadioStations[RadioStationsSize] = { 
	"RADIO_01_CLASS_ROCK", "RADIO_02_POP", "RADIO_03_HIPHOP_NEW", "RADIO_04_PUNK",
	"RADIO_05_TALK_01", "RADIO_06_COUNTRY", "RADIO_07_DANCE_01", "RADIO_08_MEXICAN",
	"RADIO_09_HIPHOP_OLD", "RADIO_11_TALK_02", "RADIO_12_REGGAE", "RADIO_13_JAZZ",
	"RADIO_14_DANCE_02", "RADIO_15_MOTOWN", "RADIO_16_SILVERLAKE", "RADIO_17_FUNK",
	"RADIO_18_90S_ROCK", "RADIO_19_USER", "RADIO_20_THELAB", "RADIO_21_DLC_XM17",
	"RADIO_22_DLC_BATTLE_MIX1_CLUB", "RADIO_22_DLC_BATTLE_MIX1_RADIO",
	"RADIO_23_DLC_BATTLE_MIX2_CLUB", "RADIO_23_DLC_XM19_RADIO",
	"RADIO_24_DLC_BATTLE_MIX3_CLUB", "RADIO_25_DLC_BATTLE_MIX4_CLUB",
	"RADIO_26_DLC_BATTLE_CLUB_WARMUP", "RADIO_27_DLC_PRHEI4",
	"RADIO_30_DLC_HEI4_MIX1_REVERB", "RADIO_31_DLC_HEI4_KM_CLUB",
	"RADIO_32_DLC_HEI4_MM_CLUB", "RADIO_33_DLC_HEI4_PT_CLUB", "RADIO_34_DLC_HEI4_KULT",
	"RADIO_35_DLC_HEI4_MLR", "RADIO_36_AUDIOPLAYER", "RADIO_37_MOTOMAMI",
	"DLC_BATTLE_MIX1_CLUB_PRIV", "DLC_BATTLE_MIX2_CLUB_PRIV", "DLC_BATTLE_MIX3_CLUB_PRIV",
	"DLC_BATTLE_MIX4_CLUB_PRIV", "HIDDEN_RADIO_01_CLASS_ROCK", "HIDDEN_RADIO_02_POP",
	"HIDDEN_RADIO_03_HIPHOP_NEW", "HIDDEN_RADIO_04_PUNK", "HIDDEN_RADIO_06_COUNTRY",
	"HIDDEN_RADIO_07_DANCE_01", "HIDDEN_RADIO_09_HIPHOP_OLD", "HIDDEN_RADIO_10_SURF",
	"HIDDEN_RADIO_12_REGGAE", "HIDDEN_RADIO_15_MOTOWN", "HIDDEN_RADIO_16_SILVERLAKE",
	"HIDDEN_RADIO_17_FUNK", "HIDDEN_RADIO_34_DLC_HEI4_KULT", "HIDDEN_RADIO_37_MOTOMAMI",
	"HIDDEN_RADIO_ADVERTS", "HIDDEN_RADIO_AMBIENT_TV", "HIDDEN_RADIO_AMBIENT_TV_BRIGHT",
	"HIDDEN_RADIO_ARCADE_DANCE", "HIDDEN_RADIO_ARCADE_EDM",
	"HIDDEN_RADIO_ARCADE_MIRROR_PARK", "HIDDEN_RADIO_ARCADE_POP",
	"HIDDEN_RADIO_ARCADE_WWFM", "HIDDEN_RADIO_ASTU_DRUMS_VOX", "HIDDEN_RADIO_ASTU_SILENCE",
	"HIDDEN_RADIO_ASTU_TRANS_01", "HIDDEN_RADIO_ASTU_TRANS_01_VOX",
	"HIDDEN_RADIO_ASTU_TRANS_02_VOX", "HIDDEN_RADIO_ASTU_VOX",
	"HIDDEN_RADIO_BIKER_CLASSIC_ROCK", "HIDDEN_RADIO_BIKER_HIP_HOP",
	"HIDDEN_RADIO_BIKER_MODERN_ROCK", "HIDDEN_RADIO_BIKER_PUNK",
	"HIDDEN_RADIO_CASINO", "HIDDEN_RADIO_CASINO_PENTHOUSE_P", "HIDDEN_RADIO_FLYLO",
	"HIDDEN_RADIO_IFRUIT", "HIDDEN_RADIO_ML_PROMOTER", "HIDDEN_RADIO_MLR",
	"HIDDEN_RADIO_MPSUM2_NEWS", "HIDDEN_RADIO_SEC_BILLIONAIRE",
	"HIDDEN_RADIO_SEC_STUDIO_LOUNGE", "HIDDEN_RADIO_STRIP_CLUB", "HIDDEN_RADIO_THE_LAB",
	"HIDDEN_RADIO_TUNER_CAR_MEET", "HIDDEN_RADIO_TUNER_MM", "HIDDEN_RADIO_TUNER_TEST_AREA"
};

constexpr int ScenariosSize = 247;
constexpr char* Scenarios[ScenariosSize] = {
	"WORLD_HUMAN_AA_COFFEE", "WORLD_HUMAN_AA_SMOKE", "WORLD_HUMAN_BINOCULARS", "WORLD_HUMAN_BUM_FREEWAY",
	"WORLD_HUMAN_BUM_SLUMPED", "WORLD_HUMAN_BUM_STANDING", "WORLD_HUMAN_BUM_WASH", "WORLD_HUMAN_CAR_PARK_ATTENDANT",
	"WORLD_HUMAN_CHEERING", "WORLD_HUMAN_CLIPBOARD", "WORLD_HUMAN_CLIPBOARD_FACILITY", "WORLD_HUMAN_CONST_DRILL",
	"WORLD_HUMAN_COP_IDLES", "WORLD_HUMAN_DRINKING", "WORLD_HUMAN_DRINKING_FACILITY", "WORLD_HUMAN_DRINKING_CASINO_TERRACE",
	"WORLD_HUMAN_DRUG_DEALER", "WORLD_HUMAN_DRUG_DEALER_HARD", "WORLD_HUMAN_DRUG_FIELD_WORKERS_RAKE",
	"WORLD_HUMAN_DRUG_FIELD_WORKERS_WEEDING", "WORLD_HUMAN_DRUG_PROCESSORS_COKE", "WORLD_HUMAN_DRUG_PROCESSORS_WEED",
	"WORLD_HUMAN_MOBILE_FILM_SHOCKING", "WORLD_HUMAN_GARDENER_LEAF_BLOWER", "WORLD_HUMAN_GARDENER_PLANT",
	"WORLD_HUMAN_GOLF_PLAYER", "WORLD_HUMAN_GUARD_PATROL", "WORLD_HUMAN_GUARD_STAND", "WORLD_HUMAN_GUARD_STAND_CASINO",
	"WORLD_HUMAN_GUARD_STAND_CLUBHOUSE", "WORLD_HUMAN_GUARD_STAND_FACILITY", "WORLD_HUMAN_GUARD_STAND_ARMY", "WORLD_HUMAN_HAMMERING",
	"WORLD_HUMAN_HANG_OUT_STREET", "WORLD_HUMAN_HANG_OUT_STREET_CLUBHOUSE", "WORLD_HUMAN_HIKER", "WORLD_HUMAN_HIKER_STANDING",
	"WORLD_HUMAN_HUMAN_STATUE", "WORLD_HUMAN_INSPECT_CROUCH", "WORLD_HUMAN_INSPECT_STAND", "WORLD_HUMAN_JANITOR",
	"WORLD_HUMAN_JOG", "WORLD_HUMAN_JOG_STANDING", "WORLD_HUMAN_LEANING", "WORLD_HUMAN_LEANING_CASINO_TERRACE",
	"WORLD_HUMAN_MAID_CLEAN", "WORLD_HUMAN_MUSCLE_FLEX", "WORLD_HUMAN_MUSCLE_FREE_WEIGHTS", "WORLD_HUMAN_MUSICIAN",
	"WORLD_HUMAN_PAPARAZZI", "WORLD_HUMAN_PARTYING", "WORLD_HUMAN_PICNIC", "WORLD_HUMAN_POWER_WALKER",
	"WORLD_HUMAN_PROSTITUTE_HIGH_CLASS", "WORLD_HUMAN_PROSTITUTE_LOW_CLASS", "WORLD_HUMAN_PUSH_UPS", "WORLD_HUMAN_SEAT_LEDGE",
	"WORLD_HUMAN_SEAT_LEDGE_EATING", "WORLD_HUMAN_SEAT_STEPS", "WORLD_HUMAN_SEAT_WALL", "WORLD_HUMAN_SEAT_WALL_EATING",
	"WORLD_HUMAN_SEAT_WALL_TABLET", "WORLD_HUMAN_SECURITY_SHINE_TORCH", "WORLD_HUMAN_SIT_UPS", "WORLD_HUMAN_SMOKING",
	"WORLD_HUMAN_SMOKING_CLUBHOUSE", "WORLD_HUMAN_SMOKING_POT", "WORLD_HUMAN_SMOKING_POT_CLUBHOUSE", "WORLD_HUMAN_STAND_FIRE",
	"WORLD_HUMAN_STAND_FISHING", "WORLD_HUMAN_STAND_IMPATIENT", "WORLD_HUMAN_STAND_IMPATIENT_CLUBHOUSE",
	"WORLD_HUMAN_STAND_IMPATIENT_FACILITY", "WORLD_HUMAN_STAND_IMPATIENT_UPRIGHT", "WORLD_HUMAN_STAND_IMPATIENT_UPRIGHT_FACILITY",
	"WORLD_HUMAN_STAND_MOBILE", "WORLD_HUMAN_STAND_MOBILE_CLU	BHOUSE", "WORLD_HUMAN_STAND_MOBILE_FACILITY",
	"WORLD_HUMAN_STAND_MOBILE_UPRIGHT", "WORLD_HUMAN_STAND_MOBILE_UPRIGHT_CLUBHOUSE", "WORLD_HUMAN_STRIP_WATCH_STAND",
	"WORLD_HUMAN_STUPOR", "WORLD_HUMAN_STUPOR_CLUBHOUSE", "WORLD_HUMAN_WINDOW_SHOP_BROWSE_SHOWROOM",
	"WORLD_HUMAN_SUNBATHE", "WORLD_HUMAN_SUNBATHE_BACK", "WORLD_HUMAN_SUPERHERO", "WORLD_HUMAN_SWIMMING",
	"WORLD_HUMAN_TENNIS_PLAYER", "WORLD_HUMAN_TOURIST_MAP", "WORLD_HUMAN_TOURIST_MOBILE", "WORLD_HUMAN_VALET",
	"WORLD_HUMAN_VEHICLE_MECHANIC", "WORLD_HUMAN_WELDING", "WORLD_HUMAN_WINDOW_SHOP_BROWSE", "WORLD_HUMAN_YOGA", "Walk",
	"Walk_Facility", "WORLD_BOAR_GRAZING", "WORLD_CAT_SLEEPING_GROUND", "WORLD_CAT_SLEEPING_LEDGE", "WORLD_COW_GRAZING",
	"WORLD_COYOTE_HOWL", "WORLD_COYOTE_REST", "WORLD_COYOTE_WANDER", "WORLD_COYOTE_WALK", "WORLD_CHICKENHAWK_FEEDING",
	"WORLD_CHICKENHAWK_STANDING", "WORLD_CORMORANT_STANDING", "WORLD_CROW_FEEDING", "WORLD_CROW_STANDING",
	"WORLD_DEER_GRAZING", "WORLD_DOG_BARKING_ROTTWEILER", "WORLD_DOG_BARKING_RETRIEVER", "WORLD_DOG_BARKING_SHEPHERD",
	"WORLD_DOG_SITTING_ROTTWEILER", "WORLD_DOG_SITTING_RETRIEVER", "WORLD_DOG_SITTING_SHEPHERD", "WORLD_DOG_BARKING_SMALL",
	"WORLD_DOG_SITTING_SMALL", "WORLD_DOLPHIN_SWIM", "WORLD_FISH_FLEE", "WORLD_FISH_IDLE", "WORLD_GULL_FEEDING",
	"WORLD_GULL_STANDING", "WORLD_HEN_FLEE", "WORLD_HEN_PECKING", "WORLD_HEN_STANDING", "WORLD_MOUNTAIN_LION_REST",
	"WORLD_MOUNTAIN_LION_WANDER", "WORLD_ORCA_SWIM", "WORLD_PIG_GRAZING", "WORLD_PIGEON_FEEDING", "WORLD_PIGEON_STANDING",
	"WORLD_RABBIT_EATING", "WORLD_RABBIT_FLEE", "WORLD_RATS_EATING", "WORLD_RATS_FLEEING", "WORLD_SHARK_SWIM",
	"WORLD_SHARK_HAMMERHEAD_SWIM", "WORLD_STINGRAY_SWIM", "WORLD_WHALE_SWIM", "DRIVE", "WORLD_VEHICLE_ATTRACTOR",
	"PARK_VEHICLE", "WORLD_VEHICLE_AMBULANCE", "WORLD_VEHICLE_BICYCLE_BMX", "WORLD_VEHICLE_BICYCLE_BMX_BALLAS",
	"WORLD_VEHICLE_BICYCLE_BMX_FAMILY", "WORLD_VEHICLE_BICYCLE_BMX_HARMONY", "WORLD_VEHICLE_BICYCLE_BMX_VAGOS",
	"WORLD_VEHICLE_BICYCLE_MOUNTAIN", "WORLD_VEHICLE_BICYCLE_ROAD", "WORLD_VEHICLE_BIKE_OFF_ROAD_RACE",
	"WORLD_VEHICLE_BIKER", "WORLD_VEHICLE_BOAT_IDLE", "WORLD_VEHICLE_BOAT_IDLE_ALAMO", "WORLD_VEHICLE_BOAT_IDLE_MARQUIS",
	"WORLD_VEHICLE_BROKEN_DOWN", "WORLD_VEHICLE_BUSINESSMEN", "WORLD_VEHICLE_HELI_LIFEGUARD", "WORLD_VEHICLE_CLUCKIN_BELL_TRAILER",
	"WORLD_VEHICLE_CONSTRUCTION_SOLO", "WORLD_VEHICLE_CONSTRUCTION_PASSENGERS", "WORLD_VEHICLE_DRIVE_PASSENGERS",
	"WORLD_VEHICLE_DRIVE_PASSENGERS_LIMITED", "WORLD_VEHICLE_DRIVE_SOLO", "WORLD_VEHICLE_FARM_WORKER",
	"WORLD_VEHICLE_FIRE_TRUCK", "WORLD_VEHICLE_EMPTY", "WORLD_VEHICLE_MARIACHI", "WORLD_VEHICLE_MECHANIC",
	"WORLD_VEHICLE_MILITARY_PLANES_BIG", "WORLD_VEHICLE_MILITARY_PLANES_SMALL", "WORLD_VEHICLE_PARK_PARALLEL",
	"WORLD_VEHICLE_PARK_PERPENDICULAR_NOSE_IN", "WORLD_VEHICLE_PASSENGER_EXIT", "WORLD_VEHICLE_POLICE_BIKE",
	"WORLD_VEHICLE_POLICE_CAR", "WORLD_VEHICLE_POLICE_NEXT_TO_CAR", "WORLD_VEHICLE_QUARRY", "WORLD_VEHICLE_SALTON",
	"WORLD_VEHICLE_SALTON_DIRT_BIKE", "WORLD_VEHICLE_SECURITY_CAR", "WORLD_VEHICLE_STREETRACE", "WORLD_VEHICLE_TOURBUS",
	"WORLD_VEHICLE_TOURIST", "WORLD_VEHICLE_TANDL", "WORLD_VEHICLE_TRACTOR", "WORLD_VEHICLE_TRACTOR_BEACH",
	"WORLD_VEHICLE_TRUCK_LOGS", "WORLD_VEHICLE_TRUCKS_TRAILERS", "PROP_BIRD_IN_TREE", "WORLD_VEHICLE_DISTANT_EMPTY_GROUND",
	"PROP_BIRD_TELEGRAPH_POLE", "PROP_HUMAN_ATM", "PROP_HUMAN_BBQ", "PROP_HUMAN_BUM_BIN", "PROP_HUMAN_BUM_SHOPPING_CART",
	"PROP_HUMAN_MUSCLE_CHIN_UPS", "PROP_HUMAN_MUSCLE_CHIN_UPS_ARMY", "PROP_HUMAN_MUSCLE_CHIN_UPS_PRISON", "PROP_HUMAN_PARKING_METER",
	"PROP_HUMAN_SEAT_ARMCHAIR", "PROP_HUMAN_SEAT_BAR", "PROP_HUMAN_SEAT_BENCH", "PROP_HUMAN_SEAT_BENCH_FACILITY",
	"PROP_HUMAN_SEAT_BENCH_DRINK", "PROP_HUMAN_SEAT_BENCH_DRINK_FACILITY", "PROP_HUMAN_SEAT_BENCH_DRINK_BEER",
	"PROP_HUMAN_SEAT_BENCH_FOOD", "PROP_HUMAN_SEAT_BENCH_FOOD_FACILITY", "PROP_HUMAN_SEAT_BUS_STOP_WAIT",
	"PROP_HUMAN_SEAT_CHAIR", "PROP_HUMAN_SEAT_CHAIR_DRINK", "PROP_HUMAN_SEAT_CHAIR_DRINK_BEER", "PROP_HUMAN_SEAT_CHAIR_FOOD",
	"PROP_HUMAN_SEAT_CHAIR_UPRIGHT", "PROP_HUMAN_SEAT_CHAIR_UPRIGHT_SHOWROOM", "PROP_HUMAN_SEAT_CHAIR_MP_PLAYER",
	"PROP_HUMAN_SEAT_COMPUTER", "PROP_HUMAN_SEAT_COMPUTER_LOW", "PROP_HUMAN_SEAT_DECKCHAIR", "PROP_HUMAN_SEAT_DECKCHAIR_DRINK",
	"PROP_HUMAN_SEAT_MUSCLE_BENCH_PRESS", "PROP_HUMAN_SEAT_MUSCLE_BENCH_PRESS_PRISON", "PROP_HUMAN_SEAT_SEWING",
	"PROP_HUMAN_SEAT_STRIP_WATCH", "PROP_HUMAN_SEAT_SUNLOUNGER", "PROP_HUMAN_STAND_IMPATIENT",
	"CODE_HUMAN_COWER", "CODE_HUMAN_CROSS_ROAD_WAIT", "CODE_HUMAN_PARK_CAR", "PROP_HUMAN_MOVIE_BULB",
	"PROP_HUMAN_MOVIE_STUDIO_LIGHT", "CODE_HUMAN_MEDIC_KNEEL", "CODE_HUMAN_MEDIC_TEND_TO_DEAD",
	"CODE_HUMAN_MEDIC_TIME_OF_DEATH", "CODE_HUMAN_POLICE_CROWD_CONTROL", "CODE_HUMAN_POLICE_INVESTIGATE", "Standing",
	"CHAINING_ENTRY", "CHAINING_EXIT", "CODE_HUMAN_STAND_COWER", "EAR_TO_TEXT", "EAR_TO_TEXT_FAT", "WORLD_LOOKAT_POINT"
};

constexpr int ScenarioGroupsSize = 235;
constexpr char* ScenarioGroups[ScenarioGroupsSize] = {
	"Agency_Heist_Peds",   "ALAMO_PLANES",   "AMMUNATION",  "ARMY_GUARD",
	"ATTRACT_PAP",   "BLIMP",   "CanyonCliffs",   "CanyonCliffs_Start",  "CAR_WASH",
	"CHINESE2_HILLBILLIES",   "Chinese2_Lunch",   "Chumash_14_Bikers",   "Chumash_14_Bikers",  "Chumash_14_Cops",
	"Chumash_14_Cops",   "Chumash_14_Hookers",   "Chumash_14_Hookers",   "Chumash_14_Vagos",  "Chumash_14_Vagos",
	"Cinema_Downtown",   "Cinema_Morningwood",   "Cinema_Textile",   "City_Banks",  "clothes_shop",
	"Countryside_Banks",   "DEALERSHIP",   "Del_Perro_16_Bikers",   "Del_Perro_16_Bikers",  "Del_Perro_16_Cops",
	"Del_Perro_16_Cops",   "Del_Perro_16_Hookers",   "Del_Perro_16_Hookers",   "Del_Perro_16_Vagos",  "Del_Perro_16_Vagos",
	"DowntownAlley_33_Bikers",   "DowntownAlley_33_Bikers",   "DowntownAlley_33_Cops",   "DowntownAlley_33_Cops",  "DowntownAlley_33_Hookers",
	"DowntownAlley_33_Hookers",   "DowntownAlley_33_Vagos",   "DowntownAlley_33_Vagos",   "E_Canals_22_Bikers",  "E_Canals_22_Cops",
	"E_Canals_22_Hookers",   "E_Canals_22_Vagos",   "E_Cypress_05_Bikers",   "E_Cypress_05_Cops",  "E_Cypress_05_Hookers",
	"E_Cypress_05_Vagos",   "E_Hollywood_39_Bikers",   "E_Hollywood_39_Cops",   "E_Hollywood_39_Hookers",  "E_Hollywood_39_Vagos",
	"E_PaletoBay_10_Bikers",   "E_PaletoBay_10_Cops",   "E_PaletoBay_10_Hookers",   "E_PaletoBay_10_Vagos",  "E_Puerto_26_Bikers",
	"E_Puerto_26_Cops",   "E_Puerto_26_Hookers",   "E_Puerto_26_Vagos",   "E_SandyShores_12_Bikers",  "E_SandyShores_12_Cops",
	"E_SandyShores_12_Hookers",   "E_SandyShores_12_Vagos",   "E_Silverlake_40_Bikers",   "E_Silverlake_40_Cops",  "E_Silverlake_40_Hookers",
	"E_Silverlake_40_Vagos",   "EastLS_BMX",   "EastLS_Chopshop_35_Bikers",   "EastLS_Chopshop_35_Cops",  "EastLS_Chopshop_35_Vagos",
	"EastLS_Skatepark_36_Bikers",   "EastLS_Skatepark_36_Cops",   "EastLS_Skatepark_36_Hookers",   "EastLS_Skatepark_36_Vagos",  "Eclipse_32_Bikers",
	"Eclipse_32_Cops",   "Eclipse_32_Hookers",   "Eclipse_32_Vagos",   "EcoFriendly",  "EcoFriendly_Start",
	"ElBurro_Shed_38_Bikers",   "ElBurro_Shed_38_Cops",   "ElBurro_Shed_38_Hookers",   "ElBurro_Shed_38_Vagos",  "ElBurro_Wreck_37_Bikers",
	"ElBurro_Wreck_37_Cops",   "ElBurro_Wreck_37_Hookers",   "ElBurro_Wreck_37_Vagos",   "FIB_AIRPORT",  "FIB_GROUP_1",
	"FIB_GROUP_2",   "Fort_Zancudo_Guards",   "GENTRY_MANNOR",   "Grapeseed_Planes",  "GrapeseedAirstrip_07_Security",
	"Guards_At_Prison",   "Harmony_ChopShop_13_Bikers",   "Harmony_ChopShop_13_Cops",   "Harmony_ChopShop_13_Hookers",  "Harmony_ChopShop_13_Vagos",
	"KORTZ_SECURITY",   "LakesideSplash",   "LakesideSplash_Start",   "LOST_BIKERS",  "LOST_HANGOUT",
	"LSA_Planes",   "Mid_Seoul_24_Bikers",   "Mid_Seoul_24_Cops",   "Mid_Seoul_24_Hookers",  "Mid_Seoul_24_Vagos",
	"MinewardSpiral",   "MinewardSpiral_Start",   "MirrorPark_41_Bikers",   "MirrorPark_41_Cops",  "MirrorPark_41_Hookers",
	"MirrorPark_41_Vagos",   "Morningwood_17_Bikers",   "Morningwood_17_Cops",   "Morningwood_17_Hookers",  "Morningwood_17_Vagos",
	"Movie_Studio_Security",   "MP_Police",   "MP_POLICE2",   "N_W_Hollywood_31_Bikers",  "N_W_Hollywood_31_Cops",
	"N_W_Hollywood_31_Hookers",   "N_W_Hollywood_31_Vagos",   "New group",   "New group 1",  "New group 2",
	"Observatory_Bikers",   "OceanHighway_15_Bikers",   "OceanHighway_15_Cops",   "OceanHighway_15_Hookers",  "OceanHighway_15_Vagos",
	"Paleto_Bank",   "Paleto_Cops",   "Pershing_04_Bikers",   "Pershing_04_Cops",  "Pershing_04_Hookers",
	"Pershing_04_Vagos",   "Pier_18_Bikers",   "Pier_18_Cops",   "Pier_18_Hookers",  "Pier_18_Vagos",
	"PLANE_WRECK",   "PLANE_WRECK_DOG",   "Police_At_Court",   "POLICE_PEDS_CITY",  "POLICE_POUND1",
	"POLICE_POUND2",   "POLICE_POUND3",   "POLICE_POUND4",   "POLICE_POUND5",  "Polo_Goon",
	"PRISON_TOWERS",   "Prison_Transport",   "QUARRY",   "Racecourse_06_Bikers",  "Racecourse_06_Cops",
	"RaceCourse_06_Hookers",   "RaceCourse_06_Vagos",   "Rampage1",   "RidgeRun",  "RidgeRun_Start",
	"S_SanPedro_28_Bikers",   "S_SanPedro_28_Cops",   "S_SanPedro_28_Hookers",   "S_SanPedro_28_Vagos",  "S_Seoul_27_Bikers",
	"S_Seoul_27_Cops",   "S_Seoul_27_Hookers",   "S_Seoul_27_Vagos",   "Sandy_Cops",  "Sandy_Planes",
	"SanPedroGarage_30_Bikers",   "SanPedroGarage_30_Cops",   "SanPedroGarage_30_Hookers",   "SanPedroGarage_30_Vagos",  "SCRAP_SECURITY",
	"Scrapyard_29_Bikers",   "Scrapyard_29_Cops",   "Scrapyard_29_Hookers",   "Scrapyard_29_Vagos",  "SenoraAirstrip_08_Security",
	"SeoulPark_23_Bikers",   "SeoulPark_23_Cops",   "SeoulPark_23_Hookers",   "SeoulPark_23_Vagos",  "SEW_MACHINE",
	"SOLOMON_GATE",   "StrawberryClub_34_Bikers",   "StrawberryClub_34_Cops",   "StrawberryClub_34_Hookers",  "StrawberryClub_34_Vagos",
	"Triathlon_1",   "Triathlon_1_Start",   "Triathlon_2",   "Triathlon_2_Start",  "Triathlon_3",
	"Triathlon_3_Start",   "VAGOS_HANGOUT",   "ValleyTrail",   "ValleyTrail_Start",  "VANGELICO",
	"Vespucci_20_Bikers",   "Vespucci_20_Cops",   "Vespucci_20_Hookers",   "Vespucci_20_Vagos",  "VespucciBeach_19_Bikers",
	"VespucciBeach_19_Cops",   "VespucciBeach_19_Hookers",   "VespucciBeach_19_Vagos",   "W_Canals_21_Bikers",  "W_Canals_21_Cops",
	"W_Canals_21_Hookers",   "W_Canals_21_Vagos",   "W_PaletoBay_09_Bikers",   "W_PaletoBay_09_Cops",  "W_PaletoBay_09_Hookers",
	"W_PaletoBay_09_Vagos",   "W_Puerto_25_Bikers",   "W_Puerto_25_Cops",   "W_Puerto_25_Hookers",  "W_Puerto_25_Vagos",
	"W_SandyShores_11_Bikers",   "W_SandyShores_11_Cops",   "W_SandyShores_11_Hookers",   "W_SandyShores_11_Vagos",  "WATERSPORTS",
	"YellowJackInn"
};

/*
constexpr int AllHandsBonetagsSize = 34;
constexpr int AllHandsBonetags[AllHandsBonetagsSize] =
						{ BONETAG_PH_R_HAND, BONETAG_R_HAND, BONETAG_R_FINGER0, BONETAG_R_FINGER01,
						  BONETAG_R_FINGER02, BONETAG_R_FINGER1, BONETAG_R_FINGER11, BONETAG_R_FINGER12,
						  BONETAG_R_FINGER2, BONETAG_R_FINGER21, BONETAG_R_FINGER22, BONETAG_R_FINGER3,
						  BONETAG_R_FINGER31, BONETAG_R_FINGER32, BONETAG_R_FINGER4, BONETAG_R_FINGER41,
						  BONETAG_R_FINGER42,
						  BONETAG_PH_L_HAND, BONETAG_L_HAND, BONETAG_L_FINGER0, BONETAG_L_FINGER01,
						  BONETAG_L_FINGER02, BONETAG_L_FINGER1, BONETAG_L_FINGER11, BONETAG_L_FINGER12,
						  BONETAG_L_FINGER2, BONETAG_L_FINGER21, BONETAG_L_FINGER22, BONETAG_L_FINGER3,
						  BONETAG_L_FINGER31, BONETAG_L_FINGER32, BONETAG_L_FINGER4, BONETAG_L_FINGER41,
						  BONETAG_L_FINGER42
						};

constexpr int RightHandBonetagsSize = 17;
constexpr int RightHandBonetags[RightHandBonetagsSize] =
						{ BONETAG_PH_R_HAND, BONETAG_R_HAND, BONETAG_R_FINGER0, BONETAG_R_FINGER01,
						  BONETAG_R_FINGER02, BONETAG_R_FINGER1, BONETAG_R_FINGER11, BONETAG_R_FINGER12,
						  BONETAG_R_FINGER2, BONETAG_R_FINGER21, BONETAG_R_FINGER22, BONETAG_R_FINGER3,
						  BONETAG_R_FINGER31, BONETAG_R_FINGER32, BONETAG_R_FINGER4, BONETAG_R_FINGER41,
						  BONETAG_R_FINGER42
						};
*/
#pragma endregion

#pragma region Task Enums
enum eScriptLookFlag {
	SLF_DEFAULT = 0,
	SLF_SLOW_TURN_RATE = 1,   // turn the head toward the target slowly
	SLF_FAST_TURN_RATE = 2,   // turn the head toward the target quickly
	SLF_EXTEND_YAW_LIMIT = 4,   // wide yaw head limits
	SLF_EXTEND_PITCH_LIMIT = 8,   // wide pitch head limit
	SLF_WIDEST_YAW_LIMIT = 16,  // widest yaw head limit
	SLF_WIDEST_PITCH_LIMIT = 32,  // widest pitch head limit
	SLF_NARROW_YAW_LIMIT = 64,  // narrow yaw head limits
	SLF_NARROW_PITCH_LIMIT = 128, // narrow pitch head limit
	SLF_NARROWEST_YAW_LIMIT = 256, // narrowest yaw head limit
	SLF_NARROWEST_PITCH_LIMIT = 512, // narrowest pitch head limit
	SLF_USE_TORSO = 1024, // use the torso aswell as the neck and head (currently disabled)
	SLF_WHILE_NOT_IN_FOV = 2048,// keep tracking the target even if they are not in the hard coded FOV
	SLF_USE_CAMERA_FOCUS = 4096,// use the camera as the target
	SLF_USE_EYES_ONLY = 8192,// only track the target with the eyes   
	SLF_USE_LOOK_DIR = 16384,// use information in look dir DOF
	SLF_FROM_SCRIPT = 32768, // internal use only
	SLF_USE_REF_DIR_ABSOLUTE = 65536  // use absolute reference direction mode for solver
};

enum eScriptLookPriority {
	SLF_LOOKAT_VERY_LOW = 0,
	SLF_LOOKAT_LOW,
	SLF_LOOKAT_MEDIUM,
	SLF_LOOKAT_HIGH,
	SLF_LOOKAT_VERY_HIGH
};

enum eTaskHandsUpFlags {
	HANDS_UP_NOTHING = 0,
	HANDS_UP_STRAIGHT_TO_LOOP = 1
};

enum eEnterExitVehicleFlags {
	// If the task is interupted (bumped, shot), dont resume.
	ECF_RESUME_IF_INTERRUPTED = 1,
	// Warps ped to entry point ready to open the door/enter seat
	ECF_WARP_ENTRY_POINT = 2,
	// Jack the ped occupying the vehicle, regardless of relationship status
	ECF_JACK_ANYONE = 8,
	// Warp the ped onto the vehicle
	ECF_WARP_PED = 16,
	// Dont wait for the vehicle to stop before exiting
	ECF_DONT_WAIT_FOR_VEHICLE_TO_STOP = 64,
	// Dont close the vehicle door
	ECF_DONT_CLOSE_DOOR = 256,
	// Allow ped to warp to the seat if entry is blocked
	ECF_WARP_IF_DOOR_IS_BLOCKED = 512,
	// Jump out of the vehicle
	ECF_JUMP_OUT = 4096,
	// TASK_LEAVE_ANY_VEHICLE auto defaults the ECF_WARP_IF_DOOR_IS_BLOCKED, set this flag to not set that
	ECF_DONT_DEFAULT_WARP_IF_DOOR_BLOCKED = 65536,
	// Use entry/exit point on the left hand side
	ECF_USE_LEFT_ENTRY = 131072,
	// Use entry/exit point on the right hand side
	ECF_USE_RIGHT_ENTRY = 262144,
	// When jacking just pull the ped out, but don't get in
	ECF_JUST_PULL_PED_OUT = 524288,
	// Disable shuffling - forces ped to use direct door only
	ECF_BLOCK_SEAT_SHUFFLING = 1048576,
	//  Allow ped to warp if the shuffle link to that seat is blocked by someone
	ECF_WARP_IF_SHUFFLE_LINK_IS_BLOCKED = 4194304,
	// Never jack anyone when entering/exiting
	ECF_DONT_JACK_ANYONE = 8388608,
	// Wait for our entry point to be clear of peds before exiting
	ECF_WAIT_FOR_ENTRY_POINT_TO_BE_CLEAR = 16777216
};

enum eTaskVehicleChaseBehaviorFlags {
	VEHICLE_CHASE_CANT_BLOCK = 1,
	VEHICLE_CHASE_CANT_BLOCK_FROM_PURSUE = 2,
	VEHICLE_CHASE_CANT_PURSUE = 4,
	VEHICLE_CHASE_CANT_RAM = 8,
	VEHICLE_CHASE_CANT_SPIN_OUT = 16,
	VEHICLE_CHASE_CANT_MAKE_AGGRESSIVE_MOVE = 32,
	VEHICLE_CHASE_CANT_CRUISE_IN_FRONT_DURING_BLOCK = 64,
	VEHICLE_CHASE_USE_CONTINUOUS_RAM = 128,
	VEHICLE_CHASE_CANT_PULL_ALONGSIDE = 256,
	VEHICLE_CHASE_CANT_PULL_ALONGSIDE_INFRONT = 512
};

enum eScriptTaskStatus
{
	WAITING_TO_START_TASK = 0,
	PERFORMING_TASK,
	DORMANT_TASK,
	VACANT_STAGE,
	GROUP_TASK_STAGE,
	ATTRACTOR_SCRIPT_TASK_STAGE,
	SECONDARY_TASK_STAGE,
	FINISHED_TASK
};

enum eEventType
{
	EVENT_INVALID = -1,
	EVENT_ACQUAINTANCE_PED_DISLIKE,
	EVENT_ACQUAINTANCE_PED_HATE,
	EVENT_ACQUAINTANCE_PED_LIKE,
	EVENT_ACQUAINTANCE_PED_RESPECT,
	EVENT_ACQUAINTANCE_PED_WANTED,
	EVENT_ACQUAINTANCE_PED_DEAD,
	EVENT_AGITATED,
	EVENT_AGITATED_ACTION,
	EVENT_ENCROACHING_PED,
	EVENT_CALL_FOR_COVER,
	EVENT_CAR_UNDRIVEABLE,
	EVENT_CLIMB_LADDER_ON_ROUTE,
	EVENT_CLIMB_NAVMESH_ON_ROUTE,
	EVENT_COMBAT_TAUNT,
	EVENT_COMMUNICATE_EVENT,
	EVENT_COP_CAR_BEING_STOLEN,
	EVENT_CRIME_REPORTED,
	EVENT_DAMAGE,
	EVENT_DEAD_PED_FOUND,
	EVENT_DEATH,
	EVENT_DRAGGED_OUT_CAR,
	EVENT_DUMMY_CONVERSION,
	EVENT_EXPLOSION,
	EVENT_EXPLOSION_HEARD,
	EVENT_FIRE_NEARBY,
	EVENT_FLUSH_TASKS,
	EVENT_FOOT_STEP_HEARD,
	EVENT_GET_OUT_OF_WATER,
	EVENT_GIVE_PED_TASK,
	EVENT_GUN_AIMED_AT,
	EVENT_HELP_AMBIENT_FRIEND,
	EVENT_INJURED_CRY_FOR_HELP,
	EVENT_CRIME_CRY_FOR_HELP,
	EVENT_IN_AIR,
	EVENT_IN_WATER,
	EVENT_INCAPACITATED,
	EVENT_LEADER_ENTERED_CAR_AS_DRIVER,
	EVENT_LEADER_ENTERED_COVER,
	EVENT_LEADER_EXITED_CAR_AS_DRIVER,
	EVENT_LEADER_HOLSTERED_WEAPON,
	EVENT_LEADER_LEFT_COVER,
	EVENT_LEADER_UNHOLSTERED_WEAPON,
	EVENT_MELEE_ACTION,
	EVENT_MUST_LEAVE_BOAT,
	EVENT_NEW_TASK,
	EVENT_NONE,
	EVENT_OBJECT_COLLISION,
	EVENT_ON_FIRE,
	EVENT_OPEN_DOOR,
	EVENT_SHOVE_PED,
	EVENT_PED_COLLISION_WITH_PED,
	EVENT_PED_COLLISION_WITH_PLAYER,
	EVENT_PED_ENTERED_MY_VEHICLE,
	EVENT_PED_JACKING_MY_VEHICLE,
	EVENT_PED_ON_CAR_ROOF,
	EVENT_PED_TO_CHASE,
	EVENT_PED_TO_FLEE,
	EVENT_PLAYER_COLLISION_WITH_PED,
	EVENT_PLAYER_LOCK_ON_TARGET,
	EVENT_POTENTIAL_BE_WALKED_INTO,
	EVENT_POTENTIAL_BLAST,
	EVENT_POTENTIAL_GET_RUN_OVER,
	EVENT_POTENTIAL_WALK_INTO_FIRE,
	EVENT_POTENTIAL_WALK_INTO_OBJECT,
	EVENT_POTENTIAL_WALK_INTO_VEHICLE,
	EVENT_PROVIDING_COVER,
	EVENT_RADIO_TARGET_POSITION,
	EVENT_RAN_OVER_PED,
	EVENT_REACTION_COMBAT_VICTORY,
	EVENT_REACTION_ENEMY_PED,
	EVENT_REACTION_INVESTIGATE_DEAD_PED,
	EVENT_REACTION_INVESTIGATE_THREAT,
	EVENT_REQUEST_HELP_WITH_CONFRONTATION,
	EVENT_RESPONDED_TO_THREAT,
	EVENT_REVIVED,
	EVENT_SCRIPT_COMMAND,
	EVENT_SHOCKING_BROKEN_GLASS,
	EVENT_SHOCKING_CAR_ALARM,
	EVENT_SHOCKING_CAR_CHASE,
	EVENT_SHOCKING_CAR_CRASH,
	EVENT_SHOCKING_BICYCLE_CRASH,
	EVENT_SHOCKING_CAR_PILE_UP,
	EVENT_SHOCKING_CAR_ON_CAR,
	EVENT_SHOCKING_DANGEROUS_ANIMAL,
	EVENT_SHOCKING_DEAD_BODY,
	EVENT_SHOCKING_DRIVING_ON_PAVEMENT,
	EVENT_SHOCKING_BICYCLE_ON_PAVEMENT,
	EVENT_SHOCKING_ENGINE_REVVED,
	EVENT_SHOCKING_EXPLOSION,
	EVENT_SHOCKING_FIRE,
	EVENT_SHOCKING_GUN_FIGHT,
	EVENT_SHOCKING_GUNSHOT_FIRED,
	EVENT_SHOCKING_HELICOPTER_OVERHEAD,
	EVENT_SHOCKING_PARACHUTER_OVERHEAD,
	EVENT_SHOCKING_PED_KNOCKED_INTO_BY_PLAYER,
	EVENT_SHOCKING_HORN_SOUNDED,
	EVENT_SHOCKING_IN_DANGEROUS_VEHICLE,
	EVENT_SHOCKING_INJURED_PED,
	EVENT_SHOCKING_MAD_DRIVER,
	EVENT_SHOCKING_MAD_DRIVER_EXTREME,
	EVENT_SHOCKING_MAD_DRIVER_BICYCLE,
	EVENT_SHOCKING_MUGGING,
	EVENT_SHOCKING_NON_VIOLENT_WEAPON_AIMED_AT,
	EVENT_SHOCKING_PED_RUN_OVER,
	EVENT_SHOCKING_PED_SHOT,
	EVENT_SHOCKING_PLANE_FLY_BY,
	EVENT_SHOCKING_POTENTIAL_BLAST,
	EVENT_SHOCKING_PROPERTY_DAMAGE,
	EVENT_SHOCKING_RUNNING_PED,
	EVENT_SHOCKING_RUNNING_STAMPEDE,
	EVENT_SHOCKING_SEEN_CAR_STOLEN,
	EVENT_SHOCKING_SEEN_CONFRONTATION,
	EVENT_SHOCKING_SEEN_GANG_FIGHT,
	EVENT_SHOCKING_SEEN_INSULT,
	EVENT_SHOCKING_SEEN_MELEE_ACTION,
	EVENT_SHOCKING_SEEN_NICE_CAR,
	EVENT_SHOCKING_SEEN_PED_KILLED,
	EVENT_SHOCKING_SEEN_VEHICLE_TOWED,
	EVENT_SHOCKING_SEEN_WEAPON_THREAT,
	EVENT_SHOCKING_SEEN_WEIRD_PED,
	EVENT_SHOCKING_SEEN_WEIRD_PED_APPROACHING,
	EVENT_SHOCKING_SIREN,
	EVENT_SHOCKING_STUDIO_BOMB,
	EVENT_SHOCKING_VISIBLE_WEAPON,
	EVENT_SHOT_FIRED,
	EVENT_SHOT_FIRED_BULLET_IMPACT,
	EVENT_SHOT_FIRED_WHIZZED_BY,
	EVENT_FRIENDLY_AIMED_AT,
	EVENT_SHOT_FRIENDLY_NEAR_MISS,
	EVENT_SHOUT_BLOCKING_LOS,
	EVENT_SHOUT_TARGET_POSITION,
	EVENT_STATIC_COUNT_REACHED_MAX,
	EVENT_STUCK_IN_AIR,
	EVENT_SUSPICIOUS_ACTIVITY,
	EVENT_SWITCH_2_NM_TASK,
	EVENT_UNIDENTIFIED_PED,
	EVENT_VEHICLE_COLLISION,
	EVENT_VEHICLE_DAMAGE_WEAPON,
	EVENT_VEHICLE_ON_FIRE,
	EVENT_WHISTLING_HEARD,
	EVENT_DISTURBANCE,
	EVENT_ENTITY_DAMAGED,
	EVENT_ENTITY_DESTROYED,
	EVENT_WRITHE,
	EVENT_HURT_TRANSITION,
	EVENT_PLAYER_UNABLE_TO_ENTER_VEHICLE,
	EVENT_SCENARIO_FORCE_ACTION,
	EVENT_STAT_VALUE_CHANGED,
	EVENT_PLAYER_DEATH,
	EVENT_PED_SEEN_DEAD_PED,
	NUM_AI_EVENTTYPE,

	// Network events
	EVENT_NETWORK_PLAYER_JOIN_SESSION,
	EVENT_NETWORK_PLAYER_LEFT_SESSION,
	EVENT_NETWORK_PLAYER_JOIN_SCRIPT,
	EVENT_NETWORK_PLAYER_LEFT_SCRIPT,
	EVENT_NETWORK_STORE_PLAYER_LEFT,
	EVENT_NETWORK_SESSION_START,
	EVENT_NETWORK_SESSION_END,
	EVENT_NETWORK_START_MATCH,
	EVENT_NETWORK_END_MATCH,
	EVENT_NETWORK_REMOVED_FROM_SESSION_DUE_TO_STALL,
	EVENT_NETWORK_REMOVED_FROM_SESSION_DUE_TO_COMPLAINTS,
	EVENT_NETWORK_CONNECTION_TIMEOUT,
	EVENT_NETWORK_PED_DROPPED_WEAPON,
	EVENT_NETWORK_PLAYER_SPAWN,
	EVENT_NETWORK_PLAYER_COLLECTED_PICKUP,
	EVENT_NETWORK_PLAYER_COLLECTED_AMBIENT_PICKUP,
	EVENT_NETWORK_PLAYER_COLLECTED_PORTABLE_PICKUP,
	EVENT_NETWORK_PLAYER_DROPPED_PORTABLE_PICKUP,
	EVENT_NETWORK_INVITE_ARRIVED,
	EVENT_NETWORK_INVITE_ACCEPTED,
	EVENT_NETWORK_INVITE_CONFIRMED,
	EVENT_NETWORK_INVITE_REJECTED,
	EVENT_NETWORK_SUMMON,
	EVENT_NETWORK_SCRIPT_EVENT,
	EVENT_NETWORK_PLAYER_SIGNED_OFFLINE,
	EVENT_NETWORK_SIGN_IN_STATE_CHANGED,
	EVENT_NETWORK_SIGN_IN_CHANGE_ACTIONED,
	EVENT_NETWORK_NETWORK_ROS_CHANGED,
	EVENT_NETWORK_NETWORK_BAIL,
	EVENT_NETWORK_HOST_MIGRATION,
	EVENT_NETWORK_FIND_SESSION,
	EVENT_NETWORK_HOST_SESSION,
	EVENT_NETWORK_JOIN_SESSION,
	EVENT_NETWORK_JOIN_SESSION_RESPONSE,
	EVENT_NETWORK_CHEAT_TRIGGERED,
	EVENT_NETWORK_DAMAGE_ENTITY,
	EVENT_NETWORK_PLAYER_ARREST,
	EVENT_NETWORK_TIMED_EXPLOSION,
	EVENT_NETWORK_PRIMARY_CLAN_CHANGED,
	EVENT_NETWORK_CLAN_JOINED,
	EVENT_NETWORK_CLAN_LEFT,
	EVENT_NETWORK_CLAN_INVITE_RECEIVED,
	EVENT_VOICE_SESSION_STARTED,
	EVENT_VOICE_SESSION_ENDED,
	EVENT_VOICE_CONNECTION_REQUESTED,
	EVENT_VOICE_CONNECTION_RESPONSE,
	EVENT_VOICE_CONNECTION_TERMINATED,
	EVENT_TEXT_MESSAGE_RECEIVED,
	EVENT_CLOUD_FILE_RESPONSE,
	EVENT_NETWORK_PICKUP_RESPAWNED,
	EVENT_NETWORK_PRESENCE_STAT_UPDATE,
	EVENT_NETWORK_PED_LEFT_BEHIND,
	EVENT_NETWORK_INBOX_MSGS_RCVD,
	EVENT_NETWORK_ATTEMPT_HOST_MIGRATION,
	EVENT_NETWORK_INCREMENT_STAT,
	EVENT_NETWORK_SESSION_EVENT,
	EVENT_NETWORK_TRANSITION_STARTED,
	EVENT_NETWORK_TRANSITION_EVENT,
	EVENT_NETWORK_TRANSITION_MEMBER_JOINED,
	EVENT_NETWORK_TRANSITION_MEMBER_LEFT,
	EVENT_NETWORK_TRANSITION_PARAMETER_CHANGED,
	EVENT_NETWORK_CLAN_KICKED,
	EVENT_NETWORK_TRANSITION_STRING_CHANGED,
	EVENT_NETWORK_TRANSITION_GAMER_INSTRUCTION,
	EVENT_NETWORK_PRESENCE_INVITE,
	EVENT_NETWORK_PRESENCE_INVITE_REMOVED,
	EVENT_NETWORK_PRESENCE_INVITE_REPLY,
	EVENT_NETWORK_CASH_TRANSACTION_LOG,
	EVENT_NETWORK_CLAN_RANK_CHANGE,
	EVENT_NETWORK_VEHICLE_UNDRIVABLE,
	EVENT_NETWORK_PRESENCE_TRIGGER,
	EVENT_NETWORK_EMAIL_RECEIVED,
	EVENT_NETWORK_FOLLOW_INVITE_RECEIVED,
	EVENT_NETWORK_ADMIN_INVITED,
	EVENT_NETWORK_SPECTATE_LOCAL,
	EVENT_NETWORK_CLOUD_EVENT,
	EVENT_NETWORK_SHOP_TRANSACTION,
	EVENT_NETWORK_PERMISSION_CHECK_RESULT,
	EVENT_NETWORK_APP_LAUNCHED,
	EVENT_NETWORK_ONLINE_PERMISSIONS_UPDATED,
	EVENT_NETWORK_SYSTEM_SERVICE_EVENT,
	EVENT_NETWORK_REQUEST_DELAY,
	EVENT_NETWORK_SOCIAL_CLUB_ACCOUNT_LINKED,
	EVENT_NETWORK_SCADMIN_PLAYER_UPDATED,
	EVENT_NETWORK_SCADMIN_RECEIVED_CASH,
	EVENT_NETWORK_CLAN_INVITE_REQUEST_RECEIVED,
	EVENT_NETWORK_MARKETING_EMAIL_RECEIVED,
	EVENT_NETWORK_STUNT_PERFORMED,
	EVENT_NETWORK_FIRED_DUMMY_PROJECTILE,
	EVENT_NETWORK_PLAYER_ENTERED_VEHICLE,
	EVENT_NETWORK_PLAYER_ACTIVATED_SPECIAL_ABILITY,
	EVENT_NETWORK_PLAYER_DEACTIVATED_SPECIAL_ABILITY,
	EVENT_NETWORK_PLAYER_SPECIAL_ABILITY_FAILED_ACTIVATION,
	EVENT_NETWORK_FIRED_VEHICLE_PROJECTILE,
	EVENT_NETWORK_SC_MEMBERSHIP_STATUS,
	EVENT_NETWORK_SC_BENEFITS_STATUS,
	NUM_NETWORK_EVENTTYPE,

	EVENT_ERRORS_UNKNOWN_ERRORS,
	EVENT_ERRORS_ARRAY_OVERFLOW,
	EVENT_ERRORS_INSTRUCTION_LIMIT,
	EVENT_ERRORS_STACK_OVERFLOW,
	NUM_ERRORS_EVENTTYPE
};

enum eScriptTaskHash
{
	SCRIPT_TASK_ANY = 0x55966344,
	SCRIPT_TASK_INVALID = 0x811E343C,
	SCRIPT_TASK_PAUSE = 0x03C990EC,
	SCRIPT_TASK_STAND_STILL = 0xC572E06A,
	DEPRECATED_SCRIPT_TASK_FALL_AND_GET_UP = 0xA6296C9D,
	SCRIPT_TASK_JUMP = 0x24415046,
	SCRIPT_TASK_COWER = 0x1C43F4CF,
	SCRIPT_TASK_HANDS_UP = 0xA573B67C,
	SCRIPT_TASK_DUCK = 0x1D415F6C,
	DEPRECATED_SCRIPT_TASK_SCRATCH_HEAD = 0xD9162485,
	DEPRECATED_SCRIPT_TASK_LOOK_ABOUT = 0x255F21CC,
	SCRIPT_TASK_ENTER_VEHICLE = 0x950B6492,
	SCRIPT_TASK_LEAVE_VEHICLE = 0x1AE73569,
	SCRIPT_TASK_VEHICLE_DRIVE_TO_COORD = 0x93A5526E,
	SCRIPT_TASK_VEHICLE_DRIVE_TO_COORD_LONGRANGE = 0x21D33957,
	SCRIPT_TASK_VEHICLE_DRIVE_WANDER = 0xF09B15B3,
	SCRIPT_TASK_GO_STRAIGHT_TO_COORD = 0x7D8F4411,
	SCRIPT_TASK_GO_STRAIGHT_TO_COORD_RELATIVE_TO_ENTITY = 0x78EC0FF6,
	DEPRECATED_SCRIPT_TASK_GO_STRAIGHT_TO_COORD_RELATIVE_TO_VEHICLE = 0x96066708,
	SCRIPT_TASK_ACHIEVE_HEADING = 0x7276D3DF,
	SCRIPT_TASK_FOLLOW_POINT_ROUTE = 0xB232526F,
	SCRIPT_TASK_GO_TO_ENTITY = 0x4924437D,
	DEPRECATED_SCRIPT_TASK_GO_TO_PED = 0xD7F626D1,
	DEPRECATED_SCRIPT_TASK_FLEE_POINT = 0xEEDD9B66,
	DEPRECATED_SCRIPT_TASK_FLEE_PED = 0x114F64E3,
	SCRIPT_TASK_SMART_FLEE_POINT = 0xF10822AA,
	SCRIPT_TASK_SMART_FLEE_PED = 0x6BA30179,
	SCRIPT_TASK_WANDER_STANDARD = 0xBBA3B7CA,
	SCRIPT_TASK_FOLLOW_NAV_MESH_TO_COORD = 0x2A89B8A7,
	SCRIPT_TASK_GO_TO_COORD_ANY_MEANS = 0x93399E79,
	SCRIPT_TASK_PERFORM_SEQUENCE = 0x0E763797,
	SCRIPT_TASK_LEAVE_ANY_VEHICLE = 0xCE98FBB3,
	SCRIPT_TASK_AIM_GUN_SCRIPTED = 0x0C69931F,
	SCRIPT_TASK_AIM_GUN_AT_ENTITY = 0x6134071B,
	SCRIPT_TASK_GO_TO_COORD_WHILE_SHOOTING = 0x9387DEAB,
	SCRIPT_TASK_TURN_PED_TO_FACE_ENTITY = 0xCBCE4595,
	DEPRECATED_SCRIPT_TASK_TURN_PED_TO_FACE_PED = 0xE51B372C,
	SCRIPT_TASK_AIM_GUN_AT_COORD = 0x49BEF36E,
	SCRIPT_TASK_SHOOT_AT_COORD = 0xD90EF188,
	DEPRECATED_SCRIPT_TASK_DESTROY_VEHICLE = 0x0B45DACC,
	DEPRECATED_SCRIPT_TASK_DIVE_AND_GET_UP = 0x7BA620DD,
	SCRIPT_TASK_SHUFFLE_TO_NEXT_VEHICLE_SEAT = 0x153011FC,
	SCRIPT_TASK_EVERYONE_LEAVE_VEHICLE = 0xA569F146,
	DEPRECATED_SCRIPT_TASK_DIVE_FROM_ATTACHMENT_AND_GET_UP = 0xC09E33A2,
	SCRIPT_TASK_GOTO_ENTITY_OFFSET = 0x87E3E0A8,
	DEPRECATED_SCRIPT_TASK_GOTO_PED_OFFSET = 0xBF57AF1C,
	DEPRECATED_SCRIPT_TASK_SIT_DOWN = 0x190DC01B,
	SCRIPT_TASK_TURN_PED_TO_FACE_COORD = 0x574BB8F5,
	SCRIPT_TASK_DRIVE_POINT_ROUTE = 0xBAE13130,
	DEPRECATED_SCRIPT_TASK_GO_TO_COORD_WHILE_AIMING = 0x7DEC090B,
	SCRIPT_TASK_VEHICLE_TEMP_ACTION = 0x81B4D53A,
	SCRIPT_TASK_BRING_VEHICLE_TO_HALT = 0x30A0DC39,
	SCRIPT_TASK_VEHICLE_MISSION = 0xB41F1A34,
	DEPRECATED_SCRIPT_TASK_GO_TO_OBJECT = 0xE4A207BD,
	DEPRECATED_SCRIPT_TASK_WEAPON_ROLL = 0xB2A2BF11,
	DEPRECATED_SCRIPT_TASK_SIDEWAYS_DIVE = 0x2C1A612F,
	SCRIPT_TASK_DRIVE_BY = 0x7D711E7D,
	SCRIPT_TASK_USE_MOBILE_PHONE = 0x37D339A1,
	SCRIPT_TASK_WARP_PED_INTO_VEHICLE = 0xBC555B9D,
	DEPRECATED_SCRIPT_TASK_USE_ATTRACTOR = 0x63694D9D,
	SCRIPT_TASK_SHOOT_AT_ENTITY = 0x0A01F8B8,
	DEPRECATED_SCRIPT_TASK_SHOOT_AT_PED = 0x15F49B5F,
	DEPRECATED_SCRIPT_TASK_FLEE_PED_ANY_MEANS = 0x3A82EBC5,
	DEPRECATED_SCRIPT_TASK_DEAD = 0xF793E251,
	DEPRECATED_SCRIPT_TASK_GOTO_VEHICLE = 0x9A2943F2,
	SCRIPT_TASK_CLIMB = 0xB802FDCA,
	SCRIPT_TASK_PERFORM_SEQUENCE_FROM_PROGRESS = 0x5485FD94,
	SCRIPT_TASK_GOTO_ENTITY_AIMING = 0x967EA21C,
	DEPRECATED_SCRIPT_TASK_GOTO_PED_AIMING = 0x1A230A59,
	DEPRECATED_SCRIPT_TASK_JETPACK = 0x6EA2E79A,
	SCRIPT_TASK_SET_PED_DECISION_MAKER = 0x4E5B453C,
	SCRIPT_TASK_SET_PED_DEFENSIVE_AREA = 0x00A101C8,
	DEPRECATED_SCRIPT_TASK_HOLD_OBJECT = 0xC8BCA367,
	DEPRECATED_SCRIPT_TASK_COMPLEX_PICKUP_OBJECT = 0xA9D6E737,
	SCRIPT_TASK_PED_SLIDE_TO_COORD = 0x3E5094A7,
	DEPRECATED_SCRIPT_TASK_SWIM_TO_COORD = 0xABFCB97C,
	SCRIPT_TASK_DRIVE_POINT_ROUTE_ADVANCED = 0xEA6A323F,
	SCRIPT_TASK_PED_SLIDE_TO_COORD_AND_PLAY_ANIM = 0x8A0970F4,
	DEPRECATED_SCRIPT_TASK_FOLLOW_PATROL_ROUTE = 0x22024D52,
	DEPRECATED_SCRIPT_TASK_GREET_PARTNER = 0xAD4CD615,
	DEPRECATED_SCRIPT_TASK_DIE_NAMED_ANIM = 0xD73264BC,
	DEPRECATED_SCRIPT_TASK_FOLLOW_FOOTSTEPS = 0xA0A7761F,
	DEPRECATED_SCRIPT_TASK_WALK_ALONGSIDE_PED = 0xA92F7B36,
	DEPRECATED_SCRIPT_TASK_USE_CLOSEST_MAP_ATTRACTOR = 0xD0D5F297,
	DEPRECATED_SCRIPT_TASK_SET_IGNORE_WEAPON_RANGE_FLAG = 0xADC7E889,
	DEPRECATED_SCRIPT_TASK_HAND_GESTURE = 0x1F53A7DA,
	SCRIPT_TASK_PLAY_ANIM = 0x87B9A382,
	DEPRECATED_SCRIPT_TASK_PLAY_ANIM_ADVANCED = 0x8ECCBFB3,
	DEPRECATED_SCRIPT_SET_TASK_PLAY_ANIM_PLAYBACK_COORDS = 0xAF35BD9C,
	DEPRECATED_SCRIPT_TASK_PED_ARREST_PED = 0xB99876B9,
	SCRIPT_TASK_ARREST_PED = 0x52FF82C0,
	SCRIPT_TASK_COMBAT = 0x2E85A751,
	SCRIPT_TASK_COMBAT_TIMED = 0xF2E41A8A,
	SCRIPT_TASK_SEEK_COVER_FROM_POS = 0xA77A06C5,
	SCRIPT_TASK_SEEK_COVER_FROM_PED = 0x71E30BDC,
	SCRIPT_TASK_SEEK_COVER_TO_COVER_POINT = 0x99AFA8A3,
	DEPRECATED_SCRIPT_TASK_SET_COMBAT_DECISION_MAKER = 0x9B95A683,
	SCRIPT_TASK_TOGGLE_DUCK = 0x0F3B8554,
	DEPRECATED_SCRIPT_TASK_USE_SKIS = 0x97AE64AB,
	SCRIPT_TASK_GUARD_DEFENSIVE_AREA = 0xDF5F4BA7,
	SCRIPT_TASK_PICKUP_AND_CARRY_OBJECT = 0x89025025,
	DEPRECATED_SCRIPT_TASK_SEEK_COVER_TO_OBJECT = 0x5A2825BB,
	SCRIPT_TASK_SEEK_COVER_TO_COORDS = 0x6C01775C,
	DEPRECATED_SCRIPT_TASK_SIT_DOWN_PLAY_ANIM = 0xBA284891,
	SCRIPT_TASK_GUARD_ANGLED_DEFENSIVE_AREA = 0x84AEE7A0,
	SCRIPT_TASK_STAND_GUARD = 0xD88F2CDE,
	SCRIPT_TASK_CLIMB_LADDER = 0x66403353,
	DEPRECATED_SCRIPT_TASK_SIT_DOWN_ON_OBJECT = 0xFD790A1B,
	SCRIPT_TASK_GUARD_SPHERE_DEFENSIVE_AREA = 0x21E8D4E4,
	SCRIPT_TASK_START_SCENARIO_IN_PLACE = 0x3B3A458F,
	SCRIPT_TASK_START_SCENARIO_AT_POSITION = 0xBE86C566,
	SCRIPT_TASK_START_VEHICLE_SCENARIO = 0x86016E38,
	SCRIPT_TASK_PUT_PED_DIRECTLY_INTO_COVER = 0x8B2F140E,
	SCRIPT_TASK_PUT_PED_DIRECTLY_INTO_COVER_FROM_TARGET = 0x9DD414F5,
	SCRIPT_TASK_PUT_PED_DIRECTLY_INTO_MELEE = 0xFBBF6F4D,
	SCRIPT_TASK_GUARD_CURRENT_POSITION = 0x8CE49D34,
	SCRIPT_TASK_USE_NEAREST_SCENARIO_TO_POS = 0x623A5EFE,
	SCRIPT_TASK_USE_NEAREST_SCENARIO_CHAIN_TO_POS = 0x9BD19AE7,
	DEPRECATED_SCRIPT_TASK_LEAVE_GROUP = 0x9F5DBCE5,
	SCRIPT_TASK_PERFORM_SEQUENCE_LOCALLY = 0xE7FBAB4F,
	SCRIPT_TASK_COMBAT_HATED_TARGETS_IN_AREA = 0x42CC4F21,
	SCRIPT_TASK_COMBAT_HATED_TARGETS_AROUND_PED = 0xAA05B492,
	DEPRECATED_SCRIPT_TASK_HOLSTERING_WEAPON = 0x81FB0B11,
	DEPRECATED_SCRIPT_TASK_COMBAT_ROLL = 0x71F49E88,
	DEPRECATED_SCRIPT_TASK_MOBILE_CONVERSATION = 0xE3380A30,
	SCRIPT_TASK_SWAP_WEAPON = 0x2AB81462,
	SCRIPT_TASK_RELOAD_WEAPON = 0xC322ED6F,
	SCRIPT_TASK_DROP_OBJECT = 0xAB4B293A,
	SCRIPT_TASK_COMBAT_HATED_TARGETS_AROUND_PED_TIMED = 0x2719C0D1,
	SCRIPT_TASK_GET_OFF_BOAT = 0x9A27A999,
	SCRIPT_TASK_FOLLOW_NAVMESH_TO_COORD_ADVANCED = 0x9C4FBCAC,
	SCRIPT_TASK_PATROL = 0xB550726C,
	SCRIPT_TASK_STAY_IN_COVER = 0xE1C16E99,
	SCRIPT_TASK_HANG_GLIDER = 0x00E1228C,
	SCRIPT_TASK_FOLLOW_TO_OFFSET_OF_ENTITY = 0x3EF867F4,
	SCRIPT_TASK_FOLLOW_TO_OFFSET_OF_PICKUP = 0x70AEF4E9,
	SCRIPT_TASK_GO_TO_COORD_WHILE_AIMING_AT_COORD = 0x19CE5AFC,
	SCRIPT_TASK_GO_TO_COORD_WHILE_AIMING_AT_ENTITY = 0x972C6757,
	DEPRECATED_SCRIPT_TASK_GO_TO_COORD_WHILE_AIMING_AT_PED = 0x0A81CE80,
	DEPRECATED_SCRIPT_TASK_GO_TO_COORD_WHILE_AIMING_AT_VEHICLE = 0xE677F9FB,
	DEPRECATED_SCRIPT_TASK_GO_TO_COORD_WHILE_AIMING_AT_OBJECT = 0x89E45204,
	SCRIPT_TASK_GO_TO_ENTITY_WHILE_AIMING_AT_COORD = 0xBAEB517C,
	DEPRECATED_SCRIPT_TASK_GO_TO_PED_WHILE_AIMING_AT_COORD = 0xA2B07D24,
	SCRIPT_TASK_GO_TO_ENTITY_WHILE_AIMING_AT_ENTITY = 0xB80BFB24,
	DEPRECATED_SCRIPT_TASK_GO_TO_PED_WHILE_AIMING_AT_PED = 0x6C095462,
	DEPRECATED_SCRIPT_TASK_GO_TO_PED_WHILE_AIMING_AT_VEHICLE = 0x7BF24249,
	DEPRECATED_SCRIPT_TASK_GO_TO_PED_WHILE_AIMING_AT_OBJECT = 0xC93D7834,
	SCRIPT_TASK_USE_WALKIE_TALKIE = 0x29BABC64,
	SCRIPT_TASK_CHAT_TO_PED = 0x0FC239CD,
	DEPRECATED_SCRIPT_TASK_WARP_PED_ONTO_VEHICLE = 0xFCC0F996,
	SCRIPT_TASK_FIRE_FLARE = 0xDEB1C08F,
	SCRIPT_TASK_BIND_POSE = 0x4929CE40,
	SCRIPT_TASK_NM_ELECTROCUTE = 0x8944A9A0,
	SCRIPT_TASK_NM_HIGH_FALL = 0x015D63E3,
	SCRIPT_TASK_NM_DANGLE = 0x0B49EAEC,
	SCRIPT_TASK_NM_SLUNG_OVER_SHOULDER = 0xF0F9FFC0,
	SCRIPT_TASK_NM_STUMBLE = 0xBACF9837,
	SCRIPT_TASK_SKY_DIVE = 0x4B65F15C,
	SCRIPT_TASK_PARACHUTE = 0x76CA4A8E,
	SCRIPT_TASK_PARACHUTE_TO_TARGET = 0x4921B47A,
	SCRIPT_TASK_FOLLOW_WAYPOINT_ROUTE = 0x9B4FC7D8,
	DEPRECATED_SCRIPT_TASK_GET_ON_SKI_LIFT = 0x536E59F9,
	SCRIPT_TASK_NM_ATTACH_TO_VEHICLE = 0x4847A94F,
	SCRIPT_TASK_SET_BLOCKING_OF_NON_TEMPORARY_EVENTS = 0x6F9C865C,
	SCRIPT_TASK_MOVE_NETWORK = 0x0494661C,
	SCRIPT_TASK_SYNCHRONIZED_SCENE = 0x6A67A5CC,
	SCRIPT_TASK_VEHICLE_SHOOT_AT_COORD = 0xAF18B824,
	SCRIPT_TASK_VEHICLE_SHOOT_AT_ENTITY = 0x20123810,
	SCRIPT_TASK_VEHICLE_PARK = 0xEFC8537E,
	SCRIPT_TASK_MOUNT_ANIMAL = 0x6F5F73AE,
	SCRIPT_TASK_DISMOUNT_ANIMAL = 0x1DE2A7BD,
	SCRIPT_TASK_THROW_PROJECTILE = 0xAD37BF03,
	SCRIPT_TASK_VEHICLE_AIM_AT_COORD = 0x00C59C52,
	SCRIPT_TASK_VEHICLE_AIM_AT_ENTITY = 0x6F30F4C1,
	SCRIPT_TASK_VEHICLE_AIM_USING_CAMERA = 0x3BDBC83C,
	SCRIPT_TASK_ADVANCE_TO_TARGET_IN_LINE = 0xCC312EC4,
	SCRIPT_TASK_RAPPEL_FROM_HELI = 0xEF8D6B40,
	SCRIPT_TASK_GENERAL_SWEEP = 0x491A782D,
	SCRIPT_TASK_DRAG_PED_TO_COORD = 0x87A3DFEA,
	SCRIPT_TASK_VEHICLE_FOLLOW_WAYPOINT_RECORDING = 0xF1F17AE7,
	SCRIPT_TASK_RAPPEL_DOWN_WALL = 0x8E29DEF2,
	SCRIPT_TASK_GO_TO_COORD_AND_AIM_AT_HATED_ENTITIES_NEAR_COORD = 0x290A02BC,
	SCRIPT_TASK_WANDER_IN_AREA = 0x370BCF53,
	SCRIPT_TASK_VEHICLE_GOTO_NAVMESH = 0xFBB43C4A,
	SCRIPT_TASK_FORCE_MOTION_STATE = 0x9E78AC1F,
	SCRIPT_TASK_IN_CUSTODY = 0x6D4411C9,
	SCRIPT_TASK_LOOK_AT_ENTITY = 0x08F5AF9D,
	SCRIPT_TASK_LOOK_AT_COORD = 0xCB842EEC,
	SCRIPT_TASK_VEHICLE_CHASE = 0x2288A57C,
	SCRIPT_TASK_STEALTH_KILL = 0x5014CC1A,
	SCRIPT_TASK_HELI_CHASE = 0x27369192,
	SCRIPT_TASK_PLANE_CHASE = 0x02DBA9BF,
	SCRIPT_TASK_PLANE_LAND = 0x043E4A56,
	SCRIPT_TASK_SHOCKING_EVENT_BACK_AWAY = 0x7F9814E9,
	SCRIPT_TASK_SHOCKING_EVENT_HURRY_AWAY = 0xEC685098,
	SCRIPT_TASK_SHOCKING_EVENT_REACT = 0x498BABE3,
	SCRIPT_TASK_WRITHE = 0x8EC23E41,
	SCRIPT_TASK_EXIT_COVER = 0x4E961D82,
	SCRIPT_TASK_PLANT_BOMB = 0x8127FD1A,
	SCRIPT_TASK_INVESTIGATE_COORDS = 0x9C250C19,
	SCRIPT_TASK_WANDER_SPECIFIC = 0xD46F7254,
	SCRIPT_TASK_SHARK_CIRCLE_COORD = 0x48EED267,
	SCRIPT_TASK_SHARK_CIRCLE_PED = 0xFD0B5826,
	SCRIPT_TASK_REACT_AND_FLEE_COORD = 0x29269FF1,
	SCRIPT_TASK_REACT_AND_FLEE_PED = 0x7DEDF098,
	SCRIPT_TASK_GO_TO_COORD_ANY_MEANS_EXTRA_PARAMS = 0x45B5A146,
	SCRIPT_TASK_USE_NEAREST_TRAIN_SCENARIO_TO_POS = 0xA5806868,
	SCRIPT_TASK_JETPACK = 0x828EBA07,
	SCRIPT_TASK_GO_TO_COORD_ANY_MEANS_EXTRA_PARAMS_WITH_CRUISE_SPEED = 0x4DE5C290,
	SCRIPT_TASK_AGITATED_ACTION = 0x548CB4B4,
	SCRIPT_TASK_WARP_PED_DIRECTLY_INTO_COVER = 0x0E802924
};

enum eCodeTask {
	CODE_TASK_INVALID_ID = -1,
	CODE_TASK_HANDS_UP = 0,
	CODE_TASK_CLIMB_LADDER = 1,
	CODE_TASK_EXIT_VEHICLE = 2,
	CODE_TASK_COMBAT_ROLL = 3,
	CODE_TASK_AIM_GUN_ON_FOOT = 4,
	CODE_TASK_MOVE_PLAYER,
	CODE_TASK_PLAYER_ON_FOOT,
	CODE_TASK_PLAYER_ON_HORSE,
	CODE_TASK_WEAPON,
	CODE_TASK_PLAYER_WEAPON,
	CODE_TASK_PLAYER_IDLES,
	CODE_TASK_COMPLEX_NETWORK_PLAYER_SETTINGS_TASKS,
	CODE_TASK_NONE,
	CODE_TASK_UNINTERRUPTABLE,
	CODE_TASK_PAUSE,
	CODE_TASK_DO_NOTHING,
	CODE_TASK_GET_UP,
	CODE_TASK_GET_UP_AND_STAND_STILL,
	CODE_TASK_FALL_OVER,
	CODE_TASK_FALL_AND_GET_UP,
	CODE_TASK_CRAWL,
	CODE_TASK_COMPLEX_FALL_AND_STAY_DOWN,
	CODE_TASK_SIT_IDLE,
	CODE_TASK_SIT_DOWN,
	CODE_TASK_HIT_RESPONSE,
	CODE_TASK_COMPLEX_ON_FIRE,
	CODE_TASK_DAMAGE_ELECTRIC,
	CODE_TASK_STEP_UP,
	CODE_TASK_TRIGGER_LOOK_AT,
	CODE_TASK_CLEAR_LOOK_AT,
	CODE_TASK_SET_CHAR_DECISION_MAKER,
	CODE_TASK_SET_PED_DEFENSIVE_AREA,
	CODE_TASK_USE_SEQUENCE,
	CODE_TASK_SIMPLE_CONTROL_MOVEMENT,
	CODE_TASK_MOVE_STAND_STILL,
	CODE_TASK_COMPLEX_CONTROL_MOVEMENT,
	CODE_TASK_COMPLEX_MOVE_SEQUENCE,
	CODE_TASK_MOVE_AROUND_COVERPOINTS,
	CODE_TASK_AMBIENT_CLIPS,
	CODE_TASK_MOVE_IN_AIR,
	CODE_TASK_NETWORK_CLONE,
	CODE_TASK_USE_CLIMB_ON_ROUTE,
	CODE_TASK_USE_DROPDOWN_ON_ROUTE,
	CODE_TASK_USE_LADDER_ON_ROUTE,
	CODE_TASK_SET_BLOCKING_OF_NON_TEMPORARY_EVENTS,
	CODE_TASK_FORCE_MOTION_STATE,
	CODE_TASK_ON_FOOT_SLOPE_SCRAMBLE,
	CODE_TASK_GO_TO_AND_CLIMB_LADDER,
	CODE_TASK_CLIMB_LADDER_FULLY,
	CODE_TASK_RAPPEL,
	CODE_TASK_VAULT,
	CODE_TASK_DROP_DOWN,
	CODE_TASK_AFFECT_SECONDARY_BEHAVIOUR,
	CODE_TASK_AMBIENT_LOOK_AT_EVENT,
	CODE_TASK_OPEN_DOOR,
	CODE_TASK_SHOVE_PED,
	CODE_TASK_SWAP_WEAPON,
	CODE_TASK_GENERAL_SWEEP,
	CODE_TASK_POLICE,
	CODE_TASK_POLICE_ORDER_RESPONSE,
	CODE_TASK_PURSUE_CRIMINAL,
	CODE_TASK_POLICE_GIVE_WARNING,
	CODE_TASK_ARREST_PED,
	CODE_TASK_ARREST_PED2,
	CODE_TASK_BUSTED,
	CODE_TASK_FIRE_PATROL,
	CODE_TASK_HELI_ORDER_RESPONSE,
	CODE_TASK_HELI_PASSENGER_RAPPEL,
	CODE_TASK_AMBULANCE_PATROL,
	CODE_TASK_POLICE_WANTED_RESPONSE,
	CODE_TASK_SWAT,
	CODE_TASK_SWAT_LEADER,
	CODE_TASK_SWAT_WANTED_RESPONSE,
	CODE_TASK_SWAT_ORDER_RESPONSE,
	CODE_TASK_SWAT_GO_TO_STAGING_AREA,
	CODE_TASK_SWAT_FOLLOW_IN_LINE,
	CODE_TASK_WITNESS,
	CODE_TASK_GANG_PATROL,
	CODE_TASK_ARMY,
	CODE_TASK_SHOCKING_EVENT,
	CODE_TASK_SHOCKING_EVENT_WATCH,
	CODE_TASK_SHOCKING_EVENT_FLEE,
	CODE_TASK_SHOCKING_EVENT_GOTO,
	CODE_TASK_SHOCKING_EVENT_HURRYAWAY,
	CODE_TASK_SHOCKING_EVENT_REACT_TO_AIRCRAFT,
	CODE_TASK_SHOCKING_EVENT_REACT,
	CODE_TASK_SHOCKING_EVENT_BACK_AWAY,
	CODE_TASK_SHOCKING_POLICE_INVESTIGATE,
	CODE_TASK_SHOCKING_EVENT_STOP_AND_STARE,
	CODE_TASK_SHOCKING_NICE_CAR_PICTURE,
	CODE_TASK_SHOCKING_EVENT_THREAT_RESPONSE,
	CODE_TASK_PUT_ON_HELMET,
	CODE_TASK_TAKE_OFF_HELMET,
	CODE_TASK_CAR_REACT_TO_VEHICLE_COLLISION,
	CODE_TASK_REACT_TO_RUNNING_PED_OVER,
	CODE_TASK_CAR_REACT_TO_VEHICLE_COLLISION_GET_OUT,
	CODE_TASK_DEAD,
	CODE_TASK_DYING_DEAD,
	CODE_TASK_DEFER_TO_SCENARIO,
	CODE_TASK_PARKED_VEHICLE_SCENARIO,
	CODE_TASK_WANDERING_SCENARIO,
	CODE_TASK_WANDERING_IN_RADIUS_SCENARIO,
	CODE_TASK_COMPLEX_DRIVING_SCENARIO,
	CODE_TASK_MOVE_BETWEEN_POINTS_SCENARIO,
	CODE_TASK_CHAT_SCENARIO,
	CODE_TASK_COMPLEX_WAIT_FOR_SEAT_TO_BE_FREE,
	CODE_TASK_COWER_SCENARIO,
	CODE_TASK_DEAD_BODY_SCENARIO,
	CODE_TASK_WAIT_FOR_MY_CAR_TO_STOP,
	CODE_TASK_COMPLEX_WAIT_FOR_DOOR_TO_BE_OPEN,
	CODE_TASK_COMPLEX_DRIVE_WANDER_FOR_TIME,
	CODE_TASK_COMPLEX_WAIT_FOR_TIME,
	CODE_TASK_COMPLEX_GET_IN_CAR_SCENARIO,
	CODE_TASK_MOBILE_PHONE_CHAT_SCENARIO,
	CODE_TASK_SAY_AUDIO,
	CODE_TASK_COMPLEX_MAKE_MOBILE_CALL,
	CODE_TASK_WAIT_FOR_STEPPING_OUT,
	CODE_TASK_COUPLE_SCENARIO,
	CODE_TASK_USE_SCENARIO,
	CODE_TASK_USE_VEHICLE_SCENARIO,
	CODE_TASK_UNALERTED,
	CODE_TASK_STEAL_VEHICLE,
	CODE_TASK_REACT_TO_PURSUIT,
	CODE_TASK_RUN_CLIP,
	CODE_TASK_RUN_NAMED_CLIP,
	CODE_TASK_HIT_WALL,
	CODE_TASK_COWER,
	CODE_TASK_CROUCH,
	CODE_TASK_MELEE,
	CODE_TASK_MOVE_MELEE_MOVEMENT,
	CODE_TASK_MELEE_ACTION_RESULT,
	CODE_TASK_MELEE_UPPERBODY_ANIM,
	CODE_TASK_MELEE_UNINTERRUPTABLE,
	CODE_TASK_MOVE_SCRIPTED,
	CODE_TASK_SCRIPTED_ANIMATION,
	CODE_TASK_SYNCHRONIZED_SCENE,
	CODE_TASK_REACH_ARM,
	CODE_TASK_COMPLEX_EVASIVE_STEP,
	CODE_TASK_MOVE_WANDER_AROUND_VEHICLE,
	CODE_TASK_WALK_ROUND_FIRE,
	CODE_TASK_COMPLEX_STUCK_IN_AIR,
	CODE_TASK_WALK_ROUND_ENTITY,
	CODE_TASK_MOVE_WALK_ROUND_VEHICLE,
	CODE_TASK_MOVE_WALK_ROUND_VEHICLE_DOOR,
	CODE_TASK_REACT_TO_GUN_AIMED_AT,
	CODE_TASK_COMPLEX_EXTINGUISH_FIRES,
	CODE_TASK_ON_FOOT_DUCK_AND_COVER,
	CODE_TASK_AGGRESSIVE_RUBBERNECK,
	CODE_TASK_COMPLEX_LEAVE_CAR_AND_FLEE,
	CODE_TASK_COMPLEX_SCREAM_IN_CAR_THEN_LEAVE,
	CODE_TASK_IN_VEHICLE_BASIC,
	CODE_TASK_CAR_DRIVE_WANDER,
	CODE_TASK_LEAVE_ANY_CAR,
	CODE_TASK_COMPLEX_GET_OFF_BOAT,
	CODE_TASK_CAR_DRIVE_POINT_ROUTE,
	CODE_TASK_CAR_SET_TEMP_ACTION,
	CODE_TASK_BRING_VEHICLE_TO_HALT,
	CODE_TASK_CAR_DRIVE,
	CODE_TASK_COMPLEX_CAR_DRIVE_MISSION_KILL_PED,
	CODE_TASK_PLAYER_DRIVE,
	CODE_TASK_ENTER_VEHICLE,
	CODE_TASK_ENTER_VEHICLE_ALIGN,
	CODE_TASK_OPEN_VEHICLE_DOOR_FROM_OUTSIDE,
	CODE_TASK_ENTER_VEHICLE_SEAT,
	CODE_TASK_CLOSE_VEHICLE_DOOR_FROM_INSIDE,
	CODE_TASK_IN_VEHICLE_SEAT_SHUFFLE,
	CODE_TASK_OPEN_VEHICLE_DOOR_FROM_INSIDE,
	CODE_TASK_EXIT_VEHICLE_SEAT,
	CODE_TASK_CLOSE_VEHICLE_DOOR_FROM_OUTSIDE,
	CODE_TASK_CONTROL_VEHICLE,
	CODE_TASK_MOTION_IN_AUTOMOBILE,
	CODE_TASK_MOTION_ON_BICYCLE,
	CODE_TASK_MOTION_ON_BICYCLE_CONTROLLER,
	CODE_TASK_MOTION_IN_VEHICLE,
	CODE_TASK_MOTION_IN_TURRET,
	CODE_TASK_REACT_TO_BEING_JACKED,
	CODE_TASK_REACT_TO_BEING_ASKED_TO_LEAVE_VEHICLE,
	CODE_TASK_TRY_TO_GRAB_VEHICLE_DOOR,
	CODE_TASK_GET_ON_TRAIN,
	CODE_TASK_GET_OFF_TRAIN,
	CODE_TASK_RIDE_TRAIN,
	CODE_TASK_MOTION_RIDE_HORSE,
	CODE_TASK_MOVE_MOUNTED,
	CODE_TASK_MOVE_MOUNTED_SPUR_CONTROL,
	CODE_TASK_MOUNT_ANIMAL,
	CODE_TASK_DISMOUNT_ANIMAL,
	CODE_TASK_MOUNT_SEAT,
	CODE_TASK_GETTING_MOUNTED,
	CODE_TASK_DISMOUNT_SEAT,
	CODE_TASK_GETTING_DISMOUNTED,
	CODE_TASK_MOUNT_THROW_PROJECTILE,
	CODE_TASK_RIDER_REARUP,
	CODE_TASK_JUMP_MOUNTED,
	CODE_TASK_MOVE_DRAFT_ANIMAL,
	CODE_TASK_MOUNT_FOLLOW_NEAREST_ROAD,
	CODE_TASK_GO_TO_CAR_DOOR_AND_STAND_STILL,
	CODE_TASK_MOVE_GO_TO_VEHICLE_DOOR,
	CODE_TASK_SET_PED_IN_VEHICLE,
	CODE_TASK_SET_PED_OUT_OF_VEHICLE,
	CODE_TASK_VEHICLE_MOUNTED_WEAPON,
	CODE_TASK_VEHICLE_GUN,
	CODE_TASK_VEHICLE_PROJECTILE,
	CODE_TASK_START_CAR,
	CODE_TASK_VEHICLE_SHUNT,
	CODE_TASK_SMASH_CAR_WINDOW,
	CODE_TASK_MOVE_GO_TO_POINT,
	CODE_TASK_MOVE_ACHIEVE_HEADING,
	CODE_TASK_MOVE_FACE_TARGET,
	CODE_TASK_MOVE_GO_TO_POINT_AND_STAND_STILL,
	CODE_TASK_MOVE_FOLLOW_POINT_ROUTE,
	CODE_TASK_MOVE_SEEK_ENTITY_STANDARD,
	CODE_TASK_MOVE_SEEK_ENTITY_LAST_NAV_MESH_INTERSECTION,
	CODE_TASK_MOVE_SEEK_ENTITY_OFFSET_ROTATE,
	CODE_TASK_MOVE_SEEK_ENTITY_OFFSET_FIXED,
	CODE_TASK_MOVE_SEEK_ENTITY_RADIUS_ANGLE,
	CODE_TASK_EXHAUSTED_FLEE,
	CODE_TASK_GROWL_AND_FLEE,
	CODE_TASK_SCENARIO_FLEE,
	CODE_TASK_SMART_FLEE,
	CODE_TASK_FLY_AWAY,
	CODE_TASK_WALK_AWAY,
	CODE_TASK_WANDER,
	CODE_TASK_WANDER_IN_AREA,
	CODE_TASK_FOLLOW_LEADER_IN_FORMATION,
	CODE_TASK_GO_TO_POINT_ANY_MEANS,
	CODE_TASK_COMPLEX_TURN_TO_FACE_ENTITY,
	CODE_TASK_FOLLOW_LEADER_ANY_MEANS,
	CODE_TASK_PLAYER_HORSE_FOLLOW,
	CODE_TASK_FLY_TO_POINT,
	CODE_TASK_FLYING_WANDER,
	CODE_TASK_GO_TO_POINT_AIMING,
	CODE_TASK_GO_TO_SCENARIO,
	CODE_TASK_FOLLOW_PATROL_ROUTE,
	CODE_TASK_SEEK_ENTITY_AIMING,
	CODE_TASK_SLIDE_TO_COORD,
	CODE_TASK_SWIMMING_WANDER,
	CODE_TASK_COMPLEX_AVOID_ENTITY,
	CODE_TASK_MOVE_TRACKING_ENTITY,
	CODE_TASK_MOVE_FOLLOW_NAVMESH,
	CODE_TASK_MOVE_GO_TO_POINT_ON_ROUTE,
	CODE_TASK_ESCAPE_BLAST,
	CODE_TASK_MOVE_WANDER,
	CODE_TASK_MOVE_BE_IN_FORMATION,
	CODE_TASK_MOVE_CROWD_AROUND_LOCATION,
	CODE_TASK_MOVE_CROSS_ROAD_AT_TRAFFIC_LIGHTS,
	CODE_TASK_MOVE_WAIT_FOR_TRAFFIC,
	CODE_TASK_MOVE_GOTO_POINT_STAND_STILL_ACHIEVE_HEADING,
	CODE_TASK_MOVE_WAIT_FOR_NAVMESH_SPECIAL_ACTION_EVENT,
	CODE_TASK_MOVE_GOTO_SAFE_POSITION_ON_NAVMESH,
	CODE_TASK_MOVE_RETURN_TO_ROUTE,
	CODE_TASK_MOVE_GOTO_SHELTER_AND_WAIT,
	CODE_TASK_MOVE_GET_ONTO_MAIN_NAVMESH,
	CODE_TASK_MOVE_SLIDE_TO_COORD,
	CODE_TASK_MOVE_GOTO_POINT_RELATIVE_TO_ENTITY_AND_STAND_STILL,
	CODE_TASK_HELICOPTER_STRAFE,
	CODE_TASK_COMPLEX_USE_MOBILE_PHONE_AND_MOVEMENT,
	CODE_TASK_GET_OUT_OF_WATER,
	CODE_TASK_COMPLEX_SEARCH_WANDER_AFTER_TIME,
	CODE_TASK_COMPLEX_MOVE_FOLLOW_NAV_NODES_ROUTE,
	CODE_TASK_MOVE_FOLLOW_ENTITY_OFFSET,
	CODE_TASK_TEST_CLIP_DISTANCE,
	CODE_TASK_FOLLOW_WAYPOINT_RECORDING,
	CODE_TASK_DROP_OVER_EDGE,
	CODE_TASK_GENERIC_MOVE_TO_POINT,
	CODE_TASK_MOTION_PED,
	CODE_TASK_MOTION_PED_LOW_LOD,
	CODE_TASK_MOTION_ANIMAL,
	CODE_TASK_MOTION_BASIC_LOCOMOTION,
	CODE_TASK_HUMAN_LOCOMOTION,
	CODE_TASK_MOTION_BASIC_LOCOMOTION_LOW_LOD,
	CODE_TASK_MOTION_STRAFING,
	CODE_TASK_MOTION_TENNIS,
	CODE_TASK_MOTION_AIMING,
	CODE_TASK_ON_FOOT_BIRD,
	CODE_TASK_ON_FOOT_FLIGHTLESS_BIRD,
	CODE_TASK_ON_FOOT_HORSE,
	CODE_TASK_ON_FOOT_DOG,
	CODE_TASK_ON_FOOT_DEER,
	CODE_TASK_ON_FOOT_FISH,
	CODE_TASK_ON_FOOT_QUAD,
	CODE_TASK_MOTION_DIVING,
	CODE_TASK_MOTION_SWIMMING,
	CODE_TASK_MOTION_PARACHUTING,
	CODE_TASK_MOTION_DRUNK,
	CODE_TASK_REPOSITION_MOVE,
	CODE_TASK_MOTION_AIMING_TRANSITION,
	CODE_TASK_THROW_PROJECTILE,
	CODE_TASK_COVER,
	CODE_TASK_MOTION_IN_COVER,
	CODE_TASK_AIM_AND_THROW_PROJECTILE,
	CODE_TASK_GUN,
	CODE_TASK_AIM_FROM_GROUND,
	CODE_TASK_AIM_GUN_STRAFE,
	CODE_TASK_AIM_GUN_CLIMBING,
	CODE_TASK_AIM_GUN_CLIMBING_LEDGE,
	CODE_TASK_AIM_GUN_VEHICLE_DRIVE_BY,
	CODE_TASK_AIM_GUN_SCRIPTED,
	CODE_TASK_AIM_GUN_RUN_AND_GUN,
	CODE_TASK_RELOAD_GUN,
	CODE_TASK_WEAPON_BLOCKED,
	CODE_TASK_ENTER_COVER,
	CODE_TASK_EXIT_COVER,
	CODE_TASK_AIM_GUN_FROM_COVER_INTRO,
	CODE_TASK_AIM_GUN_FROM_COVER_OUTRO,
	CODE_TASK_AIM_GUN_BLIND_FIRE,
	CODE_TASK_AIM_GUN_BLIND_FIRE_INTRO,
	CODE_TASK_AIM_GUN_BLIND_FIRE_OUTRO,
	CODE_TASK_COMBAT_CLOSEST_TARGET_IN_AREA,
	CODE_TASK_ADDITIONAL_COMBAT_TASK,
	CODE_TASK_IN_COVER,
	CODE_TASK_VEHICLE_DRIVE_BY,
	CODE_TASK_AIM_GUN_STANDING,
	CODE_TASK_AIM_GUN_ON_FOOT_PM,
	CODE_TASK_AIM_SWEEP,
	CODE_TASK_ARREST,
	CODE_TASK_CUFFED,
	CODE_TASK_IN_CUSTODY,
	CODE_TASK_INCAPACITATED,
	CODE_TASK_PLAY_CUFFED_SECONDARY_ANIMS,
	CODE_TASK_SHARK_CIRCLE,
	CODE_TASK_SHARK_ATTACK,
	CODE_TASK_AGITATED,
	CODE_TASK_AGITATED_ACTION,
	CODE_TASK_CONFRONT,
	CODE_TASK_INTIMIDATE,
	CODE_TASK_SHOVE,
	CODE_TASK_SHOVED,
	CODE_TASK_FINISHED,
	CODE_TASK_CROUCH_TOGGLE,
	CODE_TASK_REVIVE,
	CODE_TASK_CLONED_FSM_TEST_HARNESS,
	CODE_TASK_COMPLEX_USE_MOBILE_PHONE,
	CODE_TASK_SIMPLE_PHONE_TALK,
	CODE_TASK_SIMPLE_PHONE_IN,
	CODE_TASK_SIMPLE_PHONE_OUT,
	CODE_TASK_PARACHUTE,
	CODE_TASK_PARACHUTE_OBJECT,
	CODE_TASK_TAKE_OFF_PED_VARIATION,
	CODE_TASK_JETPACK,
	CODE_TASK_MOTION_JETPACK,
	CODE_TASK_COMBAT_SEEK_COVER,
	CODE_TASK_COMBAT_CHARGE,
	CODE_TASK_COMBAT_FLANK,
	CODE_TASK_COMBAT,
	CODE_TASK_COMBAT_MOUNTED,
	CODE_TASK_MOVE_CIRCLE,
	CODE_TASK_MOVE_COMBAT_MOUNTED,
	CODE_TASK_SEARCH,
	CODE_TASK_SEARCH_ON_FOOT,
	CODE_TASK_SEARCH_IN_AUTOMOBILE,
	CODE_TASK_SEARCH_IN_BOAT,
	CODE_TASK_SEARCH_IN_HELI,
	CODE_TASK_THREAT_RESPONSE,
	CODE_TASK_INVESTIGATE,
	CODE_TASK_STAND_GUARD_FSM,
	CODE_TASK_PATROL,
	CODE_TASK_SHOOT_AT_TARGET,
	CODE_TASK_SET_AND_GUARD_AREA,
	CODE_TASK_STAND_GUARD,
	CODE_TASK_SEPARATE,
	CODE_TASK_STAY_IN_COVER,
	CODE_TASK_VEHICLE_COMBAT,
	CODE_TASK_VEHICLE_PERSUIT,
	CODE_TASK_VEHICLE_CHASE,
	CODE_TASK_DRAGGING_TO_SAFETY,
	CODE_TASK_DRAGGED_TO_SAFETY,
	CODE_TASK_VARIED_AIM_POSE,
	CODE_TASK_MOVE_WITHIN_ATTACK_WINDOW,
	CODE_TASK_MOVE_WITHIN_DEFENSIVE_AREA,
	CODE_TASK_SHOOT_OUT_TIRE,
	CODE_TASK_SHELL_SHOCKED,
	CODE_TASK_BOAT_CHASE,
	CODE_TASK_BOAT_COMBAT,
	CODE_TASK_BOAT_STRAFE,
	CODE_TASK_HELI_CHASE,
	CODE_TASK_HELI_COMBAT,
	CODE_TASK_SUBMARINE_COMBAT,
	CODE_TASK_SUBMARINE_CHASE,
	CODE_TASK_PLANE_CHASE,
	CODE_TASK_TARGET_UNREACHABLE,
	CODE_TASK_TARGET_UNREACHABLE_IN_INTERIOR,
	CODE_TASK_TARGET_UNREACHABLE_IN_EXTERIOR,
	CODE_TASK_STEALTH_KILL,
	CODE_TASK_WRITHE,
	CODE_TASK_ADVANCE,
	CODE_TASK_CHARGE,
	CODE_TASK_MOVE_TO_TACTICAL_POINT,
	CODE_TASK_TO_HURT_TRANSIT,
	CODE_TASK_ANIMATED_HIT_BY_EXPLOSION,
	CODE_TASK_NM_RELAX,
	CODE_TASK_NM_ROLL_UP_AND_RELAX,
	CODE_TASK_NM_POSE,
	CODE_TASK_NM_BRACE,
	CODE_TASK_NM_BUOYANCY,
	CODE_TASK_NM_INJURED_ON_GROUND,
	CODE_TASK_NM_SHOT,
	CODE_TASK_NM_HIGH_FALL,
	CODE_TASK_NM_BALANCE,
	CODE_TASK_NM_ELECTROCUTE,
	CODE_TASK_NM_PROTOTYPE,
	CODE_TASK_NM_EXPLOSION,
	CODE_TASK_NM_ONFIRE,
	CODE_TASK_NM_SCRIPT_CONTROL,
	CODE_TASK_NM_JUMP_ROLL_FROM_ROAD_VEHICLE,
	CODE_TASK_NM_FLINCH,
	CODE_TASK_NM_SIT,
	CODE_TASK_NM_FALL_DOWN,
	CODE_TASK_BLEND_FROM_NM,
	CODE_TASK_NM_CONTROL,
	CODE_TASK_NM_DANGLE,
	CODE_TASK_NM_SLUNG_OVER_SHOULDER,
	CODE_TASK_NM_SHOOTING_WHILE_ATTACHED,
	CODE_TASK_NM_GENERIC_ATTACH,
	CODE_TASK_NM_DRUNK,
	CODE_TASK_NM_DRAGGING_TO_SAFETY,
	CODE_TASK_NM_THROUGH_WINDSCREEN,
	CODE_TASK_NM_RIVER_RAPIDS,
	CODE_TASK_NM_SIMPLE,
	CODE_TASK_RAGE_RAGDOLL,
	CODE_TASK_RAGDOLL_LAST,
	CODE_TASK_MOVE_SKI_BASE,
	CODE_TASK_JUMPVAULT,
	CODE_TASK_JUMP,
	CODE_TASK_FALL,
	CODE_TASK_JUMP_IN_AIR,
	CODE_TASK_REACT_AIM_WEAPON,
	CODE_TASK_CHAT,
	CODE_TASK_MOBILE_PHONE,
	CODE_TASK_REACT_TO_DEAD_PED,
	CODE_TASK_WATCH_INVESTIGATION,
	CODE_TASK_SEARCH_FOR_UNKNOWN_THREAT,
	CODE_TASK_CHECK_PED_IS_DEAD,
	CODE_TASK_BOMB,
	CODE_TASK_DETONATOR,
	CODE_TASK_SHOOTING_WHILE_ATTACHED,
	CODE_TASK_ANIMATED_ATTACH,
	CODE_TASK_ELECTROCUTE,
	CODE_TASK_GO_TO_POINT_USING_COVER,
	CODE_TASK_MOVE_IN_FORMATION,
	CODE_TASK_FOLLOW_LEADER_IN_PAIR,
	CODE_TASK_PLAY_CLIP_AND_SLIDE_TO_POSITION,
	CODE_TASK_CUTSCENE,
	CODE_TASK_REACT_TO_EXPLOSION,
	CODE_TASK_REACT_TO_IMMINENT_EXPLOSION,
	CODE_TASK_DIVE_TO_GROUND,
	CODE_TASK_REACT_AND_FLEE,
	CODE_TASK_SIDESTEP,
	CODE_TASK_CALL_POLICE,
	CODE_TASK_REACT_IN_DIRECTION,
	CODE_TASK_REACT_TO_BUDDY_SHOT,
	CODE_TASK_FRIENDLY_FIRE_NEAR_MISS,
	CODE_TASK_FRIENDLY_AIMED_AT,
	CODE_TASK_PLAYER_DEATH_REACT,
	CODE_TASK_VEHICLE_FIRST_TASK,
	CODE_TASK_VEHICLE_GOTO = CODE_TASK_VEHICLE_FIRST_TASK,
	CODE_TASK_VEHICLE_GOTO_AUTOMOBILE_NEW,
	CODE_TASK_VEHICLE_GOTO_PLANE,
	CODE_TASK_VEHICLE_GOTO_HELICOPTER,
	CODE_TASK_VEHICLE_GOTO_SUBMARINE,
	CODE_TASK_VEHICLE_GOTO_BOAT,
	CODE_TASK_VEHICLE_GOTO_POINT_AUTOMOBILE,
	CODE_TASK_VEHICLE_GOTO_POINT_WITH_AVOIDANCE_AUTOMOBILE,
	CODE_TASK_VEHICLE_PURSUE,
	CODE_TASK_VEHICLE_RAM,
	CODE_TASK_VEHICLE_SPIN_OUT,
	CODE_TASK_VEHICLE_APPROACH,
	CODE_TASK_VEHICLE_THREE_POINT_TURN,
	CODE_TASK_VEHICLE_DEAD_DRIVER,
	CODE_TASK_VEHICLE_CRUISE_NEW,
	CODE_TASK_VEHICLE_CRUISE_BOAT,
	CODE_TASK_VEHICLE_STOP,
	CODE_TASK_VEHICLE_PULL_OVER,
	CODE_TASK_VEHICLE_PASSENGER_EXIT,
	CODE_TASK_VEHICLE_FLEE,
	CODE_TASK_VEHICLE_FLEE_AIRBORNE,
	CODE_TASK_VEHICLE_FLEE_BOAT,
	CODE_TASK_VEHICLE_FOLLOW_RECORDING,
	CODE_TASK_VEHICLE_FOLLOW,
	CODE_TASK_VEHICLE_BLOCK,
	CODE_TASK_VEHICLE_BLOCK_CRUISE_IN_FRONT,
	CODE_TASK_VEHICLE_BLOCK_BRAKE_IN_FRONT,
	CODE_TASK_VEHICLE_BLOCK_BACK_AND_FORTH,
	CODE_TASK_VEHICLE_CRASH,
	CODE_TASK_VEHICLE_LAND,
	CODE_TASK_VEHICLE_LAND_PLANE,
	CODE_TASK_VEHICLE_HOVER,
	CODE_TASK_VEHICLE_ATTACK,
	CODE_TASK_VEHICLE_ATTACK_TANK,
	CODE_TASK_VEHICLE_CIRCLE,
	CODE_TASK_VEHICLE_POLICE_BEHAVIOUR,
	CODE_TASK_VEHICLE_POLICE_BEHAVIOUR_HELICOPTER,
	CODE_TASK_VEHICLE_POLICE_BEHAVIOUR_BOAT,
	CODE_TASK_VEHICLE_ESCORT,
	CODE_TASK_VEHICLE_HELI_PROTECT,
	CODE_TASK_VEHICLE_PLAYER_DRIVE,
	CODE_TASK_VEHICLE_PLAYER_DRIVE_AUTOMOBILE,
	CODE_TASK_VEHICLE_PLAYER_DRIVE_BIKE,
	CODE_TASK_VEHICLE_PLAYER_DRIVE_BOAT,
	CODE_TASK_VEHICLE_PLAYER_DRIVE_SUBMARINE,
	CODE_TASK_VEHICLE_PLAYER_DRIVE_SUBMARINECAR,
	CODE_TASK_VEHICLE_PLAYER_DRIVE_AMPHIBIOUS_AUTOMOBILE,
	CODE_TASK_VEHICLE_PLAYER_DRIVE_PLANE,
	CODE_TASK_VEHICLE_PLAYER_DRIVE_HELI,
	CODE_TASK_VEHICLE_PLAYER_DRIVE_AUTOGYRO,
	CODE_TASK_VEHICLE_PLAYER_DRIVE_DIGGER_ARM,
	CODE_TASK_VEHICLE_PLAYER_DRIVE_TRAIN,
	CODE_TASK_VEHICLE_PLANE_CHASE,
	CODE_TASK_VEHICLE_NO_DRIVER,
	CODE_TASK_VEHICLE_ANIMATION,
	CODE_TASK_VEHICLE_CONVERTIBLE_ROOF,
	CODE_TASK_VEHICLE_PARK_NEW,
	CODE_TASK_VEHICLE_FOLLOW_WAYPOINT_RECORDING,
	CODE_TASK_VEHICLE_GOTO_NAVMESH,
	CODE_TASK_VEHICLE_REACT_TO_COP_SIREN,
	CODE_TASK_VEHICLE_GOTO_LONGRANGE,
	CODE_TASK_VEHICLE_WAIT,
	CODE_TASK_VEHICLE_REVERSE,
	CODE_TASK_VEHICLE_BRAKE,
	CODE_TASK_VEHICLE_HANDBRAKE,
	CODE_TASK_VEHICLE_TURN,
	CODE_TASK_VEHICLE_GO_FORWARD,
	CODE_TASK_VEHICLE_SWERVE,
	CODE_TASK_VEHICLE_FLY_DIRECTION,
	CODE_TASK_VEHICLE_HEADON_COLLISION,
	CODE_TASK_VEHICLE_BOOST_USE_STEERING_ANGLE,
	CODE_TASK_VEHICLE_SHOT_TIRE,
	CODE_TASK_VEHICLE_BURNOUT,
	CODE_TASK_VEHICLE_REV_ENGINE,
	CODE_TASK_VEHICLE_SURFACE_IN_SUBMARINE,
	CODE_TASK_VEHICLE_PULL_ALONGSIDE,
	CODE_TASK_VEHICLE_TRANSFORM_TO_SUBMARINE,
	CODE_TASK_ANIMATED_FALLBACK,
	CODE_MAX_NUM_TASK_TYPES
};
#pragma endregion

#pragma region Control Enums
enum eControlType {
	PLAYER_CONTROL = 0,
	CAMERA_CONTROL,
	FRONTEND_CONTROL
};

enum eControlAction {
	INPUT_NEXT_CAMERA = 0,
	INPUT_LOOK_LR,
	INPUT_LOOK_UD,
	INPUT_LOOK_UP_ONLY,
	INPUT_LOOK_DOWN_ONLY,
	INPUT_LOOK_LEFT_ONLY,
	INPUT_LOOK_RIGHT_ONLY,
	INPUT_CINEMATIC_SLOWMO,
	INPUT_SCRIPTED_FLY_UD,
	INPUT_SCRIPTED_FLY_LR,
	INPUT_SCRIPTED_FLY_ZUP,
	INPUT_SCRIPTED_FLY_ZDOWN,
	INPUT_WEAPON_WHEEL_UD,
	INPUT_WEAPON_WHEEL_LR,
	INPUT_WEAPON_WHEEL_NEXT,
	INPUT_WEAPON_WHEEL_PREV,
	INPUT_SELECT_NEXT_WEAPON,
	INPUT_SELECT_PREV_WEAPON,
	INPUT_SKIP_CUTSCENE,
	INPUT_CHARACTER_WHEEL,
	INPUT_MULTIPLAYER_INFO,
	INPUT_SPRINT,
	INPUT_JUMP,
	INPUT_ENTER,
	INPUT_ATTACK,
	INPUT_AIM,
	INPUT_LOOK_BEHIND,
	INPUT_PHONE,
	INPUT_SPECIAL_ABILITY,
	INPUT_SPECIAL_ABILITY_SECONDARY,
	INPUT_MOVE_LR,
	INPUT_MOVE_UD,
	INPUT_MOVE_UP_ONLY,
	INPUT_MOVE_DOWN_ONLY,
	INPUT_MOVE_LEFT_ONLY,
	INPUT_MOVE_RIGHT_ONLY,
	INPUT_DUCK,
	INPUT_SELECT_WEAPON,
	INPUT_PICKUP,
	INPUT_SNIPER_ZOOM,
	INPUT_SNIPER_ZOOM_IN_ONLY,
	INPUT_SNIPER_ZOOM_OUT_ONLY,
	INPUT_SNIPER_ZOOM_IN_SECONDARY,
	INPUT_SNIPER_ZOOM_OUT_SECONDARY,
	INPUT_COVER,
	INPUT_RELOAD,
	INPUT_TALK,
	INPUT_DETONATE,
	INPUT_HUD_SPECIAL,
	INPUT_ARREST,
	INPUT_ACCURATE_AIM,
	INPUT_CONTEXT,
	INPUT_CONTEXT_SECONDARY,
	INPUT_WEAPON_SPECIAL,
	INPUT_WEAPON_SPECIAL_TWO,
	INPUT_DIVE,
	INPUT_DROP_WEAPON,
	INPUT_DROP_AMMO,
	INPUT_THROW_GRENADE,
	INPUT_VEH_MOVE_LR,
	INPUT_VEH_MOVE_UD,
	INPUT_VEH_MOVE_UP_ONLY,
	INPUT_VEH_MOVE_DOWN_ONLY,
	INPUT_VEH_MOVE_LEFT_ONLY,
	INPUT_VEH_MOVE_RIGHT_ONLY,
	INPUT_VEH_SPECIAL,
	INPUT_VEH_GUN_LR,
	INPUT_VEH_GUN_UD,
	INPUT_VEH_AIM,
	INPUT_VEH_ATTACK,
	INPUT_VEH_ATTACK2,
	INPUT_VEH_ACCELERATE,
	INPUT_VEH_BRAKE,
	INPUT_VEH_DUCK,
	INPUT_VEH_HEADLIGHT,
	INPUT_VEH_EXIT,
	INPUT_VEH_HANDBRAKE,
	INPUT_VEH_HOTWIRE_LEFT,
	INPUT_VEH_HOTWIRE_RIGHT,
	INPUT_VEH_LOOK_BEHIND,
	INPUT_VEH_CIN_CAM,
	INPUT_VEH_NEXT_RADIO,
	INPUT_VEH_PREV_RADIO,
	INPUT_VEH_NEXT_RADIO_TRACK,
	INPUT_VEH_PREV_RADIO_TRACK,
	INPUT_VEH_RADIO_WHEEL,
	INPUT_VEH_HORN,
	INPUT_VEH_FLY_THROTTLE_UP,
	INPUT_VEH_FLY_THROTTLE_DOWN,
	INPUT_VEH_FLY_YAW_LEFT,
	INPUT_VEH_FLY_YAW_RIGHT,
	INPUT_VEH_PASSENGER_AIM,
	INPUT_VEH_PASSENGER_ATTACK,
	INPUT_VEH_SPECIAL_ABILITY_FRANKLIN,
	INPUT_VEH_STUNT_UD,
	INPUT_VEH_CINEMATIC_UD,
	INPUT_VEH_CINEMATIC_UP_ONLY,
	INPUT_VEH_CINEMATIC_DOWN_ONLY,
	INPUT_VEH_CINEMATIC_LR,
	INPUT_VEH_SELECT_NEXT_WEAPON,
	INPUT_VEH_SELECT_PREV_WEAPON,
	INPUT_VEH_ROOF,
	INPUT_VEH_JUMP,
	INPUT_VEH_GRAPPLING_HOOK,
	INPUT_VEH_SHUFFLE,
	INPUT_VEH_DROP_PROJECTILE,
	INPUT_VEH_MOUSE_CONTROL_OVERRIDE,
	INPUT_VEH_FLY_ROLL_LR,
	INPUT_VEH_FLY_ROLL_LEFT_ONLY,
	INPUT_VEH_FLY_ROLL_RIGHT_ONLY,
	INPUT_VEH_FLY_PITCH_UD,
	INPUT_VEH_FLY_PITCH_UP_ONLY,
	INPUT_VEH_FLY_PITCH_DOWN_ONLY,
	INPUT_VEH_FLY_UNDERCARRIAGE,
	INPUT_VEH_FLY_ATTACK,
	INPUT_VEH_FLY_SELECT_NEXT_WEAPON,
	INPUT_VEH_FLY_SELECT_PREV_WEAPON,
	INPUT_VEH_FLY_SELECT_TARGET_LEFT,
	INPUT_VEH_FLY_SELECT_TARGET_RIGHT,
	INPUT_VEH_FLY_VERTICAL_FLIGHT_MODE,
	INPUT_VEH_FLY_DUCK,
	INPUT_VEH_FLY_ATTACK_CAMERA,
	INPUT_VEH_FLY_MOUSE_CONTROL_OVERRIDE,
	INPUT_VEH_SUB_TURN_LR,
	INPUT_VEH_SUB_TURN_LEFT_ONLY,
	INPUT_VEH_SUB_TURN_RIGHT_ONLY,
	INPUT_VEH_SUB_PITCH_UD,
	INPUT_VEH_SUB_PITCH_UP_ONLY,
	INPUT_VEH_SUB_PITCH_DOWN_ONLY,
	INPUT_VEH_SUB_THROTTLE_UP,
	INPUT_VEH_SUB_THROTTLE_DOWN,
	INPUT_VEH_SUB_ASCEND,
	INPUT_VEH_SUB_DESCEND,
	INPUT_VEH_SUB_TURN_HARD_LEFT,
	INPUT_VEH_SUB_TURN_HARD_RIGHT,
	INPUT_VEH_SUB_MOUSE_CONTROL_OVERRIDE,
	INPUT_VEH_PUSHBIKE_PEDAL,
	INPUT_VEH_PUSHBIKE_SPRINT,
	INPUT_VEH_PUSHBIKE_FRONT_BRAKE,
	INPUT_VEH_PUSHBIKE_REAR_BRAKE,
	INPUT_MELEE_ATTACK_LIGHT,
	INPUT_MELEE_ATTACK_HEAVY,
	INPUT_MELEE_ATTACK_ALTERNATE,
	INPUT_MELEE_BLOCK,
	INPUT_PARACHUTE_DEPLOY,
	INPUT_PARACHUTE_DETACH,
	INPUT_PARACHUTE_TURN_LR,
	INPUT_PARACHUTE_TURN_LEFT_ONLY,
	INPUT_PARACHUTE_TURN_RIGHT_ONLY,
	INPUT_PARACHUTE_PITCH_UD,
	INPUT_PARACHUTE_PITCH_UP_ONLY,
	INPUT_PARACHUTE_PITCH_DOWN_ONLY,
	INPUT_PARACHUTE_BRAKE_LEFT,
	INPUT_PARACHUTE_BRAKE_RIGHT,
	INPUT_PARACHUTE_SMOKE,
	INPUT_PARACHUTE_PRECISION_LANDING,
	INPUT_MAP,
	INPUT_SELECT_WEAPON_UNARMED,
	INPUT_SELECT_WEAPON_MELEE,
	INPUT_SELECT_WEAPON_HANDGUN,
	INPUT_SELECT_WEAPON_SHOTGUN,
	INPUT_SELECT_WEAPON_SMG,
	INPUT_SELECT_WEAPON_AUTO_RIFLE,
	INPUT_SELECT_WEAPON_SNIPER,
	INPUT_SELECT_WEAPON_HEAVY,
	INPUT_SELECT_WEAPON_SPECIAL,
	INPUT_SELECT_CHARACTER_MICHAEL,
	INPUT_SELECT_CHARACTER_FRANKLIN,
	INPUT_SELECT_CHARACTER_TREVOR,
	INPUT_SELECT_CHARACTER_MULTIPLAYER,
	INPUT_SAVE_REPLAY_CLIP,
	INPUT_SPECIAL_ABILITY_PC,
	INPUT_CELLPHONE_UP,
	INPUT_CELLPHONE_DOWN,
	INPUT_CELLPHONE_LEFT,
	INPUT_CELLPHONE_RIGHT,
	INPUT_CELLPHONE_SELECT,
	INPUT_CELLPHONE_CANCEL,
	INPUT_CELLPHONE_OPTION,
	INPUT_CELLPHONE_EXTRA_OPTION,
	INPUT_CELLPHONE_SCROLL_FORWARD,
	INPUT_CELLPHONE_SCROLL_BACKWARD,
	INPUT_CELLPHONE_CAMERA_FOCUS_LOCK,
	INPUT_CELLPHONE_CAMERA_GRID,
	INPUT_CELLPHONE_CAMERA_SELFIE,
	INPUT_CELLPHONE_CAMERA_DOF,
	INPUT_CELLPHONE_CAMERA_EXPRESSION,
	INPUT_FRONTEND_DOWN,
	INPUT_FRONTEND_UP,
	INPUT_FRONTEND_LEFT,
	INPUT_FRONTEND_RIGHT,
	INPUT_FRONTEND_RDOWN,
	INPUT_FRONTEND_RUP,
	INPUT_FRONTEND_RLEFT,
	INPUT_FRONTEND_RRIGHT,
	INPUT_FRONTEND_AXIS_X,
	INPUT_FRONTEND_AXIS_Y,
	INPUT_FRONTEND_RIGHT_AXIS_X,
	INPUT_FRONTEND_RIGHT_AXIS_Y,
	INPUT_FRONTEND_PAUSE,
	INPUT_FRONTEND_PAUSE_ALTERNATE,
	INPUT_FRONTEND_ACCEPT,
	INPUT_FRONTEND_CANCEL,
	INPUT_FRONTEND_X,
	INPUT_FRONTEND_Y,
	INPUT_FRONTEND_LB,
	INPUT_FRONTEND_RB,
	INPUT_FRONTEND_LT,
	INPUT_FRONTEND_RT,
	INPUT_FRONTEND_LS,
	INPUT_FRONTEND_RS,
	INPUT_FRONTEND_LEADERBOARD,
	INPUT_FRONTEND_SOCIAL_CLUB,
	INPUT_FRONTEND_SOCIAL_CLUB_SECONDARY,
	INPUT_FRONTEND_DELETE,
	INPUT_FRONTEND_ENDSCREEN_ACCEPT,
	INPUT_FRONTEND_ENDSCREEN_EXPAND,
	INPUT_FRONTEND_SELECT,
	INPUT_SCRIPT_LEFT_AXIS_X,
	INPUT_SCRIPT_LEFT_AXIS_Y,
	INPUT_SCRIPT_RIGHT_AXIS_X,
	INPUT_SCRIPT_RIGHT_AXIS_Y,
	INPUT_SCRIPT_RUP,
	INPUT_SCRIPT_RDOWN,
	INPUT_SCRIPT_RLEFT,
	INPUT_SCRIPT_RRIGHT,
	INPUT_SCRIPT_LB,
	INPUT_SCRIPT_RB,
	INPUT_SCRIPT_LT,
	INPUT_SCRIPT_RT,
	INPUT_SCRIPT_LS,
	INPUT_SCRIPT_RS,
	INPUT_SCRIPT_PAD_UP,
	INPUT_SCRIPT_PAD_DOWN,
	INPUT_SCRIPT_PAD_LEFT,
	INPUT_SCRIPT_PAD_RIGHT,
	INPUT_SCRIPT_SELECT,
	INPUT_CURSOR_ACCEPT,
	INPUT_CURSOR_CANCEL,
	INPUT_CURSOR_X,
	INPUT_CURSOR_Y,
	INPUT_CURSOR_SCROLL_UP,
	INPUT_CURSOR_SCROLL_DOWN,
	INPUT_ENTER_CHEAT_CODE,
	INPUT_INTERACTION_MENU,
	INPUT_MP_TEXT_CHAT_ALL,
	INPUT_MP_TEXT_CHAT_TEAM,
	INPUT_MP_TEXT_CHAT_FRIENDS,
	INPUT_MP_TEXT_CHAT_CREW,
	INPUT_PC_PUSH_TO_TALK,
	INPUT_CREATOR_LS,
	INPUT_CREATOR_RS,
	INPUT_CREATOR_LT,
	INPUT_CREATOR_RT,
	INPUT_CREATOR_MENU_TOGGLE,
	INPUT_CREATOR_ACCEPT,
	INPUT_CREATOR_DELETE,
	INPUT_ATTACK2,
	INPUT_RAPPEL_JUMP,
	INPUT_RAPPEL_LONG_JUMP,
	INPUT_RAPPEL_SMASH_WINDOW,
	INPUT_PREV_WEAPON,
	INPUT_NEXT_WEAPON,
	INPUT_MELEE_ATTACK1,
	INPUT_MELEE_ATTACK2,
	INPUT_WHISTLE,
	INPUT_MOVE_LEFT,
	INPUT_MOVE_RIGHT,
	INPUT_MOVE_UP,
	INPUT_MOVE_DOWN,
	INPUT_LOOK_LEFT,
	INPUT_LOOK_RIGHT,
	INPUT_LOOK_UP,
	INPUT_LOOK_DOWN,
	INPUT_SNIPER_ZOOM_IN,
	INPUT_SNIPER_ZOOM_OUT,
	INPUT_SNIPER_ZOOM_IN_ALTERNATE,
	INPUT_SNIPER_ZOOM_OUT_ALTERNATE,
	INPUT_VEH_MOVE_LEFT,
	INPUT_VEH_MOVE_RIGHT,
	INPUT_VEH_MOVE_UP,
	INPUT_VEH_MOVE_DOWN,
	INPUT_VEH_GUN_LEFT,
	INPUT_VEH_GUN_RIGHT,
	INPUT_VEH_GUN_UP,
	INPUT_VEH_GUN_DOWN,
	INPUT_VEH_LOOK_LEFT,
	INPUT_VEH_LOOK_RIGHT,
	INPUT_REPLAY_START_STOP_RECORDING,
	INPUT_REPLAY_START_STOP_RECORDING_SECONDARY,
	INPUT_SCALED_LOOK_LR,
	INPUT_SCALED_LOOK_UD,
	INPUT_SCALED_LOOK_UP_ONLY,
	INPUT_SCALED_LOOK_DOWN_ONLY,
	INPUT_SCALED_LOOK_LEFT_ONLY,
	INPUT_SCALED_LOOK_RIGHT_ONLY,
	INPUT_REPLAY_MARKER_DELETE,
	INPUT_REPLAY_CLIP_DELETE,
	INPUT_REPLAY_PAUSE,
	INPUT_REPLAY_REWIND,
	INPUT_REPLAY_FFWD,
	INPUT_REPLAY_NEWMARKER,
	INPUT_REPLAY_RECORD,
	INPUT_REPLAY_SCREENSHOT,
	INPUT_REPLAY_HIDEHUD,
	INPUT_REPLAY_STARTPOINT,
	INPUT_REPLAY_ENDPOINT,
	INPUT_REPLAY_ADVANCE,
	INPUT_REPLAY_BACK,
	INPUT_REPLAY_TOOLS,
	INPUT_REPLAY_RESTART,
	INPUT_REPLAY_SHOWHOTKEY,
	INPUT_REPLAY_CYCLEMARKERLEFT,
	INPUT_REPLAY_CYCLEMARKERRIGHT,
	INPUT_REPLAY_FOVINCREASE,
	INPUT_REPLAY_FOVDECREASE,
	INPUT_REPLAY_CAMERAUP,
	INPUT_REPLAY_CAMERADOWN,
	INPUT_REPLAY_SAVE,
	INPUT_REPLAY_TOGGLETIME,
	INPUT_REPLAY_TOGGLETIPS,
	INPUT_REPLAY_PREVIEW,
	INPUT_REPLAY_TOGGLE_TIMELINE,
	INPUT_REPLAY_TIMELINE_PICKUP_CLIP,
	INPUT_REPLAY_TIMELINE_DUPLICATE_CLIP,
	INPUT_REPLAY_TIMELINE_PLACE_CLIP,
	INPUT_REPLAY_CTRL,
	INPUT_REPLAY_TIMELINE_SAVE,
	INPUT_REPLAY_PREVIEW_AUDIO,
	INPUT_VEH_DRIVE_LOOK,
	INPUT_VEH_DRIVE_LOOK2,
	INPUT_VEH_FLY_ATTACK2,
	INPUT_RADIO_WHEEL_UD,
	INPUT_RADIO_WHEEL_LR,
	INPUT_VEH_SLOWMO_UP,
	INPUT_VEH_SLOWMO_UP_ONLY,
	INPUT_VEH_SLOWMO_DOWN_ONLY,
	INPUT_VEH_HYDRAULICS_CONTROL_TOGGLE,
	INPUT_VEH_HYDRAULICS_CONTROL_LEFT,
	INPUT_VEH_HYDRAULICS_CONTROL_RIGHT,
	INPUT_VEH_HYDRAULICS_CONTROL_UP,
	INPUT_VEH_HYDRAULICS_CONTROL_DOWN,
	INPUT_VEH_HYDRAULICS_CONTROL_LR,
	INPUT_VEH_HYDRAULICS_CONTROL_UD,
	INPUT_SWITCH_VISOR,
	INPUT_VEH_MELEE_HOLD,
	INPUT_VEH_MELEE_LEFT,
	INPUT_VEH_MELEE_RIGHT,
	INPUT_MAP_POI,
	INPUT_REPLAY_SNAPMATIC_PHOTO,
	INPUT_VEH_CAR_JUMP,
	INPUT_VEH_ROCKET_BOOST,
	INPUT_VEH_FLY_BOOST,
	INPUT_VEH_PARACHUTE,
	INPUT_VEH_BIKE_WINGS,
	INPUT_VEH_FLY_BOMB_BAY,
	INPUT_VEH_FLY_COUNTER,
	INPUT_VEH_TRANSFORM,
	INPUT_QUAD_LOCO_REVERSE,
	INPUT_RESPAWN_FASTER,
	INPUT_HUDMARKER_SELECT,
	INPUT_EAT_SNACK,
	INPUT_USE_ARMOR,
	MAX_INPUTS
};
#pragma endregion

#pragma region Animations
enum eAnimationFlags
{
	AF_DEFAULT = 0,                                     //                  
	AF_LOOPING = 1,                                     // Repeat the animation 
	AF_HOLD_LAST_FRAME = 2,                             // Hold on the last frame of the animation 
	AF_REPOSITION_WHEN_FINISHED = 4,                    // When the animation finishes pop the peds physical reprsentation position to match the visual representations position Note that the animator must not unwind the animation and must have an independent mover node 
	AF_NOT_INTERRUPTABLE = 8,                           // Can the task not be interupted by extenal events 
	AF_UPPERBODY = 16,                                  // Only plays the upper body part of the animation. Dampens any motion caused by the lower body animation.Note that the animation should include the root node 
	AF_SECONDARY = 32,                                  // The task will run in the secondary task slot. This means it can be used aswell as a movement task (for instance) 
	AF_REORIENT_WHEN_FINISHED = 64,                     // When the animation finishes pop the peds physical reprsentation direction to match the visual representations direction. Note that the animator must not unwind the animation and must have an independent mover node 
	AF_ABORT_ON_PED_MOVEMENT = 128,                     // Ends the animation early if the ped attemps to move e.g. if the player tries to move using the controller. Can also be used to blend out automatically when an ai ped starts moving by combining it with the AF_SECONDARY flag.
	AF_ADDITIVE = 256,                                  // Play back the animation additively. Note, this will only produce sensible results on specifically authored additive animations!
	AF_TURN_OFF_COLLISION = 512,                        // Do not react to collision detection whilst this anim is playing
	AF_OVERRIDE_PHYSICS = 1024,                         // Do not apply any physics forces whilst the anim is playing. Automatically turns off collision, extracts any initial offset provided in the clip and uses per frame mover extraction.
	AF_IGNORE_GRAVITY = 2048,                           // Do not apply gravity while the anim is playing
	AF_EXTRACT_INITIAL_OFFSET = 4096,                   // Extract an initial offset from the playback position authored by the animator
	// Use this flag when playing back anims on different peds which have been authored
	// to sync with each other
	AF_EXIT_AFTER_INTERRUPTED = 8192,                   // Exit the animation task if it is interrupted by another task (ie Natural Motion).  Without this flag bing set looped animations will restart ofter the NM task

	// Tag synchronizer flags - sync the anim against ped movement (walking / running / etc)
	AF_TAG_SYNC_IN = 16384,                             // Sync the anim whilst blending in (use for seamless transitions from walk / run into a full body anim)
	AF_TAG_SYNC_OUT = 32768,                            // Sync the anim whilst blending out (use for seamless transitions from a full body anim into walking / running behaviour)
	AF_TAG_SYNC_CONTINUOUS = 65536,                     // Sync all the time (Only usefull to synchronize a partial anim e.g. an upper body)

	AF_FORCE_START = 131072,                            // Force the anim task to start even if the ped is falling / ragdolling / etc. Can fix issues with peds not playing their anims immediately after a warp / etc
	AF_USE_KINEMATIC_PHYSICS = 262144,                  // Use the kinematic physics mode on the entity for the duration of the anim (it should push other entities out of the way, and not be pushed around by players / etc
	AF_USE_MOVER_EXTRACTION = 524288,                   // Updates the peds capsule position every frame based on the animation. Use in conjunction with AF_USE_KINEMATIC_PHYSICS to create characters that cannot be pushed off course by other entities / geometry / etc whilst playing the anim.

	AF_HIDE_WEAPON = 1048576,                           // Indicates that the ped's weapon should be hidden while this animation is playing.

	AF_ENDS_IN_DEAD_POSE = 2097152,                     // When the anim ends, kill the ped and use the currently playing anim as the dead pose
	AF_ACTIVATE_RAGDOLL_ON_COLLISION = 4194304,         // If the peds ragdoll bounds make contact with something physical (that isn't flat ground) activate the ragdoll and fall over.
	AF_DONT_EXIT_ON_DEATH = 8388608,					// Currently used only on secondary anim tasks. Secondary anim tasks will end automatically when the ped dies. Setting this flag stops that from happening."
	AF_ABORT_ON_WEAPON_DAMAGE = 16777216,				// Allow aborting from damage events (including non-ragdoll damage events) even when blocking other ai events using AF_NOT_INTERRUPTABLE.
	AF_DISABLE_FORCED_PHYSICS_UPDATE = 33554432,		// Prevent adjusting the capsule on the enter state (useful if script is doing a sequence of scripted anims and they are known to more or less stand still) 
	AF_PROCESS_ATTACHMENTS_ON_START = 67108864,			// Force the attachments to be processed at the start of the clip
	AF_EXPAND_PED_CAPSULE_FROM_SKELETON = 134217728,	// Expands the capsule to the extents of the skeleton
	AF_USE_ALTERNATIVE_FP_ANIM = 268435456,				// Plays an alternative first person version of the clip on the player when in first person mode (the first person clip must be in the same dictionary, and be named the same as the anim you're playing, but with _FP appended on the end)
	AF_BLENDOUT_WRT_LAST_FRAME = 536870912,				// Start blending out the anim early, so that the blend out duration completes at the end of the animation.
	AF_USE_FULL_BLENDING = 1073741824                 	// Use full blending for this anim and override the heading/position adjustment in CTaskScriptedAnimation::CheckIfClonePlayerNeedsHeadingPositionAdjust(), so that we don't correct errors (special case such as scrip-side implemented AI tasks, i.e. diving)

};

constexpr int defaultAF = AF_NOT_INTERRUPTABLE | AF_TAG_SYNC_IN | AF_TAG_SYNC_OUT | AF_HIDE_WEAPON | AF_ABORT_ON_WEAPON_DAMAGE | AF_EXIT_AFTER_INTERRUPTED;
constexpr int upperAF = AF_NOT_INTERRUPTABLE | AF_UPPERBODY | AF_TAG_SYNC_IN | AF_TAG_SYNC_OUT | AF_HIDE_WEAPON | AF_ABORT_ON_WEAPON_DAMAGE | AF_EXIT_AFTER_INTERRUPTED;
constexpr int upperSecondaryAF = AF_NOT_INTERRUPTABLE | AF_UPPERBODY | AF_TAG_SYNC_CONTINUOUS | AF_SECONDARY | AF_HIDE_WEAPON | AF_ABORT_ON_WEAPON_DAMAGE | AF_EXIT_AFTER_INTERRUPTED;

enum eAnimPriorityFlags {
	AF_PRIORITY_LOW = 0,
	AF_PRIORITY_MEDIUM = 1,
	AF_PRIORITY_HIGH = 2
};

enum eIKControlFlags {
	AIK_NONE = 0,
	AIK_DISABLE_LEG_IK = 1,
	AIK_DISABLE_ARM_IK = 2,
	AIK_DISABLE_HEAD_IK = 4,
	AIK_DISABLE_TORSO_IK = 8,
	AIK_DISABLE_TORSO_REACT_IK = 16,
	AIK_USE_LEG_ALLOW_TAGS = 32,
	AIK_USE_LEG_BLOCK_TAGS = 64,
	AIK_USE_ARM_ALLOW_TAGS = 128,
	AIK_USE_ARM_BLOCK_TAGS = 256,
	AIK_PROCESS_WEAPON_HAND_GRIP = 512,
	AIK_USE_FP_ARM_LEFT = 1024,
	AIK_USE_FP_ARM_RIGHT = 2048,
	AIK_DISABLE_TORSO_VEHICLE_IK = 4096,
	AIK_LINKED_FACIAL = 8192
};

enum eAnimFilters {
	BONEMASK_ALL = Joaat("BONEMASK_ALL"),
	BONEMASK_ARMONLY_L = Joaat("BONEMASK_ARMONLY_L"),
	BONEMASK_ARMONLY_R = Joaat("BONEMASK_ARMONLY_R"),
	BONEMASK_ARMS = Joaat("BONEMASK_ARMS"),
	BONEMASK_BODYONLY = Joaat("BONEMASK_BODYONLY"),
	BONEMASK_HEADONLY = Joaat("BONEMASK_HEADONLY"),
	BONEMASK_HEAD_NECK_AND_ARMS = Joaat("BONEMASK_HEAD_NECK_AND_ARMS"),
	BONEMASK_HEAD_NECK_AND_L_ARM = Joaat("BONEMASK_HEAD_NECK_AND_L_ARM"),
	BONEMASK_HEAD_NECK_AND_R_ARM = Joaat("BONEMASK_HEAD_NECK_AND_R_ARM"),
	BONEMASK_LOD_LO = Joaat("BONEMASK_LOD_LO"),
	BONEMASK_SPINEONLY = Joaat("BONEMASK_SPINEONLY"),
	BONEMASK_SPINE_FEATHERED = Joaat("BONEMASK_SPINE_FEATHERED"),
	BONEMASK_UPPERONLY = Joaat("BONEMASK_UPPERONLY"),
	FILTER_ARMSHEADNOMOVER = Joaat("ArmsHeadNoMover_filter"),
	FILTER_BOTHARMS = Joaat("BothArms_filter"),
	//FILTER_BOTHARMS_NOSPINE = Joaat("BothArms__NoSpine_filter"),
	FILTER_BOTHARMS_NOSPINE = Joaat("BothArms_NoSpine_filter"),
	FILTER_GRIP_R_ONLY = Joaat("Grip_R_Only_filter"),
	FILTER_IGNOREMOVERBLEND = Joaat("IgnoreMoverBlend_filter"),
	FILTER_IGNOREMOVERBLENDROTATIONONLY = Joaat("IgnoreMoverBlendRotationOnly_filter"),
	FILTER_LEGSONLY = Joaat("LegsOnly_filter"),
	FILTER_LOWERBODY = Joaat("Lowerbody_filter"),
	FILTER_MOVERONLY = Joaat("MoverOnly_Filter"),
	FILTER_NOMOVER = Joaat("NoMover_filter"),
	FILTER_ROOTONLY = Joaat("RootOnly_filter"),
	FILTER_ROOTUPPERBODY = Joaat("RootUpperbody_filter"),
	FILTER_UPPERBODYNOARM = Joaat("UpperBodyNoArms_filter"),
	FILTER_UPPERBODYANDIK = Joaat("UpperbodyAndIk_filter"),
	FILTER_UPPERBODYFEATHERED_NOLEFTTARM = Joaat("UpperbodyFeathered_NoLefttArm_filter"),
	FILTER_UPPERBODYFEATHERED_NORIGHTARM = Joaat("UpperbodyFeathered_NoRightArm_filter"),
	FILTER_UPPERBODYFEATHERED = Joaat("UpperbodyFeathered_filter"),
	FILTER_UPPERBODYNOMOVER = Joaat("UpperbodyNoMover_filter"),
	FILTER_UPPERBODY = Joaat("Upperbody_filter"),
	FILTER_UPPERBODY_FROM_SPINE3 = Joaat("Upperbody_from_Spine3_filter"),
	FILTER_UPPERBODYBICYCLE_DRIVEBY = Joaat("UpperBodyBicycleDriveby_filter")
};

constexpr float WALK_BLEND_IN = 1.5f;				// 20frms
constexpr float WALK_BLEND_OUT = -1.5f;				// 20frms
constexpr float REALLY_SLOW_BLEND_IN = 2.0f;		// 15frms
constexpr float REALLY_SLOW_BLEND_OUT = -2.0f;		// 15frms
constexpr float SLOW_BLEND_IN = 4.0f;				// 8frms
constexpr float SLOW_BLEND_OUT = -4.0f;				// 8frms
constexpr float NORMAL_BLEND_IN = 8.0f;				// 4frms
constexpr float NORMAL_BLEND_OUT = -8.0f;			// 4frms
constexpr float FAST_BLEND_IN = 16.0f;				// 2frms
constexpr float FAST_BLEND_OUT = -16.0f;			// 2frms
constexpr float INSTANT_BLEND_IN = 1000.0f;			// 0frms
constexpr float INSTANT_BLEND_OUT = -1000.0f;		// 0frms

// For scripted anims ONLY
constexpr float SLOW_BLEND_DURATION = 0.25f;
constexpr float NORMAL_BLEND_DURATION = 0.125f;
constexpr float FAST_BLEND_DURATION = 0.0625f;
constexpr float INSTANT_BLEND_DURATION = 0.0f;

enum eAnimationPlaybackType {
	APT_EMPTY = 0,
	APT_SINGLE_ANIM = 1,
	APT_3_WAY_BLEND = 2
};

struct AnimData {
	alignas(8) int type;

	alignas(8) char* dictionary0;
	alignas(8) char* anim0;
	alignas(8) float phase0;
	alignas(8) float rate0;
	alignas(8) float weight0;

	alignas(8) char* dictionary1;
	alignas(8) char* anim1;
	alignas(8) float phase1;
	alignas(8) float rate1;
	alignas(8) float weight1;

	alignas(8) char* dictionary2;
	alignas(8) char* anim2;
	alignas(8) float phase2;
	alignas(8) float rate2;
	alignas(8) float weight2;

	alignas(8) int filter;			//eg. GET_HASH_KEY("BONEMASK_HEAD_NECK_AND_ARMS")
	alignas(8) float blendInDelta;
	alignas(8) float blendOutDelta;
	alignas(8) int timeToPlay;
	alignas(8) int flags;
	alignas(8) int ikFlags;

	AnimData(
		const char* _dictionary0 = "",
		const char* _anim0 = "",
		const float _phase0 = 0.0f,
		const float _rate0 = 1.0f,
		const float _weight0 = 1.0f,

		const int _type = APT_EMPTY,
		const int _filter = 0,
		const float _blendInDelta = NORMAL_BLEND_DURATION,
		const float _blendOutDelta = NORMAL_BLEND_DURATION,
		const int _timeToPlay = -1,
		const int _flags = AF_DEFAULT,
		const int _ikFlags = AIK_NONE,

		const char* _dictionary1 = "",
		const char* _anim1 = "",
		const float _phase1 = 0.0f,
		const float _rate1 = 1.0f,
		const float _weight1 = 1.0f,

		const char* _dictionary2 = "",
		const char* _anim2 = "",
		const float _phase2 = 0.0f,
		const float _rate2 = 1.0f,
		const float _weight2 = 1.0f
	)
	{
		dictionary0 = const_cast<char*>(_dictionary0);
		anim0 = const_cast<char*>(_anim0);
		phase0 = _phase0;
		rate0 = _rate0;
		weight0 = _weight0;

		type = _type;
		filter = _filter;
		blendInDelta = _blendInDelta;
		blendOutDelta = _blendOutDelta;
		timeToPlay = _timeToPlay;
		flags = _flags;
		ikFlags = _ikFlags;

		dictionary1 = const_cast<char*>(_dictionary1);
		anim1 = const_cast<char*>(_anim1);
		phase1 = _phase1;
		rate1 = _rate1;
		weight1 = _weight1;

		dictionary2 = const_cast<char*>(_dictionary2);
		anim2 = const_cast<char*>(_anim2);
		phase2 = _phase2;
		rate2 = _rate2;
		weight2 = _weight2;
	}
};

inline constexpr void SetAnimData(
	AnimData& data,
	const char* dictionary0 = "",
	const char* anim0 = "",
	const float phase0 = 0.0f,
	const float rate0 = 1.0f,
	const float weight0 = 1.0f,

	const int type = APT_EMPTY,
	const int filter = 0,
	const float blendInDelta = NORMAL_BLEND_DURATION,
	const float blendOutDelta = NORMAL_BLEND_DURATION,
	const int timeToPlay = -1,
	const int flags = AF_DEFAULT,
	const int ikFlags = AIK_NONE,

	const char* dictionary1 = "",
	const char* anim1 = "",
	const float phase1 = 0.0f,
	const float rate1 = 1.0f,
	const float weight1 = 1.0f,

	const char* dictionary2 = "",
	const char* anim2 = "",
	const float phase2 = 0.0f,
	const float rate2 = 1.0f,
	const float weight2 = 1.0f
)
{
	data.dictionary0 = const_cast<char*>(dictionary0);
	data.anim0 = const_cast<char*>(anim0);
	data.phase0 = phase0;
	data.rate0 = rate0;
	data.weight0 = weight0;

	data.type = type;
	data.filter = filter;
	data.blendInDelta = blendInDelta;
	data.blendOutDelta = blendOutDelta;
	data.timeToPlay = timeToPlay;
	data.flags = flags;
	data.ikFlags = ikFlags;

	data.dictionary1 = const_cast<char*>(dictionary1);
	data.anim1 = const_cast<char*>(anim1);
	data.phase1 = phase1;
	data.rate1 = rate1;
	data.weight1 = weight1;

	data.dictionary2 = const_cast<char*>(dictionary2);
	data.anim2 = const_cast<char*>(anim2);
	data.phase2 = phase2;
	data.rate2 = rate2;
	data.weight2 = weight2;
	return;
}
#pragma endregion