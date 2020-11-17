#pragma once
#include "Behaviour.h"
#include "CollisionManager.h"

class CCollider : public CBehaviour
{
public:
	CCollider(CGameObject& aParent, ECollisionLayer aCollisionLayer, uint64_t someCollisionFlags);
	virtual bool Collided(CCircleColliderComponent* aCollidedGameObject) = 0;
	virtual bool Collided(CTriangleColliderComponent* aCollidedGameObject) = 0;
	virtual bool Collided(CRectangleColliderComponent* aCollidedGameObject) = 0;
	virtual bool Collided(CCollider* aCollidedGameObject) = 0;
	void OnEnable() override;
	void OnDisable() override;
	
public:
	const ECollisionLayer GetCollisionLayer() const;
	void SetCollisionLayer(ECollisionLayer aCollisionLayer);
	const DirectX::SimpleMath::Vector3 GetPosition() const;
	void SetPosition(DirectX::SimpleMath::Vector3 aPosition);
	const uint64_t GetCollisionFlags() const;
	void SetCollisionFlags(uint64_t someCollisionFlags);

private:
	uint64_t myCollisionFlags;
	ECollisionLayer myCollisionLayer;
	DirectX::SimpleMath::Vector3 myPosition;
};