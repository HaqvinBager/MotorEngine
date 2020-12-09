#include "stdafx.h"
#include "PopupTextService.h"
#include "TextFactory.h"
#include "TextInstance.h"
#include "SpriteFactory.h"
#include "SpriteInstance.h"
#include "Engine.h"
#include "WindowHandler.h"
#include "RandomNumberGenerator.h"
#include "JsonReader.h"
#include <algorithm>

#include "GameObject.h"
#include "TransformComponent.h"
#include "Scene.h"
#include "Engine.h"
#include "Utility.h"

#include "Scene.h"
#include "CameraComponent.h"

using namespace rapidjson;

bool CPopupTextService::Init()
{
	Document document = CJsonReader::LoadDocument("Json/PopupTextServiceInit.json");
	ENGINE_BOOL_POPUP(!document.HasParseError(), "Could not load 'Json/PopupTextServiceInit.json'!");

	const unsigned int damageNumbersPoolSize = document["Damage Numbers Pool Size"].GetInt();
	
	for (unsigned int i = 0; i < damageNumbersPoolSize; ++i)
	{
		myTextPool.push(new CTextInstance());
		myTextPool.back()->Init(CTextFactory::GetInstance()->GetText(document["Damage Numbers Font and Size"].GetString()));
		myAnimatedDataPool.push(new STextAnimationData());
	}

	myDamageColors[0] = { 1.0f, 1.0f, 1.0f, 1.0f };									// Normal
	myDamageColors[1] = { 255.0f / 255.0f, 203.0f / 255.0f, 5.0f / 255.0f, 1.0f };	// Crit
	myDamageColors[2] = { 232.0f / 255.0f, 126.0f / 255.0f, 4.0f / 255.0f, 1.0f };	// Ultracrit
	myDamageColors[3] = { 1.0f, 0.0f, 0.0f, 1.0f };									// Enemy
	myDamageColors[4] = { 0.0f, 230.0f / 255.0f, 64.0f / 255.0f, 1.0f };			// Healing

	myTutorialText = new CTextInstance();
	myTutorialText->Init(CTextFactory::GetInstance()->GetText(document["Tutorial Message Font and Size"].GetString()));
	myTutorialAnimationData = new STextAnimationData();

	myWarningText = new CTextInstance();
	myWarningText->Init(CTextFactory::GetInstance()->GetText(document["Warning Message Font and Size"].GetString()));
	myWarningAnimationData = new STextAnimationData();

	auto skillIconPaths = document["Skill Icon Paths"].GetArray();
	for (unsigned int i = 0; i < skillIconPaths.Size(); ++i)
	{
		mySkillIcons.emplace_back(new CSpriteInstance());
		mySkillIcons.back()->Init(CSpriteFactory::GetInstance()->GetSprite(skillIconPaths[i]["Path"].GetString()));
		mySkillIcons.back()->SetPosition({ document["Skill Icon Position X"].GetFloat(), document["Skill Icon Position Y"].GetFloat() });
	}
	
	myInfoBoxBackground = new CSpriteInstance();
	myInfoBoxBackground->Init(CSpriteFactory::GetInstance()->GetSprite(document["Skill Info Box Background Path"].GetString()));
	myInfoBoxBackground->SetPosition({ document["Info Box Position X"].GetFloat(), document["Info Box Position Y"].GetFloat() });

	myInfoBoxText = new CTextInstance();
	myInfoBoxText->Init(CTextFactory::GetInstance()->GetText(document["Skill Info Font and Size"].GetString()));
	myInfoAnimationData = new STextAnimationData();

	auto skillTexts = document["Skill Info Texts"].GetArray();
	for (unsigned int i = 0; i < skillTexts.Size(); ++i)
	{
		myStoredSkillInfoStrings.emplace_back(skillTexts[i]["Text"].GetString());
	}

	return true;
}

/// <summary>
/// Spawns a floating damage number, tutorial window or text prompt.
/// </summary>
/// <param name="aType:">
/// EPopupType::Damage assumes a SDamagePopupData to be set in someData.
/// EPopupType::Info assumes a filepath string to be set in someData.
/// EPopupType::Tutorial assumes a text string to be set in someData.
/// Is connected to a certain data structure to be set in someData.
/// </param>
/// <param name="someData"></param>
void CPopupTextService::SpawnPopup(EPopupType aType, void* someData)
{
	switch (aType)
	{
	case EPopupType::Damage:
		SpawnDamageNumber(someData);
		break;
	default:
		break;
	}
}

void CPopupTextService::SpawnPopup(EPopupType aType, std::string aNameOrText)
{
	switch (aType)
	{
	case EPopupType::Info:
		SpawnInfoBox(aNameOrText);
		break;
	case EPopupType::Tutorial:
		SpawnTutorialText(aNameOrText);
		break;
	case EPopupType::Warning:
		SpawnWarningText(aNameOrText);
		break;
	default:
		break;
	}
}

void CPopupTextService::EmplaceTexts(std::vector<CTextInstance*>& someTexts)
{
	UpdateResources();
	
	for (unsigned int i = 0; i < myActiveDamageNumbers.size(); ++i)
	{
		someTexts.emplace_back(myActiveDamageNumbers[i]);
	}

	if (myActiveTutorialText) {
		someTexts.emplace_back(myActiveTutorialText);
	}

	if (myActiveWarningText) {
		someTexts.emplace_back(myActiveWarningText);
	}

	if (myActiveInfoBoxText) {
		someTexts.emplace_back(myActiveInfoBoxText);
	}
}

void CPopupTextService::EmplaceSprites(std::vector<CSpriteInstance*>& someSprites) const
{
	if (myActiveInfoBoxText) {
		someSprites.emplace_back(myInfoBoxBackground);
		someSprites.emplace_back(myActiveSkillSprite);
	}
}

CPopupTextService::CPopupTextService()
{
	myActiveTutorialText = nullptr;
	myTutorialText = nullptr;
	myTutorialAnimationData = nullptr;

	myActiveWarningText = nullptr;
	myWarningText = nullptr;
	myWarningAnimationData = nullptr;

	myActiveSkillSprite = nullptr;
	myInfoBoxBackground = nullptr;
	myActiveInfoBoxText = nullptr;
	myInfoBoxText = nullptr;
	myInfoAnimationData = nullptr;
}

CPopupTextService::~CPopupTextService()
{
	delete myActiveTutorialText;
	myActiveTutorialText = nullptr;
	delete myTutorialText;
	myTutorialText = nullptr;
	delete myTutorialAnimationData;
	myTutorialAnimationData = nullptr;

	delete myActiveWarningText;
	myActiveWarningText = nullptr;
	delete myWarningText;
	myWarningText = nullptr;
	delete myWarningAnimationData;
	myWarningAnimationData = nullptr;

	delete myActiveSkillSprite;
	myActiveSkillSprite = nullptr;
	delete myInfoBoxBackground;
	myInfoBoxBackground = nullptr;
	delete myActiveInfoBoxText;
	myActiveInfoBoxText = nullptr;
	delete myInfoBoxText;
	myInfoBoxText = nullptr;
	delete myInfoAnimationData;
	myInfoAnimationData = nullptr;

	myActiveDamageNumbers.clear();
	myDamageAnimationData.clear();
	myActiveSprites.clear();

	std::queue<CTextInstance*>().swap(myTextPool);
	std::queue<STextAnimationData*>().swap(myAnimatedDataPool);

	mySkillIcons.clear();
	myStoredSkillInfoStrings.clear();
}

void CPopupTextService::SpawnDamageNumber(void* someData)
{
	if (myTextPool.empty()) return;

	float damage = 0.0f;
	int hitType = 0;
	DirectX::SimpleMath::Vector3 worldPos;
	std::string text = "";

	int directionPicker = 0;
	DirectX::SimpleMath::Vector2 direction = { 0.0f, 0.0f };
	DirectX::SimpleMath::Vector4 color;

	SDamagePopupData data = *static_cast<SDamagePopupData*>(someData);
	damage = data.myDamage;
	text = std::to_string(damage);
	text = text.substr(0, text.find_first_of("."));
	worldPos = data.myGameObject->myTransform->Position();
	myActiveGameObject[text] = data.myGameObject;


	myActiveDamageNumbers.emplace_back(myTextPool.front());
	myTextPool.pop();

	myDamageAnimationData.emplace_back(myAnimatedDataPool.front());
	myAnimatedDataPool.pop();

	DirectX::SimpleMath::Vector2 screen = CUtility::WorldToScreen(worldPos);

	myActiveDamageNumbers.back()->SetPivot({ 0.5f, 0.5f });
	myActiveDamageNumbers.back()->SetGameObjectPosition({ screen.x, screen.y });
	//Needs to be -1.f, -1.f!
	myActiveDamageNumbers.back()->SetPosition({ -1.f,-1.f });
	myActiveDamageNumbers.back()->SetText(text);

	hitType = data.myHitType;
	switch (hitType)
	{
	case 0: // Normal
		myDamageAnimationData.back()->myMinScale = { 0.5f, 0.5f };
		myDamageAnimationData.back()->myMaxScale = { 1.5f, 1.5f };
		break;
	case 1: // Crit
		myDamageAnimationData.back()->myMinScale = { 1.5f, 1.5f };
		myDamageAnimationData.back()->myMaxScale = { 2.0f, 2.0f };
		CEngine::GetInstance()->GetActiveScene().GetMainCamera()->SetTrauma(0.45f);
		break;
	case 2: // Ultracrit
		myDamageAnimationData.back()->myMinScale = { 1.0f, 1.0f };
		myDamageAnimationData.back()->myMaxScale = { 2.5f, 2.5f };
		CEngine::GetInstance()->GetActiveScene().GetMainCamera()->SetTrauma(0.65f);
		break;
	case 3: // Enemy
		myDamageAnimationData.back()->myMinScale = { 0.5f, 0.5f };
		myDamageAnimationData.back()->myMaxScale = { 1.0f, 1.0f };
		break;
	case 4: // Healing
		myDamageAnimationData.back()->myMinScale = { 0.5f, 0.5f };
		myDamageAnimationData.back()->myMaxScale = { 1.0f, 1.0f };
		break;
	default:
		break;
	}

	color = myDamageColors[hitType];

	myDamageAnimationData.back()->myStartColor = color;
	myDamageAnimationData.back()->myEndColor = { color.x, color.y, color.z, 0.0f };

	directionPicker = Random(0, 3);
	direction = { 0.0f, 0.0f };
	switch (directionPicker)
	{
	case 0:
		direction = { 0.25f, -0.5f };
		break;
	case 1:
		direction = { 0.1f, -0.75f };
		break;
	case 2:
		direction = { -0.1f, -0.75f };
		break;
	case 3:
		direction = { -0.25f, -0.5f };
		break;
	default:
		break;
	}
	myDamageAnimationData.back()->myStartSpeed = direction;

	myDamageAnimationData.back()->myLifespan = 1.0f;
	myDamageAnimationData.back()->myTimer = 0.0f;
}

void CPopupTextService::SpawnInfoBox(std::string someInfoIdentifier)
{
	unsigned int skillPicker = 0;
	if (someInfoIdentifier == "Skill 1")
	{
		skillPicker = 0;
	}
	else if (someInfoIdentifier == "Skill 2") 
	{
		skillPicker = 1;
	}
	else if (someInfoIdentifier == "Skill 3") 
	{
		skillPicker = 2;
	}

	myActiveSkillSprite = mySkillIcons[skillPicker];
	myInfoBoxText->SetPivot({ 0.0f, 0.5f });
	myInfoBoxText->SetPosition({ -0.205f, -0.5f });
	myInfoBoxText->SetScale({ 1.0f, 1.0f });
	myInfoBoxText->SetText(myStoredSkillInfoStrings[skillPicker]);

	myInfoAnimationData->myStartColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	myInfoAnimationData->myEndColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	myInfoAnimationData->myLifespan = 6.0f;
	myInfoAnimationData->myFadeOutThreshold = 4.5f;
	myInfoAnimationData->myTimer = 0.0f;

	myActiveInfoBoxText = myInfoBoxText;
}

void CPopupTextService::SpawnTutorialText(std::string aText)
{
	myTutorialText->SetPivot({ 0.5f, 0.5f });
	myTutorialText->SetPosition({ 0.0f, 0.65f });
	myTutorialText->SetText(aText);
	myTutorialAnimationData->myStartColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	myTutorialAnimationData->myEndColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	myTutorialAnimationData->myMinScale = { 1.0f, 1.0f };
	myTutorialAnimationData->myMaxScale = { 1.1f, 1.1f };
	myTutorialAnimationData->myLifespan = 3.0f;
	myTutorialAnimationData->myFadeOutThreshold = 2.0f;
	myTutorialAnimationData->myTimer = 0.0f;
	myActiveTutorialText = myTutorialText;
}

void CPopupTextService::SpawnWarningText(std::string aText)
{
	myWarningText->SetPivot({ 0.5f, 0.5f });
	myWarningText->SetPosition({ 0.0f, -0.75f });
	myWarningText->SetScale({ 1.0f, 1.0f });
	myWarningText->SetText(aText);
	myWarningAnimationData->myStartColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	myWarningAnimationData->myEndColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	myWarningAnimationData->myMinScale = { 1.0f, 1.0f };
	myWarningAnimationData->myMaxScale = { 1.1f, 1.1f };
	myWarningAnimationData->myLifespan = 3.0f;
	myWarningAnimationData->myFadeOutThreshold = 2.0f;
	myWarningAnimationData->myTimer = 0.0f;
	myActiveWarningText = myWarningText;
}

void CPopupTextService::UpdateResources()
{
	std::vector<int> indicesOfTextsToRemove;

	float quotient = 0.0f;
	for (unsigned int i = 0; i < myActiveDamageNumbers.size(); ++i)
	{
		auto& text = myActiveDamageNumbers[i];
		auto& animationData = myDamageAnimationData[i];

		animationData->myTimer += CTimer::Dt();
		if (animationData->myTimer > animationData->myLifespan)
		{
			indicesOfTextsToRemove.push_back(i);
		}

		//Text space position
		DirectX::SimpleMath::Vector2 newPos = text->GetPosition();
		newPos.x /= CEngine::GetInstance()->GetWindowHandler()->GetResolution().x;
		newPos.y /= CEngine::GetInstance()->GetWindowHandler()->GetResolution().y;
		newPos.x -= 0.5f;
		newPos.y -= 0.5f;
		newPos *= 2.0f;

		//Attached Gameobject space position
		DirectX::SimpleMath::Vector3 worldPos = myActiveGameObject[text->GetText()]->myTransform->Position();
		DirectX::SimpleMath::Vector2 screen = CUtility::WorldToScreen(worldPos);

		quotient = animationData->myTimer / animationData->myLifespan;
		animationData->mySpeed = DirectX::SimpleMath::Vector2::Lerp(animationData->myStartSpeed, { 0.0f, 1.0f }, quotient);
		
		text->SetScale(DirectX::SimpleMath::Vector2::Lerp(animationData->myMinScale, animationData->myMaxScale, DamageSizeCurve(quotient)));
		text->SetColor(DirectX::SimpleMath::Vector4::Lerp(animationData->myStartColor, animationData->myEndColor, quotient));

		newPos += animationData->mySpeed * CTimer::Dt();
		
		//offset for text to be over attached Gameobject
		screen.y -= 0.35f;

		text->SetPosition(newPos);
		text->SetGameObjectPosition(screen);
	}

	std::sort(indicesOfTextsToRemove.begin(), indicesOfTextsToRemove.end(), [](UINT a, UINT b) { return a > b; });
	for (UINT i = 0; i < indicesOfTextsToRemove.size(); ++i) {
		std::swap(myActiveDamageNumbers[indicesOfTextsToRemove[i]], myActiveDamageNumbers.back());
		std::swap(myDamageAnimationData[indicesOfTextsToRemove[i]], myDamageAnimationData.back());
		myTextPool.push(myActiveDamageNumbers.back());
		myAnimatedDataPool.push(myDamageAnimationData.back());
		myActiveDamageNumbers.pop_back();
		myDamageAnimationData.pop_back();
	}

	float fadeOutQuotient = 0.0f;
	if (myActiveTutorialText) {
		myTutorialAnimationData->myTimer += CTimer::Dt();
		
		if (myTutorialAnimationData->myTimer > myTutorialAnimationData->myFadeOutThreshold)
		{
			fadeOutQuotient = (myTutorialAnimationData->myTimer - myTutorialAnimationData->myFadeOutThreshold)
				/ (myTutorialAnimationData->myLifespan - myTutorialAnimationData->myFadeOutThreshold);
		}
		
		quotient = myTutorialAnimationData->myTimer / myTutorialAnimationData->myLifespan;
		
		myActiveTutorialText->SetScale(DirectX::SimpleMath::Vector2::Lerp(myTutorialAnimationData->myMinScale, myTutorialAnimationData->myMaxScale, abs(sin(6.0f * 3.14159265f * quotient))));
		myActiveTutorialText->SetColor(DirectX::SimpleMath::Vector4::Lerp(myTutorialAnimationData->myStartColor, myTutorialAnimationData->myEndColor, fadeOutQuotient));
	
		if (myTutorialAnimationData->myTimer > myTutorialAnimationData->myLifespan)
		{
			myActiveTutorialText = nullptr;
		}
	}

	fadeOutQuotient = 0.0f;
	if (myActiveWarningText) {
		myWarningAnimationData->myTimer += CTimer::Dt();

		if (myWarningAnimationData->myTimer > myWarningAnimationData->myFadeOutThreshold)
		{
			fadeOutQuotient = (myWarningAnimationData->myTimer - myWarningAnimationData->myFadeOutThreshold)
				/ (myWarningAnimationData->myLifespan - myWarningAnimationData->myFadeOutThreshold);
		}

		myActiveWarningText->SetColor(DirectX::SimpleMath::Vector4::Lerp(myWarningAnimationData->myStartColor, myWarningAnimationData->myEndColor, fadeOutQuotient));

		if (myWarningAnimationData->myTimer > myWarningAnimationData->myLifespan)
		{
			myActiveWarningText = nullptr;
		}
	}

	fadeOutQuotient = 0.0f;
	if (myActiveInfoBoxText) {
		myInfoAnimationData->myTimer += CTimer::Dt();

		if (myInfoAnimationData->myTimer > myInfoAnimationData->myFadeOutThreshold)
		{
			fadeOutQuotient = (myInfoAnimationData->myTimer - myInfoAnimationData->myFadeOutThreshold)
				/ (myInfoAnimationData->myLifespan - myInfoAnimationData->myFadeOutThreshold);
		}

		myActiveInfoBoxText->SetColor(DirectX::SimpleMath::Vector4::Lerp(myInfoAnimationData->myStartColor, myInfoAnimationData->myEndColor, fadeOutQuotient));
		myInfoBoxBackground->SetColor(DirectX::SimpleMath::Vector4::Lerp(myInfoAnimationData->myStartColor, myInfoAnimationData->myEndColor, fadeOutQuotient));
		myActiveSkillSprite->SetColor(DirectX::SimpleMath::Vector4::Lerp(myInfoAnimationData->myStartColor, myInfoAnimationData->myEndColor, fadeOutQuotient));
		
		if (myInfoAnimationData->myTimer > myInfoAnimationData->myLifespan)
		{
			myActiveInfoBoxText = nullptr;
		}
	}

}

float CPopupTextService::DamageSizeCurve(float x)
{
	//return 0.5f + 2.5f * x - 3.0f * (x * x);
	return 0.6f + 4.25f * x - 10.5f * (x * x) + 5.91f * (x * x * x);
}
