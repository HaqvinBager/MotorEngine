#pragma once
#include "AIBehavior.h"

class CGameObject;

class CEnemyBehavior : public IAIBehavior
{
public:
	CEnemyBehavior(CGameObject* aPlayerObject);
	~CEnemyBehavior() override;

	void Update(CGameObject* aParent) override;
	void Collided(CGameObject* aParent, CGameObject* aCollidedWithGameObject) override;

	void FindATarget();

	void TakeDamage(float someDamage);

	void Die();

private:
	CGameObject* myPlayer;
	CGameObject* myCurrentParent;
};

