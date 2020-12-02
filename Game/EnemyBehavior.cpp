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

CEnemyBehavior::CEnemyBehavior(CGameObject* aPlayerObject)
	: myPlayer(aPlayerObject)
{}

CEnemyBehavior::~CEnemyBehavior()
{}

void CEnemyBehavior::Update(CGameObject* aParent)
{
	if (aParent->GetComponent<CAnimationComponent>())
	{
		aParent->GetComponent<CAnimationComponent>()->PlayAnimation(EEnemyAnimationID::Idle, true);
	}

	//enemy logic
	SBaseStats baseStats = aParent->GetComponent<CStatsComponent>()->GetBaseStats();
	SStats stats = aParent->GetComponent<CStatsComponent>()->GetStats();

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

	FindATarget(*aParent);
}

void CEnemyBehavior::Collided(CGameObject* /*aGameObject*/)
{
	std::cout << __FUNCTION__ << " The enemy says: OUCH! " << std::endl;
}

void CEnemyBehavior::FindATarget(CGameObject& aParent)
{
	DirectX::SimpleMath::Vector3 parentPos = aParent.GetComponent<CTransformComponent>()->Position();
	DirectX::SimpleMath::Vector3 targetPos = myPlayer->GetComponent<CTransformComponent>()->Position();
	SBaseStats baseStats = aParent.GetComponent<CStatsComponent>()->GetBaseStats();
	SStats stats = aParent.GetComponent<CStatsComponent>()->GetStats();

	float dist = DirectX::SimpleMath::Vector3::DistanceSquared(parentPos, targetPos);
	if (dist <= baseStats.myBaseVisionRange) {
			// NON NAVMESH MOVEMENT
		DirectX::SimpleMath::Vector3 dir = targetPos - parentPos;
		dir.Normalize();
		aParent.GetComponent<CTransformComponent>()->Move(dir * baseStats.myMoveSpeed * CTimer::Dt());
			// NON NAVMESH MOVEMENT
		if (dist <= baseStats.myBaseAttackRange) {
			if (stats.myTokenSlot == nullptr) {
				stats.myTokenSlot = CTokenPool::GetInstance()->Request();
			}
			myPlayer->GetComponent<CStatsComponent>();
			if (aParent.GetComponent<CAnimationComponent>())
			{
				aParent.GetComponent<CAnimationComponent>()->PlayAnimation(EEnemyAnimationID::Attack);
			}
			aParent.GetComponent<CAbilityComponent>()->UseAbility(EAbilityType::EnemyAbility, aParent.myTransform->Position());
		}
		else {
			if (stats.myTokenSlot != nullptr) {
				CTokenPool::GetInstance()->GiveBack(*stats.myTokenSlot, false);
				stats.myTokenSlot = nullptr;
			}
		}
		// FOR NAVMESH
		//aParent.GetComponent<CNavMeshComponent>()->CalculatePath(targetPos);
	}
}

void CEnemyBehavior::TakeDamage(float /*aDamage*/)
{
	//TODO: decrease stats.myHealth
}
