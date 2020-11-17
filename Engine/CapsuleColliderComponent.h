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
	CCapsuleColliderComponent(CGameObject& aParent, float aRadius, float aHeight, ECollisionLayer aCollisionLayer, uint64_t someCollisionFlags);
	~CCapsuleColliderComponent() override;

public:
	void Awake() override;
	void Start() override;
	void Update()override;

public:
	bool Collided(CCircleColliderComponent* aCollidedGameObject) override;
	bool Collided(CTriangleColliderComponent* aCollidedGameObject) override;
	bool Collided(CRectangleColliderComponent* aCollidedGameObject) override;
	bool Collided(CCollider* aCollidedGameObject) override;

public:
	void OnEnable() override;
	void OnDisable() override;

public:
	float const GetRadius() const;
	void SetRadius(float aRadius);
	float const GetHeight() const;
	void SetHeight(float aHeight);

private:
	float myRadius;
	float myHeight;
	DirectX::SimpleMath::Vector3 myTop;
};

