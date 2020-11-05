#include "stdafx.h"
#include "TriangleColliderComponent.h"
#include "CollisionManager.h"
#include "TransformComponent.h"

CTriangleColliderComponent::CTriangleColliderComponent(CGameObject& aParent, float aWidth, float aHeight) : 
	CComponent(aParent),
	myWidth(aWidth),
	myHeight(aHeight)
{
	CCollisionManager::GetInstance()->RegisterCollider(this);
}

CTriangleColliderComponent::~CTriangleColliderComponent() {
}

void CTriangleColliderComponent::Awake() {
	DirectX::SimpleMath::Vector3 vector = GetParent().GetComponent<CTransformComponent>()->Position() + GetParent().GetComponent<CTransformComponent>()->Position().Forward * myHeight;

	myVertices[0] = GetParent().GetComponent<CTransformComponent>()->Position();
	myVertices[1] = vector - GetParent().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f);
	myVertices[2] = vector + GetParent().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f);
}

void CTriangleColliderComponent::Start() {
}

void CTriangleColliderComponent::Update() {
	DirectX::SimpleMath::Vector3 vector = GetParent().GetComponent<CTransformComponent>()->Position() + GetParent().GetComponent<CTransformComponent>()->Position().Forward * myHeight;

	myVertices[0] = GetParent().GetComponent<CTransformComponent>()->Position();
	myVertices[1] = vector - GetParent().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f);
	myVertices[2] = vector + GetParent().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f);
}

void CTriangleColliderComponent::Collided(CGameObject* /*aCollidedGameObject*/) {
}
