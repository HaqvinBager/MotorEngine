#pragma once
// Toggle by commenting before compilation.
//#define ANIMATIONS_GROUP_3
#define ANIMATIONS_GROUP_4

// The values used for the Enums are from Json/StringIDs_Animation.json
#ifdef ANIMATIONS_GROUP_3
enum class EPlayerAnimationID
{
	AbilityThrow	= 5004,
	AbilityExplo	= 5003,
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
	Attack		= 5014,
	Dead		= 5015,
	Idle		= 5016,
	Walk		= 5017
};

enum class ECrateAnimationID
{
	Explode
};
#endif

#ifdef ANIMATIONS_GROUP_4
enum class EPlayerAnimationID
{
	AbilityThrow	= 5004,
	AbilityExplo	= 5003,
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
	Attack		= 5014,
	Dead		= 5015,
	Idle		= 5016,
	Walk		= 5017
};

enum class ECrateAnimationID
{
	Idle	= 0,// Idle = exists
	Walk	= 0,// It can't walk, just for clarity
	Dead	= 5019 // Explode
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
#elif defined(_G4_DEBUG) || defined(G4_NDEBUG)

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