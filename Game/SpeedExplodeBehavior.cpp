#include "stdafx.h"
#include "SpeedExplodeBehavior.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Timer.h"
#include "MouseTracker.h"
#include "Engine.h"
#include "CircleColliderComponent.h"

CSpeedExplodeBehavior::CSpeedExplodeBehavior(float aDuration, float aExplosionDelay, float aMovementSpeedMultiplier, CGameObject* aParent)
{
	myDuration = aDuration;
	myTimer = 0.0f;
	myExplosionDelay = aExplosionDelay;
	myMovementSpeedMutiplier = aMovementSpeedMultiplier;
	myParent = aParent;
	myCaster = nullptr;
}

CSpeedExplodeBehavior::~CSpeedExplodeBehavior()
{
	myParent = nullptr;
	myCaster = nullptr;
}

void CSpeedExplodeBehavior::Init(CGameObject* aGameObject)
{
	myCaster = aGameObject;
	myParent->GetComponent<CCircleColliderComponent>()->Enabled(false); //TODO: getting a collider like this is not good as it limits it to one circle collider ability 
																		//Is probably fine for this ability, since you probably only want 1 explosion and speed up at a time
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
			//TODO: explodes and does damage
			myParent->GetComponent<CCircleColliderComponent>()->Enabled(true);
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
