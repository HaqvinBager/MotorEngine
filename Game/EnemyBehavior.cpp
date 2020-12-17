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
#include "BoomerangBehavior.h"
#include "AbilityBehavior.h"
#include "RandomNumberGenerator.h"
#include "DamageUtility.h"
#include "PostMaster.h"
#include "PopupTextService.h"
#include "HealthBarComponent.h"
#include "Canvas.h"
#include "AnimatedUIElement.h"

CEnemyBehavior::CEnemyBehavior(CGameObject* aPlayerObject)
	: myPlayer(aPlayerObject)
	, myHasSentAudio(false)
	, myCurrentParent(nullptr)
{}

CEnemyBehavior::~CEnemyBehavior()
{}

void CEnemyBehavior::Update(CGameObject* aParent)
{
	if (Input::GetInstance()->IsKeyPressed('P')) {
		TakeDamage(0.1f, CEngine::GetInstance()->GetActiveScene().GetEnemies()[0]);
	}
	myCurrentParent = aParent;

	//enemy logic
	SBaseStats baseStats = myCurrentParent->GetComponent<CStatsComponent>()->GetBaseStats();
	SStats stats = myCurrentParent->GetComponent<CStatsComponent>()->GetStats();

	if (stats.myHealth <= 0) {
		CMainSingleton::PostMaster().SendLate({EMessageType::EnemyDied, this});
		if (stats.myTokenSlot != nullptr) {
			CTokenPool::GetInstance()->GiveBack(*stats.myTokenSlot, false);
			stats.myTokenSlot = nullptr;
		}
		//aParent->Active(false);
	}

	FindATarget();
}

// Sending in the parent feels safer than relying on myCurrentParent
void CEnemyBehavior::Collided(CGameObject* /*aParent*/, CGameObject* /*aCollidedWithGameObject*/)
{
	//	CCollider* collider = reinterpret_cast<CCollider*>(aCollidedWithGameObject->GetComponent<CTriangleColliderComponent>());
	//	if (!collider)
	//	{
	//			collider = reinterpret_cast<CCollider*>(aCollidedWithGameObject->GetComponent<CCircleColliderComponent>());
	//		if (!collider)
	//		{
	//			collider = reinterpret_cast<CCollider*>(aCollidedWithGameObject->GetComponent<CRectangleColliderComponent>());
	//		}
	//	}
	//
	//	if (!collider) { return; }
	//
	//	if (collider->GetCollisionLayer() == ECollisionLayer::PLAYERABILITY)
	//	{
	//		std::cout << __FUNCTION__ << " Enemy collided with player ability " << std::endl;
	//		//CStatsComponent* playerStats = myPlayer->GetComponent<CStatsComponent>();
	//		myCurrentParent = aParent;
	//		//std::cout << aCollidedWithGameObject->GetComponent<CAbilityComponent>().myDamage << std::endl;
	//#ifdef _G4_DEBUG
	//		TakeDamage(aCollidedWithGameObject->GetComponent<CBoomerangBehavior>()->myDamage);
	//
	//#endif // _G4_DEBUG
	//
	//		return;
	//	}
	//
	//	CAIBehaviorComponent* enemyBehavior = aCollidedWithGameObject->GetComponent<CAIBehaviorComponent>();
	//	if (enemyBehavior)
	//	{
	//		std::cout << __FUNCTION__ << " Enemy collided with Enemy " << std::endl;
	//	}
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
			myCurrentParent->GetComponent<CTransformComponent>()->ClearPath();
			if (stats.myTokenSlot == nullptr && stats.hadToken == false) {
				stats.myTokenSlot = CTokenPool::GetInstance()->Request();
			} else if (stats.myTokenSlot != nullptr) {
				myPlayer->GetComponent<CStatsComponent>();
				myCurrentParent->GetComponent<CAnimationComponent>()->PlayAttack01ID();
				myCurrentParent->GetComponent<CAbilityComponent>()->UseAbility(EAbilityType::EnemyAbility, myCurrentParent->myTransform->Position());
				//CTokenPool::GetInstance()->GiveBack(*stats.myTokenSlot, false);
				//stats.myTokenSlot = nullptr;
				//myCurrentParent->GetComponent<CStatsComponent>()->NextTokenCooldown();
				SendAudio();
			}
		} else {
			if (stats.myTokenSlot != nullptr) {
				CTokenPool::GetInstance()->GiveBack(*stats.myTokenSlot, false);
				stats.myTokenSlot = nullptr;
				myCurrentParent->GetComponent<CStatsComponent>()->NextTokenCooldown();
				myHasSentAudio = false;
			}
		}
	} else {
		//myCurrentParent->GetComponent<CTransformComponent>()->ClearPath();
		
		stats.myRandomWalkTime -= CTimer::Dt();
		if (stats.myRandomWalkTime <= 0) {
			//DirectX::SimpleMath::Vector3 randomPos = { myCurrentParent->myTransform->Position().x + Random(-1.f, 1.f) , myCurrentParent->myTransform->Position().y, myCurrentParent->myTransform->Position().z + Random(-2.f, 2.f) };
			float randomA = Random(0.f, 1.f) * 2.f * 3.14f;
			float randomR = 2.f * sqrtf(Random(0.f, 1.f));

			float X = randomR * cosf(randomA);
			float Y = randomR * sinf(randomA);
			DirectX::SimpleMath::Vector3 randomPos = {myCurrentParent->myTransform->Position().x + X, myCurrentParent->myTransform->Position().y, myCurrentParent->myTransform->Position().z + Y};
			if (CEngine::GetInstance()->GetActiveScene().GetNavMesh()->GetTriangleAtPoint(randomPos)) {
				myCurrentParent->GetComponent<CNavMeshComponent>()->CalculatePath(randomPos);
				stats.myRandomWalkTime = Random(baseStats.myBaseRandomWalkTime - 1.f, baseStats.myBaseRandomWalkTime);
			}
		}
	}
}

void CEnemyBehavior::TakeDamage(float aDamageMultiplier, CGameObject* aGameObject)
{
	myCurrentParent = aGameObject;
	SStats& stats = myCurrentParent->GetComponent<CStatsComponent>()->GetStats();
	EHitType hitType = EHitType::Normal;
	float damage = CDamageUtility::CalculateDamage(hitType, myPlayer->GetComponent<CStatsComponent>()->GetBaseStats().myDamage, aDamageMultiplier, 0.25f, 1.5f);

	if (myCurrentParent->GetComponent<CStatsComponent>()->AddDamage(damage)) {
		SDamagePopupData data = {damage, static_cast<int>(hitType), myCurrentParent};
		CMainSingleton::PopupTextService().SpawnPopup(EPopupType::Damage, &data);
		float baseHealth = myCurrentParent->GetComponent<CStatsComponent>()->GetBaseStats().myBaseHealth;
		float difference = baseHealth - myCurrentParent->GetComponent<CStatsComponent>()->GetStats().myHealth;

		//if (myPlayer->GetComponent<CPlayerControllerComponent>()->AuraActive()) {
		float regenerationPercentage = myPlayer->GetComponent<CPlayerControllerComponent>()->RegenerationPercentage();
			if ((myPlayer->GetComponent<CStatsComponent>()->GetStats().myHealth + (difference * regenerationPercentage))
				< myPlayer->GetComponent<CStatsComponent>()->GetBaseStats().myBaseHealth)
				myPlayer->GetComponent<CStatsComponent>()->GetStats().myHealth += difference * regenerationPercentage;
			else
				myPlayer->GetComponent<CStatsComponent>()->GetStats().myHealth = myPlayer->GetComponent<CStatsComponent>()->GetBaseStats().myBaseHealth;
		//}

		difference = (baseHealth - difference) / baseHealth;
		if (difference <= 0.0)
		{
			difference = 0.0f;
		}

		myCurrentParent->GetComponent<CHealthBarComponent>()->GetCanvas()->GetAnimatedUI()[0]->Level(difference);
		myCurrentParent->GetComponent<CHealthBarComponent>()->GetCanvas2()->GetAnimatedUI()[0]->Level(difference);
	}

	if (stats.myHealth <= 0)
	{
		Die();
	}
}

void CEnemyBehavior::Die()
{
	assert(myCurrentParent->GetComponent<CCircleColliderComponent>() != nullptr && "Was nullptr");
	myCurrentParent->GetComponent<CCircleColliderComponent>()->Enabled(false);
	myCurrentParent->GetComponent<CAIBehaviorComponent>()->Enabled(false);
	myCurrentParent->GetComponent<CAbilityComponent>()->Enabled(false);
	myCurrentParent->GetComponent<CNavMeshComponent>()->Enabled(false);
	myCurrentParent->GetComponent<CAnimationComponent>()->DeadState();
	//myCurrentParent->GetComponent<CHealthBarComponent>()->Enabled(false);
	myCurrentParent->GetComponent<CStatsComponent>()->Enabled(false);
	SMessage message;
	message.myMessageType = EMessageType::EnemyDied;
	message.data = &myCurrentParent->GetComponent<CStatsComponent>()->GetStats().myExperience;
	CMainSingleton::PostMaster().Send({ EMessageType::DemonIdle2, nullptr });
	CMainSingleton::PostMaster().SendLate(message);

	SStats stats = myCurrentParent->GetComponent<CStatsComponent>()->GetStats();
	if (stats.myTokenSlot != nullptr) {
		CTokenPool::GetInstance()->GiveBack(*stats.myTokenSlot, false);
		stats.myTokenSlot = nullptr;
	}
}

void CEnemyBehavior::SendAudio()
{
	if (!myHasSentAudio)
	{
		CMainSingleton::PostMaster().Send({ EMessageType::DemonIdle1, nullptr });
		myHasSentAudio = true;
	}
}
