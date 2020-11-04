#include "stdafx.h"
#include "CircleColliderComponent.h"
#include "CollisionManager.h"
#include "TransformComponent.h"

CCircleColliderComponent::CCircleColliderComponent(CGameObject& aParent, float aRadius, bool isStatic) :
	CComponent(aParent),
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
}

void CCircleColliderComponent::Collided(CGameObject* /*aCollidedGameObject*/) {
}
