#include "stdafx.h"
#include "Game.h"
#include "MenuState.h"
#include "LoadLevelState.h"
#include "InGameState.h"
#include "PauseState.h"

#ifdef _DEBUG
#pragma comment(lib, "Engine_Debug.lib")
#endif // _DEBUG
#ifdef NDEBUG
#pragma comment(lib, "Engine_Release.lib")
#endif // NDEBUG

CGame::CGame()
{
}

CGame::~CGame()
{
}

void CGame::Init()
{
	myStateStack.Init(
		{ 
		CStateStack::EState::MainMenu, 
		CStateStack::EState::LoadLevel,
		CStateStack::EState::InGame,
		CStateStack::EState::PauseMenu 
		}, 
		CStateStack::EState::MainMenu);

	myStateStack.Awake();
	myStateStack.Start();
}

bool CGame::Update()
{
	return myStateStack.Update();
}

