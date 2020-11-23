#include "stdafx.h"
#include "PlayerControllerComponent.h"
#include "InputMapper.h"
#include "NavMeshComponent.h"
#include "MouseTracker.h"

CPlayerControllerComponent::CPlayerControllerComponent(CGameObject& aParent) : CBehaviour(aParent)
{
	//IS PROBABLY RONG // ALEX LIVHOLTS
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::MoveClick, this);
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::AttackClick, this);

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
	if (aEvent == IInputObserver::EInputEvent::MoveClick) {
		std::vector<DirectX::SimpleMath::Vector3> path;
		DirectX::SimpleMath::Vector3 finalPosition;
		CNavMeshComponent* navMeshComp = this->GameObject().GetComponent<CNavMeshComponent>();
		path = navMeshComp->CalculatePath(MouseTracker::WorldSpacePick(), finalPosition);
		this->GameObject().myTransform->SetPath(path, finalPosition);
	}

	switch (aEvent)
	{
	//case IInputObserver::EInputEvent::MoveClick:
	//	//std::cout << "Hejsan" << std::endl;
	//	//DO STUFF
	//	break;
	case IInputObserver::EInputEvent::AttackClick:
		//ALSO DO STUFF TOO
		break;
	default:
		break;
	}
}
