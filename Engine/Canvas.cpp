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
#include "SpriteFactory.h"
#include "Sprite.h"
#include "rapidjson\document.h"
#include "rapidjson\istreamwrapper.h"

CCanvas::CCanvas(std::vector<EMessageType> someMessageTypes,
	std::vector<IInputObserver::EInputEvent> someInputEvents,
	std::vector<IInputObserver::EInputAction> someInputActions):
	myMessageTypes(someMessageTypes),
	myInputEvents(someInputEvents),
	myInputActions(someInputActions),
	myBackground(nullptr)
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
			SButtonData data;
			auto buttonData = buttonDataArray[i].GetObjectW();
			data.myText = buttonData["Text"].GetString();
			data.myPosition = { buttonData["Position X"].GetFloat(), buttonData["Position Y"].GetFloat() };
			data.myDimensions = { buttonData["Pixel Width"].GetFloat(), buttonData["Pixel Height"].GetFloat() };
			data.mySpritePaths.at(0) = buttonData["Idle Sprite Path"].GetString();
			data.mySpritePaths.at(1) = buttonData["Hover Sprite Path"].GetString();
			data.mySpritePaths.at(2) = buttonData["Click Sprite Path"].GetString();

			auto messageDataArray = buttonData["Messages"].GetArray();
			data.myMessagesToSend.resize(messageDataArray.Size());

			for (unsigned int j = 0; j < messageDataArray.Size(); ++j) {

				data.myMessagesToSend[j] = static_cast<EMessageType>(messageDataArray[j].GetInt());
			}

			myButtons.emplace_back(new CButton(data));
		}
	}

	if (document.HasMember("Animated UI Elements")) {
		auto animatedDataArray = document["Animated UI Elements"].GetArray();
		for (unsigned int i = 0; i < animatedDataArray.Size(); ++i)
		{
			myAnimatedUIs.emplace_back(new CAnimatedUIElement(animatedDataArray[i]["Path"].GetString()));
			float x = animatedDataArray[i]["Position X"].GetFloat();
			float y = animatedDataArray[i]["Position Y"].GetFloat();
			myAnimatedUIs.back()->SetPosition({x, y});
		}
	}

	if (document.HasMember("Sprites")) {
		auto spriteDataArray = document["Sprites"].GetArray();
		for (unsigned int i = 0; i < spriteDataArray.Size(); ++i) {
			CSpriteInstance* spriteInstance = new CSpriteInstance();
			spriteInstance->Init(CSpriteFactory::GetInstance()->GetSprite(spriteDataArray[i]["Path"].GetString()));
			mySprites.emplace_back(spriteInstance);
			float x = spriteDataArray[i]["Position X"].GetFloat();
			float y = spriteDataArray[i]["Position Y"].GetFloat();
			mySprites.back()->SetPosition({ x, y });
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

	for (unsigned int i = 0; i < myInputActions.size(); ++i) {
		CInputMapper::GetInstance()->MapEvent(myInputActions[i], myInputEvents[i]);
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
	case IInputObserver::EInputEvent::Ability1:

		break;
	case IInputObserver::EInputEvent::Ability2:
		
		break;
	case IInputObserver::EInputEvent::Ability3:
		
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