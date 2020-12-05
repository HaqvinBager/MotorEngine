#include "stdafx.h"
#include "PopupTextService.h"
#include "TextFactory.h"
#include "TextInstance.h"
#include "SpriteFactory.h"
#include "SpriteInstance.h"
#include "Engine.h"
#include "WindowHandler.h"
#include "RandomNumberGenerator.h"
#include "../Game/DamageUtility.h"
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
	float damage = 0.0f;
	int hitType = 0;
	std::string aFilePath = "";
	std::string text = "";

	int directionPicker = 0;
	DirectX::SimpleMath::Vector2 direction = { 0.0f, 0.0f };
	DirectX::SimpleMath::Vector4 color;

	switch (aType)
	{
	case EPopupType::Damage:

		if (myTextPool.empty()) break;

		SDamagePopupData data = *static_cast<SDamagePopupData*>(someData);
		damage = data.myDamage;
		text = std::to_string(damage);
		text = text.substr(0, text.find_first_of("."));
		
		myActiveTexts.emplace_back(myTextPool.front());
		myTextPool.pop();

		myTextAnimationData.emplace_back(myAnimatedDataPool.front());
		myAnimatedDataPool.pop();

		myActiveTexts.back()->SetPivot({ 0.5f, 0.5f });
		myActiveTexts.back()->SetPosition({ 0.0f, 0.0f });
		myActiveTexts.back()->SetText(text);

		hitType = data.myHitType;
		switch (hitType)
		{
		case 0: // Normal
			color = { 1.0f, 1.0f, 1.0f, 1.0f };
			myTextAnimationData.back()->myMinScale = { 0.5f, 0.5f };
			myTextAnimationData.back()->myMaxScale = { 1.5f, 1.5f };
			break;
		case 1: // Crit
			color = { 255.0f / 255.0f, 203.0f / 255.0f, 5.0f / 255.0f, 1.0f };
			myTextAnimationData.back()->myMinScale = { 1.5f, 1.5f };
			myTextAnimationData.back()->myMaxScale = { 2.0f, 2.0f };
			break;
		case 2: // Ultracrit
			color = { 232.0f / 255.0f, 126.0f / 255.0f, 4.0f / 255.0f, 1.0f };
			myTextAnimationData.back()->myMinScale = { 1.0f, 1.0f };
			myTextAnimationData.back()->myMaxScale = { 2.5f, 2.5f };
			break;
		case 3: // Enemy
			color = { 1.0f, 0.0f, 0.0f, 1.0f };
			myTextAnimationData.back()->myMinScale = { 0.5f, 0.5f };
			myTextAnimationData.back()->myMaxScale = { 1.0f, 1.0f };
			break;
		default:
			break;
		}

		myTextAnimationData.back()->myStartColor = color;
		myTextAnimationData.back()->myEndColor = { color.x, color.y, color.z, 0.0f };

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
		myTextAnimationData.back()->myStartSpeed = direction;
		
		myTextAnimationData.back()->myLifespan = 1.0f;
		myTextAnimationData.back()->myTimer = 0.0f;
		break;
	case EPopupType::Info:
		break;
	case EPopupType::Tutorial:
		break;
	default:
		break;
	}
}

const std::vector<CTextInstance*>& CPopupTextService::GetTexts()
{
	UpdateTexts();
	return myActiveTexts;
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

void CPopupTextService::UpdateTexts()
{
	std::vector<int> indicesOfTextsToRemove;

	float quotient = 0.0f;
	for (unsigned int i = 0; i < myActiveTexts.size(); ++i)
	{
		auto& text = myActiveTexts[i];
		auto& animationData = myTextAnimationData[i];

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
		
		text->SetScale(DirectX::SimpleMath::Vector2::Lerp(animationData->myMinScale, animationData->myMaxScale, SizeCurve(quotient)));
		text->SetColor(DirectX::SimpleMath::Vector4::Lerp(animationData->myStartColor, animationData->myEndColor, quotient));

		newPos += animationData->mySpeed * CTimer::Dt();
		text->SetPosition(newPos);
	}

	std::sort(indicesOfTextsToRemove.begin(), indicesOfTextsToRemove.end(), [](UINT a, UINT b) { return a > b; });
	for (UINT i = 0; i < indicesOfTextsToRemove.size(); ++i) {
		std::swap(myActiveTexts[indicesOfTextsToRemove[i]], myActiveTexts.back());
		std::swap(myTextAnimationData[indicesOfTextsToRemove[i]], myTextAnimationData.back());
		myTextPool.push(myActiveTexts.back());
		myAnimatedDataPool.push(myTextAnimationData.back());
		myActiveTexts.pop_back();
		myTextAnimationData.pop_back();
	}
}

void CPopupTextService::UpdateSprites()
{

}

float CPopupTextService::SizeCurve(float x)
{
	//return 0.5f + 2.5f * x - 3.0f * (x * x);
	return 0.6f + 4.25f * x - 10.5f * (x * x) + 5.91f * (x * x * x);
}
