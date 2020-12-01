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

class CRectangleColliderComponent : public CCollider
{
public:
	CRectangleColliderComponent(CGameObject& aParent);
	CRectangleColliderComponent(CGameObject& aParent, float aWidth, float aHeight);
	CRectangleColliderComponent(CGameObject& aParent, float aWidth, float aHeight, ECollisionLayer aCollisionLayer, uint64_t someCollisionFlags);
	~CRectangleColliderComponent() override;

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
	float const GetHeight() const;
	void SetHeight(float aHeight);
	float const GetWidth() const;
	void SetWidth(float aWidth);
	DirectX::SimpleMath::Vector3 const GetMin() const { return myMin; }
	DirectX::SimpleMath::Vector3 const GetMax() const { return myMax; }

private:
	float myWidth;
	float myHeight;
	DirectX::SimpleMath::Vector3 myMin;
	DirectX::SimpleMath::Vector3 myMax;

#ifdef _DEBUG
private:
	CLineInstance* myVisualizer;
#endif
};


