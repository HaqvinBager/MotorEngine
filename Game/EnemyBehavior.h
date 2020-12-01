#pragma once
#include "AIBehavior.h"

class CGameObject;

class CEnemyBehavior : public IAIBehavior
{
public:
	CEnemyBehavior(CGameObject* aPlayerObject);
	~CEnemyBehavior() override;

	void Update(CGameObject* aParent) override;
	void Collided(CGameObject* aGameObject) override;

	void FindATarget(CGameObject& aParent);

	void TakeDamage(float aDamage);

private:
	CGameObject* myPlayer;
};

