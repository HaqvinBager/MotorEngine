#include "stdafx.h"
#include "TriangleColliderComponent.h"
#include "CollisionManager.h"

CTriangleColliderComponent::CTriangleColliderComponent(CGameObject& aParent, std::vector<DirectX::SimpleMath::Vector3> someVertices) : 
	CComponent(aParent),
	myVertices(someVertices) 
{
	CCollisionManager::GetInstance()->RegisterCollider(this);
}

CTriangleColliderComponent::~CTriangleColliderComponent()
{
}

void CTriangleColliderComponent::Awake()
{
}

void CTriangleColliderComponent::Start()
{
}

void CTriangleColliderComponent::Update()
{
}

void CTriangleColliderComponent::Collided(CGameObject* /*aCollidedGameObject*/)
{
}
