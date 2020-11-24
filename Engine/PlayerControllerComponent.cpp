#include "stdafx.h"
#include "PlayerControllerComponent.h"
#include "InputMapper.h"
#include "NavMeshComponent.h"
#include "MouseTracker.h"

CPlayerControllerComponent::CPlayerControllerComponent(CGameObject& aParent) : CBehaviour(aParent)
{
}

CPlayerControllerComponent::~CPlayerControllerComponent()
{
}

void CPlayerControllerComponent::Awake()
{
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::MoveClick, this);
#ifdef _DEBUG
	std::cout << __FUNCTION__ << " \n\tHello, this is a reminder that I am adding myself as an observer for EInputEvent::MoveClick event to CInputMapper. This might be incorrect, please, someone who knows look at if this is OK. :) Forgot what was decided (Aki)" << std::endl;
#endif
}

void CPlayerControllerComponent::Start()
{
}

void CPlayerControllerComponent::Update()
{
	this->GameObject().myTransform->MoveAlongPath();
}

void CPlayerControllerComponent::OnEnable()
{
}

void CPlayerControllerComponent::OnDisable()
{
}

void CPlayerControllerComponent::RecieveEvent(const IInputObserver::EInputEvent aEvent)
{
	switch (aEvent)
	{
	case IInputObserver::EInputEvent::MoveClick:
		this->GameObject().GetComponent<CNavMeshComponent>()->CalculatePath();
		break;
	case IInputObserver::EInputEvent::AttackClick:
		//ALSO DO STUFF TOO
		break;
	default:
		break;
	}
}
