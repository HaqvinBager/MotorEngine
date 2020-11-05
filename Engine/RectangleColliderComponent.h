#pragma once
#include "Component.h"

namespace DirectX {
	namespace SimpleMath {
		struct Vector3;
	}
}

class CRectangleColliderComponent : public CComponent {
	friend class CIntersectionManager;

public:
	CRectangleColliderComponent(CGameObject& aParent, float aWidth, float aHeight, bool isStatic);
	~CRectangleColliderComponent() override;

	void Awake() override;
	void Start() override;
	void Update()override;
	void Collided(CGameObject* aCollidedGameObject) override;

private:
	bool myIsStatic;
	float myWidth;
	float myHeight;
	std::vector<DirectX::SimpleMath::Vector3> myVertices;
	DirectX::SimpleMath::Vector3 myPosition;
};


