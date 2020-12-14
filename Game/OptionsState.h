#pragma once
#include "State.h"
#include "StateStack.h"
#include "Observer.h"

class CScene;
class CSpriteInstance;
class CCanvas;

class COptionsState : public CState, public IObserver
{
public:
	COptionsState(CStateStack& aStateStack, const CStateStack::EState aState = CStateStack::EState::Options);
	~COptionsState() override;

	void Awake() override;
	void Start() override;
	void Stop() override;
	void Update() override;

	void Receive(const SMessage& aMessage) override;

private:
	CScene* myScene;
	CSpriteInstance* myBackground;
	CCanvas* myCanvas;
};
