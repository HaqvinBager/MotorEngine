#include "stdafx.h"
#include "InGameState.h"
#include "StateStack.h"
#include "Scene.h"
#include "GameObject.h"
#include "SpriteInstance.h"
#include "SpriteFactory.h"


CInGameState::CInGameState(CStateStack& aStateStack) : CState(aStateStack) {
	CSpriteInstance* spriteInstance = new CSpriteInstance();
	spriteInstance->Init(CSpriteFactory::GetInstance()->GetSprite("tempUI.dds"));
	spriteInstance->SetSize({ 2.0f,2.0f });
	spriteInstance->SetPosition({ 0.0f,-0.85f });
	CScene::GetInstance()->AddInstance(spriteInstance);
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
