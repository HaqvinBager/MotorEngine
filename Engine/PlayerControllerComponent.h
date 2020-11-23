#pragma once
#include "Behaviour.h"
#include "InputObserver.h"

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

	void RecieveEvent(const EInputEvent aEvent) override;

private:

};