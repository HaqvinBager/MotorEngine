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
#include "PlayerControllerComponent.h"
#include "AnimationComponent.h"

CMeleeAttackBehavior::CMeleeAttackBehavior(float aDuration, float aDamage, CGameObject* aParent)
{
	myDirection = { 0.0f, 0.0f, 0.0f };
	myDuration = aDuration;
	myTimer = 0.0f;
	myParent = aParent;
	myDamageMultiplier = aDamage;
}

CMeleeAttackBehavior::~CMeleeAttackBehavior()
{
	myCaster = nullptr;
}

void CMeleeAttackBehavior::Init(CGameObject* aCaster)
{
	myTimeToActivateCollider = aCaster->GetComponent<CAnimationComponent>()->ReturnToIdleTimer() / 2.0f;
	CTriangleColliderComponent* triangleCollider = myParent->GetComponent<CTriangleColliderComponent>();
	DirectX::SimpleMath::Vector3 vector = myCaster->GetComponent<CTransformComponent>()->Position() + myCaster->GetComponent<CTransformComponent>()->Transform().Forward() * triangleCollider->GetHeight() * 100.0f;
	triangleCollider->SetPosition(myCaster->GetComponent<CTransformComponent>()->Position());
	DirectX::SimpleMath::Vector3 aLeftVertex = vector - myCaster->myTransform->Transform().Right() * (triangleCollider->GetWidth() / 2.0f) * 100.0f;
	DirectX::SimpleMath::Vector3 aRightVertex = vector + myCaster->myTransform->Transform().Right() * (triangleCollider->GetWidth() / 2.0f) * 100.0f;
	triangleCollider->SetLeftVertex(aLeftVertex);
	triangleCollider->SetRightVertex(aRightVertex);
	triangleCollider->Enabled(false);

	myDirection = MouseTracker::ScreenPositionToWorldPosition() - aCaster->GetComponent<CTransformComponent>()->Position();
	myDirection.Normalize();

	CPlayerControllerComponent* playerController = myCaster->GetComponent<CPlayerControllerComponent>();
	if (playerController) {
		myCaster->myTransform->Rotation({0, DirectX::XMConvertToDegrees(atan2f(myDirection.x, myDirection.z)) + 180.f, 0});
	}
}

void CMeleeAttackBehavior::Update(CGameObject* aParent)
{
	if (myCaster)
	{
		CTriangleColliderComponent* triangleCollider = myParent->GetComponent<CTriangleColliderComponent>();
		if (myTimeToActivateCollider >= 0.0f) {
			myTimeToActivateCollider -= CTimer::Dt();
		}
		if (myTimeToActivateCollider <= 0.0f) {
			triangleCollider->Enabled(true);
		}

		if (triangleCollider->Enabled()) {
			DirectX::SimpleMath::Vector3 vector = myCaster->GetComponent<CTransformComponent>()->Position() + myCaster->GetComponent<CTransformComponent>()->Transform().Forward() * triangleCollider->GetHeight() * 100.0f;
			triangleCollider->SetPosition(myCaster->GetComponent<CTransformComponent>()->Position());
			DirectX::SimpleMath::Vector3 aLeftVertex = vector - myCaster->myTransform->Transform().Right() * (triangleCollider->GetWidth() / 2.0f) * 100.0f;
			DirectX::SimpleMath::Vector3 aRightVertex = vector + myCaster->myTransform->Transform().Right() * (triangleCollider->GetWidth() / 2.0f) * 100.0f;
			triangleCollider->SetLeftVertex(aLeftVertex);
			triangleCollider->SetRightVertex(aRightVertex);
		}

		myTimer += CTimer::Dt();
		if (myTimer > myDuration)
		{
			myTimer = 0.0f;
			aParent->Active(false);
		}

		aParent->GetComponent<CTransformComponent>()->Position(myCaster->myTransform->Position());
	}
}
