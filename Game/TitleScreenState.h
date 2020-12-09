#pragma once
#include "State.h"
#include "StateStack.h"
#include "InputObserver.h"
#include "Observer.h"

class CScene;
class CSpriteInstance;

class CTitleScreenState : public CState, public IInputObserver, public IStringObserver
{
public:
	CTitleScreenState(CStateStack& aStateStack, const CStateStack::EState aState = CStateStack::EState::TitleScreenState);
	~CTitleScreenState() override;

	void Awake() override;
	void Start() override;
	void Stop() override;
	void Update() override;

	void ReceiveEvent(const EInputEvent anEvent) override;
	void Receive(const SStringMessage& aMessage) override;

private:
	CScene* myScene;
	CSpriteInstance* myBackground;
	bool myTitleScreenActive;
};