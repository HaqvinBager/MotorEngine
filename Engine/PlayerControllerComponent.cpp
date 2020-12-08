#include "stdafx.h"
#include "PlayerControllerComponent.h"
#include "InputMapper.h"
#include "NavMeshComponent.h"
#include "MouseTracker.h"
#include "StatsComponent.h"
#include "AnimationComponent.h"
#include "MainSingleton.h"
#include "TransformComponent.h"
#include "PopupTextService.h"
#include "MouseSelection.h"
#include <DamageUtility.h>
#include <AbilityBehaviorComponent.h>
#include <AbilityBehavior.h>
#include "CircleColliderComponent.h"
#include "TriangleColliderComponent.h"
#include "TransformComponent.h"

CPlayerControllerComponent::CPlayerControllerComponent(CGameObject& aParent):
	CBehaviour(aParent),
	myLastHP(0.0f),
	myRegenerationSpeed(5.0f), //TODO: read from unity
	mySelection(new CMouseSelection()),
	myIsMoving(false),
	myTargetEnemy(nullptr)
{
	myLastPosition = {0.0f,0.0f,0.0f};
}

CPlayerControllerComponent::~CPlayerControllerComponent()
{
	CInputMapper::GetInstance()->RemoveObserver(IInputObserver::EInputEvent::MoveClick, this);
	CInputMapper::GetInstance()->RemoveObserver(IInputObserver::EInputEvent::MoveDown, this);
	CInputMapper::GetInstance()->RemoveObserver(IInputObserver::EInputEvent::AttackClick, this);
}

void CPlayerControllerComponent::Awake()
{
	myLastHP = GameObject().GetComponent<CStatsComponent>()->GetStats().myHealth;
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::MoveClick, this);
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::MoveDown, this);
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::AttackClick, this);
}

void CPlayerControllerComponent::Start() {}

void CPlayerControllerComponent::Update()
{
	this->GameObject().myTransform->MoveAlongPath();

	if(myTargetEnemy){
		if(myTargetEnemy->GetComponent<CStatsComponent>()->GetStats().myHealth > 0){
			if (myIsMoving) {
				float abilityLength = GameObject().GetComponent<CAbilityComponent>()->MeleeAttackRange();
				if (DirectX::SimpleMath::Vector3::Distance(myTargetEnemy->myTransform->Position(), GameObject().myTransform->Position())
					< (myTargetEnemy->GetComponent<CCircleColliderComponent>()->GetRadius() + abilityLength)) {
					this->GameObject().GetComponent<CAnimationComponent>()->PlayAnimation(EPlayerAnimationID::AttackLight);
					GameObject().GetComponent<CAbilityComponent>()->UseAbility(EAbilityType::PlayerMelee, GameObject().myTransform->Position());
				}
			}
		}
	}

	if (myLastPosition != GameObject().myTransform->Position()) {
		myIsMoving = true;
		myLastPosition = GameObject().myTransform->Position();
	} else {
		myIsMoving = false;
	}

	if (!PlayerIsAlive()) {
		ResetPlayer();
	} else {
		RegenerateMana();
	}
}

void CPlayerControllerComponent::OnEnable() {}

void CPlayerControllerComponent::OnDisable() {}

void CPlayerControllerComponent::ReceiveEvent(const IInputObserver::EInputEvent aEvent)
{
	switch (aEvent)
	{
	case IInputObserver::EInputEvent::MoveClick:
		if(this->GameObject().GetComponent<CNavMeshComponent>() != nullptr)// Temp, ok to remove. Used for testing barebones scene
			this->GameObject().GetComponent<CNavMeshComponent>()->CalculatePath();
		break;
	case IInputObserver::EInputEvent::MoveDown:
		this->GameObject().GetComponent<CNavMeshComponent>()->CalculatePath();
		myTargetEnemy = mySelection->FindSelectedEnemy();
		break;
	case IInputObserver::EInputEvent::AttackClick:
		this->GameObject().GetComponent<CAnimationComponent>()->PlayAnimation(EPlayerAnimationID::AttackLight);
		break;

	default:
		break;
	}
}

void CPlayerControllerComponent::ResetPlayer()
{
	GameObject().GetComponent<CTransformComponent>()->Position(GameObject().GetComponent<CTransformComponent>()->StartPosition());
	GameObject().GetComponent<CStatsComponent>()->GetStats().myHealth = GameObject().GetComponent<CStatsComponent>()->GetBaseStats().myBaseHealth;
	GameObject().GetComponent<CStatsComponent>()->GetStats().myResource = GameObject().GetComponent<CStatsComponent>()->GetBaseStats().myBaseResource;
	MessagePostmaster(EMessageType::PlayerHealthChanged, 1.0f);
	MessagePostmaster(EMessageType::PlayerResourceChanged, 1.0f);
}

void CPlayerControllerComponent::MessagePostmaster(EMessageType aMessageType, float aValue)
{
	SMessage message;
	message.myMessageType = aMessageType;
	message.data = &aValue;
	CMainSingleton::PostMaster().Send(message);
}

bool CPlayerControllerComponent::PlayerIsAlive()
{
	if (myLastHP != GameObject().GetComponent<CStatsComponent>()->GetStats().myHealth)
	{
		float difference = GameObject().GetComponent<CStatsComponent>()->GetBaseStats().myBaseHealth - GameObject().GetComponent<CStatsComponent>()->GetStats().myHealth;
		difference = (100.0f - difference) / 100.0f;
		MessagePostmaster(EMessageType::PlayerHealthChanged, difference);

		myLastHP = GameObject().GetComponent<CStatsComponent>()->GetStats().myHealth;
	}

	return myLastHP > 0.0f;
}

void CPlayerControllerComponent::TakeDamage(float aDamageMultiplier, CGameObject* aGameObject)
{
	SStats& stats = GameObject().GetComponent<CStatsComponent>()->GetStats();

	EHitType hitType = EHitType::Normal;
	float damage = CDamageUtility::CalculateDamage(hitType, aGameObject->GetComponent<CStatsComponent>()->GetBaseStats().myDamage, aDamageMultiplier, 0.0f, 0.0f);

	if (GameObject().GetComponent<CStatsComponent>()->AddDamage(damage)) {
		SDamagePopupData data = {damage, static_cast<int>(hitType)};
		CMainSingleton::PopupTextService().SpawnPopup(EPopupType::Damage, &data);
		std::cout << __FUNCTION__ << " Player current health: " << stats.myHealth << std::endl;/*
		CMainSingleton::PostMaster().Send({EMessageType::PlayerHealthChanged, &stats.myHealth});*/
	}
	//stats.myCanTakeDamage = false;
}


void CPlayerControllerComponent::RegenerateMana()
{
	if (GameObject().GetComponent<CStatsComponent>()->GetBaseStats().myBaseResource > GameObject().GetComponent<CStatsComponent>()->GetStats().myResource) {
		GameObject().GetComponent<CStatsComponent>()->GetStats().myResource += myRegenerationSpeed * CTimer::Dt();
		float difference = GameObject().GetComponent<CStatsComponent>()->GetBaseStats().myBaseResource - GameObject().GetComponent<CStatsComponent>()->GetStats().myResource;
		difference = (100.0f - difference) / 100.0f;
		MessagePostmaster(EMessageType::PlayerResourceChanged, difference);
	}
}
