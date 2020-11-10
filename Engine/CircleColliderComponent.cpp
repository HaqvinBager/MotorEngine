#include "stdafx.h"
#include "CircleColliderComponent.h"
#include "CollisionManager.h"
#include "TransformComponent.h"
#include "Debug.h"

CCircleColliderComponent::CCircleColliderComponent(CGameObject& aParent, float aRadius, bool isStatic) :
	CBehaviour(aParent),
	myRadius(aRadius),
	myIsStatic(isStatic)
{
	CCollisionManager::GetInstance()->RegisterCollider(this);
}

CCircleColliderComponent::~CCircleColliderComponent() {
}

void CCircleColliderComponent::Awake() {
	myPosition = GetParent().GetComponent<CTransformComponent>()->Position();
}

void CCircleColliderComponent::Start() {
}

void CCircleColliderComponent::Update() {
	if (!myIsStatic) {
		myPosition = GetParent().GetComponent<CTransformComponent>()->Position();
	}

	if (GetAsyncKeyState('C')) {
		CDebug::GetInstance()->DrawLine({ myPosition.x - (myRadius / 2.0f), myPosition.y, myPosition.z }, { myPosition.x + (myRadius / 2.0f), myPosition.y, myPosition.z });
		CDebug::GetInstance()->DrawLine({ myPosition.x, myPosition.y, myPosition.z - (myRadius / 2.0f) }
		

		, { myPosition.x, myPosition.y, myPosition.z + (myRadius / 2.0f) });
	}
}

void CCircleColliderComponent::Collided(CGameObject* /*aCollidedGameObject*/) {
}

void CCircleColliderComponent::OnEnable()
{

}
void CCircleColliderComponent::OnDisable()
{

}
