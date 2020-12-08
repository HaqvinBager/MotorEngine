#include "stdafx.h"
#include "StateStack.h"
#include "State.h"
#include "MenuState.h"
#include "LoadLevelState.h"
#include "InGameState.h"
#include "PauseState.h"

CStateStack::~CStateStack()
{
	while (!myStateStack.empty())// change this
	{
		delete myStateStack.top();
		myStateStack.pop();
	}
}

bool CStateStack::Awake(std::initializer_list<CStateStack::EState> someStates, const EState aStartState)
{
	for (const CStateStack::EState state : someStates)
	{
		switch (state)
		{
		case CStateStack::EState::MainMenu:
			myStateMap[state] = new CMenuState(*this);
			myStateMap[state]->Awake();
			break;
		case CStateStack::EState::LoadLevel:
			myStateMap[state] = new CLoadLevelState(*this);
			myStateMap[state]->Awake();
			break;
		case CStateStack::EState::InGame:
			myStateMap[state] = new CInGameState(*this);
			myStateMap[state]->Awake();
			break;
		case CStateStack::EState::PauseMenu:
			myStateMap[state] = new CPauseState(*this);
			myStateMap[state]->Awake();
			break;
		default:
			assert(0 && "State Is not Implemented yet.");
			break;
		}
	}

	return PushState(aStartState);
}

bool CStateStack::PushState(const EState aState)
{
	if (myStateMap.find(aState) == myStateMap.end())
		return false;

	myStateStack.push(myStateMap[aState]);
	myStateStack.top()->Start();
	return true;
}

bool CStateStack::PopState()
{
	ENGINE_ERROR_BOOL_MESSAGE(!myStateStack.empty(), "Trying to pop an empty stack");
	myStateStack.top()->Stop();
	myStateStack.pop();
	if (myStateStack.size() == 0)// == 0 < myStateStack.size()
		return false;

	return true;
}

bool CStateStack::PopUntil(const EState aState)
{
	ENGINE_ERROR_BOOL_MESSAGE(!myStateStack.empty(), "Trying to pop an empty stack");

	while(myStateStack.top()->GetState() != aState)
	{
		myStateStack.top()->Stop();
		myStateStack.pop();
	}
	myStateStack.top()->Start();
	return true;
}

bool CStateStack::PopTopAndPush(const EState aState)
{
	if (!PopState())
		return false;

	return PushState(aState);
}

void CStateStack::Awake()
{
	myStateStack.top()->Awake();
}

//void CStateStack::Start()
//{
//	myStateStack.top()->Start();
//}
//
//void CStateStack::Stop()
//{
//	myStateStack.top()->Stop();
//}

bool CStateStack::Update()
{
	myStateStack.top()->Update();
	return myStateStack.size() > 0;
}