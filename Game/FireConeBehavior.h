#pragma once
#include "AbilityBehavior.h"

namespace DirectX
{
	namespace SimpleMath
	{
		struct Vector3;
	}
}

namespace SM = DirectX::SimpleMath;

class CFireConeBehavior : public IAbilityBehavior {
public:
	CFireConeBehavior(float aDuration, float aResourceCost, float aDamage, CGameObject* aParent);
	~CFireConeBehavior() override;

	void Init(CGameObject* aCaster) override;
	void Update(CGameObject* aParent) override;

private:
	DirectX::SimpleMath::Vector3 myDirection;
	CGameObject* myParent;
};
