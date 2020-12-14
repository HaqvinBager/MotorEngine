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
	InitDev();
	//InitRealGame();
}

bool CGame::Update()
{
	return myStateStack.Update();
}

void CGame::InitDev()
{
	myStateStack.Awake(
		{
			CStateStack::EState::LoadLevel,
			CStateStack::EState::InGame,
			CStateStack::EState::PauseMenu
		},
		CStateStack::EState::LoadLevel);
}

void CGame::InitRealGame()
{
	myStateStack.Awake(
		{
		CStateStack::EState::BootUp,
		CStateStack::EState::TitleScreen,
		CStateStack::EState::MainMenu,
		CStateStack::EState::Credits,
		CStateStack::EState::Intro,
		CStateStack::EState::LoadLevel,
		CStateStack::EState::InGame,
		CStateStack::EState::PauseMenu
		},
		CStateStack::EState::BootUp);
}
