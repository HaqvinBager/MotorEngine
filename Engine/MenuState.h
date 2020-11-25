#pragma once
#include "State.h"
#include "Observer.h"

class CSpriteInstance;
class CCanvas;

class CMenuState : public CState, public IObserver{
public:
	CMenuState(CStateStack& aStateStack);
	~CMenuState() override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void Receive(const SMessage& aMessage) override;

private:
	CCanvas* myCanvas;
};