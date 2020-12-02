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
	CSpeedExplodeBehavior(float aDuration, float aExpodeTime, float aMovementSpeedMultiplier, CGameObject* anAbilityObject);
	~CSpeedExplodeBehavior() override;

	void Init(CGameObject* aCaster) override;
	void Update(CGameObject* aParent) override;

private:
	float myExplosionDelay;
	float myMovementSpeedMutiplier;
	float myOriginalMovementSpeed;
	float myMultipliedSpeed;
	CGameObject* myCaster; //Ex Player
	CGameObject* myParent; //AbilityComponent Object
	CTransformComponent* myCasterTransform;
};

