#include "stdafx.h"
#include "Canvas.h"
#include "MainSingleton.h"
#include "PostMaster.h"
#include "Button.h"
#include "SpriteInstance.h"
#include "TextInstance.h"
#include "AnimatedUIElement.h"
#include "InputMapper.h"
#include "Input.h"

#include "rapidjson\document.h"
#include "rapidjson\istreamwrapper.h"

CCanvas::CCanvas(std::vector<EMessageType> someMessageTypes, std::vector<IInputObserver::EInputEvent> someInputEvents) : myMessageTypes(someMessageTypes), myInputEvents(someInputEvents), myBackground(nullptr)
{
	SubscribeToMessages();
}

CCanvas::~CCanvas()
{
	UnsubscribeToMessages();
	myMessageTypes.clear();
	delete myBackground;
	myBackground = nullptr;
	myAnimatedUIs.clear();
	myButtons.clear();
	mySprites.clear();
	myTexts.clear();
}

void CCanvas::Init(std::string aFilePath)
{
	using namespace rapidjson;

	std::ifstream inputStream(aFilePath);
	IStreamWrapper inputWrapper(inputStream);
	Document document;
	document.ParseStream(inputWrapper);

	if (document.HasMember("Buttons")) {
		auto buttonDataArray = document["Buttons"].GetArray();
		for (unsigned int i = 0; i < buttonDataArray.Size(); ++i)
		{
			auto buttonData = buttonDataArray[i].GetArray();
			SButtonData data;
			data.myText = buttonData[0]["Text"].GetString();
			data.myPosition = { buttonData[1]["Position X"].GetFloat(), buttonData[2]["Position Y"].GetFloat() };
			data.myDimensions = { buttonData[3]["Pixel Width"].GetFloat(), buttonData[4]["Pixel Height"].GetFloat() };
			data.mySpritePaths.at(0) = buttonData[5]["Idle Sprite Path"].GetString();
			data.mySpritePaths.at(1) = buttonData[6]["Hover Sprite Path"].GetString();
			data.mySpritePaths.at(2) = buttonData[7]["Click Sprite Path"].GetString();
			myButtons.emplace_back(new CButton(data));
		}
	}
}

void CCanvas::Update(float /*aDeltaTime*/)
{
	DirectX::SimpleMath::Vector2 mousePos = { static_cast<float>(CommonUtilities::Input::GetInstance()->MouseX()), static_cast<float>(CommonUtilities::Input::GetInstance()->MouseY()) };
	for (unsigned int i = 0; i < myButtons.size(); ++i) 
	{
		myButtons[i]->CheckMouseCollision(mousePos);
	}

	if (CommonUtilities::Input::GetInstance()->IsMousePressed(CommonUtilities::Input::MouseButton::Left)) {
		for (unsigned int i = 0; i < myButtons.size(); ++i)
		{
			myButtons[i]->Click(true, nullptr);
		}
	}

	if (CommonUtilities::Input::GetInstance()->IsMouseReleased(CommonUtilities::Input::MouseButton::Left)) {
		for (unsigned int i = 0; i < myButtons.size(); ++i)
		{
			myButtons[i]->Click(false, nullptr);
		}
	}
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

	for (auto inputEvent : myInputEvents) {
		CInputMapper::GetInstance()->AddObserver(inputEvent, this);
	}
}

void CCanvas::UnsubscribeToMessages()
{
	for (auto messageType : myMessageTypes) {
		CMainSingleton::PostMaster().Unsubscribe(messageType, this);
	}

	for (auto inputEvent : myInputEvents) {
		CInputMapper::GetInstance()->RemoveObserver(inputEvent, this);
	}
}

void CCanvas::RecieveEvent(const IInputObserver::EInputEvent aEvent)
{
	switch (aEvent)
	{
	case IInputObserver::EInputEvent::AttackClick:
		break;
	default:
		break;
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
	//assert(myIsEnabled == isEnabled, "Enable is already the same as the input");
	if (myIsEnabled != isEnabled) {
		myIsEnabled = isEnabled;

		for (auto button : myButtons) {
			button->myEnabled = myIsEnabled;
		}

		for (auto sprite : mySprites) {
			sprite->SetShouldRender(myIsEnabled);
		}
	}
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