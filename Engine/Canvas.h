#pragma once
#include "Observer.h"

class CButton;
class CSpriteInstance;
class CTextInstance;
class CAnimatedUIElement;
class CScene;

class CCanvas : public IObserver
{
public:
	CCanvas();
	~CCanvas();

public:
	void Init(std::string aFilePath, CScene& aScene);
	void Update(/*float aDeltaTime*/);

public:
	void Receive(const SMessage& aMessage) override;
	void SubscribeToMessages();
	void UnsubscribeToMessages();

public:
	bool GetEnabled();
	void SetEnabled(bool isEnabled);

	std::vector<CButton*> GetButtons() { return myButtons; }

private:
	bool myIsEnabled;
	CSpriteInstance* myBackground;

	std::vector<CAnimatedUIElement*> myAnimatedUIs;
	std::vector<CButton*> myButtons;
	std::vector<CSpriteInstance*> mySprites;
	std::vector<CTextInstance*> myTexts;
	std::vector<EMessageType> myMessageTypes;
};