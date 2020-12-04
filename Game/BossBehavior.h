#pragma once
#include "AIBehavior.h"

class CGameObject;
class CAbilityComponent;

class CBossBehavior : public IAIBehavior
{
public:
	CBossBehavior(CGameObject* aPlayerObject);
	~CBossBehavior() override;

	void Update(CGameObject* aParent) override;
	void Collided(CGameObject* aGameObject) override;

	bool FindATarget(CGameObject& aParent);

	void TakeDamage(float aDamage);

private:
	void IdlePhase(CGameObject* aParent);
	void StartPhase(CGameObject* aParent);
	void MidPhase(CGameObject* aParent);
	void FinalPhase(CGameObject* aParent);

	enum class Phase
	{
		Idle,
		Start,
		Mid,
		Final
	};

	float myTempAttackTimer;
	CGameObject* myPlayer;
	CAbilityComponent* myAblilityComponent;
	Phase myPhase;
};

