#pragma once
#include <Behaviour.h>
#include "AbilityComponent.h"

class CGameObject;
class IAbilityBehavior;

class CAbilityBehaviorComponent : public CBehaviour
{
public:
	CAbilityBehaviorComponent(CGameObject& aParent, IAbilityBehavior* aBehavior, EAbilityType anAbilityType);
	~CAbilityBehaviorComponent() override;

	void Awake() override;
	void Start() override;
	void Update() override;

	void OnEnable() override;
	void OnDisable() override;

	EAbilityType GetAbilityType() const;

	void Init(DirectX::SimpleMath::Vector3 aCasterPosition);

private:
	IAbilityBehavior* myBehavior;
	EAbilityType myAbilityType;
};
