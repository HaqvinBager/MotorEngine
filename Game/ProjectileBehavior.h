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
	CProjectileBehavior(float aSpeed, float aDuration, float aDamage);
	~CProjectileBehavior() override;

	void Update(CGameObject* aParent) override;
	void Collided(CGameObject* aGameObject) override;
	void Init(CGameObject* aCaster) override;

private:
	DirectX::SimpleMath::Vector3 myDirection;
	float mySpeed;
};

