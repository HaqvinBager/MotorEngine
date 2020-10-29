#include "stdafx.h"
#include "InGameState.h"
#include "StateStack.h"
#include "Scene.h"
#include "GameObject.h"
#include "SpriteInstance.h"
#include "SpriteFactory.h"
#include "AnimationComponent.h"
#include "ModelComponent.h"
#include "Model.h"
#include "Animation.h"
#include "TransformComponent.h"


CInGameState::CInGameState(CStateStack& aStateStack) : CState(aStateStack) {
	CSpriteInstance* spriteInstance = new CSpriteInstance();
	spriteInstance->Init(CSpriteFactory::GetInstance()->GetSprite("tempUI.dds"));
	spriteInstance->SetSize({ 2.0f,2.0f });
	spriteInstance->SetPosition({ 0.0f,-0.85f });
	CScene::GetInstance()->AddInstance(spriteInstance);

	CGameObject* enemy = new CGameObject();
	enemy->AddComponent<CTransformComponent>(*enemy);
	enemy->AddComponent<CModelComponent>(*enemy, "Assets/3D/Character/Enemy1/enemy_sk.fbx");

	CAnimation* anim = new CAnimation();
	std::vector < std::string> animationClips = {
		"Assets/3D/Character/Enemy1/enemy_excitedJump.fbx"
	};
	anim->Init("Assets/3D/Character/Enemy1/enemy_sk.fbx", animationClips);
	enemy->AddComponent<CAnimationComponent>(*enemy);
	CScene::GetInstance()->AddInstance(enemy);
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
