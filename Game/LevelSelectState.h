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
	void Group3Sub();
	void Group3Unsub();
	void Group3Receive(const SMessage& aMessage);

	void Group4Sub();
	void Group4Unsub();
	void Group4Receive(const SMessage& aMessage);

private:
	UINT myGrupp;
	CScene* myScene;
	CSpriteInstance* myBackground;
	CCanvas* myCanvas;
};
