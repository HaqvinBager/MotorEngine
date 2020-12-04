#pragma once
#include "State.h"
#include "Observer.h"

class CSpriteInstance;
class CCanvas;
class CScene;

class CMenuState : public CState, public IObserver {
public:
	CMenuState(CStateStack& aStateStack, const CStateStack::EState = CStateStack::EState::MainMenu);
	~CMenuState() override;

	void Awake() override;
	void Start() override;
	void Stop() override;
	void Update() override;
	void Receive(const SMessage &aMessage) override;
private:
	CCanvas* myCanvas;
	CScene* myScene;
};