#include "stdafx.h"
#include "StateStack.h"
#include "State.h"
#include "MenuState.h"
#include "LoadLevelState.h"
#include "InGameState.h"
#include "PauseState.h"

CStateStack::~CStateStack()
{
	while (!myStateStack.empty())
	{
		delete myStateStack.top();
		myStateStack.pop();
	}
}

bool CStateStack::Init(std::initializer_list<CStateStack::EState> someStates, const EState aStartState)
{
	for (const CStateStack::EState state : someStates)
	{
		switch (state)
		{
		case CStateStack::EState::MainMenu:
			myStateMap[state] = new CMenuState(*this);
			break;
		case CStateStack::EState::LoadLevel:
			myStateMap[state] = new CLoadLevelState(*this);
			break;
		case CStateStack::EState::InGame:
			myStateMap[state] = new CInGameState(*this);
			break;
		case CStateStack::EState::PauseMenu:
			myStateMap[state] = new CPauseState(*this);
			break;
		default:
			assert(0 && "State Is not Implemented yet.");
			break;
		}
	}

	return PushState(myStateMap[aStartState]);
}

bool CStateStack::PushState(CState* aState)
{
	ENGINE_ERROR_BOOL_MESSAGE(aState, "Trying to push aState that was nullptr");
	myStateStack.push(aState);
	return true;
}

bool CStateStack::PopState()
{
	ENGINE_ERROR_BOOL_MESSAGE(!myStateStack.empty(), "Trying to pop an empty stack");
	delete myStateStack.top();
	myStateStack.pop();

	if (0 < myStateStack.size())
		myStateStack.top()->MakeSceneActive();

	return true;
}

bool CStateStack::PopUntil(EState aState)
{
	ENGINE_ERROR_BOOL_MESSAGE(!myStateStack.empty(), "Trying to pop an empty stack");

	if (aState != myStateStack.top()->GetState())
	{
		delete myStateStack.top();
		myStateStack.pop();
		PopUntil(aState);
	}
	myStateStack.top()->MakeSceneActive();

	return true;
}

void CStateStack::Awake()
{
	myStateStack.top()->Awake();
}

void CStateStack::Start()
{
	myStateStack.top()->Start();
}

bool CStateStack::Update()
{
	if (myStateStack.size() <= 0)
		return false;

	myStateStack.top()->Update();
	return true;
}
