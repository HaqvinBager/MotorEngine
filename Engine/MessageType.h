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
	Count
};