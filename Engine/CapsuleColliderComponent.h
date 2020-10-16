#pragma once
#include "Component.h"

namespace DirectX {
	namespace SimpleMath {
		struct Vector3;
	}
}

class CCapsuleColliderComponent : public CComponent
{
	friend class CIntersectionManager;

public:
	CCapsuleColliderComponent(CGameObject& aParent, float aRadius, float aHeight);
	~CCapsuleColliderComponent() override;

	void Awake() override;
	void Start() override;
	void Update()override;
	void Collided(CGameObject* aCollidedGameObject) override;

private:
	float myRadius;
	float myHeight;
	DirectX::SimpleMath::Vector3 myBase;
	DirectX::SimpleMath::Vector3 myTip;
};

