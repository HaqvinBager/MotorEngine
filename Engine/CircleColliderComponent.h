#pragma once
#include "Component.h"

namespace DirectX {
	namespace SimpleMath {
		struct Vector3;
	}
}

class CCircleColliderComponent : public CComponent {
	friend class CIntersectionManager;

public:
	CCircleColliderComponent(CGameObject& aParent, float aRadius, bool isStatic);
	~CCircleColliderComponent() override;

	void Awake() override;
	void Start() override;
	void Update()override;
	void Collided(CGameObject* aCollidedGameObject) override;

private:
	float myRadius;
	bool myIsStatic;
	DirectX::SimpleMath::Vector3 myPosition;
};


