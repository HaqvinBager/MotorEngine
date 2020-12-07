#pragma once
// Toggle by commenting before compilation.
//#define ANIMATIONS_GROUP_3
#define ANIMATIONS_GROUP_3

// The values used for the Enums are from Json/StringIDs_Animation.json
#ifdef ANIMATIONS_GROUP_3
enum class EPlayerAnimationID
{
	AbilityOne		= 5001,
	AbilityTwo		= 5002,
	AbilityThree	= 5002,
	AttackLight		= 5006,
	AttackHeavy		= 5004,
	Dying			= 5003,
	Dead			= 5003,
	Idle			= 5005,
	IdleCombat		= 5005,
	Run				= 5007
};

enum class EEnemyAnimationID
{
	Attack		= 5005,
	Dead		= 5005,
	Idle		= 5005,
	Walk		= 5005
};

enum class EBossAnimationID
{
	AttackOne	= 5005,
	AttackTwo	= 5005,
	AttackThree	= 5005,
	Dead		= 5005,
	Idle		= 5005,
	Walk		= 5005,
	Spawn		= 5005
};

enum class ECrateAnimationID
{
	Explode		= 5005
};
#endif

#ifdef ANIMATIONS_GROUP_4
enum class EPlayerAnimationID
{
	AbilityOne		= 5004,
	AbilityTwo		= 5003,
	AbilityThree	= 5003,
	AttackLight		= 5005,
	AttackHeavy		= 5006,
	Dying			= 5001,
	Dead			= 5002,
	Idle			= 5007,
	IdleCombat		= 5008,
	Run				= 5009
};

enum class EEnemyAnimationID
{
	Attack		= 5010,
	Dead		= 5011,
	Idle		= 5012,
	Walk		= 5013
};

enum class EBossAnimationID
{
	AttackOne	= 5014,
	AttackTwo	= 5014,
	AttackThree	= 5014,
	Dead		= 5015,
	Idle		= 5016,
	Walk		= 5017,
	Spawn		= 5017
};

enum class ECrateAnimationID
{
	Explode
};
#endif

// This didnt work 2020 12 02. Didn't get changes when compiling in _G4_DEBUG
#if defined(_G3_DEBUG) || defined(G3_NDEBUG)
enum class EPlayerAnimationID
{
	AbilityThrow	= 5001,
	AbilityExplo	= 5002,
	Attack01		= 5003,
	Attack02		= 5004,
	Die				= 5005,
	Idle01			= 5006,
	Idle02			= 5007,
	Run01			= 5008
};

enum class EEnemyAnimationID
{
	Attack01	= 5009,
	Dead		= 5010,
	Idle		= 5011,
	Walk		= 5012
};

enum class EBossAnimationID
{
};

enum class ECrateAnimationID
{
	Explode
};
#endif

#ifdef _G4_DEBUG
enum class EPlayerAnimationID
{
	AbilityThrow = 5004,
	AbilityExplo = 5003,
	AttackLight = 5005,
	AttackHeavy = 5006,
	Dying = 5001,
	Dead = 5002,
	Idle = 5007,
	IdleCombat = 5008,
	Run = 5009
};

enum class EEnemyAnimationID
{
	Attack = 5010,
	Dead = 5011,
	Idle = 5012,
	Walk = 5013
};

enum class EBossAnimationID
{
	Attack = 5014,
	Dead = 5015,
	Idle = 5016,
	Walk = 5017
};

enum class ECrateAnimationID
{
	Explode
};
#endif