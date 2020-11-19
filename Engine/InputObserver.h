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
		MoveClick,
		AttackClick,
		OpenMenuPress
	};

public:
	IInputObserver() = default;
	virtual ~IInputObserver() = default;
	virtual void RecieveEvent(const EInputEvent aEvent) = 0;
};