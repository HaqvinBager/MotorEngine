#pragma once
#include "Behaviour.h"
#include "Observer.h"

class CGameObject;
class CToken;
class CPointLight;

class CStatsComponent : public CBehaviour, public IObserver
{
public:
	CStatsComponent(CGameObject& aParent, float aHealth = 0.f, float aDamage = 0.f, float aMoveSpeed = 0.f, float aDamageCooldown = 0.f);
	~CStatsComponent() override;

	void Awake() override;
	void Start() override;
	void Update() override;

	void OnEnable() override;
	void OnDisable() override;

	void Receive(const SMessage& aMessage) override;

	void FindATarget(CGameObject& aTarget);

	void TakeDamage(float aDamage);
	float GetDamage();

	float GetHealth() const;

	float GetMoveSpeed() const;

	CToken* GetToken() const;

private:
	float myHealth;
	float myDamage;
	float myMoveSpeed;
	float myDamageCooldown;
	float myBaseDamageCooldown;
	bool canTakeDamage;
	bool canAttack;
	CToken* myTokenSlot;
	CPointLight* myPointLight;
};