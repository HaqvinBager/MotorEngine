#pragma once
#include "PostMaster.h"

class CCanvas;
class CSpriteInstance;

struct SButtonData {
	std::vector<EMessageType> myMessagesToSend;
	std::array<std::string, 3> mySpritePaths;
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

private:
	CButton(SButtonData& someData);
	~CButton();

private:
	bool myEnabled;
	EButtonState myState;
	std::vector<EMessageType> myMessagesToSend;
	std::array<CSpriteInstance*, 3> mySprites;
};

