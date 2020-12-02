#pragma once
#include "Behaviour.h"
#include "InputObserver.h"
#include "PostMaster.h"

class CPlayerControllerComponent : public CBehaviour, public IInputObserver
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

	void ResetPlayer();
	void MessagePostmaster(EMessageType aMessageType, float aValue);
	bool PlayerIsAlive();
	void RegenerateMana();
private:
	float myLastHP;
	float myRegenerationSpeed;
};