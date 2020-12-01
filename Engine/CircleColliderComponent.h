#pragma once
#include "Collider.h"

#ifdef _DEBUG
class CLineInstance;
#endif

namespace DirectX {
	namespace SimpleMath {
		struct Vector3;
	}
}

class CCircleColliderComponent : public CCollider
{
public:
	CCircleColliderComponent(CGameObject& aParent);
	CCircleColliderComponent(CGameObject& aParent, float aRadius);
	CCircleColliderComponent(CGameObject& aParent, float aRadius, ECollisionLayer aCollisionLayer, uint64_t someCollisionFlags);
	~CCircleColliderComponent() override;

public:
	void Awake() override;
	void Start() override;
	void Update()override;

public:
	bool Collided(CCircleColliderComponent* aCollidedGameObject) override;
	bool Collided(CRectangleColliderComponent* aCollidedGameObject) override;
	bool Collided(CTriangleColliderComponent* aCollidedGameObject) override;
	bool Collided(CCollider* aCollidedGameObject) override;

public:
	void OnEnable() override;
	void OnDisable() override;

public:
	float const GetRadius() const;
	void SetRadius(float aRadius);

private:
	float myRadius;

#ifdef _DEBUG
private:
	CLineInstance* myVisualizer;
#endif
};


