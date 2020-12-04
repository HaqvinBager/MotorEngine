#pragma once
#include "AIBehavior.h"

class CGameObject;

class CBossBehavior : public IAIBehavior
{
public:
	CBossBehavior(CGameObject* aPlayerObject);
	~CBossBehavior() override;

	void Update(CGameObject* aParent) override;
	void Collided(CGameObject* aGameObject) override;

	void FindATarget(CGameObject& aParent);

	void TakeDamage(float aDamage);

private:
	CGameObject* myPlayer;
};

