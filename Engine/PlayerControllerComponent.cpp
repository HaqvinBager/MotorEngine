#include "stdafx.h"
#include "PlayerControllerComponent.h"
#include "InputMapper.h"
#include "NavMeshComponent.h"
#include "MouseTracker.h"
#include "StatsComponent.h"
#include "AnimationComponent.h"
#include "MainSingleton.h"
#include "TransformComponent.h"

CPlayerControllerComponent::CPlayerControllerComponent(CGameObject& aParent): 
	CBehaviour(aParent), 
	myLastHP(0.0f), 
	myRegenerationSpeed(5.0f) //TODO: read from unity
{
}

CPlayerControllerComponent::~CPlayerControllerComponent()
{
	CInputMapper::GetInstance()->RemoveObserver(IInputObserver::EInputEvent::MoveClick, this);
}

void CPlayerControllerComponent::Awake()
{
	myLastHP = GameObject().GetComponent<CStatsComponent>()->GetStats().myHealth;
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::MoveClick, this);
}

void CPlayerControllerComponent::Start() {}

void CPlayerControllerComponent::Update()
{
	this->GameObject().myTransform->MoveAlongPath();

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
		this->GameObject().GetComponent<CNavMeshComponent>()->CalculatePath();
		// TEMP, Ok to remove
		if(this->GameObject().GetComponent<CAnimationComponent>() != nullptr)
			this->GameObject().GetComponent<CAnimationComponent>()->PlayAnimation(EPlayerAnimationID::Run);
		// ! TEMP
			break;
	case IInputObserver::EInputEvent::AttackClick:
		//ALSO DO STUFF TOO
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


void CPlayerControllerComponent::RegenerateMana()
{
	if (GameObject().GetComponent<CStatsComponent>()->GetBaseStats().myBaseResource > GameObject().GetComponent<CStatsComponent>()->GetStats().myResource) {
		GameObject().GetComponent<CStatsComponent>()->GetStats().myResource += myRegenerationSpeed * CTimer::Dt();
		float difference = GameObject().GetComponent<CStatsComponent>()->GetBaseStats().myBaseResource - GameObject().GetComponent<CStatsComponent>()->GetStats().myResource;
		difference = (100.0f - difference) / 100.0f;
		MessagePostmaster(EMessageType::PlayerResourceChanged, difference);
	}
}
