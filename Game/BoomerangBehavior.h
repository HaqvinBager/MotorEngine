#pragma once
#include "AbilityBehavior.h"

namespace DirectX {
	namespace SimpleMath {
		struct Vector3;
	}
}

class CBoomerangBehavior: public IAbilityBehavior
{
public:
	CBoomerangBehavior(float aSpeed, float aDuration, float aResourceCost, float aRotationalSpeed, float aDamage);
	~CBoomerangBehavior() override;

	void Update(CGameObject* aParent) override;
	void CalculateDirection(DirectX::SimpleMath::Vector3 aFirstPosition, DirectX::SimpleMath::Vector3 aSecondPosition);
	void Init(CGameObject* aCaster) override;

private:
	DirectX::SimpleMath::Vector3 myDirection;
	DirectX::SimpleMath::Vector3 myTargetPosition;
	CGameObject* myCaster;

	float mySpeed;
	float myHalfLife;
	float myRotationalSpeed;
	bool myIsReturning;
};

