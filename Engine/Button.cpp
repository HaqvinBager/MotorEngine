#include "Button.h"
#include "stdafx.h"
#include "MainSingleton.h"
#include "SpriteFactory.h"
#include "SpriteInstance.h"
#include "Sprite.h"
#include "Engine.h"
#include "WindowHandler.h"
#include "Scene.h"

void CButton::OnHover()
{
	myState = EButtonState::Hover;
	mySprites.at(static_cast<size_t>(EButtonState::Idle))->SetShouldRender(false);
	mySprites.at(static_cast<size_t>(EButtonState::Hover))->SetShouldRender(true);
}

void CButton::OnClickDown()
{	
	myState = EButtonState::Click;
	mySprites.at(static_cast<size_t>(EButtonState::Hover))->SetShouldRender(false);
	mySprites.at(static_cast<size_t>(EButtonState::Click))->SetShouldRender(true);
}

void CButton::OnClickUp(void* someData)
{
	myState = EButtonState::Hover;
	mySprites.at(static_cast<size_t>(EButtonState::Click))->SetShouldRender(false);
	mySprites.at(static_cast<size_t>(EButtonState::Hover))->SetShouldRender(true);

	for (unsigned int i = 0; i < myMessagesToSend.size(); ++i)
	{
		CMainSingleton::PostMaster().Send({ myMessagesToSend[i],  someData });
	}
}

void CButton::OnLeave()
{
	myState = EButtonState::Idle;
	mySprites.at(static_cast<size_t>(EButtonState::Click))->SetShouldRender(false);
	mySprites.at(static_cast<size_t>(EButtonState::Hover))->SetShouldRender(false);
	mySprites.at(static_cast<size_t>(EButtonState::Idle))->SetShouldRender(true);
}

void CButton::Click(bool anIsPressed, void* someData)
{
	switch (myState)
	{
	case EButtonState::Hover:
		if (anIsPressed && myIsMouseHover) {
			OnClickDown();
		}
		break;
	case EButtonState::Click:
		if (!anIsPressed && myIsMouseHover) {
			OnClickUp(someData);
		}
		else if (!myIsMouseHover) {
			OnLeave();
		}
		break;
	default:
		break;
	}
}

void CButton::CheckMouseCollision(DirectX::SimpleMath::Vector2 aScreenSpacePosition)
{
	if 
		(!(
			myRect.myBottom < aScreenSpacePosition.y ||
			aScreenSpacePosition.y < myRect.myTop    ||
			myRect.myRight < aScreenSpacePosition.x  ||
			aScreenSpacePosition.x < myRect.myLeft
		)) 
	{
		myIsMouseHover = true;

		switch (myState)
		{
		case EButtonState::Idle:
			OnHover();
			break;
		default:
			break;
		}
	}
	else {
		myIsMouseHover = false;

		switch (myState)
		{
		case EButtonState::Hover:
			OnLeave();
			break;
		case EButtonState::Click:
			OnLeave();
			break;
		default:
			break;
		}
	}
}

CButton::CButton(SButtonData& someData)
	: myMessagesToSend(someData.myMessagesToSend)
	, myState(EButtonState::Idle)
	, myEnabled(true)
	, myIsMouseHover(false)
{
	//Load Sprites
	for (unsigned int i = 0; i < 3; ++i) 
	{
		mySprites.at(i) = new CSpriteInstance();
		mySprites.at(i)->Init(CSpriteFactory::GetInstance()->GetSprite(someData.mySpritePaths.at(i)));
		mySprites.at(i)->SetRenderOrder(ERenderOrder::ForegroundLayer);
		CEngine::GetInstance()->GetActiveScene().AddInstance(mySprites.at(i));
	}

	mySprites.at(static_cast<size_t>(EButtonState::Hover))->SetShouldRender(false);
	mySprites.at(static_cast<size_t>(EButtonState::Click))->SetShouldRender(false);


	float windowWidth = CEngine::GetInstance()->GetWindowHandler()->GetResolution().x;
	float windowHeight = CEngine::GetInstance()->GetWindowHandler()->GetResolution().y;
	
	DirectX::SimpleMath::Vector2 normalizedPosition = someData.myPosition;
	normalizedPosition /= 2.0f;
	normalizedPosition += { 0.5f, 0.5f };

	DirectX::SimpleMath::Vector2 spriteDimensions = someData.myDimensions;
	spriteDimensions /= 2.0f;
	myRect.myTop = normalizedPosition.y * windowHeight - spriteDimensions.y;
	myRect.myBottom = normalizedPosition.y * windowHeight + spriteDimensions.y;
	myRect.myLeft = normalizedPosition.x * windowWidth - spriteDimensions.x;
	myRect.myRight = normalizedPosition.x * windowWidth + spriteDimensions.x;

	for (unsigned int i = 0; i < 3; ++i)
	{
		mySprites.at(i)->SetPosition({ someData.myPosition.x, someData.myPosition.y });
	}
}

CButton::~CButton()
{
	delete mySprites.at(static_cast<size_t>(EButtonState::Idle));
	delete mySprites.at(static_cast<size_t>(EButtonState::Hover));
	delete mySprites.at(static_cast<size_t>(EButtonState::Click));
}
