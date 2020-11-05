#include "stdafx.h"
#include "RectangleColliderComponent.h"
#include "CollisionManager.h"
#include "TransformComponent.h"

CRectangleColliderComponent::CRectangleColliderComponent(CGameObject& aParent, float aWidth, float aHeight, bool isStatic) :
	CComponent(aParent),
	myWidth(aWidth),
	myHeight(aHeight),
	myIsStatic(isStatic)
{
	CCollisionManager::GetInstance()->RegisterCollider(this);
}

CRectangleColliderComponent::~CRectangleColliderComponent() {
}

void CRectangleColliderComponent::Awake() {
	DirectX::SimpleMath::Vector3 vector = GetParent().GetComponent<CTransformComponent>()->Position() + GetParent().GetComponent<CTransformComponent>()->Position().Forward * (myHeight / 2.0f);

	myPosition = GetParent().GetComponent<CTransformComponent>()->Position();
	myVertices[0] = vector - GetParent().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f);
	myVertices[1] = vector + GetParent().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f);
	
	vector = GetParent().GetComponent<CTransformComponent>()->Position() - GetParent().GetComponent<CTransformComponent>()->Position().Forward * (myHeight / 2.0f);

	myVertices[2] = vector - GetParent().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f);
	myVertices[3] = vector + GetParent().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f);
}

void CRectangleColliderComponent::Start() {
}

void CRectangleColliderComponent::Update() {
	if (!myIsStatic) {
		DirectX::SimpleMath::Vector3 vector = GetParent().GetComponent<CTransformComponent>()->Position() + GetParent().GetComponent<CTransformComponent>()->Position().Forward * (myHeight / 2.0f);

		myPosition = GetParent().GetComponent<CTransformComponent>()->Position();
		myVertices[0] = vector - GetParent().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f);
		myVertices[1] = vector + GetParent().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f);

		vector = GetParent().GetComponent<CTransformComponent>()->Position() - GetParent().GetComponent<CTransformComponent>()->Position().Forward * (myHeight / 2.0f);

		myVertices[2] = vector - GetParent().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f);
		myVertices[3] = vector + GetParent().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f);
	}
}

void CRectangleColliderComponent::Collided(CGameObject* /*aCollidedGameObject*/) {
}
