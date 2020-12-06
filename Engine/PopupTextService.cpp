#include "stdafx.h"
#include "PopupTextService.h"
#include "TextFactory.h"
#include "TextInstance.h"
#include "SpriteFactory.h"
#include "SpriteInstance.h"
#include "Engine.h"
#include "WindowHandler.h"
#include "RandomNumberGenerator.h"
#include <algorithm>

bool CPopupTextService::Init()
{
	const unsigned int textPoolSize = 5;
	for (unsigned int i = 0; i < textPoolSize; ++i)
	{
		myTextPool.push(new CTextInstance(false));
		myTextPool.back()->Init(CTextFactory::GetInstance()->GetText("Text/baskerville16"));
		myAnimatedDataPool.push(new STextAnimationData());
	}

	myDamageColors[0] = { 1.0f, 1.0f, 1.0f, 1.0f };									// Normal
	myDamageColors[1] = { 255.0f / 255.0f, 203.0f / 255.0f, 5.0f / 255.0f, 1.0f };	// Crit
	myDamageColors[2] = { 232.0f / 255.0f, 126.0f / 255.0f, 4.0f / 255.0f, 1.0f };	// Ultracrit
	myDamageColors[3] = { 1.0f, 0.0f, 0.0f, 1.0f };									// Enemy
	myDamageColors[4] = { 0.0f, 230.0f / 255.0f, 64.0f / 255.0f, 1.0f };			// Healing

	myTutorialText = new CTextInstance(false);
	myTutorialText->Init(CTextFactory::GetInstance()->GetText("Text/baskerville16"));
	myTutorialAnimationData = new STextAnimationData();

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
		break;
	case EPopupType::Tutorial:
		SpawnTutorialText(aNameOrText);
		break;
	default:
		break;
	}
}

const std::vector<CTextInstance*> CPopupTextService::GetTexts()
{
	UpdateTexts();
	
	std::vector<CTextInstance*> activeTexts = myActiveDamageNumbers;
	if (myActiveTutorialText) {
		activeTexts.emplace_back(myActiveTutorialText);
	}

	return activeTexts;
}

const std::vector<CSpriteInstance*>& CPopupTextService::GetSprites()
{
	UpdateSprites();
	return myActiveSprites;
}

CPopupTextService::CPopupTextService()
{
}

CPopupTextService::~CPopupTextService()
{
}

void CPopupTextService::SpawnDamageNumber(void* someData)
{
	if (myTextPool.empty()) return;

	float damage = 0.0f;
	int hitType = 0;
	std::string text = "";

	int directionPicker = 0;
	DirectX::SimpleMath::Vector2 direction = { 0.0f, 0.0f };
	DirectX::SimpleMath::Vector4 color;

	SDamagePopupData data = *static_cast<SDamagePopupData*>(someData);
	damage = data.myDamage;
	text = std::to_string(damage);
	text = text.substr(0, text.find_first_of("."));

	myActiveDamageNumbers.emplace_back(myTextPool.front());
	myTextPool.pop();

	myDamageAnimationData.emplace_back(myAnimatedDataPool.front());
	myAnimatedDataPool.pop();

	myActiveDamageNumbers.back()->SetPivot({ 0.5f, 0.5f });
	myActiveDamageNumbers.back()->SetPosition({ 0.0f, 0.0f });
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
		break;
	case 2: // Ultracrit
		myDamageAnimationData.back()->myMinScale = { 1.0f, 1.0f };
		myDamageAnimationData.back()->myMaxScale = { 2.5f, 2.5f };
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

void CPopupTextService::SpawnInfoBox(void* /*someData*/)
{
}

void CPopupTextService::SpawnTutorialText(std::string aText)
{
	myTutorialText->SetPivot({ 0.5f, 0.5f });
	myTutorialText->SetPosition({ 0.0f, 0.0f });
	myTutorialText->SetText(aText);
	myTutorialAnimationData->myStartColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	myTutorialAnimationData->myStartColor = { 1.0f, 1.0f, 1.0f, 0.0f };
	myTutorialAnimationData->myMinScale = { 1.0f, 1.0f };
	myTutorialAnimationData->myMaxScale = { 1.1f, 1.1f };
	myTutorialAnimationData->myLifespan = 3.0f;
	myTutorialAnimationData->myTimer = 0.0f;
	myActiveTutorialText = myTutorialText;
}

void CPopupTextService::UpdateTexts()
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

		auto newPos = text->GetPosition();
		newPos.x /= CEngine::GetInstance()->GetWindowHandler()->GetResolution().x;
		newPos.y /= CEngine::GetInstance()->GetWindowHandler()->GetResolution().y;
		newPos.x -= 0.5f;
		newPos.y -= 0.5f;
		newPos *= 2.0f;

		quotient = animationData->myTimer / animationData->myLifespan;
		animationData->mySpeed = DirectX::SimpleMath::Vector2::Lerp(animationData->myStartSpeed, { 0.0f, 1.0f }, quotient);
		
		text->SetScale(DirectX::SimpleMath::Vector2::Lerp(animationData->myMinScale, animationData->myMaxScale, DamageSizeCurve(quotient)));
		text->SetColor(DirectX::SimpleMath::Vector4::Lerp(animationData->myStartColor, animationData->myEndColor, quotient));

		newPos += animationData->mySpeed * CTimer::Dt();
		text->SetPosition(newPos);
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

	if (!myActiveTutorialText) return;

	myTutorialAnimationData->myTimer += CTimer::Dt();
	quotient = myTutorialAnimationData->myTimer / myTutorialAnimationData->myLifespan;
	myActiveTutorialText->SetScale(DirectX::SimpleMath::Vector2::Lerp(myTutorialAnimationData->myMinScale, myTutorialAnimationData->myMaxScale, abs(sin(6.0f * 3.14159265f * quotient))));
	myActiveTutorialText->SetColor(DirectX::SimpleMath::Vector4::Lerp(myTutorialAnimationData->myStartColor, myTutorialAnimationData->myEndColor, quotient));
	
	if (myTutorialAnimationData->myTimer > myTutorialAnimationData->myLifespan)
	{
		myActiveTutorialText = nullptr;
	}
}

void CPopupTextService::UpdateSprites()
{

}

float CPopupTextService::DamageSizeCurve(float x)
{
	//return 0.5f + 2.5f * x - 3.0f * (x * x);
	return 0.6f + 4.25f * x - 10.5f * (x * x) + 5.91f * (x * x * x);
}
