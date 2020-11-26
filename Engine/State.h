#pragma once
#include "StateStack.h"
//class CStateStack;

class CState  {

public:
	CState(CStateStack& aStateStack);
	virtual ~CState();
	virtual void MakeSceneActive() = 0;
	virtual void Awake() = 0;
	virtual void Start() = 0;
	virtual void Update() = 0;

	CStateStack::EStates GetState() { return myState; }

protected:
	CStateStack& myStateStack;
	CStateStack::EStates myState;
	unsigned int myActiveScene;
};

