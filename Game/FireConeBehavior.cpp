#include "stdafx.h"
#include "FireConeBehavior.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Timer.h"
#include "MouseTracker.h"
#include "Engine.h"
#include "StatsComponent.h"
#include "MainSingleton.h"

CFireConeBehavior::CFireConeBehavior(float aDuration, float aResourceCost)
{
	myDirection = { 0.0f, 0.0f, 0.0f };
	myDuration = aDuration;
	myResourceCost = aResourceCost;
	myTimer = 0.0f;
	myCaster = nullptr;
}

CFireConeBehavior::~CFireConeBehavior()
{
	myCaster = nullptr;
}

void CFireConeBehavior::Init(CGameObject* aCaster)
{
	if (aCaster->GetComponent<CStatsComponent>()->GetStats().myResource > myResourceCost)
	{
		myCaster = aCaster;
		myCaster->GetComponent<CStatsComponent>()->GetStats().myResource -= myResourceCost;

		myDirection = MouseTracker::ScreenPositionToWorldPosition() - aCaster->GetComponent<CTransformComponent>()->Position();
		myDirection.Normalize();

		float difference = myCaster->GetComponent<CStatsComponent>()->GetBaseStats().myBaseResource - myCaster->GetComponent<CStatsComponent>()->GetStats().myResource;
		difference = (100.0f - difference) / 100.0f;

		SMessage message;
		message.myMessageType = EMessageType::PlayerResourceChanged;
		message.data = &difference;
		CMainSingleton::PostMaster().Send(message);
	}
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
	else
	{
		aParent->Active(false);
	}
}
