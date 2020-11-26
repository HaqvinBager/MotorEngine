#pragma once

class IInputObserver {
public:
	enum class EInputAction
	{
		MouseLeft,
		MouseRight,
		MouseMiddle,
		KeyEscape,
		KeyEnter,
		KeyShift,
		Key1,
		Key2,
		Key3,
		KeyA,
		KeyS,
		KeyD
	};

	enum class EInputEvent
	{
		PopState,
		MoveClick,
		AttackClick,
		OpenMenuPress,
		Ability1,
		Ability2,
		Ability3,
		LoadLevel,
		PauseGame,
		QuitGame
	};

public:
	IInputObserver() = default;
	virtual ~IInputObserver() = default;
	virtual void ReceiveEvent(const EInputEvent aEvent) = 0;
};