#pragma once
#include "State.h"
#include "StateStack.h"
#include "InputObserver.h"
#include "Observer.h"

class CScene;
class CSpriteInstance;

class CBootUpState : public CState, public IInputObserver, public IStringObserver
{
public:
	CBootUpState(CStateStack& aStateStack, const CStateStack::EState aState = CStateStack::EState::BootUp);
	~CBootUpState() override;

	void Awake() override;
	void Start() override;
	void Stop() override;
	void Update() override;

	void ReceiveEvent(const EInputEvent anEvent) override;
	void Receive(const SStringMessage& aMessage) override;

private:
	CScene* myScene;
	std::vector<CSpriteInstance*> myLogos;
	float myTimer;
	float myLogoDisplayDuration;
	float myFadeOutStart;
	int myLogoToRender;
};

