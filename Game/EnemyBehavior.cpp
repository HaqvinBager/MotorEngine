#include "stdafx.h"
#include "EnemyBehavior.h"
#include "GameObject.h"
#include "AIBehaviorComponent.h"
#include "TransformComponent.h"
#include "StatsComponent.h"
#include "Timer.h"
#include "ObjectPool.h"
#include "TokenPool.h"
#include "Engine.h"
#include "Scene.h"
#include "MainSingleton.h"
#include "NavMeshComponent.h"
#include "AnimationComponent.h"
#include "AbilityComponent.h"
#include "NavMeshComponent.h"
#include "PlayerControllerComponent.h"
#include "Collider.h"
#include "RectangleColliderComponent.h"
#include "TriangleColliderComponent.h"
#include "CircleColliderComponent.h"
#include "AIBehaviorComponent.h"
#include "AbilityBehavior.h"

CEnemyBehavior::CEnemyBehavior(CGameObject* aPlayerObject)
	: myPlayer(aPlayerObject)
{}

CEnemyBehavior::~CEnemyBehavior()
{}

void CEnemyBehavior::Update(CGameObject* aParent)
{
	myCurrentParent = aParent;

	//enemy logic
	SBaseStats baseStats = myCurrentParent->GetComponent<CStatsComponent>()->GetBaseStats();
	SStats stats = myCurrentParent->GetComponent<CStatsComponent>()->GetStats();

	if (stats.myCanTakeDamage == false) {
		stats.myDamageCooldown -= CTimer::Dt();
		if (stats.myDamageCooldown <= 0) {
			stats.myCanTakeDamage = true;
			stats.myDamageCooldown = baseStats.myBaseDamageCooldown;
		}
	}

	if (stats.myHealth <= 0) {
		CMainSingleton::PostMaster().Send({ EMessageType::EnemyDied, this });
		if (stats.myTokenSlot != nullptr) {
			CTokenPool::GetInstance()->GiveBack(*stats.myTokenSlot, false);
			stats.myTokenSlot = nullptr;
		}
		//aParent->Active(false);
	}

	FindATarget();
}

// Sending in the parent feels safer than relying on myCurrentParent
void CEnemyBehavior::Collided(CGameObject* aParent, CGameObject* aCollidedWithGameObject)
{
	CCollider* collider = reinterpret_cast<CCollider*>(aCollidedWithGameObject->GetComponent<CTriangleColliderComponent>());
	if (!collider) 
	{
			collider = reinterpret_cast<CCollider*>(aCollidedWithGameObject->GetComponent<CCircleColliderComponent>());
		if (!collider)
		{
			collider = reinterpret_cast<CCollider*>(aCollidedWithGameObject->GetComponent<CRectangleColliderComponent>());
		}
	}

	if (!collider) { return; }

	if (collider->GetCollisionLayer() == ECollisionLayer::PLAYERABILITY1)
	{
		std::cout << __FUNCTION__ << " Enemy collided with player ability " << std::endl;
		//CStatsComponent* playerStats = myPlayer->GetComponent<CStatsComponent>();
		myCurrentParent = aParent;
		
		//TakeDamage(aCollidedWithGameObject->GetComponent<>);
		return;
	}

	CAIBehaviorComponent* enemyBehavior = aCollidedWithGameObject->GetComponent<CAIBehaviorComponent>();
	if (enemyBehavior)
	{
		std::cout << __FUNCTION__ << " Enemy collided with Enemy " << std::endl;
	}
}

void CEnemyBehavior::FindATarget()
{
	DirectX::SimpleMath::Vector3 parentPos = myCurrentParent->GetComponent<CTransformComponent>()->Position();
	DirectX::SimpleMath::Vector3 targetPos = myPlayer->GetComponent<CTransformComponent>()->Position();
	SBaseStats baseStats = myCurrentParent->GetComponent<CStatsComponent>()->GetBaseStats();
	SStats& stats = myCurrentParent->GetComponent<CStatsComponent>()->GetStats();

	float dist = DirectX::SimpleMath::Vector3::DistanceSquared(parentPos, targetPos);
	if (dist <= baseStats.myBaseVisionRange) {

		//NavMesh movement
		myCurrentParent->GetComponent<CNavMeshComponent>()->CalculatePath(targetPos);
		if (dist <= baseStats.myBaseAttackRange) {
			if (stats.myTokenSlot == nullptr) {
				stats.myTokenSlot = CTokenPool::GetInstance()->Request();
			}
			myPlayer->GetComponent<CStatsComponent>();
			if (myCurrentParent->GetComponent<CAnimationComponent>())
			{
				myCurrentParent->GetComponent<CAnimationComponent>()->PlayAnimation(EEnemyAnimationID::Attack);
			}
			myCurrentParent->GetComponent<CAbilityComponent>()->UseAbility(EAbilityType::EnemyAbility, myCurrentParent->myTransform->Position());
		}
		else {
			if (stats.myTokenSlot != nullptr) {
				CTokenPool::GetInstance()->GiveBack(*stats.myTokenSlot, false);
				stats.myTokenSlot = nullptr;
			}
		}
	}
}

void CEnemyBehavior::TakeDamage(float someDamage)
{
	SStats& stats = myCurrentParent->GetComponent<CStatsComponent>()->GetStats();

	stats.myHealth -= someDamage;
	CMainSingleton::PostMaster().Send({ EMessageType::EnemyHealthChanged, &stats.myHealth });
	
	if (stats.myHealth < 0)
	{
		Die();
	}
}

void CEnemyBehavior::Die()
{
	std::cout << __FUNCTION__ << " Enemy collided with player ability " << std::endl;
	myCurrentParent->GetComponent<CAIBehaviorComponent>()->Enabled(false);
	myCurrentParent->GetComponent<CAbilityComponent>()->Enabled(false);
	myCurrentParent->GetComponent<CCircleColliderComponent>()->Enabled(false);
	myCurrentParent->GetComponent<CAnimationComponent>()->DeadState();
	CMainSingleton::PostMaster().Send({ EMessageType::EnemyDied, 0 });
}
