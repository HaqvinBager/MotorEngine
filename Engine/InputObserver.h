#pragma once

class CInputObserver {
public:
	enum class EInputAction
	{
		MouseLeft,
		MouseRight,
		MouseMiddle,
		KeyEscape,
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
	CInputObserver() = default;
	virtual ~CInputObserver() = default;
	virtual void RecieveEvent(const EInputEvent aEvent, const float aValue) = 0;
};