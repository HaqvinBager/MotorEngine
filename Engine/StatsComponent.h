#pragma once
#include "Behaviour.h"
#include "Observer.h"

class CGameObject;
class CToken;
class CPointLight;

struct SBaseStats {
	const float myBaseHealth;
	const float myBaseResource;
	const float myDamage;
	const float myMoveSpeed;
	const float myBaseDamageCooldown;
	const float myBaseVisionRange;
	const float myBaseAttackRange;
	const float mBaseRandomWalkTime;
};

struct SStats {
	float myHealth;
	float myResource;
	float myDamageCooldown;
	float myDamageMultiplier;
	bool myCanTakeDamage;
	bool myCanAttack;
	CToken* myTokenSlot;
	float myRandomWalkTime;
};

class CStatsComponent : public CBehaviour
{
public:
	CStatsComponent(CGameObject& aParent, float aHealth = 0.f, float aDamage = 0.f, float aMoveSpeed = 0.f, float aDamageCooldown = 0.f, float aVisionRange = 0.f, float aAttackRange = 0.f);
	~CStatsComponent() override;

	void Awake() override;
	void Start() override;
	void Update() override;

	void OnEnable() override;
	void OnDisable() override;

	const SBaseStats& GetBaseStats() const;
	SStats& GetStats();

private:
	SBaseStats myBaseStats;
	SStats myStats;
};