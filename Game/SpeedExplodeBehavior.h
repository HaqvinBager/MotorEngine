#pragma once
#include "AbilityBehavior.h"

class CTransformComponent;

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
	CSpeedExplodeBehavior(float aDuration, float aResourceCost, float aExplosionDelay, float aMovementSpeedMultiplier, CGameObject* anAbilityObject);
	~CSpeedExplodeBehavior() override;

	void Init(CGameObject* aCaster) override;
	void Update(CGameObject* aParent) override;

	float ResourceCost() const;

private:
	float myExplosionDelay;
	float myMovementSpeedMutiplier;
	float myOriginalMovementSpeed;
	float myMultipliedSpeed;
	CGameObject* myCaster;
	CGameObject* myParent;
	CTransformComponent* myCasterTransform;
};

