#pragma once
#include "Collider.h"

namespace DirectX {
	namespace SimpleMath {
		struct Vector3;
	}
}

class CCapsuleColliderComponent : public CCollider
{
	friend class CIntersectionManager;

public:
	CCapsuleColliderComponent(CGameObject& aParent, float aRadius, float aHeight, ECollisionLayer aCollisionLayer);
	~CCapsuleColliderComponent() override;

	void Awake() override;
	void Start() override;
	void Update()override;

	bool Collided(CCircleColliderComponent* aCollidedGameObject) override;
	bool Collided(CTriangleColliderComponent* aCollidedGameObject) override;
	bool Collided(CRectangleColliderComponent* aCollidedGameObject) override;
	bool Collided(CCollider* aCollidedGameObject) override;

	void OnEnable() override;
	void OnDisable() override;

private:
	float myRadius;
	float myHeight;
	DirectX::SimpleMath::Vector3 myTop;
};

