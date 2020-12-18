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
#include "StatsComponent.h"

CMeleeAttackBehavior::CMeleeAttackBehavior(float aDuration, float aDamage, CGameObject* aParent)
{
	myDirection = {0.0f, 0.0f, 0.0f};
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
	myCaster = aCaster;
	myDuration = aCaster->GetComponent<CAnimationComponent>()->GetCurrentAnimationPercent();
	std::cout << __FUNCTION__ << " Duration: " << myDuration << std::endl;
	myTimeToActivateCollider = 0.5f;
	std::cout << __FUNCTION__ << " Time to activate collider: " << myTimeToActivateCollider << std::endl;
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
		if (myCaster->GetComponent<CStatsComponent>()->GetStats().myHealth <= 0.0f) {
			aParent->Active(false);
		}

		CTriangleColliderComponent* triangleCollider = myParent->GetComponent<CTriangleColliderComponent>();

		if (triangleCollider->Enabled()) {
			DirectX::SimpleMath::Vector3 vector = myCaster->GetComponent<CTransformComponent>()->Position() + myCaster->GetComponent<CTransformComponent>()->Transform().Forward() * triangleCollider->GetHeight() * 100.0f;
			triangleCollider->SetPosition(myCaster->GetComponent<CTransformComponent>()->Position());
			DirectX::SimpleMath::Vector3 aLeftVertex = vector - myCaster->myTransform->Transform().Right() * (triangleCollider->GetWidth() / 2.0f) * 100.0f;
			DirectX::SimpleMath::Vector3 aRightVertex = vector + myCaster->myTransform->Transform().Right() * (triangleCollider->GetWidth() / 2.0f) * 100.0f;
			triangleCollider->SetLeftVertex(aLeftVertex);
			triangleCollider->SetRightVertex(aRightVertex);
		}

		myTimer += CTimer::Dt();

		if (!triangleCollider->Enabled()) {
			if (myCaster->GetComponent<CAnimationComponent>()->GetCurrentAnimationPercent() > myTimeToActivateCollider) {
				triangleCollider->Enabled(true);
			}
		}

		if (myCaster->GetComponent<CAnimationComponent>()->GetCurrentAnimationPercent() >= 0.95f)
		{
			//myTimer = 0.0f;
			aParent->Active(false);
		}

		aParent->GetComponent<CTransformComponent>()->Position(myCaster->myTransform->Position());
	}
}
