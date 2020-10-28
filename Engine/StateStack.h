#pragma once
#include <stack>

class CState;

class CStateStack {
	friend class CShowCase;
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
