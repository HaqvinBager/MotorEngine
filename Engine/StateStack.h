#pragma once
#include <stack>
#include <unordered_map>

class CState;
class CStateStack {
	friend class CShowCase;
public:

	enum class EState {
		MainMenu,
		LoadLevel,
		InGame,
		PauseMenu
	};

	CStateStack() = default;
	~CStateStack();

	bool Awake(std::initializer_list<CStateStack::EState> someStates, const EState aStartState = EState::MainMenu);

	CState* GetTop() { return myStateStack.top(); }
	bool PushState(const EState aState);
	bool PopState();
	bool PopUntil(EState aState);
	void Awake();//Remove,should only be called on PushState.
	void Start();
	bool Update();

private:
	std::stack<CState*> myStateStack;
	std::unordered_map<EState, CState*> myStateMap;
};
