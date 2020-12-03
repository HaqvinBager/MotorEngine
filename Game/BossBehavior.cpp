#include "stdafx.h"
#include "BossBehavior.h"
#include "GameObject.h"
#include "AIBehaviorComponent.h"
#include "TransformComponent.h"
#include "StatsComponent.h"
#include "Timer.h"
#include "Engine.h"
#include "Scene.h"
#include "MainSingleton.h"
#include "NavMeshComponent.h"
#include "AnimationComponent.h"
#include "AbilityComponent.h"
#include "NavMeshComponent.h"

CBossBehavior::CBossBehavior(CGameObject* aPlayerObject)
	: myPlayer(aPlayerObject)
{
}

CBossBehavior::~CBossBehavior()
{
}

void CBossBehavior::Update(CGameObject* aParent)
{
	if (aParent->GetComponent<CAnimationComponent>())
	{
		aParent->GetComponent<CAnimationComponent>()->PlayAnimation(EBossAnimationID::Idle, true);
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
		CMainSingleton::PostMaster().Send({ EMessageType::BossDied, this });
	}

	FindATarget(*aParent);
}

void CBossBehavior::Collided(CGameObject* /*aGameObject*/)
{
}

void CBossBehavior::FindATarget(CGameObject& aParent)
{
	DirectX::SimpleMath::Vector3 parentPos = aParent.GetComponent<CTransformComponent>()->Position();
	DirectX::SimpleMath::Vector3 targetPos = myPlayer->GetComponent<CTransformComponent>()->Position();
	SBaseStats baseStats = aParent.GetComponent<CStatsComponent>()->GetBaseStats();
	SStats stats = aParent.GetComponent<CStatsComponent>()->GetStats();

	float dist = DirectX::SimpleMath::Vector3::DistanceSquared(parentPos, targetPos);
	if (dist <= baseStats.myBaseVisionRange) {

		aParent.GetComponent<CNavMeshComponent>()->CalculatePath(targetPos);
		if (dist <= baseStats.myBaseAttackRange) {
			myPlayer->GetComponent<CStatsComponent>();
			if (aParent.GetComponent<CAnimationComponent>())
			{
				aParent.GetComponent<CAnimationComponent>()->PlayAnimation(EEnemyAnimationID::Attack);
			}
			aParent.GetComponent<CAbilityComponent>()->UseAbility(EAbilityType::BossAbility1, aParent.myTransform->Position());
		}
	}
}

void CBossBehavior::TakeDamage(float /*aDamage*/)
{
	//TODO: decrease stats.myHealth
}
