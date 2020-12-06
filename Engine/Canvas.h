#pragma once
#include "Observer.h"

class CButton;
class CSpriteInstance;
class CTextInstance;
class CAnimatedUIElement;

class CCanvas : public IObserver
{
public:
	CCanvas();
	~CCanvas();

public:
	void Init(std::string aFilePath);
	void Update(/*float aDeltaTime*/);

public:
	void Receive(const SMessage& aMessage) override;
	void SubscribeToMessages();
	void UnsubscribeToMessages();

public:
	bool GetEnabled();
	void SetEnabled(bool isEnabled);

	void AddToScene(CCanvas* aSprite);

	std::vector<CButton*> GetButtons() { return myButtons; }
	std::vector<CSpriteInstance*> GetSprites() { return mySprites; }
	std::vector<CAnimatedUIElement*> GetAnimatedUI() { return myAnimatedUIs; }

private:
	bool myIsEnabled;
	CSpriteInstance* myBackground;

	std::vector<CAnimatedUIElement*> myAnimatedUIs;
	std::vector<CButton*> myButtons;
	std::vector<CSpriteInstance*> mySprites;
	std::vector<CTextInstance*> myTexts;
	std::vector<EMessageType> myMessageTypes;
};