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
	myIsMoving(true),
	myTargetEnemy(nullptr),
	myMiddleMousePressed(false),
	myAuraActive(false)
{
	myLastPosition = {0.0f,0.0f,0.0f};
}

CPlayerControllerComponent::~CPlayerControllerComponent()
{
	delete mySelection;
	mySelection = nullptr;
	CInputMapper::GetInstance()->RemoveObserver(IInputObserver::EInputEvent::MoveDown, this);
	CInputMapper::GetInstance()->RemoveObserver(IInputObserver::EInputEvent::StandStill, this);
	CInputMapper::GetInstance()->RemoveObserver(IInputObserver::EInputEvent::MiddleMouseMove, this);
	CInputMapper::GetInstance()->RemoveObserver(IInputObserver::EInputEvent::Moving, this);
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::EnemyDied, this);
}

void CPlayerControllerComponent::Awake()
{
	myLastHP = GameObject().GetComponent<CStatsComponent>()->GetStats().myHealth;
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::MoveDown, this);
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::StandStill, this);
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::MiddleMouseMove, this);
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::Moving, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::EnemyDied, this);
}

void CPlayerControllerComponent::Start() {}

void CPlayerControllerComponent::Update()
{
	if (myIsMoving) {
		this->GameObject().myTransform->MoveAlongPath();
	}

	if (myTargetEnemy) {
		if (myTargetEnemy->GetComponent<CStatsComponent>()->GetStats().myHealth > 0) {
			float abilityLength = GameObject().GetComponent<CAbilityComponent>()->MeleeAttackRange();
			if (DirectX::SimpleMath::Vector3::Distance(myTargetEnemy->myTransform->Position(), GameObject().myTransform->Position())
				< (myTargetEnemy->GetComponent<CCircleColliderComponent>()->GetRadius() + abilityLength)) {
				this->GameObject().GetComponent<CAbilityComponent>()->UseAbility(EAbilityType::PlayerMelee, GameObject().myTransform->Position());
				this->GameObject().GetComponent<CAnimationComponent>()->PlayAnimation(EPlayerAnimationID::AttackLight);
			}
		}
	}

	/*if (myLastPosition != GameObject().myTransform->Position()) {
		myIsMoving = true;
		myLastPosition = GameObject().myTransform->Position();
	} else {
		myIsMoving = false;
	}*/

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
	case  IInputObserver::EInputEvent::StandStill:
		myMiddleMousePressed = false;

		myIsMoving = false;
		this->GameObject().GetComponent<CTransformComponent>()->ClearPath();
		break;
	case  IInputObserver::EInputEvent::Moving:
		myMiddleMousePressed = false;
		myIsMoving = true;
		break;
	case IInputObserver::EInputEvent::MoveDown:
		myMiddleMousePressed = false;

		if (myIsMoving) {
			this->GameObject().GetComponent<CNavMeshComponent>()->CalculatePath();
			if (mySelection)
			{
				myTargetEnemy = mySelection->FindSelectedEnemy();
			}
		} else {
			this->GameObject().GetComponent<CAbilityComponent>()->UseAbility(EAbilityType::PlayerMelee, GameObject().myTransform->Position());
			this->GameObject().GetComponent<CAnimationComponent>()->PlayAnimation(EPlayerAnimationID::AttackLight);
		}
		break;
	case IInputObserver::EInputEvent::MiddleMouseMove:
		if (myIsMoving) {
			this->GameObject().GetComponent<CNavMeshComponent>()->CalculatePath();
		}
		break;
	default:
		break;
	}
}

void CPlayerControllerComponent::Receive(const SMessage& aMessage)
{
	
	switch (aMessage.myMessageType)
	{
	case EMessageType::EnemyDied:
		UpdateExperience(aMessage);
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
	//SStats& stats = GameObject().GetComponent<CStatsComponent>()->GetStats();

	EHitType hitType = EHitType::Enemy;
	float damage = CDamageUtility::CalculateDamage(hitType, aGameObject->GetComponent<CStatsComponent>()->GetBaseStats().myDamage, aDamageMultiplier);

	if (GameObject().GetComponent<CStatsComponent>()->AddDamage(damage)) {
		SDamagePopupData data = {damage, static_cast<int>(hitType), &GameObject()};
		CMainSingleton::PopupTextService().SpawnPopup(EPopupType::Damage, &data);
		//std::cout << __FUNCTION__ << " Player current health: " << stats.myHealth << std::endl;/*
		//CMainSingleton::PostMaster().Send({EMessageType::PlayerHealthChanged, &stats.myHealth});*/
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

void CPlayerControllerComponent::UpdateExperience(const SMessage& aMessage)
{
	float difference;
	float maxValue;
	//float currentExperience;

	if (this->GameObject().GetComponent<CStatsComponent>()->GetBaseStats().myMaxLevel
				> this->GameObject().GetComponent<CStatsComponent>()->GetStats().myLevel)
	{

		this->GameObject().GetComponent<CStatsComponent>()->GetStats().myExperience += *static_cast<float*>(aMessage.data);

		maxValue = this->GameObject().GetComponent<CStatsComponent>()->GetBaseStats().myExperienceToLevelUp;

		if (maxValue <= this->GameObject().GetComponent<CStatsComponent>()->GetStats().myExperience)
		{

			this->GameObject().GetComponent<CStatsComponent>()->GetStats().myLevel += 1;
			int level = this->GameObject().GetComponent<CStatsComponent>()->GetStats().myLevel;
			std::string abilityInfo = "Skill ";
			abilityInfo += std::to_string(level);

			CMainSingleton::PopupTextService().SpawnPopup(EPopupType::Info, abilityInfo);

			////This is for group 4
			////Comment this in before last build
			/*if (level == 2) {
				this->GameObject().GetComponent<CAbilityComponent>()->UseAbility(EAbilityType::PlayerAbility2, GameObject().myTransform->Position());
				myAuraActive = true;
			}*/

			this->GameObject().GetComponent<CAbilityComponent>()->ResetCooldown(this->GameObject().GetComponent<CStatsComponent>()->GetStats().myLevel);

			std::cout << this->GameObject().GetComponent<CStatsComponent>()->GetStats().myLevel << std::endl;
			if (this->GameObject().GetComponent<CStatsComponent>()->GetBaseStats().myMaxLevel
				== this->GameObject().GetComponent<CStatsComponent>()->GetStats().myLevel) {
				MessagePostmaster(EMessageType::PlayerExperienceChanged, 1.0f);
			} else {
				difference = this->GameObject().GetComponent<CStatsComponent>()->GetStats().myExperience - maxValue;
				this->GameObject().GetComponent<CStatsComponent>()->GetStats().myExperience = difference;

				difference = difference / maxValue;
				MessagePostmaster(EMessageType::PlayerExperienceChanged, difference);
			}
		} else
		{
			difference = this->GameObject().GetComponent<CStatsComponent>()->GetStats().myExperience / maxValue;
			MessagePostmaster(EMessageType::PlayerExperienceChanged, difference);
		}
	}
}
