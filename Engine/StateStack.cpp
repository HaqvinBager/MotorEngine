#include "stdafx.h"
#include "StateStack.h"
#include "State.h"

CStateStack::~CStateStack() {
	while (!myStateStack.empty()) {
		delete myStateStack.top();
		myStateStack.pop();
	}
}

bool CStateStack::PushState(CState* aState) {
	ENGINE_ERROR_BOOL_MESSAGE(aState , "Trying to push aState that was nullptr");
	myStateStack.push(aState);
	return true;
}

bool CStateStack::PopState() {
	ENGINE_ERROR_BOOL_MESSAGE(!myStateStack.empty(), "Trying to pop an empty stack");
	delete myStateStack.top();
	myStateStack.pop();

	if(0 < myStateStack.size() )
		myStateStack.top()->MakeSceneActive();
	
	return true;
}

bool CStateStack::PopUntil(EStates aState) {
	ENGINE_ERROR_BOOL_MESSAGE(!myStateStack.empty(), "Trying to pop an empty stack");
	
	if (aState != myStateStack.top()->GetState()) {
		delete myStateStack.top();
		myStateStack.pop();
		PopUntil(aState);
	}
	myStateStack.top()->MakeSceneActive();

	return true;
}

void CStateStack::Awake() {
	myStateStack.top()->Awake();
}

void CStateStack::Start() {
	myStateStack.top()->Start();
}

bool CStateStack::Update() {
	if (myStateStack.size() <= 0)
		return false;

	myStateStack.top()->Update();
	return true;
}
