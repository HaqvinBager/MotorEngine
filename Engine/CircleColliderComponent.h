#pragma once
#include "Collider.h"

namespace DirectX {
	namespace SimpleMath {
		struct Vector3;
	}
}

class CCircleColliderComponent : public CCollider
{
	friend class CIntersectionManager;

public:
	CCircleColliderComponent(CGameObject& aParent, float aRadius, ECollisionLayer aCollisionLayer);
	~CCircleColliderComponent() override;

	void Awake() override;
	void Start() override;
	void Update()override;

	bool Collided(CCircleColliderComponent* aCollidedGameObject) override;
	bool Collided(CRectangleColliderComponent* aCollidedGameObject) override;
	bool Collided(CTriangleColliderComponent* aCollidedGameObject) override;
	bool Collided(CCollider* aCollidedGameObject) override;

	void OnEnable() override;
	void OnDisable() override;

public:
	float const GetRadius() const { return myRadius; };

private:
	float myRadius;
};


