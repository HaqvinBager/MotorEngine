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
#include "DestructibleComponent.h"
#include <PlayerGlobalState.h>
#include "VFXComponent.h"
#include "VFXFactory.h"
#include "ParticleEmitterComponent.h"
#include "ParticleFactory.h"

CPlayerControllerComponent::CPlayerControllerComponent(CGameObject& aParent):
	CBehaviour(aParent),
	myLastHP(0.0f),
	mySecourceRegenerationSpeed(4.0f), //TODO: read from unity
	mySelection(new CMouseSelection()),
	myIsMoving(true),
	myTargetEnemy(nullptr),
	myTargetDestructible(nullptr),
	myMiddleMousePressed(false),
	myAuraActive(false)
{
	myLastPosition = {0.0f,0.0f,0.0f};

	myPathMarker = new CGameObject(-1337);
	myPathMarker->AddComponent<CParticleEmitterComponent>(*myPathMarker);
	std::vector<std::string> particlePaths;
	particlePaths.emplace_back("Json/PD_MouseRing.json");
	myPathMarker->GetComponent<CParticleEmitterComponent>()->Init(CParticleFactory::GetInstance()->GetParticleSet(particlePaths));
	myPathMarker->Active(true);
	myMarkerDuration = myPathMarker->GetComponent<CParticleEmitterComponent>()->EmitterDurations().back();
	myPathMarker->myTransform->Position({GameObject().myTransform->Position().x, GameObject().myTransform->Position().y , GameObject().myTransform->Position().z});
}

CPlayerControllerComponent::~CPlayerControllerComponent()
{
	delete mySelection;
	mySelection = nullptr;
	CInputMapper::GetInstance()->RemoveObserver(IInputObserver::EInputEvent::MoveClick, this);
	CInputMapper::GetInstance()->RemoveObserver(IInputObserver::EInputEvent::MoveDown, this);
	CInputMapper::GetInstance()->RemoveObserver(IInputObserver::EInputEvent::StandStill, this);
	CInputMapper::GetInstance()->RemoveObserver(IInputObserver::EInputEvent::MiddleMouseMove, this);
	CInputMapper::GetInstance()->RemoveObserver(IInputObserver::EInputEvent::Moving, this);
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::EnemyDied, this);
}

void CPlayerControllerComponent::Awake()
{
	myLastHP = GameObject().GetComponent<CStatsComponent>()->GetStats().myHealth;
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::MoveClick, this);
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::MoveDown, this);
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::StandStill, this);
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::MiddleMouseMove, this);
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::Moving, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::EnemyDied, this);

	CEngine::GetInstance()->GetActiveScene().AddInstance(myPathMarker);
}

void CPlayerControllerComponent::Start()
{
	GameObject().GetComponent<CStatsComponent>()->GetStats().myExperience = CMainSingleton::PlayerGlobalState().GetSavedExperience();

	const int level = CMainSingleton::PlayerGlobalState().GetSavedPlayerLevel();
	GameObject().GetComponent<CStatsComponent>()->GetStats().myLevel = level;
	switch (level)
	{
	case 3: // Activate ability 3
		this->GameObject().GetComponent<CAbilityComponent>()->ResetCooldown(3);
	case 2: // Activate ability 2
		this->GameObject().GetComponent<CAbilityComponent>()->UseAbility(EAbilityType::PlayerAbility2, GameObject().myTransform->Position());
		myAuraActive = true;
		this->GameObject().GetComponent<CAbilityComponent>()->ResetCooldown(2);
	case 1: // Activate ability 1
		this->GameObject().GetComponent<CAbilityComponent>()->ResetCooldown(1);
	case 0:
		break;
	}

	if (this->GameObject().GetComponent<CStatsComponent>()->GetBaseStats().myMaxLevel
		== this->GameObject().GetComponent<CStatsComponent>()->GetStats().myLevel)
	{
		MessagePostmaster(EMessageType::PlayerExperienceChanged, 1.0f);
	} else {
		MessagePostmaster(EMessageType::PlayerExperienceChanged, this->GameObject().GetComponent<CStatsComponent>()->GetStats().myExperience);
	}
}
void CPlayerControllerComponent::Update()
{
	if (Input::GetInstance()->IsKeyPressed('L'))
	{

		const int level = 3;
		GameObject().GetComponent<CStatsComponent>()->GetStats().myLevel = level;
		switch (level)
		{
		case 3: // Activate ability 3
			this->GameObject().GetComponent<CAbilityComponent>()->ResetCooldown(3);
		case 2: // Activate ability 2
			this->GameObject().GetComponent<CAbilityComponent>()->UseAbility(EAbilityType::PlayerAbility2, GameObject().myTransform->Position());
			myAuraActive = true;
			this->GameObject().GetComponent<CAbilityComponent>()->ResetCooldown(2);
		case 1: // Activate ability 1
			this->GameObject().GetComponent<CAbilityComponent>()->ResetCooldown(1);
		case 0:
			break;
		}

	}

	if (myIsMoving) {
		this->GameObject().myTransform->MoveAlongPath();
	}

	if (myTargetEnemy) {
		if (myTargetEnemy->GetComponent<CStatsComponent>()->GetStats().myHealth > 0) {
			float abilityLength = GameObject().GetComponent<CAbilityComponent>()->MeleeAttackRange();
			this->GameObject().GetComponent<CNavMeshComponent>()->CalculatePath(myTargetEnemy->myTransform->Position());
			if (DirectX::SimpleMath::Vector3::Distance(myTargetEnemy->myTransform->Position(), GameObject().myTransform->Position())
				< (myTargetEnemy->GetComponent<CCircleColliderComponent>()->GetRadius() + abilityLength)) {
				this->GameObject().GetComponent<CTransformComponent>()->ClearPath();
				this->GameObject().GetComponent<CAbilityComponent>()->UseAbility(EAbilityType::PlayerMelee, GameObject().myTransform->Position());
				this->GameObject().GetComponent<CAnimationComponent>()->PlayAttack01ID();
			}
		}
	}

	if (myTargetDestructible) {
		if (myTargetDestructible->GetComponent<CDestructibleComponent>()->IsDead() == false) {
			if (DirectX::SimpleMath::Vector3::Distance(myTargetDestructible->myTransform->Position(), GameObject().myTransform->Position()) < myTargetDestructible->GetComponent<CCircleColliderComponent>()->GetRadius()) {
				myTargetDestructible->GetComponent<CDestructibleComponent>()->IsDead(true);
				this->GameObject().GetComponent<CAnimationComponent>()->PlayAttack01ID();
				this->GameObject().GetComponent<CTransformComponent>()->ClearPath();
			}
		}
	}

	if (!PlayerIsAlive()) {
		ResetPlayer();
	} else {
		RegenerateMana();
	}
	if (myPathMarker->Active()) {
		std::cout << "Position at X:" << myPathMarker->GetComponent<CParticleEmitterComponent>()->GetTransform()._41 << std::endl;
		std::cout << "Position at Y:" << myPathMarker->GetComponent<CParticleEmitterComponent>()->GetTransform()._42 << std::endl;
		std::cout << "Position at Z:" << myPathMarker->GetComponent<CParticleEmitterComponent>()->GetTransform()._43 << std::endl;
		if (myMarkerDuration >= 0.0f) {
			myMarkerDuration -= CTimer::Dt();
		}
		if (myMarkerDuration <= 0.0f) {
			myPathMarker->Active(false);
		}
	}
}

void CPlayerControllerComponent::OnEnable() {}

void CPlayerControllerComponent::OnDisable() {}

void CPlayerControllerComponent::ReceiveEvent(const IInputObserver::EInputEvent aEvent)
{
	if (CEngine::GetInstance()->GetActiveScene().GetNavMesh()) {
		switch (aEvent)
		{
		case IInputObserver::EInputEvent::MoveClick:
			myPathMarker->Active(true);
			myMarkerDuration = myPathMarker->GetComponent<CParticleEmitterComponent>()->EmitterDurations().back();
			myPathMarker->myTransform->Position(mySelection->GetPositionAtNavmesh());
			break;
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
				if (mySelection)
				{
					if (CEngine::GetInstance()->GetActiveScene().GetBoss()) {
						myTargetEnemy = mySelection->FindSelectedBoss();
					}
					else {
						myTargetEnemy = mySelection->FindSelectedEnemy();
					}
					if (myTargetEnemy && myTargetEnemy->GetComponent<CStatsComponent>()->GetStats().myHealth > 0.f) {

						this->GameObject().GetComponent<CNavMeshComponent>()->CalculatePath(myTargetEnemy->myTransform->Position());
					}
					else {
						this->GameObject().GetComponent<CNavMeshComponent>()->CalculatePath();
					}

					myTargetDestructible = mySelection->FindSelectedDestructible();
					if (myTargetDestructible && myTargetDestructible->GetComponent<CDestructibleComponent>()->IsDead() == false) {

						this->GameObject().GetComponent<CNavMeshComponent>()->CalculatePath(myTargetDestructible->myTransform->Position());
					}
					else {
						this->GameObject().GetComponent<CNavMeshComponent>()->CalculatePath();
					}
				}
			}
			else {
				this->GameObject().GetComponent<CAbilityComponent>()->UseAbility(EAbilityType::PlayerMelee, GameObject().myTransform->Position());
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
	GameObject().GetComponent<CTransformComponent>()->ClearPath();
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
		float baseHealth = GameObject().GetComponent<CStatsComponent>()->GetBaseStats().myBaseHealth;
		float difference = baseHealth - GameObject().GetComponent<CStatsComponent>()->GetStats().myHealth;
		difference = (baseHealth - difference) / baseHealth;
		MessagePostmaster(EMessageType::PlayerHealthChanged, difference);

		myLastHP = GameObject().GetComponent<CStatsComponent>()->GetStats().myHealth;
	}

	return myLastHP > 0.0f;
}

void CPlayerControllerComponent::TakeDamage(float aDamageMultiplier, CGameObject* aGameObject)
{
	EHitType hitType = EHitType::Enemy;
	float damage = CDamageUtility::CalculateDamage(hitType, aGameObject->GetComponent<CStatsComponent>()->GetBaseStats().myDamage, aDamageMultiplier);

	if (GameObject().GetComponent<CStatsComponent>()->AddDamage(damage)) {
		SDamagePopupData data = {damage, static_cast<int>(hitType), &GameObject()};
		CMainSingleton::PopupTextService().SpawnPopup(EPopupType::Damage, &data);
	}
}


void CPlayerControllerComponent::RegenerateMana()
{
	if (GameObject().GetComponent<CStatsComponent>()->GetBaseStats().myBaseResource > GameObject().GetComponent<CStatsComponent>()->GetStats().myResource) {
		GameObject().GetComponent<CStatsComponent>()->GetStats().myResource += mySecourceRegenerationSpeed * CTimer::Dt();
		float difference = GameObject().GetComponent<CStatsComponent>()->GetBaseStats().myBaseResource - GameObject().GetComponent<CStatsComponent>()->GetStats().myResource;
		difference = (100.0f - difference) / 100.0f;
		MessagePostmaster(EMessageType::PlayerResourceChanged, difference);
	}
}

void CPlayerControllerComponent::UpdateExperience(const SMessage& aMessage)
{
	if (this->GameObject().GetComponent<CStatsComponent>()->GetBaseStats().myMaxLevel
		> this->GameObject().GetComponent<CStatsComponent>()->GetStats().myLevel)
	{
		float difference;
		float maxValue = this->GameObject().GetComponent<CStatsComponent>()->GetBaseStats().myExperienceToLevelUp;
		float experience = *static_cast<float*>(aMessage.data);
		float experienceModifier = static_cast<float>(this->GameObject().GetComponent<CStatsComponent>()->GetStats().myLevel) + 1.0f;
		this->GameObject().GetComponent<CStatsComponent>()->GetStats().myExperience += experience / experienceModifier;

		if (maxValue <= this->GameObject().GetComponent<CStatsComponent>()->GetStats().myExperience)
		{
			this->GameObject().GetComponent<CStatsComponent>()->GetStats().myLevel += 1;
			int level = this->GameObject().GetComponent<CStatsComponent>()->GetStats().myLevel;
			std::string abilityInfo = "Skill ";
			abilityInfo += std::to_string(level);

			CMainSingleton::PopupTextService().SpawnPopup(EPopupType::Info, abilityInfo);

			////This is for group 4
			////Comment this in before last build
			if (level == 2) {
				this->GameObject().GetComponent<CAbilityComponent>()->UseAbility(EAbilityType::PlayerAbility2, GameObject().myTransform->Position());
				myAuraActive = true;
			}

			this->GameObject().GetComponent<CAbilityComponent>()->ResetCooldown(this->GameObject().GetComponent<CStatsComponent>()->GetStats().myLevel);
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

	CMainSingleton::PlayerGlobalState().SetStatsToSave(GameObject().GetComponent<CStatsComponent>()->GetStats());
}
