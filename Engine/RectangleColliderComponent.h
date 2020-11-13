#pragma once
#include "Collider.h"

namespace DirectX {
	namespace SimpleMath {
		struct Vector3;
	}
}

class CRectangleColliderComponent : public CCollider
{
	friend class CIntersectionManager;

public:
	CRectangleColliderComponent(CGameObject& aParent, float aWidth, float aHeight, ECollisionLayer aCollisionLayer, uint64_t someCollisionFlags);
	~CRectangleColliderComponent() override;

	void Awake() override;
	void Start() override;
	void Update()override;

	bool Collided(CCircleColliderComponent* aCollidedGameObject) override;
	bool Collided(CTriangleColliderComponent* aCollidedGameObject) override;
	bool Collided(CRectangleColliderComponent* aCollidedGameObject) override;
	bool Collided(CCollider* aCollidedGameObject) override;

	void OnEnable() override;
	void OnDisable() override;

public:
	float const GetHeight() const { return myHeight; }
	float const GetWidth() const { return myWidth; }
	DirectX::SimpleMath::Vector3 const GetMin() const { return myMin; }
	DirectX::SimpleMath::Vector3 const GetMax() const { return myMax; }

private:
	float myWidth;
	float myHeight;
	DirectX::SimpleMath::Vector3 myMin;
	DirectX::SimpleMath::Vector3 myMax;
};


