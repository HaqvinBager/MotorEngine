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

CBoomerangBehavior::CBoomerangBehavior(float aSpeed, float aResourceCost)
{
	myDirection = {0.0f, 0.0f, 0.0f};
	mySpeed = aSpeed;
	myTimer = 0.0f;
	myCaster = nullptr;
	myIsReturning = false;
	myResourceCost = aResourceCost;
}

CBoomerangBehavior::~CBoomerangBehavior()
{
	//myCaster = nullptr;
}

void CBoomerangBehavior::Update(CGameObject* aParent)
{
	if (myCaster != nullptr) {
		if (!myIsReturning)
		{
			myIsReturning = CheckDistance(aParent->GetComponent<CTransformComponent>()->Position(), myTargetPosition);
			aParent->GetComponent<CTransformComponent>()->Move(myDirection * mySpeed * CTimer::Dt());
		} else if (!CheckDistance(aParent->GetComponent<CTransformComponent>()->Position(), myCaster->GetComponent<CTransformComponent>()->Position()))
		{
			CalculateDirection(aParent->GetComponent<CTransformComponent>()->Position(), myCaster->GetComponent<CTransformComponent>()->Position());
			aParent->GetComponent<CTransformComponent>()->Move(-myDirection * mySpeed * CTimer::Dt());
		} else
		{
			aParent->Active(false);
		}
	}

}

bool CBoomerangBehavior::CheckDistance(DirectX::SimpleMath::Vector3 aFirstPosition, DirectX::SimpleMath::Vector3 aSecondPosition)
{
	int x1 = static_cast<int>(aFirstPosition.x);
	int x2 = static_cast<int>(aSecondPosition.x);

	if (x1 == x2) {
		int z1 = static_cast<int>(aFirstPosition.z);
		int z2 = static_cast<int>(aSecondPosition.z);
		if (z1 == z2) {
			return true;
		}
	}

	return false;
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
