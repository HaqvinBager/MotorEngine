#pragma once

class CGameObject;

class IAbilityBehavior {
public:
	virtual ~IAbilityBehavior() = 0;

	virtual void Update(CGameObject* aParent) = 0;

	virtual void Init(CGameObject* aCaster) = 0;

public:
	float myDuration;
	float myTimer;
};