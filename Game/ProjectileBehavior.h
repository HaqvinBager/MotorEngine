#pragma once
#include "AbilityBehavior.h"

namespace DirectX {
	namespace SimpleMath {
		struct Vector3;
	}
}

class CProjectileBehavior : public IAbilityBehavior
{
public:
	CProjectileBehavior(float aSpeed, float aDuration);
	~CProjectileBehavior() override;

	void Update(CGameObject* aParent) override;

	void Init(DirectX::SimpleMath::Vector3 aCasterPosition) override;

private:
	DirectX::SimpleMath::Vector3 myDirection;
	float mySpeed;
};

