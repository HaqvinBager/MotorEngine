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
	myStateStack.Awake(
		{ 
		CStateStack::EState::BootUpState,
		CStateStack::EState::TitleScreenState,
		CStateStack::EState::MainMenu, 
		CStateStack::EState::IntroState,
		CStateStack::EState::LoadLevel,
		CStateStack::EState::InGame,
		CStateStack::EState::PauseMenu 
		}, 
		CStateStack::EState::BootUpState);
}

bool CGame::Update()
{
	return myStateStack.Update();
}

