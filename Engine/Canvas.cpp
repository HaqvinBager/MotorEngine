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

using namespace rapidjson;

CCanvas::CCanvas() :
	myBackground(nullptr)
{
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
	std::ifstream inputStream(aFilePath);
	IStreamWrapper inputWrapper(inputStream);
	Document document;
	document.ParseStream(inputWrapper);

	if (document.HasMember("Buttons"))
	{
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

			for (unsigned int j = 0; j < messageDataArray.Size(); ++j)
			{

				data.myMessagesToSend[j] = static_cast<EMessageType>(messageDataArray[j].GetInt());
			}

			myButtons.emplace_back(new CButton(data));
		}
	}

	if (document.HasMember("Animated UI Elements"))
	{
		auto animatedDataArray = document["Animated UI Elements"].GetArray();
		for (unsigned int i = 0; i < animatedDataArray.Size(); ++i)
		{
			myAnimatedUIs.emplace_back(new CAnimatedUIElement(animatedDataArray[i]["Path"].GetString()));
			float x = animatedDataArray[i]["Position X"].GetFloat();
			float y = animatedDataArray[i]["Position Y"].GetFloat();
			myAnimatedUIs.back()->SetPosition({ x, y });
		}
	}

	if (document.HasMember("Background"))
	{
		myBackground = new CSpriteInstance();
		myBackground->Init(CSpriteFactory::GetInstance()->GetSprite(document["Background"]["Path"].GetString()));
		myBackground->SetRenderOrder(ERenderOrder::BackgroundLayer);
	}

	if (document.HasMember("Sprites"))
	{
		auto spriteDataArray = document["Sprites"].GetArray();
		for (unsigned int i = 0; i < spriteDataArray.Size(); ++i)
		{
			CSpriteInstance* spriteInstance = new CSpriteInstance();
			spriteInstance->Init(CSpriteFactory::GetInstance()->GetSprite(spriteDataArray[i]["Path"].GetString()));
			mySprites.emplace_back(spriteInstance);
			float x = spriteDataArray[i]["Position X"].GetFloat();
			float y = spriteDataArray[i]["Position Y"].GetFloat();
			mySprites.back()->SetPosition({ x, y });
		}
	}

	if (document.HasMember("PostmasterEvents"))
	{
		auto messageDataArray = document["PostmasterEvents"]["Events"].GetArray();
		myMessageTypes.resize(messageDataArray.Size());

		for (unsigned int j = 0; j < messageDataArray.Size(); ++j)
		{
			myMessageTypes[j] = static_cast<EMessageType>(messageDataArray[j].GetInt());
		}
	}

	SubscribeToMessages();
}

void CCanvas::Update(/*float aDeltaTime*/)
{
	DirectX::SimpleMath::Vector2 mousePos = { static_cast<float>(Input::GetInstance()->MouseX()), static_cast<float>(Input::GetInstance()->MouseY()) };
	for (unsigned int i = 0; i < myButtons.size(); ++i)
	{
		myButtons[i]->CheckMouseCollision(mousePos);
	}

	if (Input::GetInstance()->IsMousePressed(Input::MouseButton::Left))
	{
		for (unsigned int i = 0; i < myButtons.size(); ++i)
		{
			myButtons[i]->Click(true, nullptr);
		}
	}

	if (Input::GetInstance()->IsMouseReleased(Input::MouseButton::Left))
	{
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
	case EMessageType::AbilityOneCooldown:
		myAnimatedUIs[0]->Level(*static_cast<float*>(aMessage.data));
		std::cout << "Used ability 1 value: " << *static_cast<float*>(aMessage.data) << std::endl;
		break;
	case EMessageType::AbilityTwoCooldown:
		myAnimatedUIs[1]->Level(*static_cast<float*>(aMessage.data));
		std::cout << "Used ability 2 value: " << *static_cast<float*>(aMessage.data) << std::endl;
		break;
	case EMessageType::AbilityThreeCooldown:
		myAnimatedUIs[2]->Level(*static_cast<float*>(aMessage.data));
		std::cout << "Used ability 3 value: " << *static_cast<float*>(aMessage.data) << std::endl;
		break;
	default:
		break;
	}
}

void CCanvas::SubscribeToMessages()
{
	for (auto messageType : myMessageTypes)
	{
		CMainSingleton::PostMaster().Subscribe(messageType, this);
	}
}

void CCanvas::UnsubscribeToMessages()
{
	for (auto messageType : myMessageTypes)
	{
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
	if (myIsEnabled != isEnabled)
	{
		myIsEnabled = isEnabled;

		for (auto button : myButtons)
		{
			button->myEnabled = myIsEnabled;
		}

		for (auto sprite : mySprites)
		{
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