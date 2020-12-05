#include "stdafx.h"
#include "Canvas.h"
#include "MainSingleton.h"
#include "PostMaster.h"
#include "Button.h"
#include "SpriteInstance.h"
#include "TextInstance.h"
#include "TextFactory.h"
#include "AnimatedUIElement.h"
#include "InputMapper.h"
#include "Input.h"
#include "SpriteFactory.h"
#include "Sprite.h"
#include "MainSingleton.h"
#include "rapidjson\document.h"
#include "rapidjson\istreamwrapper.h"
#include "..\..\Game\LoadLevelState.h"

using namespace rapidjson;

CCanvas::CCanvas() :
	myBackground(nullptr),
	myIsEnabled(true)
{
}

CCanvas::~CCanvas()
{
	UnsubscribeToMessages();
	myMessageTypes.clear();
	delete myBackground;
	myBackground = nullptr;

	myAnimatedUIs.clear();// Destruction is done in CScene

	for (size_t i = 0; i < myButtons.size(); ++i)
	{
			delete myButtons[i];
			myButtons[i] = nullptr;
	}
	myButtons.clear();

	for (size_t i = 0; i < mySprites.size(); ++i)
	{
			delete mySprites[i];
			mySprites[i] = nullptr;
	}
	mySprites.clear();

	for (size_t i = 0; i < myTexts.size(); ++i)
	{
			delete myTexts[i];
			myTexts[i] = nullptr;
	}
	myTexts.clear();
}

void CCanvas::Init(std::string aFilePath, CScene& aScene)
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
			
			myTexts.emplace_back(new CTextInstance(aScene));
			myTexts.back()->Init(CTextFactory::GetInstance()->GetText(buttonData["FontAndFontSize"].GetString()));
			myTexts.back()->SetText(buttonData["Text"].GetString());
			myTexts.back()->SetColor({ buttonData["Text Color R"].GetFloat(), buttonData["Text Color G"].GetFloat(), buttonData["Text Color B"].GetFloat(), 1.0f });
			myTexts.back()->SetPosition({ buttonData["Text Position X"].GetFloat(), buttonData["Text Position Y"].GetFloat() });
			myTexts.back()->SetPivot({ buttonData["Text Pivot X"].GetFloat(), buttonData["Text Pivot Y"].GetFloat() });

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

			myButtons.emplace_back(new CButton(data, aScene));
		}
	}

	if (document.HasMember("Texts"))
	{
		auto textDataArray = document["Texts"].GetArray();
		for (unsigned int i = 0; i < textDataArray.Size(); ++i)
		{
			auto textData = textDataArray[i].GetObjectW();
			myTexts.emplace_back(new CTextInstance(aScene));
			myTexts.back()->Init(CTextFactory::GetInstance()->GetText(textData["FontAndFontSize"].GetString()));
			myTexts.back()->SetText(textData["Text"].GetString());
			myTexts.back()->SetColor({ textData["Color R"].GetFloat(), textData["Color G"].GetFloat(), textData["Color B"].GetFloat(), 1.0f });
			myTexts.back()->SetPosition({ textData["Position X"].GetFloat(), textData["Position Y"].GetFloat() });
			myTexts.back()->SetPivot({ textData["Pivot X"].GetFloat(), textData["Pivot Y"].GetFloat() });
		}
	}

	if (document.HasMember("Animated UI Elements"))
	{
		auto animatedDataArray = document["Animated UI Elements"].GetArray();
		for (unsigned int i = 0; i < animatedDataArray.Size(); ++i)
		{
			myAnimatedUIs.emplace_back(new CAnimatedUIElement(animatedDataArray[i]["Path"].GetString(), aScene));
			float x = animatedDataArray[i]["Position X"].GetFloat();
			float y = animatedDataArray[i]["Position Y"].GetFloat();
			myAnimatedUIs.back()->SetPosition({ x, y });
		}
	}

	if (document.HasMember("Background"))
	{
		myBackground = new CSpriteInstance(aScene);
		myBackground->Init(CSpriteFactory::GetInstance()->GetSprite(document["Background"]["Path"].GetString()));
		myBackground->SetRenderOrder(ERenderOrder::BackgroundLayer);
	}

	if (document.HasMember("Sprites"))
	{
		auto spriteDataArray = document["Sprites"].GetArray();
		for (unsigned int i = 0; i < spriteDataArray.Size(); ++i)
		{
			CSpriteInstance* spriteInstance = new CSpriteInstance(aScene);
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
		SubscribeToMessages();
	}

}

void CCanvas::Update()
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
		break;
	case EMessageType::AbilityTwoCooldown:
		myAnimatedUIs[1]->Level(*static_cast<float*>(aMessage.data));
		break;
	case EMessageType::AbilityThreeCooldown:
		myAnimatedUIs[2]->Level(*static_cast<float*>(aMessage.data));
		break;
	case EMessageType::PlayerHealthChanged:
		myAnimatedUIs[3]->Level(*static_cast<float*>(aMessage.data));
		break;
	case EMessageType::PlayerResourceChanged:
		myAnimatedUIs[4]->Level(*static_cast<float*>(aMessage.data));
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
