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
	CBoomerangBehavior(float aSpeed);
	~CBoomerangBehavior() override;

	void Update(CGameObject* aParent) override;
	bool CheckDistance(DirectX::SimpleMath::Vector3 aFirstPosition, DirectX::SimpleMath::Vector3 aSecondPosition);
	void CalculateDirection(DirectX::SimpleMath::Vector3 aFirstPosition, DirectX::SimpleMath::Vector3 aSecondPosition);
	void Init(CGameObject* aGameObject) override;

private:
	DirectX::SimpleMath::Vector3 myDirection;
	DirectX::SimpleMath::Vector3 myTargetPosition;
	CGameObject* myCaster;

	float mySpeed;
	bool myIsReturning;
};

