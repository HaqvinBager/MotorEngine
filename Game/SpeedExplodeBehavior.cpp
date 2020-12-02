#include "stdafx.h"
#include "SpeedExplodeBehavior.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Timer.h"
#include "MouseTracker.h"
#include "Engine.h"
#include "CircleColliderComponent.h"
#include "VFXComponent.h"

CSpeedExplodeBehavior::CSpeedExplodeBehavior(float aDuration, float aExplosionDelay, float aMovementSpeedMultiplier, CGameObject* aParent)
{
	myDuration = aDuration;
	myTimer = 0.0f;
	myExplosionDelay = aExplosionDelay;
	myMovementSpeedMutiplier = aMovementSpeedMultiplier;
	myMultipliedSpeed = 0.0f;
	myOriginalMovementSpeed = 0.0f;
	myParent = aParent;
	myCasterTransform = nullptr;
	myCaster = nullptr;
}

CSpeedExplodeBehavior::~CSpeedExplodeBehavior()
{
	myParent = nullptr;
	myCaster = nullptr;
}

void CSpeedExplodeBehavior::Init(CGameObject* aCaster)
{
	myCaster = aCaster;
	myParent->GetComponent<CCircleColliderComponent>()->Enabled(false);
	myParent->GetComponent<CVFXComponent>();
	myCasterTransform = myCaster->GetComponent<CTransformComponent>();
	myOriginalMovementSpeed = myCasterTransform->MovementSpeed();
	myMultipliedSpeed = myOriginalMovementSpeed * myMovementSpeedMutiplier;
	myCasterTransform->MovementSpeed(myMultipliedSpeed);
}

void CSpeedExplodeBehavior::Update(CGameObject* aParent)
{
	if (myCaster)
	{
		myTimer += CTimer::Dt();

		if (myTimer > myExplosionDelay)
		{
			myParent->GetComponent<CCircleColliderComponent>()->Enabled(true);
			//TODO: play audio
		}

		if (myTimer > myDuration)
		{
			myCasterTransform->MovementSpeed(myOriginalMovementSpeed);
			myTimer = 0.0f;
			aParent->Active(false);
		}

		aParent->GetComponent<CTransformComponent>()->Position(myCaster->myTransform->Position());
	}
}
