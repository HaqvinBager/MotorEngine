#pragma once

class CGameObject;

class IAbilityBehavior {
public:
	IAbilityBehavior();
	virtual ~IAbilityBehavior() = 0;

	virtual void Update(CGameObject* aParent) = 0;
	void Collided(CGameObject* aGameObjec);
	virtual void Init(CGameObject* aCaster) = 0;

public:
	DirectX::SimpleMath::Vector3 myDirection;
	CGameObject* myCaster;
	float myDuration;
	float myTimer;
	float myResourceCost;
	float myDamageMultiplier;
};
