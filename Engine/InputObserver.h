#pragma once

class IInputObserver {
public:
	enum class EInputAction
	{
		MouseLeftPressed,
		MouseLeftDown,
		MouseRight,
		MouseMiddle,
		KeyEscape,
		KeyEnter,
		KeyShiftDown,
		KeyShiftRelease,
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
		MoveDown,
		AttackClick,
		OpenMenuPress,
		Ability1,
		Ability2,
		Ability3,
		LoadLevel,
		PauseGame,
		QuitGame,
		MiddleMouseMove,
		StandStill,
		Moving
	};

public:
	IInputObserver() = default;
	virtual ~IInputObserver() = default;
	virtual void ReceiveEvent(const EInputEvent aEvent) = 0;
};