#pragma once
enum class EMessageType
{
	ColliderAdded,
	ColliderRemoved,
	EnemyDied,
	MainMenu,
	Credits,
	Options,
	LoadLevel,
	StartGame,
	LevelSelect,
	Quit,
	AbilityOneCooldown,
	AbilityTwoCooldown,
	AbilityThreeCooldown,
	Resume,
	PlayerHealthChanged,
	PlayerResourceChanged,
	PlayerDied,
	BossFightStart,
	BossPhase2,
	BossPhase3,
	BossDied,
	BossHealthChanged,
	EnemyHealthChanged,
	PlayerTakeDamage,
	LoadDialogue,
	IntroStarted,
	IntroEnded,
	PlayAmbienceCastle,
	PlayAmbienceCave1,
	PlayAmbienceCave2,
	PlayAmbienceDungeon,
	PlayAmbienceSwamp1,
	PlayAmbienceSwamp2,
	UIButtonPress,
	PlayerExperienceChanged,
	PlayVoiceLine,
	StopDialogue,
	FadeInComplete,
	FadeOutComplete,
	AttackHits,
	DemonIdle1,
	DemonIdle2,
	LightAttack,
	HeavyAttack,
	HitDestructible,
	BossMeleeAttack,
	HealingAura,
	ShieldSpell,
	LoadLevel1,
	LoadLevel2,
	LoadLevel3,
	LoadLevel4,
	LoadLevel5,
	LoadLevel6,
	LoadLevel7,
	LoadLevel8,
	LoadLevel9,
	LoadLevel10,
	LoadDungeon,
	LoadGardens,
	LoadCastle,
	LoadBossRoom,
	PlayMainMenuMusic,
	PlayDungeonMusic,
	PlayGardensMusic,
	PlayCastleMusic,
	StopMusic,
	PlayBossDeathSFX,
	SetResolution1280x720,
	SetResolution1600x900,
	SetResolution1920x1080,
	PlayExplosionSFX,
	PlayBossExplosionSFX,
	PlayAmbienceGarden,
	PlayLevelUpSFX,
	Count
};
