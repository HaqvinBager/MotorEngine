#include "stdafx.h"
#include "FireConeBehavior.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Timer.h"
#include "MouseTracker.h"
#include "Engine.h"
#include "StatsComponent.h"
#include "MainSingleton.h"
#include "TriangleColliderComponent.h"
#include "AIBehavior.h"
#include "AIBehaviorComponent.h"
#include "PlayerControllerComponent.h"

CFireConeBehavior::CFireConeBehavior(float aDuration, float aResourceCost, float aDamage, CGameObject* aParent)
{
	myDirection = { 0.0f, 0.0f, 0.0f };
	myDuration = aDuration;
	myResourceCost = aResourceCost;
	myTimer = 0.0f;
	myParent = aParent;
	myDamageMultiplier = aDamage;
}

CFireConeBehavior::~CFireConeBehavior()
{
	myCaster = nullptr;
}

#include <iostream>
void CFireConeBehavior::Init(CGameObject* aCaster)
{
	if (aCaster->GetComponent<CStatsComponent>()->GetStats().myResource > myResourceCost)
	{
		myCaster = aCaster;
		myCaster->GetComponent<CStatsComponent>()->GetStats().myResource -= myResourceCost;

		CTriangleColliderComponent* triangleCollider = myParent->GetComponent<CTriangleColliderComponent>();
		DirectX::SimpleMath::Vector3 vector = myCaster->GetComponent<CTransformComponent>()->Position() + myCaster->GetComponent<CTransformComponent>()->Transform().Forward() * triangleCollider->GetHeight() * 100.0f;
		triangleCollider->SetPosition(myCaster->GetComponent<CTransformComponent>()->Position());
		DirectX::SimpleMath::Vector3 aLeftVertex = vector - myCaster->myTransform->Transform().Right() * (triangleCollider->GetWidth() / 2.0f) * 100.0f;
		DirectX::SimpleMath::Vector3 aRightVertex = vector + myCaster->myTransform->Transform().Right() * (triangleCollider->GetWidth() / 2.0f) * 100.0f;
		triangleCollider->SetLeftVertex(aLeftVertex);
		triangleCollider->SetRightVertex(aRightVertex);

		myDirection = MouseTracker::ScreenPositionToWorldPosition() - aCaster->GetComponent<CTransformComponent>()->Position();
		myDirection.Normalize();

		CPlayerControllerComponent* playerController = aCaster->GetComponent<CPlayerControllerComponent>();
		if (playerController) {
			aCaster->myTransform->Rotation({0, DirectX::XMConvertToDegrees(atan2f(myDirection.x, myDirection.z)) + 180.f, 0});
		}

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
		CTriangleColliderComponent* triangleCollider = myParent->GetComponent<CTriangleColliderComponent>();
		DirectX::SimpleMath::Vector3 vector = myCaster->GetComponent<CTransformComponent>()->Position() + myCaster->GetComponent<CTransformComponent>()->Transform().Forward() * triangleCollider->GetHeight() * 100.0f;
		triangleCollider->SetPosition(myCaster->GetComponent<CTransformComponent>()->Position());
		DirectX::SimpleMath::Vector3 aLeftVertex = vector - myCaster->myTransform->Transform().Right() * (triangleCollider->GetWidth() / 2.0f) * 100.0f;
		DirectX::SimpleMath::Vector3 aRightVertex = vector + myCaster->myTransform->Transform().Right() * (triangleCollider->GetWidth() / 2.0f) * 100.0f;
		triangleCollider->SetLeftVertex(aLeftVertex);
		triangleCollider->SetRightVertex(aRightVertex);

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
