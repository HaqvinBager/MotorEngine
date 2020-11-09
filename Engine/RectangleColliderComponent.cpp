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
	myVertices.emplace_back(vector - GetParent().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f));
	myVertices.emplace_back(vector + GetParent().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f));
	
	vector = GetParent().GetComponent<CTransformComponent>()->Position() - GetParent().GetComponent<CTransformComponent>()->Position().Forward * (myHeight / 2.0f);

	myVertices.emplace_back(vector - GetParent().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f));
	myVertices.emplace_back(vector + GetParent().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f));
}

void CRectangleColliderComponent::Start() {
}

#include "Debug.h"
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

	if (GetAsyncKeyState('C')) {
		CDebug::GetInstance()->DrawLine(myVertices[0], myVertices[1]);
		CDebug::GetInstance()->DrawLine(myVertices[1], myVertices[3]);
		CDebug::GetInstance()->DrawLine(myVertices[0], myVertices[2]);
		CDebug::GetInstance()->DrawLine(myVertices[2], myVertices[3]);
	}
}

void CRectangleColliderComponent::Collided(CGameObject* /*aCollidedGameObject*/) {
}
