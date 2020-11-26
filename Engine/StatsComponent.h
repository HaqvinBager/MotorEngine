#pragma once
#include "Behaviour.h"
#include "Observer.h"

class CGameObject;
class CToken;
class CPointLight;

struct SBaseStats {
	const float myBaseHealth;
	const float myDamage;
	const float myMoveSpeed;
	const float myBaseDamageCooldown;
};

struct SStats {
	float myHealth;
	float myDamageCooldown;
	bool myCanTakeDamage;
	bool myCanAttack;
	CToken* myTokenSlot;
};

class CStatsComponent : public CBehaviour
{
public:
	CStatsComponent(CGameObject& aParent, float aHealth = 0.f, float aDamage = 0.f, float aMoveSpeed = 0.f, float aDamageCooldown = 0.f);
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