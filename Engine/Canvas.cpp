#include "stdafx.h"
#include "Canvas.h"
#include "MainSingleton.h"
#include "PostMaster.h"
#include "Button.h"
#include "SpriteInstance.h"
#include "TextInstance.h"
#include "AnimatedUIElement.h"

CCanvas::CCanvas(std::vector<EMessageType> someMessageTypes) : myMessageTypes(someMessageTypes), myBackground(nullptr)
{
	SubscribeToMessages();
}

CCanvas::~CCanvas()
{
	UnsubscribeToMessages();
	myMessageTypes.clear();
}

void CCanvas::Init()
{
}

void CCanvas::Update(float /*aDeltaTime*/)
{
}

void CCanvas::Receive(const SMessage& aMessage)
{
	switch (aMessage.myMessageType)
	{
	case EMessageType::ColliderAdded:
		break;
	case EMessageType::ColliderRemoved:
		break;
	case EMessageType::EnemyDied:
		break;
	case EMessageType::MainMenu:
		break;
	case EMessageType::Count:
		break;
	default:
		break;
	}
}

void CCanvas::SubscribeToMessages()
{
	for (auto messageType : myMessageTypes) {
		CMainSingleton::PostMaster().Subscribe(messageType, this);
	}
}

void CCanvas::UnsubscribeToMessages()
{
	for (auto messageType : myMessageTypes) {
		CMainSingleton::PostMaster().Unsubscribe(messageType, this);
	}
}

bool CCanvas::GetEnabled()
{
	return myIsEnabled;
}

CSpriteInstance* CCanvas::GetBackground()
{
	return myBackground;
}

std::vector<CAnimatedUIElement*> CCanvas::GetAnimatedUIs()
{
	return myAnimatedUIs;
}

std::vector<CButton*> CCanvas::GetButtons()
{
	return myButtons;
}

std::vector<CSpriteInstance*> CCanvas::GetSprites()
{
	return mySprites;
}

std::vector<CTextInstance*> CCanvas::GetTexts()
{
	return myTexts;
}

void CCanvas::SetEnabled(bool isEnabled)
{
	myIsEnabled = isEnabled;
}

void CCanvas::SetBackground(CSpriteInstance* aBackground)
{
	myBackground = aBackground;
}

void CCanvas::AddAnimatedUI(CAnimatedUIElement* anAnimatedUIElement)
{
	myAnimatedUIs.emplace_back(anAnimatedUIElement);
}

void CCanvas::AddButton(CButton* aButton)
{
	myButtons.emplace_back(aButton);
}

void CCanvas::AddSprite(CSpriteInstance* aSprite)
{
	mySprites.emplace_back(aSprite);
}

void CCanvas::AddText(CTextInstance* aText)
{
	myTexts.emplace_back(aText);
}