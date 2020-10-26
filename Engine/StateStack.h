#pragma once
#include <stack>

class CState;

class CStateStack {

public:
		
	CStateStack() = default;
	 ~CStateStack();

	bool PushState(CState* aState);
	bool PopState();
	
	void Awake();
	void Start();
	void Update();

private:

	std::stack<CState*> myStateStack;
};
