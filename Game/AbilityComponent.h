#pragma once
#include "Behaviour.h"
#include "InputObserver.h"

class CGameObject;

enum class EAbilityType {
	//TEMP TYPES TODO: CHANGE TO PROPER NAMES
	PlayerAbility1,
	PlayerAbility2,
	PlayerAbility3,
	PlayerMelee,
	PlayerHeavyMelee,
	EnemyAbility,
	BossAbility1,
	BossAbility2,
	BossAbility3,

	AbilityTest,
	Count = AbilityTest
};

class CAbilityComponent : public CBehaviour, public IInputObserver
{
public:
	CAbilityComponent(CGameObject& aParent, std::vector<std::pair<EAbilityType, unsigned int>> someAbilities);
	~CAbilityComponent();

	void Awake() override;
	void Start() override;
	void Update() override;

	void OnEnable() override;
	void OnDisable() override;

	bool UseAbility(EAbilityType anAbilityType, DirectX::SimpleMath::Vector3 aSpawnPosition);

	void SendEvent();
	void ReceiveEvent(const EInputEvent aEvent) override;
	const float MeleeAttackRange() const;
	void ResetCooldown(int anIndex);
	float Cooldown(int anIndex);

private:
	CGameObject* LoadAbilityFromFile(EAbilityType anAbilityType);

private:
	std::vector<std::pair<EAbilityType, unsigned int>> myAbilityPoolDescriptions;
	std::map<EAbilityType, std::vector<CGameObject*>> myAbilityPools;
	std::map<EAbilityType, std::string> myFilePaths;
	std::vector<CGameObject*> myActiveAbilities;
	std::vector<float> myCurrentCooldowns;
	std::vector<float> myMaxCooldowns;
	float myMeleeAttackRange;
};
