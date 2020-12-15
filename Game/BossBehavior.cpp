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
#include "StatsComponent.h"
#include "Canvas.h"
#include "AnimatedUIElement.h"

#include <iostream>
#include "EnemyBehavior.h"
#include "DamageUtility.h"
#include <PlayerControllerComponent.h>
#include "HealthBarComponent.h"
#include "RandomNumberGenerator.h"

CBossBehavior::CBossBehavior(CGameObject* aPlayerObject, Vector2 aPhaseOne, Vector2 aPhaseTwo, Vector2 aPhaseThree)
	: myPlayer(aPlayerObject)
{
	myPhasePercents.emplace_back(aPhaseOne);
	myPhasePercents.emplace_back(aPhaseTwo);
	myPhasePercents.emplace_back(aPhaseThree);
	myPhase = Phase::Idle;
	myTempAttackTimer = 0.0f;
}

CBossBehavior::~CBossBehavior()
{
}

void CBossBehavior::Update(CGameObject* aParent)
{
	//SBaseStats baseStats = aParent->GetComponent<CStatsComponent>()->GetBaseStats();
	SStats stats = aParent->GetComponent<CStatsComponent>()->GetStats();

	//if (stats.myCanTakeDamage == false)
	//{
	//	stats.myDamageCooldown -= CTimer::Dt();
	//	if (stats.myDamageCooldown <= 0)
	//	{
	//		stats.myCanTakeDamage = true;
	//		stats.myDamageCooldown = baseStats.myBaseDamageCooldown;
	//	}
	//}

	//myPhase = UpdatePhase();
	//StartPhase(aParent);
	float precentHealth = stats.myHealth / aParent->GetComponent<CStatsComponent>()->GetBaseStats().myBaseHealth;
	precentHealth *= 100.f;

	if (precentHealth >= myPhasePercents[0].x && precentHealth <= myPhasePercents[0].y) {
		myPhase = CBossBehavior::Phase::Start;
	} else if (precentHealth >= myPhasePercents[1].x && precentHealth <= myPhasePercents[1].y) {
		myPhase = CBossBehavior::Phase::Mid;
	} else if (precentHealth >= myPhasePercents[2].x && precentHealth <= myPhasePercents[2].y) {
		myPhase = CBossBehavior::Phase::Final;
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
	}

	//FindATarget(*aParent);

	if (stats.myHealth <= 0)
	{
		CMainSingleton::PostMaster().Send({ EMessageType::BossDied, this });
	}
}

void CBossBehavior::Collided(CGameObject* /*aParent*/, CGameObject* /*aCollidedWithGameObject*/)
{
}

bool CBossBehavior::FindATarget(CGameObject& aParent)
{
	DirectX::SimpleMath::Vector3 targetPos = myPlayer->GetComponent<CTransformComponent>()->Position();

	float dist = DirectX::SimpleMath::Vector3::DistanceSquared(aParent.GetComponent<CTransformComponent>()->Position(), targetPos);
	if (!(dist <= aParent.GetComponent<CStatsComponent>()->GetBaseStats().myBaseVisionRange)) {
		aParent.GetComponent<CTransformComponent>()->ClearPath();
		return false;
	}

	aParent.GetComponent<CNavMeshComponent>()->CalculatePath(targetPos);
	return true;
}

void CBossBehavior::TakeDamage(float aDamage, CGameObject* aGameObject)
{
	//TODO: decrease stats.myHealth
	
	SStats& stats = aGameObject->GetComponent<CStatsComponent>()->GetStats();
	EHitType hitType = EHitType::Normal;
	float damage = CDamageUtility::CalculateDamage(hitType, myPlayer->GetComponent<CStatsComponent>()->GetBaseStats().myDamage, aDamage, 0.25f, 1.5f);
	CStatsComponent* statsComponent = aGameObject->GetComponent<CStatsComponent>();
	//static int count = 0;
	if (statsComponent->AddDamage(damage))
	{
		//std::cout << "Boss Take Damage" << ++count << std::endl;
		SDamagePopupData data = { damage, static_cast<int>(hitType), aGameObject };
		CMainSingleton::PopupTextService().SpawnPopup(EPopupType::Damage, &data);
		float baseHealth = statsComponent->GetBaseStats().myBaseHealth;
		float difference = baseHealth - statsComponent->GetStats().myHealth;

		if (myPlayer->GetComponent<CPlayerControllerComponent>()->AuraActive())
		{
			if ((myPlayer->GetComponent<CStatsComponent>()->GetStats().myHealth + (difference / 2.5f))
				< myPlayer->GetComponent<CStatsComponent>()->GetBaseStats().myBaseHealth)
				myPlayer->GetComponent<CStatsComponent>()->GetStats().myHealth += difference / 2.5f;
			else
				myPlayer->GetComponent<CStatsComponent>()->GetStats().myHealth = myPlayer->GetComponent<CStatsComponent>()->GetBaseStats().myBaseHealth;
		}

		difference = (baseHealth - difference) / baseHealth;
		if (difference <= 0.0)
		{
			difference = 0.0f;
		}

		aGameObject->GetComponent<CHealthBarComponent>()->GetCanvas()->GetAnimatedUI()[0]->Level(difference);
		aGameObject->GetComponent<CHealthBarComponent>()->GetCanvas2()->GetAnimatedUI()[0]->Level(difference);
	}


	if (stats.myHealth <= 0)
	{
		Die();
	}
}

void CBossBehavior::IdlePhase(CGameObject* aParent)
{
	if (FindATarget(*aParent))
		myPhase = Phase::Start;
}

void CBossBehavior::StartPhase(CGameObject* aParent)
{
	//aParent->GetComponent<CNavMeshComponent>()->CalculatePath(myPlayer->GetComponent<CTransformComponent>()->Position());
	FindATarget(*aParent);
	float dist = DirectX::SimpleMath::Vector3::DistanceSquared(aParent->myTransform->Position(), myPlayer->GetComponent<CTransformComponent>()->Position());
	CStatsComponent* stats = aParent->GetComponent<CStatsComponent>();
	if (dist <= stats->GetBaseStats().myBaseAttackRange)
	{
		aParent->GetComponent<CTransformComponent>()->ClearPath();
		myTempAttackTimer += CTimer::Dt();

		float playerBaseDamageCooldown = myPlayer->GetComponent<CStatsComponent>()->GetBaseStats().myBaseDamageCooldown;
		if (myTempAttackTimer > playerBaseDamageCooldown)
		{
			myTempAttackTimer -= playerBaseDamageCooldown;
	
			if (aParent->GetComponent<CAnimationComponent>())
				aParent->GetComponent<CAnimationComponent>()->PlayAttack01ID();

			aParent->GetComponent<CAbilityComponent>()->UseAbility(EAbilityType::BossAbility1, aParent->myTransform->Position());
		}	
	}
}

void CBossBehavior::MidPhase(CGameObject* aParent)
{
	FindATarget(*aParent);
	float dist = DirectX::SimpleMath::Vector3::DistanceSquared(aParent->myTransform->Position(), myPlayer->GetComponent<CTransformComponent>()->Position());
	CStatsComponent* stats = aParent->GetComponent<CStatsComponent>();
	if (dist <= stats->GetBaseStats().myBaseAttackRange)
	{
		aParent->GetComponent<CTransformComponent>()->ClearPath();
		myTempAttackTimer += CTimer::Dt();

		float playerBaseDamageCooldown = myPlayer->GetComponent<CStatsComponent>()->GetBaseStats().myBaseDamageCooldown;
		if (myTempAttackTimer > playerBaseDamageCooldown)
		{
			myTempAttackTimer -= playerBaseDamageCooldown;

			if (aParent->GetComponent<CAnimationComponent>())
				aParent->GetComponent<CAnimationComponent>()->PlayAttack01ID();

			int attackType = Random(1, 2);
			if (attackType == 1) {
				aParent->GetComponent<CAbilityComponent>()->UseAbility(EAbilityType::BossAbility1, aParent->myTransform->Position());
			}else if (attackType == 2) {
				aParent->GetComponent<CAbilityComponent>()->UseAbility(EAbilityType::BossAbility2, aParent->myTransform->Position());
			}

		}
	}
}

void CBossBehavior::FinalPhase(CGameObject* aParent)
{
	FindATarget(*aParent);
	float dist = DirectX::SimpleMath::Vector3::DistanceSquared(aParent->myTransform->Position(), myPlayer->GetComponent<CTransformComponent>()->Position());
	CStatsComponent* stats = aParent->GetComponent<CStatsComponent>();
	if (dist <= stats->GetBaseStats().myBaseAttackRange)
	{
		aParent->GetComponent<CTransformComponent>()->ClearPath();
		myTempAttackTimer += CTimer::Dt();

		float playerBaseDamageCooldown = myPlayer->GetComponent<CStatsComponent>()->GetBaseStats().myBaseDamageCooldown;
		if (myTempAttackTimer > playerBaseDamageCooldown)
		{
			myTempAttackTimer -= playerBaseDamageCooldown;

			if (aParent->GetComponent<CAnimationComponent>())
				aParent->GetComponent<CAnimationComponent>()->PlayAttack01ID();

			int attackType = Random(1, 3);
			if (attackType == 1) {
				aParent->GetComponent<CAbilityComponent>()->UseAbility(EAbilityType::BossAbility1, aParent->myTransform->Position());
			}
			else if (attackType == 2) {
				aParent->GetComponent<CAbilityComponent>()->UseAbility(EAbilityType::BossAbility2, aParent->myTransform->Position());
			}else if (attackType == 3) {
				aParent->GetComponent<CAbilityComponent>()->UseAbility(EAbilityType::BossAbility3, aParent->myTransform->Position());
			}
		}
	}
}

void CBossBehavior::Die()
{
}
