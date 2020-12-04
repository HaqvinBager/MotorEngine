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

	void Awake() override;
	void Start() override;
	void Update() override;
	void Receive(const SMessage& aMessage) override;
	void MakeSceneActive() override;
private:
	CCanvas* myCanvas;
	CScene* myScene;
};