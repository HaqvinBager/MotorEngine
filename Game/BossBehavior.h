#pragma once
#include "AIBehavior.h"

class CGameObject;
class CAbilityComponent;
class CCanvas;

class CBossBehavior : public IAIBehavior
{
public:
	CBossBehavior(CGameObject* aPlayerObject, CScene& aScene, Vector2 aPhaseOne = { 0,0 }, Vector2 aPhaseTwo = { 0,0 }, Vector2 aPhaseThree = {0,0});//Add Phases Here maybe..? :DD
	~CBossBehavior() override;

	void Update(CGameObject* aParent) override;
	void Collided(CGameObject* aParent, CGameObject* aCollidedWithGameObject) override;

	bool FindATarget(CGameObject& aParent);

	void TakeDamage(float aDamage, CGameObject* aGameObject) override;

private:
	void IdlePhase(CGameObject* aParent);
	void StartPhase(CGameObject* aParent);
	void MidPhase(CGameObject* aParent);
	void FinalPhase(CGameObject* aParent);
	void Die(CGameObject* aParent);

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
	CCanvas* myCanvas;
	Phase myPhase;
	std::vector<Vector2> myPhasePercents;

	bool myIsVeryDead;
	float mySendDeathMessageTimer = 5.0f;
	bool myFoundPlayer;
	
	bool myHasFadedOut;
};

