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
