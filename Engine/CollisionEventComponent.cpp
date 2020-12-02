#include "stdafx.h"
#include "CollisionEventComponent.h"
#include "RectangleColliderComponent.h"
#include "RectangleColliderComponent.h"
#include "GameObject.h"
#include "CollisionManager.h"
#include "MainSingleton.h"
#include "PostMaster.h"
#include "Debug.h"

CCollisionEventComponent::CCollisionEventComponent(CGameObject& aGameObject, EMessageType aMessageType, std::string anEventString, float aWidth, float aHeight, ECollisionLayer aCollisionLayer, uint64_t someCollisionFlags)
	: CComponent(aGameObject)
	, myMessageType(aMessageType)
	, myTextMessage(anEventString)
{	
	aGameObject.AddComponent<CRectangleColliderComponent>(aGameObject, aWidth, aHeight, aCollisionLayer, someCollisionFlags);
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
	CRectangleColliderComponent* collider = GameObject().GetComponent<CRectangleColliderComponent>();
	CDebug::GetInstance()->DrawLine(collider->GetPosition(), { collider->GetPosition().x + 1.0f, collider->GetPosition().y, collider->GetPosition().z + 1.0f });
}

void CCollisionEventComponent::Collided(CGameObject* aCollidedGameObject)
{
	SStringMessage stringMessage = { myTextMessage.c_str(), aCollidedGameObject };
	CMainSingleton::PostMaster().Send(stringMessage);
}

const std::string& CCollisionEventComponent::GetEventMessage() const
{
	return myTextMessage;
}
