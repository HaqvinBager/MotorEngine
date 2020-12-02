#include "stdafx.h"
#include "FireConeBehavior.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Timer.h"
#include "MouseTracker.h"
#include "Engine.h"

CFireConeBehavior::CFireConeBehavior(float aDuration)
{
	myDirection = { 0.0f, 0.0f, 0.0f };
	myDuration = aDuration;
	myTimer = 0.0f;
	myCaster = nullptr;
}

CFireConeBehavior::~CFireConeBehavior()
{
	myCaster = nullptr;
}

void CFireConeBehavior::Init(CGameObject* aCaster)
{
	myCaster = aCaster;

	myDirection = MouseTracker::ScreenPositionToWorldPosition() - aCaster->GetComponent<CTransformComponent>()->Position();
	myDirection.Normalize();
}

void CFireConeBehavior::Update(CGameObject* aParent)
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
