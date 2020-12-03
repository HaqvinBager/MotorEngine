#include "stdafx.h"
#include "BoomerangBehavior.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Timer.h"
#include "MouseTracker.h"
#include "Engine.h"
#include "MouseTracker.h"
#include "StatsComponent.h"
#include "PostMaster.h"
#include "MainSingleton.h"

namespace SM = DirectX::SimpleMath;

CBoomerangBehavior::CBoomerangBehavior(float aSpeed, float aDuration, float aResourceCost)
{
	myDirection = {0.0f, 0.0f, 0.0f};
	mySpeed = aSpeed;
	myTimer = 0.0f;
	myCaster = nullptr;
	myIsReturning = false;
	myDuration = aDuration;
	myResourceCost = aResourceCost;
	myHalfLife = myDuration / 2.0f;
}

CBoomerangBehavior::~CBoomerangBehavior()
{
	myCaster = nullptr;
}

void CBoomerangBehavior::Update(CGameObject* aParent)
{
	if (myCaster != nullptr) {
		myTimer += CTimer::Dt();

		if (myTimer < myHalfLife) {
			aParent->GetComponent<CTransformComponent>()->Move(myDirection * mySpeed * CTimer::Dt());

		} else if (myTimer < myDuration) {
			CalculateDirection(myCaster->GetComponent<CTransformComponent>()->Position(), aParent->GetComponent<CTransformComponent>()->Position());
			aParent->GetComponent<CTransformComponent>()->Move(myDirection * mySpeed * CTimer::Dt());
		}
		else 
		{
			myTimer = 0.0f;
			aParent->Active(false);
		}
	} else {
		aParent->Active(false);
	}

}

void CBoomerangBehavior::CalculateDirection(DirectX::SimpleMath::Vector3 aFirstPosition, DirectX::SimpleMath::Vector3 aSecondPosition)
{
	myDirection = aFirstPosition - aSecondPosition;
	myDirection.Normalize();
}

void CBoomerangBehavior::Init(CGameObject* aCaster)
{
	if (aCaster->GetComponent<CStatsComponent>()->GetStats().myResource > myResourceCost) {
		myCaster = aCaster;

		myTargetPosition = MouseTracker::ScreenPositionToWorldPosition();
		CalculateDirection(MouseTracker::ScreenPositionToWorldPosition(), myCaster->GetComponent<CTransformComponent>()->Position());
		myIsReturning = false;

		myCaster->GetComponent<CStatsComponent>()->GetStats().myResource -= myResourceCost;

		float difference = myCaster->GetComponent<CStatsComponent>()->GetBaseStats().myBaseResource - myCaster->GetComponent<CStatsComponent>()->GetStats().myResource;
		difference = (100.0f - difference) / 100.0f;

		SMessage message;
		message.myMessageType = EMessageType::PlayerResourceChanged;
		message.data = &difference;
		CMainSingleton::PostMaster().Send(message);
	} else {
		myCaster = nullptr;
	}
}
