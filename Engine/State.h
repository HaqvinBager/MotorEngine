#pragma once
#include "StateStack.h"

class CState  
{
	friend CStateStack;

public:
	CState(CStateStack& aStateStack, const CStateStack::EState aState);
	virtual ~CState();
	/*virtual void MakeSceneActive() = 0;*/
	virtual void Stop() = 0;
	virtual void Start() = 0;
	virtual void Update() = 0;

	CStateStack::EState GetState() { return myState; }

protected:
	virtual void Awake() = 0;

	CStateStack& myStateStack;
	CStateStack::EState myState;
	unsigned int myActiveScene;
};

