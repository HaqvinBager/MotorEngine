#pragma once
#include "State.h"
#include "Observer.h"

class CSpriteInstance;
class CCanvas;
class CScene;

class CPauseState : public CState, public IObserver {
public:
	CPauseState(CStateStack& aStateStack, const CStateStack::EState aState = CStateStack::EState::PauseMenu);
	~CPauseState() override;

	void Start() override;
	void Stop() override;
	void Update() override;
	void Receive(const SMessage& aMessage) override;
private:
	void Awake() override;

	CCanvas* myCanvas;
	CScene* myScene;
};