#include "stdafx.h"
#include "CapsuleColliderComponent.h"
#include "TransformComponent.h"
#include "CollisionManager.h"

CCapsuleColliderComponent::CCapsuleColliderComponent(CGameObject& aParent, float aRadius, float aHeight) 
	: CBehaviour(aParent)
	, myRadius(aRadius)
	, myHeight(aHeight) 
{
	CCollisionManager::GetInstance()->RegisterCollider(this);
}

CCapsuleColliderComponent::~CCapsuleColliderComponent()
{

}

void CCapsuleColliderComponent::Awake()
{
	myBase = GetParent().GetComponent<CTransformComponent>()->Position();
	myTip = myBase;
	if (myHeight < (myRadius * 2)) {
		myHeight = myRadius * 2;
	}
	myBase.y -= myHeight / 2.0f;
	myTip.y += myHeight / 2.0f;

}

void CCapsuleColliderComponent::Start()
{
}

void CCapsuleColliderComponent::Update()
{
	myBase = GetParent().GetComponent<CTransformComponent>()->Position();
	myTip = myBase;
	if (myHeight < (myRadius * 2))
	{
		myHeight = myRadius * 2;
	}
	myBase.y -= myHeight / 2.0f;
	myTip.y += myHeight / 2.0f;
}

void CCapsuleColliderComponent::Collided(CGameObject* /*aCollidedGameObject*/)
{
	//GetParent().Collided(aCollidedGameObject);
}

void CCapsuleColliderComponent::OnEnable()
{
}

void CCapsuleColliderComponent::OnDisable()
{
}