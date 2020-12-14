#include "stdafx.h"
#include "SpeedExplodeBehavior.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Timer.h"
#include "MouseTracker.h"
#include "Engine.h"
#include "CircleColliderComponent.h"
#include "VFXComponent.h"
#include "StatsComponent.h"
#include "MainSingleton.h"
#include "AIBehavior.h"
#include "AIBehaviorComponent.h"
#include "PlayerControllerComponent.h"

CSpeedExplodeBehavior::CSpeedExplodeBehavior(float aDuration, float aResourceCost, float aExplosionDelay, float aMovementSpeedMultiplier, float aDamage, CGameObject* aParent)
{
	myDuration = aDuration;
	myTimer = 0.0f;
	myResourceCost = aResourceCost;
	myExplosionDelay = aExplosionDelay;
	myMovementSpeedMutiplier = aMovementSpeedMultiplier;
	myMultipliedSpeed = 0.0f;
	myOriginalMovementSpeed = 0.0f;
	myParent = aParent;
	myCasterTransform = nullptr;
	myDamageMultiplier = aDamage;
}

CSpeedExplodeBehavior::~CSpeedExplodeBehavior()
{
	myParent = nullptr;
	myCaster = nullptr;
}

void CSpeedExplodeBehavior::Init(CGameObject* aCaster)
{
	myParent->GetComponent<CCircleColliderComponent>()->Enabled(false);

	if (aCaster->GetComponent<CStatsComponent>()->GetStats().myResource > myResourceCost)
	{
		myCaster = aCaster;
		myCaster->GetComponent<CStatsComponent>()->GetStats().myResource -= myResourceCost;

		myParent->GetComponent<CVFXComponent>();
		myCasterTransform = myCaster->GetComponent<CTransformComponent>();
		myOriginalMovementSpeed = myCasterTransform->MovementSpeed();
		myMultipliedSpeed = myOriginalMovementSpeed * myMovementSpeedMutiplier;
		myCasterTransform->MovementSpeed(myMultipliedSpeed);

		float difference = myCaster->GetComponent<CStatsComponent>()->GetBaseStats().myBaseResource - myCaster->GetComponent<CStatsComponent>()->GetStats().myResource;
		difference = (100.0f - difference) / 100.0f;

		SMessage message;
		message.myMessageType = EMessageType::PlayerResourceChanged;
		message.data = &difference;
		CMainSingleton::PostMaster().Send(message);
	}
}

void CSpeedExplodeBehavior::Update(CGameObject* aParent)
{
	if (myCaster)
	{
		myTimer += CTimer::Dt();

		if (myTimer > myExplosionDelay)
		{
			myParent->GetComponent<CCircleColliderComponent>();
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
	else
	{
		aParent->Active(false);
	}
}

float CSpeedExplodeBehavior::ResourceCost() const
{
	return myResourceCost;
}
