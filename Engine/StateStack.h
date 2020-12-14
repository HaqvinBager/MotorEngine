#pragma once
#include <stack>
#include <unordered_map>

class CState;
class CStateStack {
	friend class CShowCase;
public:

	enum class EState {
		BootUp,
		TitleScreen,
		MainMenu,
		Credits,
		Intro,
		LoadLevel,
		InGame,
		PauseMenu,
		NoState
	};

	CStateStack() = default;
	~CStateStack();

	bool Awake(std::initializer_list<CStateStack::EState> someStates, const EState aStartState = EState::MainMenu);

	CState* GetTop() { return myStateStack.top(); }
	bool PushState(const EState aState);
	bool PopState();
	bool PopUntil(const EState aState);
	bool PopTopAndPush(const EState aState);

	bool Update();



private:
	//void Start();
	//void Stop();
	void Awake();//Remove,should only be called on PushState.

	std::stack<CState*> myStateStack;
	std::unordered_map<EState, CState*> myStateMap;
};
