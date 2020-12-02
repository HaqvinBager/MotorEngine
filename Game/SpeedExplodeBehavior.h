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

class CSpeedExplodeBehavior : public IAbilityBehavior {
public:
	CSpeedExplodeBehavior(float aDuration, float aExpodeTime, CGameObject* anAbilityObject);
	~CSpeedExplodeBehavior() override;

	void Init(CGameObject* aGameObject) override;
	void Update(CGameObject* aParent) override;

private:
	float myDelay;
	CGameObject* myCaster;
	CGameObject* myParent;
};

