#include "stdafx.h"
#include "BoomerangBehavior.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Timer.h"
#include "MouseTracker.h"
#include "Engine.h"
#include "MouseTracker.h"
namespace SM = DirectX::SimpleMath;

CBoomerangBehavior::CBoomerangBehavior(float aSpeed)
{
	myDirection = {0.0f, 0.0f, 0.0f};
	mySpeed = aSpeed;
	myTimer = 0.0f;
	myCaster = nullptr;
	myIsReturning = false;
	//std::cout << "X " << myTargetPosition.x << " Y " << myTargetPosition.y << " Z " << myTargetPosition.z << std::endl;
}

CBoomerangBehavior::~CBoomerangBehavior()
{
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
	int z1 = static_cast<int>(aFirstPosition.z);
	int x2 = static_cast<int>(aSecondPosition.x);
	int z2 = static_cast<int>(aSecondPosition.z);

	if (x1 == x2) {
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

void CBoomerangBehavior::Init(CGameObject* aGameObject)
{
	myCaster = aGameObject;
	myTargetPosition = MouseTracker::ScreenPositionToWorldPosition();
	CalculateDirection(MouseTracker::ScreenPositionToWorldPosition(), myCaster->GetComponent<CTransformComponent>()->Position());
	myIsReturning = false;
}
