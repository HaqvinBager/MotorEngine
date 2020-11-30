#include "stdafx.h"
#include "Game.h"
#include "LoadLevelState.h"
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
{
}

CGame::~CGame()
{
}

void CGame::Init()
{
	myStateStack.PushState(new CMenuState(myStateStack));
	myStateStack.Awake();
	myStateStack.Start();
}

bool CGame::Update()
{
	return myStateStack.Update();
}
	
