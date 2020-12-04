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
class CGameObject;

class CMeleeAttackBehavior : public IAbilityBehavior {
public:
	CMeleeAttackBehavior(float aDuration, CGameObject* aParent);
	~CMeleeAttackBehavior() override;

	void Init(CGameObject* aCaster) override;
	void Update(CGameObject* aParent) override;

private:
	DirectX::SimpleMath::Vector3 myDirection;
	CGameObject* myCaster;
	CGameObject* myParent;
};
