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

class CTriangleColliderComponent : public CCollider
{
public:
	/**
	* Initializes collider with 0.5f in width and height
	* Initializes collider with no collision layer and flags
	*/
	CTriangleColliderComponent(CGameObject& aParent);
	/**
	* Initializes collider with no collision layer and flags
	*/
	CTriangleColliderComponent(CGameObject& aParent, float aWidth, float aHeight);
	/**
	* Initializes with custom data
	*/
	CTriangleColliderComponent(CGameObject& aParent, float aWidth, float aHeight, ECollisionLayer aCollisionLayer, uint64_t someCollisionFlags);
	~CTriangleColliderComponent() override;

public:
	void Awake() override;
	void Start() override;
	void Update()override;
	
	bool Collided(CCircleColliderComponent* aCollidedGameObject) override;
	bool Collided(CTriangleColliderComponent* aCollidedGameObject) override;
	bool Collided(CRectangleColliderComponent* aCollidedGameObject) override;
	bool Collided(CCollider* aCollidedGameObject) override;

	void OnEnable() override;
	void OnDisable() override;

	float const GetHeight() const;
	void SetHeight(float aHeight);
	float const GetWidth() const;
	void SetWidth(float aWidth);
	DirectX::SimpleMath::Vector3 const GetLeftVertex() const { return myLeftVertex; }
	DirectX::SimpleMath::Vector3 const GetRightVertex() const { return myRightVertex; }
	void SetLeftVertex(DirectX::SimpleMath::Vector3& aLeftVertex);
	void SetRightVertex(DirectX::SimpleMath::Vector3& aRightVertex);

private:
	float myHeight;
	float myWidth;
	DirectX::SimpleMath::Vector3 myLeftVertex;
	DirectX::SimpleMath::Vector3 myRightVertex;

#ifdef _DEBUG
private:
	CLineInstance* myVisualizer;
#endif
};

