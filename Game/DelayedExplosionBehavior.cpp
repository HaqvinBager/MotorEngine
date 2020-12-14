#include "stdafx.h"
#include "DelayedExplosionBehavior.h"
#include "GameObject.h"
#include "Timer.h"
#include "TransformComponent.h"
#include "Engine.h"
#include "CircleColliderComponent.h"
#include "StatsComponent.h"
#include "PostMaster.h"
#include "MainSingleton.h"
#include "AIBehavior.h"
#include "AIBehaviorComponent.h"

CDelayedExplosionBehavior::CDelayedExplosionBehavior(float aDuration, float aDelay, float aResourceCost, float aDamage, CGameObject* aParent):
	myDelay(aDelay),
	myParent(aParent)
{
	myDuration = aDuration;
	myTimer = 0.0f;
	myResourceCost = aResourceCost;
	myDamageMultiplier = aDamage;
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
	} else {
		aParent->Active(false);
	}
}

void CDelayedExplosionBehavior::Init(CGameObject* aCaster)
{
	if (aCaster->GetComponent<CStatsComponent>()->GetStats().myResource > myResourceCost) {

		myParent->GetComponent<CCircleColliderComponent>()->Enabled(false);
		myCaster = aCaster;

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
