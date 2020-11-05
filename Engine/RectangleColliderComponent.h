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
	CRectangleColliderComponent(CGameObject& aParent, DirectX::SimpleMath::Vector2 aColliderSize, bool isStatic);
	~CRectangleColliderComponent() override;

	void Awake() override;
	void Start() override;
	void Update()override;
	void Collided(CGameObject* aCollidedGameObject) override;

private:
	bool myIsStatic;
	DirectX::SimpleMath::Vector2 myColliderSize;
	DirectX::SimpleMath::Vector2 myMinPoint;
	DirectX::SimpleMath::Vector2 myMaxPoint;
	DirectX::SimpleMath::Vector3 myPosition;
};


