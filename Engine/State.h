#pragma once
#include "StateStack.h"
//class CStateStack;

class CState  {

public:
	CState(CStateStack& aStateStack, const CStateStack::EState aState);
	virtual ~CState();
	virtual void MakeSceneActive() = 0;
	virtual void Awake() = 0;
	virtual void Start() = 0;
	virtual void Update() = 0;

	CStateStack::EState GetState() { return myState; }

protected:
	CStateStack& myStateStack;
	CStateStack::EState myState;
	unsigned int myActiveScene;
};

