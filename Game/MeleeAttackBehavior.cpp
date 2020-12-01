#include "stdafx.h"
#include "MeleeAttackBehavior.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Timer.h"
#include "MouseTracker.h"
#include "Engine.h"

CMeleeAttackBehavior::CMeleeAttackBehavior(float aDuration)
{
	myDirection = { 0.0f, 0.0f, 0.0f };
	myDuration = aDuration;
	myTimer = 0.0f;
	myCaster = nullptr;
}

CMeleeAttackBehavior::~CMeleeAttackBehavior()
{
	myCaster = nullptr;
}

void CMeleeAttackBehavior::Init(CGameObject* aGameObject)
{
	myCaster = aGameObject;

	myDirection = MouseTracker::ScreenPositionToWorldPosition() - aGameObject->GetComponent<CTransformComponent>()->Position();
	myDirection.Normalize();
}

void CMeleeAttackBehavior::Update(CGameObject* aParent)
{
	if (myCaster)
	{
		myTimer += CTimer::Dt();
		if (myTimer > myDuration)
		{
			myTimer = 0.0f;
			aParent->Active(false);
		}

		aParent->GetComponent<CTransformComponent>()->Position(myCaster->myTransform->Position());
	}
}
