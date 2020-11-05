#pragma once
#include "Component.h"

namespace DirectX {
	namespace SimpleMath {
		struct Vector3;
	}
}

class CTriangleColliderComponent : public CComponent
{
	friend class CIntersectionManager;

public:
	CTriangleColliderComponent(CGameObject& aParent, std::vector<DirectX::SimpleMath::Vector3> someVertices);
	~CTriangleColliderComponent() override;

	void Awake() override;
	void Start() override;
	void Update()override;
	void Collided(CGameObject* aCollidedGameObject) override;

private:
	std::vector<DirectX::SimpleMath::Vector3> myVertices;
};

