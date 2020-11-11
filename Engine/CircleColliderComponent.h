#pragma once
#include "Behaviour.h"

namespace DirectX {
	namespace SimpleMath {
		struct Vector3;
	}
}

class CCircleColliderComponent : public CBehaviour
{
	friend class CIntersectionManager;

public:
	CCircleColliderComponent(CGameObject& aParent, float aRadius, bool isStatic);
	~CCircleColliderComponent() override;

	void Awake() override;
	void Start() override;
	void Update()override;

	void Collided(CGameObject* aCollidedGameObject) override;

	void OnEnable() override;
	void OnDisable() override;

private:
	float myRadius;
	bool myIsStatic;
	DirectX::SimpleMath::Vector3 myPosition;
};


