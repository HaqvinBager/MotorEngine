#pragma once
#include "Behaviour.h"
#include "InputObserver.h"
#include "PostMaster.h"
#include "ObjectPool.h"

class CMouseSelection;

class CPlayerControllerComponent : public CBehaviour, public IInputObserver, public IObserver
{
public:
	CPlayerControllerComponent(CGameObject& aParent);
	~CPlayerControllerComponent() override;
	void Awake() override;
	void Start() override;
	void Update() override;

	void OnEnable() override;
	void OnDisable() override;
	void ReceiveEvent(const EInputEvent aEvent) override;
	void Receive(const SMessage& aMessage) override;
	void ResetPlayer();
	void MessagePostmaster(EMessageType aMessageType, float aValue);
	bool PlayerIsAlive();
	void TakeDamage(float aDamageMultiplier, CGameObject* aGameObject);
	void RegenerateMana();
	void UpdateExperience(const SMessage& aMessage);
	bool AuraActive(){ return myAuraActive; }
private:
	float myLastHP;
	float myRegenerationSpeed;
	CMouseSelection* mySelection;
	CGameObject* myTargetEnemy;
	bool myIsMoving;
	bool myMiddleMousePressed;
	bool myAuraActive;
	DirectX::SimpleMath::Vector3 myLastPosition;
};