#include "stdafx.h"
#include "RectangleColliderComponent.h"
#include "CollisionManager.h"
#include "TransformComponent.h"

CRectangleColliderComponent::CRectangleColliderComponent(CGameObject& aParent, float aWidth, float aHeight, bool isStatic) :
	CBehaviour(aParent),
	myWidth(aWidth),
	myHeight(aHeight),
	myIsStatic(isStatic)
{
	CCollisionManager::GetInstance()->RegisterCollider(this);
}

CRectangleColliderComponent::~CRectangleColliderComponent() {
}

void CRectangleColliderComponent::Awake() {
	myPosition = GetParent().GetComponent<CTransformComponent>()->Position();
	DirectX::SimpleMath::Vector3 vector = GetParent().GetComponent<CTransformComponent>()->Position() + GetParent().GetComponent<CTransformComponent>()->Transform().Forward() * (myHeight / 2.0f);

	myMin = vector - GetParent().GetComponent<CTransformComponent>()->Transform().Right() * (myWidth / 2.0f);
	vector = GetParent().GetComponent<CTransformComponent>()->Position() - GetParent().GetComponent<CTransformComponent>()->Transform().Forward() * (myHeight / 2.0f);

	myMax = vector + GetParent().GetComponent<CTransformComponent>()->Transform().Right() * (myWidth / 2.0f);
}

void CRectangleColliderComponent::Start() {
}

#include "Debug.h"
void CRectangleColliderComponent::Update() {
	if (!myIsStatic) {
		myPosition = GetParent().GetComponent<CTransformComponent>()->Position();
		DirectX::SimpleMath::Vector3 vector = GetParent().GetComponent<CTransformComponent>()->Position() + GetParent().GetComponent<CTransformComponent>()->Transform().Forward() * (myHeight / 2.0f);

		myMin = vector - GetParent().GetComponent<CTransformComponent>()->Transform().Right() * (myWidth / 2.0f);
		vector = GetParent().GetComponent<CTransformComponent>()->Position() - GetParent().GetComponent<CTransformComponent>()->Transform().Forward() * (myHeight / 2.0f);
		

		myMax = vector + GetParent().GetComponent<CTransformComponent>()->Transform().Right() * (myWidth / 2.0f);
	}

	if (GetAsyncKeyState('C')) {
		CDebug::GetInstance()->DrawLine(myMin, myMax);
	}
}

void CRectangleColliderComponent::Collided(CGameObject* /*aCollidedGameObject*/) {
}

void CRectangleColliderComponent::OnEnable()
{
}

void CRectangleColliderComponent::OnDisable()
{
}
