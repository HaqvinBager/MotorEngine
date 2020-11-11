#pragma once
#include "Behaviour.h"
#include "InputObserver.h"

class CPlayerControllerComponent : public CBehaviour, public CInputObserver
{
public:
	CPlayerControllerComponent(CGameObject& aParent);
	~CPlayerControllerComponent() override;
	void Awake() override;
	void Start() override;
	void Update() override;

	void OnEnable() override;
	void OnDisable() override;

	void RecieveEvent(const CInputObserver::EInputEvent aEvent, const float aValue) override;
private:

};