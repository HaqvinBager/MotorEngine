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

CEnemyBehavior::CEnemyBehavior(CGameObject* aPlayerObject)
	: myPlayer(aPlayerObject)
{}

CEnemyBehavior::~CEnemyBehavior()
{}

void CEnemyBehavior::Update(CGameObject* aParent)
{
	myCurrentParent = aParent;

	if (myCurrentParent->GetComponent<CAnimationComponent>())
	{
		myCurrentParent->GetComponent<CAnimationComponent>()->PlayAnimation(EEnemyAnimationID::Idle, true);
	}

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

void CEnemyBehavior::Collided(CGameObject* /*aGameObject*/)
{
	std::cout << __FUNCTION__ << " The enemy says: OUCH! " << std::endl;
}

void CEnemyBehavior::FindATarget()
{
	DirectX::SimpleMath::Vector3 parentPos = myCurrentParent->GetComponent<CTransformComponent>()->Position();
	DirectX::SimpleMath::Vector3 targetPos = myPlayer->GetComponent<CTransformComponent>()->Position();
	SBaseStats baseStats = myCurrentParent->GetComponent<CStatsComponent>()->GetBaseStats();
	SStats stats = myCurrentParent->GetComponent<CStatsComponent>()->GetStats();

	float dist = DirectX::SimpleMath::Vector3::DistanceSquared(parentPos, targetPos);
	if (dist <= baseStats.myBaseVisionRange) {
		DirectX::SimpleMath::Vector3 dir = targetPos - parentPos;
		dir.Normalize();

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

void CEnemyBehavior::TakeDamage(float /*someDamage*/)
{
	SBaseStats baseStats = myCurrentParent->GetComponent<CStatsComponent>()->GetBaseStats();
	SStats stats = myCurrentParent->GetComponent<CStatsComponent>()->GetStats();
	//TODO: decrease stats.myHealth
}
