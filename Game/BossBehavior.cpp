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

#include <iostream>

CBossBehavior::CBossBehavior(CGameObject* aPlayerObject)
	: myPlayer(aPlayerObject)
{
	myPhase = Phase::Idle;
	myTempAttackTimer = 0.0f;
}

CBossBehavior::~CBossBehavior()
{
}

void CBossBehavior::Update(CGameObject* aParent)
{
	SBaseStats baseStats = aParent->GetComponent<CStatsComponent>()->GetBaseStats();
	SStats stats = aParent->GetComponent<CStatsComponent>()->GetStats();

	if (stats.myCanTakeDamage == false)
	{
		stats.myDamageCooldown -= CTimer::Dt();
		if (stats.myDamageCooldown <= 0)
		{
			stats.myCanTakeDamage = true;
			stats.myDamageCooldown = baseStats.myBaseDamageCooldown;
		}
	}

	if (stats.myHealth <= 0)
	{
		CMainSingleton::PostMaster().Send({ EMessageType::BossDied, this });
	}

	switch (myPhase)
	{
	case CBossBehavior::Phase::Idle:
		IdlePhase(aParent);
		break;
	case CBossBehavior::Phase::Start:
		StartPhase(aParent);
		break;
	case CBossBehavior::Phase::Mid:
		MidPhase(aParent);
		break;
	case CBossBehavior::Phase::Final:
		FinalPhase(aParent);
		break;
	default:
		break;
	}
}

void CBossBehavior::Collided(CGameObject* /*aParent*/, CGameObject* /*aCollidedWithGameObject*/)
{
}

bool CBossBehavior::FindATarget(CGameObject& aParent)
{
	DirectX::SimpleMath::Vector3 targetPos = myPlayer->GetComponent<CTransformComponent>()->Position();

	float dist = DirectX::SimpleMath::Vector3::DistanceSquared(aParent.GetComponent<CTransformComponent>()->Position(), targetPos);
	if (!(dist <= aParent.GetComponent<CStatsComponent>()->GetBaseStats().myBaseVisionRange))
		return false;

	aParent.GetComponent<CNavMeshComponent>()->CalculatePath(targetPos);
	return true;
}

void CBossBehavior::TakeDamage(float /*aDamage*/, CGameObject* /*aGameObject*/)
{
	//TODO: decrease stats.myHealth
}

void CBossBehavior::IdlePhase(CGameObject* aParent)
{
	if (FindATarget(*aParent))
		myPhase = Phase::Start;
}

void CBossBehavior::StartPhase(CGameObject* aParent)
{
	aParent->GetComponent<CStatsComponent>()->GetStats().myDamageCooldown += CTimer::Dt();

	aParent->GetComponent<CNavMeshComponent>()->CalculatePath(myPlayer->GetComponent<CTransformComponent>()->Position());
	float dist = DirectX::SimpleMath::Vector3::DistanceSquared(aParent->myTransform->Position(), myPlayer->GetComponent<CTransformComponent>()->Position());

	if (dist <= aParent->GetComponent<CStatsComponent>()->GetBaseStats().myBaseAttackRange)
	{
		if (aParent->GetComponent<CStatsComponent>()->GetStats().myDamageCooldown >= aParent->GetComponent<CStatsComponent>()->GetBaseStats().myBaseDamageCooldown)
		{
			if (aParent->GetComponent<CAnimationComponent>())
				aParent->GetComponent<CAnimationComponent>()->PlayAttack01ID();

			aParent->GetComponent<CAbilityComponent>()->UseAbility(EAbilityType::BossAbility1, aParent->myTransform->Position());
			aParent->GetComponent<CStatsComponent>()->GetStats().myDamageCooldown -= aParent->GetComponent<CStatsComponent>()->GetStats().myDamageCooldown;
		}
	}
}

void CBossBehavior::MidPhase(CGameObject* /*aParent*/)
{
}

void CBossBehavior::FinalPhase(CGameObject* /*aParent*/)
{
}
