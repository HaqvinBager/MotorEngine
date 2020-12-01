#include "stdafx.h"
#include "SpeedExplodeBehavior.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Timer.h"
#include "MouseTracker.h"
#include "Engine.h"
#include "CircleColliderComponent.h"

CSpeedExplodeBehavior::CSpeedExplodeBehavior(float aDuration, float aExpodeTime, CGameObject* anAbilityObject)
{
	myDirection = { 0.0f, 0.0f, 0.0f };
	myDuration = aDuration;
	myTimer = 0.0f;
	myExplodesAfter = aExpodeTime;
	myAbilityObject = anAbilityObject;
	myCaster = nullptr;
}

CSpeedExplodeBehavior::~CSpeedExplodeBehavior()
{
	myCaster = nullptr;
}

void CSpeedExplodeBehavior::Init(CGameObject* aGameObject)
{
	myCaster = aGameObject;
	
	myAbilityObject->GetComponent<CCircleColliderComponent>()->Enabled(false); //TODO: getting a collider like this is not good as it limits it to one circle collider ability

	myDirection = MouseTracker::ScreenPositionToWorldPosition() - aGameObject->GetComponent<CTransformComponent>()->Position();
	myDirection.Normalize();
}

#include <iostream>
void CSpeedExplodeBehavior::Update(CGameObject* aParent)
{
	if (myCaster)
	{
		myTimer += CTimer::Dt();
		//TODO: speed up player

		if (myTimer > myExplodesAfter)
		{
			//TODO: explodess
			aParent->GetComponent<CCircleColliderComponent>()->Enabled(true);
			std::cout << "exploooode\n";
		}

		if (myTimer > myDuration)
		{
			myTimer = 0.0f;
			aParent->Active(false);
		}

		aParent->GetComponent<CTransformComponent>()->Position(myCaster->myTransform->Position());
	}
}
