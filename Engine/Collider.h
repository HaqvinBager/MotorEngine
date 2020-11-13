#pragma once
#include "Behaviour.h"
#include "CollisionManager.h"

class CCollider : public CBehaviour
{
public:
	CCollider(CGameObject& aParent) : CBehaviour(aParent), myCollisionLayer(ECollisionLayer::NONE) {}
	virtual bool Collided(CCircleColliderComponent* aCollidedGameObject) = 0;
	virtual bool Collided(CTriangleColliderComponent* aCollidedGameObject) = 0;
	virtual bool Collided(CRectangleColliderComponent* aCollidedGameObject) = 0;
	virtual bool Collided(CCollider* aCollidedGameObject) = 0;
	void OnEnable() override;
	void OnDisable() override;
	
public:
	const ECollisionLayer GetCollisionLayer() const { return myCollisionLayer;}
	void SetCollisionLayer(ECollisionLayer aCollisionLayer);
	const DirectX::SimpleMath::Vector3 GetPosition() const { return myPosition;}
	void SetPosition(DirectX::SimpleMath::Vector3 aPosition);

private:
	ECollisionLayer myCollisionLayer;
	DirectX::SimpleMath::Vector3 myPosition;
};