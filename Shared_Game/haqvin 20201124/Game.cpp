#include "stdafx.h"
#include "Game.h"
#include "MainSingleton.h"
#include "MenuState.h"

#ifdef _DEBUG
#pragma comment(lib, "Engine_Debug.lib")
#pragma comment(lib, "UnityImporterApp_Debug.lib")
#endif // _DEBUG
#ifdef NDEBUG
#pragma comment(lib, "Engine_Release.lib")
#pragma comment(lib, "UnityImporterApp_Release.lib")
#endif // NDEBUG


CGame::CGame()
	:  myWindowHeight(0), myWindowWhidth(0)
{
}

CGame::~CGame()
{
}

void CGame::Init() {
	CMainSingleton::StateStack().PushState(new CMenuState(CMainSingleton::StateStack()));
	CMainSingleton::StateStack().Awake();
	CMainSingleton::StateStack().Start();
}

bool CGame::Update() {
	return CMainSingleton::StateStack().Update();
}
