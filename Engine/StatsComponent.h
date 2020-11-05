#pragma once
#include "Component.h"

class CGameObject;

class CStatsComponent : public CComponent
{
public:
	CStatsComponent(CGameObject& aParent, float aHealth = 0.f, float aDamage = 0.f, float aMoveSpeed = 0.f, float aDamageCooldown = 0.f);
	~CStatsComponent() override;

	void Awake() override;
	void Start() override;
	void Update() override;

	void TakeDamage(float aDamage);
	float GetDamage() const;

	float GetHealth() const;

	float GetMoveSpeed() const;

private:
	float myHealth;
	float myDamage;
	float myMoveSpeed;
	float myDamageCooldown;
	float myBaseDamageCooldown;
	bool canTakeDamage;
};

