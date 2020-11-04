#include "stdafx.h"
#include "RectangleColliderComponent.h"
#include "CollisionManager.h"
#include "TransformComponent.h"

CRectangleColliderComponent::CRectangleColliderComponent(CGameObject& aParent, DirectX::SimpleMath::Vector2 aColliderSize, bool isStatic) :
	CComponent(aParent),
	myColliderSize(aColliderSize),
	myIsStatic(isStatic)
{
	CCollisionManager::GetInstance()->RegisterCollider(this);
}

CRectangleColliderComponent::~CRectangleColliderComponent() {
}

void CRectangleColliderComponent::Awake() {
	myPosition = GetParent().GetComponent<CTransformComponent>()->Position();

	myMinPoint = { myPosition.x - myColliderSize.x / 2.0f, myPosition.y - myColliderSize.y / 2.0f };
	myMaxPoint = { myPosition.x + myColliderSize.x / 2.0f, myPosition.y + myColliderSize.y / 2.0f };
}

void CRectangleColliderComponent::Start() {
}

void CRectangleColliderComponent::Update() {
	if (!myIsStatic) {
		myPosition = GetParent().GetComponent<CTransformComponent>()->Position();

		myMinPoint = { myPosition.x - myColliderSize.x / 2.0f, myPosition.y - myColliderSize.y / 2.0f };
		myMaxPoint = { myPosition.x + myColliderSize.x / 2.0f, myPosition.y + myColliderSize.y / 2.0f };
	}
}

void CRectangleColliderComponent::Collided(CGameObject* /*aCollidedGameObject*/) {
}
