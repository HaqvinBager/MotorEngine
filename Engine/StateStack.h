#pragma once
#include <stack>

class CState;

class CStateStack {
	friend class CShowCase;
public:

	enum class EStates {
		MainMenu,
		LoadLevel,
		InGame,
		PauseMenu
	};

	CStateStack() = default;
	~CStateStack();


	CState* GetTop() { return myStateStack.top(); }
	bool PushState(CState* aState);
	bool PopState();
	bool PopUntil(EStates aState);
	void Awake();
	void Start();
	bool Update();

private:
	std::stack<CState*> myStateStack;
};
