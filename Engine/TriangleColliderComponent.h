#pragma once
#include "Behaviour.h"

namespace DirectX {
	namespace SimpleMath {
		struct Vector3;
	}
}

class CTriangleColliderComponent : public CBehaviour
{
	friend class CIntersectionManager;

public:
	CTriangleColliderComponent(CGameObject& aParent, float aWidth, float aHeight);
	~CTriangleColliderComponent() override;

	void Awake() override;
	void Start() override;
	void Update()override;
	void Collided(CGameObject* aCollidedGameObject) override;

	void OnEnable() override;
	void OnDisable() override;

private:
	const float myHeight;
	const float myWidth;
	std::vector<DirectX::SimpleMath::Vector3> myVertices;
};

