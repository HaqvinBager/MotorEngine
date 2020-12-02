#include "stdafx.h"
#include "DelayedExplosionBehavior.h"
#include "GameObject.h"
#include "Timer.h"
#include "TransformComponent.h"
#include "Engine.h"
#include "CircleColliderComponent.h"

CDelayedExplosionBehavior::CDelayedExplosionBehavior(float aDuration, float aDelay, CGameObject* aParent):
	myDelay(aDelay),
	myParent(aParent),
	myCaster(nullptr)
{
	myDuration = 0.0f;
	myTimer = 0.0f;
}

CDelayedExplosionBehavior::~CDelayedExplosionBehavior()
{
	myCaster = nullptr;
	myParent = nullptr;
}

void CDelayedExplosionBehavior::Update(CGameObject* aParent)
{
	if (myCaster)
	{
		myTimer += CTimer::Dt();

		if (myTimer > myDelay)
		{
			aParent->GetComponent<CCircleColliderComponent>()->Enabled(true);
		}

		if (myTimer > myDuration)
		{
			myTimer = 0.0f;
			aParent->Active(false);
		}

		aParent->GetComponent<CTransformComponent>()->Position(myCaster->myTransform->Position());
	}
}

void CDelayedExplosionBehavior::Init(CGameObject* aCaster)
{
	myCaster = aCaster;
	myParent->GetComponent<CCircleColliderComponent>()->Enabled(false);
}
