#pragma once
#include "Collider.h"

namespace DirectX {
	namespace SimpleMath {
		struct Vector3;
	}
}

class CTriangleColliderComponent : public CCollider
{
	friend class CIntersectionManager;

public:
	CTriangleColliderComponent(CGameObject& aParent, float aWidth, float aHeight, ECollisionLayer aCollisionLayer, uint64_t someCollisionFlags);
	~CTriangleColliderComponent() override;

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
	DirectX::SimpleMath::Vector3 const GetLeftVertex() const { return myLeftVertex; }
	DirectX::SimpleMath::Vector3 const GetRightVertex() const { return myRightVertex; }

private:
	const float myHeight;
	const float myWidth;
	DirectX::SimpleMath::Vector3 myLeftVertex;
	DirectX::SimpleMath::Vector3 myRightVertex;
};

