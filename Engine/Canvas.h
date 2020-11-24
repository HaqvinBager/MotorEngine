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

	CSpriteInstance* GetBackground();
	std::vector<CAnimatedUIElement*> GetAnimatedUIs();
	std::vector<CButton*> GetButtons();
	std::vector<CSpriteInstance*> GetSprites();
	std::vector<CTextInstance*> GetTexts();

public:
	void SetEnabled(bool isEnabled);

	void SetBackground(CSpriteInstance* aBackground);
	void AddAnimatedUI(CAnimatedUIElement* anAnimatedUIElement);
	void AddButton(CButton* aButton);
	void AddSprite(CSpriteInstance* aSprite);
	void AddText(CTextInstance* aText);

private:
	bool myIsEnabled;
	CSpriteInstance* myBackground;

	std::vector<CAnimatedUIElement*> myAnimatedUIs;
	std::vector<CButton*> myButtons;
	std::vector<CSpriteInstance*> mySprites;
	std::vector<CTextInstance*> myTexts;
	std::vector<EMessageType> myMessageTypes;
};