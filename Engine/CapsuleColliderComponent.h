#pragma once
#include "Component.h"

namespace DirectX {
	namespace SimpleMath {
		struct Vector3;
	}
}

class CCapsuleColliderComponent : public CComponent
{
public:
	CCapsuleColliderComponent(CGameObject& aParent) : CComponent(aParent) { }
	~CCapsuleColliderComponent() override;

	void Awake() override;
	void Start() override;
	void Update()override;

private:
	float myRadius;
	DirectX::SimpleMath::Vector3 myBase;
	DirectX::SimpleMath::Vector3 myTip;
};

