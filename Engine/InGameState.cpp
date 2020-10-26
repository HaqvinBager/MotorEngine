#include "stdafx.h"
#include "InGameState.h"
#include "StateStack.h"
#include "Scene.h"
#include "GameObject.h"


CInGameState::CInGameState(CStateStack& aStateStack) : CState(aStateStack)
{
	
}

CInGameState::~CInGameState()
{

}

void CInGameState::Awake()
{
	for (auto& gameObject : CScene::GetInstance()->myGameObjects)
	{
		gameObject->Awake();
	}
}

void CInGameState::Start()
{
	for (auto& gameObject : CScene::GetInstance()->myGameObjects)
	{
		gameObject->Start();
	}
}

void CInGameState::Update()
{
	for (auto& gameObject : CScene::GetInstance()->myGameObjects)
	{
		gameObject->Update();
	}
}
