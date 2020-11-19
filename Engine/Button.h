#pragma once
#include "PostMaster.h"

class CCanvas;
class CSpriteInstance;

struct SButtonData {
	std::vector<EMessageType> myMessagesToSend;
	std::array<std::string, 3> mySpritePaths;
	// POSITION IS IN SHADER SPACE -1 -> 1 RIGHT NOW
	DirectX::SimpleMath::Vector2 myPosition;
	DirectX::SimpleMath::Vector2 myDimensions;
	std::string myText;
};

struct SUIRect {
	float myTop, myBottom, myLeft, myRight;
};

enum class EButtonState {
	Idle,
	Hover,
	Click
};

class CButton {
	friend CCanvas;
public:
	void OnHover();
	void OnClickDown();
	void OnClickUp(void* someData);
	void OnLeave();

	void Click(bool anIsPressed, void* someData);
	void CheckMouseCollision(DirectX::SimpleMath::Vector2 aScreenSpacePosition);

private:
	CButton(SButtonData& someData);
	~CButton();

private:
	std::vector<EMessageType> myMessagesToSend;
	std::array<CSpriteInstance*, 3> mySprites;
	SUIRect myRect;
	EButtonState myState;
	bool myEnabled;
	bool myIsMouseHover;
	
};

