#include "stdafx.h"
#include "TriangleColliderComponent.h"
#include "CollisionManager.h"
#include "TransformComponent.h"

CTriangleColliderComponent::CTriangleColliderComponent(CGameObject& aParent, float aWidth, float aHeight) : 
	CBehaviour(aParent),
	myWidth(aWidth),
	myHeight(aHeight)
{
	CCollisionManager::GetInstance()->RegisterCollider(this);
}

CTriangleColliderComponent::~CTriangleColliderComponent() {
}


void CTriangleColliderComponent::Awake() {
	DirectX::SimpleMath::Vector3 vector = GameObject().GetComponent<CTransformComponent>()->Position() + GameObject().GetComponent<CTransformComponent>()->Position().Forward * myHeight;

	myVertices.emplace_back(GameObject().GetComponent<CTransformComponent>()->Position()); //Verex 0
	myVertices.emplace_back(vector - GameObject().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f)); //Verex 1
	myVertices.emplace_back(vector + GameObject().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f)); //Verex 2
}

void CTriangleColliderComponent::Start() {
}

#include "Debug.h"
void CTriangleColliderComponent::Update() {
<<<<<<< HEAD
	DirectX::SimpleMath::Vector3 vector = GetParent().GetComponent<CTransformComponent>()->Position() + GetParent().GetComponent<CTransformComponent>()->Transform().Forward() * myHeight * 100.0f;

	myVertices[0] = GetParent().GetComponent<CTransformComponent>()->Position();
	myVertices[1] = vector - GetParent().GetComponent<CTransformComponent>()->Transform().Right() * (myWidth / 2.0f) * 100.0f;
	myVertices[2] = vector + GetParent().GetComponent<CTransformComponent>()->Transform().Right() * (myWidth / 2.0f) * 100.0f;
=======
	DirectX::SimpleMath::Vector3 vector = GameObject().GetComponent<CTransformComponent>()->Position() + GameObject().GetComponent<CTransformComponent>()->Position().Forward * myHeight;

	myVertices[0] = GameObject().GetComponent<CTransformComponent>()->Position();
	myVertices[1] = vector - GameObject().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f);
	myVertices[2] = vector + GameObject().GetComponent<CTransformComponent>()->Position().Right * (myWidth / 2.0f);
>>>>>>> f7652e0... Overhaul of everything

	if (GetAsyncKeyState('C')) {

		CDebug::GetInstance()->DrawLine(myVertices[0], myVertices[1]);
		CDebug::GetInstance()->DrawLine(myVertices[0], myVertices[2]);
		CDebug::GetInstance()->DrawLine(myVertices[1], myVertices[2]);
	}
}

void CTriangleColliderComponent::Collided(CGameObject* /*aCollidedGameObject*/) {
}

void CTriangleColliderComponent::OnEnable()
{
}

void CTriangleColliderComponent::OnDisable()
{
}
