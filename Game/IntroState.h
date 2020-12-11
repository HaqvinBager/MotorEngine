#pragma once
#include "State.h"
#include "StateStack.h"
#include "Observer.h"

class CScene;
class CSpriteInstance;

class CIntroState : public CState, public IObserver
{
public:
	CIntroState(CStateStack& aStateStack, const CStateStack::EState aState = CStateStack::EState::IntroState);
	~CIntroState() override;

	void Awake() override;
	void Start() override;
	void Stop() override;
	void Update() override;

	void Receive(const SMessage& aMessage) override;

private:
	CScene* myScene;
	CSpriteInstance* myBackground;
	float myFeedbackTimer;
	float myFeedbackThreshold;
	bool myHasShownTutorial;
	bool myWillShowTutorial;
	bool myIntroStateActive;
	bool myNarrationIsFinished;
	bool myIntroDialogueStarted;
};