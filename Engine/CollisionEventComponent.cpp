#include "stdafx.h"
#include "CollisionEventComponent.h"
#include "RectangleColliderComponent.h"
#include "RectangleColliderComponent.h"
#include "GameObject.h"
#include "CollisionManager.h"
#include "MainSingleton.h"
#include "PostMaster.h"
#include "Debug.h"

CCollisionEventComponent::CCollisionEventComponent(CGameObject& aGameObject, EMessageType aMessageType, std::string anEventString)
	: CComponent(aGameObject)
	, myMessageType(aMessageType)
	, myTextMessage(anEventString)
{	
	//
}

CCollisionEventComponent::~CCollisionEventComponent()
{

}

void CCollisionEventComponent::Awake()
{
}

void CCollisionEventComponent::Start()
{
}

void CCollisionEventComponent::Update()
{
}

void CCollisionEventComponent::Collided(CGameObject* aCollidedGameObject)
{
	SStringMessage stringMessage = { myTextMessage.c_str(), aCollidedGameObject };
	CMainSingleton::PostMaster().Send(stringMessage);
	GameObject().Active(false);
}

const std::string& CCollisionEventComponent::GetEventMessage() const
{
	return myTextMessage;
}
