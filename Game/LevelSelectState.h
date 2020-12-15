#pragma once
#include "State.h"
#include "StateStack.h"
#include "Observer.h"

class CScene;
class CSpriteInstance;
class CCanvas;

class CLevelSelectState : public CState, public IObserver
{
public:
	CLevelSelectState(CStateStack& aStateStack, const CStateStack::EState aState = CStateStack::EState::LevelSelect);
	~CLevelSelectState() override;

	void Awake() override;
	void Start() override;
	void Stop() override;
	void Update() override;

	void Receive(const SMessage& aMessage) override;

private:
	UINT myGrupp;
	CScene* myScene;
	CSpriteInstance* myBackground;
	CCanvas* myCanvas;
};
