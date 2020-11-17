#include "stdafx.h"
#include "CapsuleColliderComponent.h"
#include "TransformComponent.h"
#include "CollisionManager.h"

CCapsuleColliderComponent::CCapsuleColliderComponent(CGameObject& aParent, float aRadius, float aHeight, ECollisionLayer aCollisionLayer, uint64_t someCollisionFlags)
	: CCollider(aParent, aCollisionLayer, someCollisionFlags)
	, myRadius(aRadius)
	, myHeight(aHeight) 
{
	CCollisionManager::GetInstance()->RegisterCollider(this);
	SetCollisionLayer(aCollisionLayer);
}

CCapsuleColliderComponent::~CCapsuleColliderComponent()
{

}

void CCapsuleColliderComponent::Awake()
{
	SetPosition(GameObject().GetComponent<CTransformComponent>()->Position());
	myTop = GetPosition();
	if (myHeight < (myRadius * 2)) {
		myHeight = myRadius * 2;
	}
	SetPosition({GetPosition().x, GetPosition().y - myHeight / 2.0f, GetPosition().z});
	myTop.y += myHeight / 2.0f;

}

void CCapsuleColliderComponent::Start()
{
}

void CCapsuleColliderComponent::Update()
{
	SetPosition(GameObject().GetComponent<CTransformComponent>()->Position());
	myTop = GetPosition();
	if (myHeight < (myRadius * 2))
	{
		myHeight = myRadius * 2;
	}
	SetPosition({ GetPosition().x, GetPosition().y - myHeight / 2.0f, GetPosition().z });
	myTop.y += myHeight / 2.0f;
}

bool CCapsuleColliderComponent::Collided(CCircleColliderComponent* /*aCollidedGameObject*/)
{
	//NO INTERSECTION TEST IMPLEMENTED AS OF YET
	return false;
}

bool CCapsuleColliderComponent::Collided(CTriangleColliderComponent* /*aCollidedGameObject*/)
{
	//NO INTERSECTION TEST IMPLEMENTED AS OF YET
	return false;
}

bool CCapsuleColliderComponent::Collided(CRectangleColliderComponent* /*aCollidedGameObject*/)
{
	//NO INTERSECTION TEST IMPLEMENTED AS OF YET
	return false;
}

bool CCapsuleColliderComponent::Collided(CCollider* /*aCollidedGameObject*/)
{
	//GetParent().Collided(aCollidedGameObject);
	return false;
}

void CCapsuleColliderComponent::OnEnable()
{
}

void CCapsuleColliderComponent::OnDisable()
{
}

float const CCapsuleColliderComponent::GetRadius() const
{
	return myRadius;
}

void CCapsuleColliderComponent::SetRadius(float aRadius)
{
	myRadius = aRadius;
}

float const CCapsuleColliderComponent::GetHeight() const
{
	return myHeight;
}

void CCapsuleColliderComponent::SetHeight(float aHeight)
{
	myHeight = aHeight;
}
