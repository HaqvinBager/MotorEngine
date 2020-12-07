#include "stdafx.h"
#include "MeleeAttackBehavior.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Timer.h"
#include "MouseTracker.h"
#include "Engine.h"
#include "TriangleColliderComponent.h"
#include "AIBehavior.h"
#include "AIBehaviorComponent.h"

CMeleeAttackBehavior::CMeleeAttackBehavior(float aDuration, float aDamage, CGameObject* aParent)
{
	myDirection = { 0.0f, 0.0f, 0.0f };
	myDuration = aDuration;
	myTimer = 0.0f;
	myParent = aParent;
	myCaster = nullptr;
	myDamageMultiplier = aDamage;
}

CMeleeAttackBehavior::~CMeleeAttackBehavior()
{
	myCaster = nullptr;
}

void CMeleeAttackBehavior::Init(CGameObject* aCaster)
{
	myCaster = aCaster;

	CTriangleColliderComponent* triangleCollider = myParent->GetComponent<CTriangleColliderComponent>();
	DirectX::SimpleMath::Vector3 vector = myCaster->GetComponent<CTransformComponent>()->Position() + myCaster->GetComponent<CTransformComponent>()->Transform().Forward() * triangleCollider->GetHeight() * 100.0f;
	triangleCollider->SetPosition(myCaster->GetComponent<CTransformComponent>()->Position());
	DirectX::SimpleMath::Vector3 aLeftVertex = vector - myCaster->myTransform->Transform().Right() * (triangleCollider->GetWidth() / 2.0f) * 100.0f;
	DirectX::SimpleMath::Vector3 aRightVertex = vector + myCaster->myTransform->Transform().Right() * (triangleCollider->GetWidth() / 2.0f) * 100.0f;
	triangleCollider->SetLeftVertex(aLeftVertex);
	triangleCollider->SetRightVertex(aRightVertex);

	myDirection = MouseTracker::ScreenPositionToWorldPosition() - aCaster->GetComponent<CTransformComponent>()->Position();
	myDirection.Normalize();
}

void CMeleeAttackBehavior::Collided(CGameObject* /*aGameObject*/)
{
	//TODO add damage
}

void CMeleeAttackBehavior::Update(CGameObject* aParent)
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
}
