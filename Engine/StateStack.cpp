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
}

bool CStateStack::PopState() {
	ENGINE_ERROR_BOOL_MESSAGE(!myStateStack.empty(), "Trying to pop an empty stack");
	myStateStack.pop();
}

void CStateStack::Awake() {
	myStateStack.top()->Awake();
}

void CStateStack::Start() {
	myStateStack.top()->Start();
}

void CStateStack::Update() {
	myStateStack.top()->Update();
}
