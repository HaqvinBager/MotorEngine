#include "stdafx.h"
#include "PlayerControllerComponent.h"
#include "InputMapper.h"

CPlayerControllerComponent::CPlayerControllerComponent(CGameObject& aParent) : CBehaviour(aParent)
{
	//IS PROBABLY RONG // ALEX LIVHOLTS
	CInputMapper::GetInstance()->AddObserver(CInputObserver::EInputEvent::MoveClick, this);
	CInputMapper::GetInstance()->AddObserver(CInputObserver::EInputEvent::AttackClick, this);

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
}

void CPlayerControllerComponent::OnEnable()
{
}

void CPlayerControllerComponent::OnDisable()
{
}

void CPlayerControllerComponent::RecieveEvent(const CInputObserver::EInputEvent aEvent, const float /*aValue*/)
{
	switch (aEvent)
	{
	case CInputObserver::EInputEvent::MoveClick:
		//std::cout << "Hejsan" << std::endl;
		//DO STUFF
		break;
	case CInputObserver::EInputEvent::AttackClick:
		//ALSO DO STUFF TOO
		break;
	default:
		break;
	}
}
