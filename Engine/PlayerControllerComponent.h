#pragma once
#include "Component.h"
#include "InputObserver.h"

class CPlayerControllerComponent : public CComponent, public CInputObserver
{
public:
	CPlayerControllerComponent(CGameObject& aParent);
	~CPlayerControllerComponent() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void RecieveEvent(const CInputObserver::EInputEvent aEvent, const float aValue) override;
private:

};