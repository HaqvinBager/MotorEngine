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
#include "DialogueSystem.h"
#include "Timer.h"
#include "CameraComponent.h"
#include "Engine.h"
#include "WindowHandler.h"
#include "AnimatedUIElement.h"
#include "InputMapper.h"
#include "PostMaster.h"
#include "Canvas.h"
#include "AbilityComponent.h"
#include "CircleColliderComponent.h"
#include "RectangleColliderComponent.h"
#include "TriangleColliderComponent.h"
#include "TokenPool.h"
#include "StatsComponent.h"
#include "EnemyBehavior.h"
#include "PlayerControllerComponent.h"
#include "AIBehaviorComponent.h"
#include "TransformComponent.h"
#include "PauseState.h"
#include "PostMaster.h"
#include "PopupTextService.h"
#include "RandomNumberGenerator.h"
#include "MainSingleton.h"
#include <iostream>

//testing
#include "AIBehaviorComponent.h"
#include "BossBehavior.h"
#include "NavMeshComponent.h"
#include "CameraComponent.h"
#include "Scene.h"
#include "MouseSelection.h"
#include "ColliderPushManager.h"

CInGameState::CInGameState(CStateStack& aStateStack) : CState(aStateStack) {
	myCanvas = new CCanvas();
	myCanvas->Init("Json/UI_InGame_Description.json");
	myActiveScene = CEngine::GetInstance()->ScenesSize();

	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::PauseGame, this);
	myTokenPool = new CTokenPool(4, 4.0f);

	mySelection = new CMouseSelection();
	myColliderPusher = new CColliderPushManager();
}

CInGameState::~CInGameState()
{
	CEngine::GetInstance()->PopBackScene();

	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::PauseGame, this);
}
void CInGameState::Awake()
{
	std::vector<CGameObject*>& gameObjects = CEngine::GetInstance()->GetActiveScene().myGameObjects;
	size_t currentSize = gameObjects.size();

	for (size_t i = 0; i < currentSize; ++i)
	{
		if (gameObjects[i])
		{
			gameObjects[i]->Awake();
		}

	}

	size_t newSize = gameObjects.size();

	//Late awake
	for (size_t j = currentSize; j < newSize; ++j)
	{
		if (gameObjects[j])
		{
			gameObjects[j]->Awake();
		}
	}

	//myEnemy = new CGameObject();
	////myEnemy->AddComponent<CModelComponent>(*myEnemy, "Assets/3D/Character/CH_NPC_enemy_01_19G4_1_19/CH_NPC_enemy_01_19G4_1_19.fbx");
	//myEnemy->AddComponent<CModelComponent>(*myEnemy, "Assets/Graphics/Skeletons/CH_E_Melee_SK.fbx");
	//myEnemy->AddComponent<CStatsComponent>(*myEnemy, 10.0f, 10.0f, 3.0f);
	//CEnemyBehavior* enemyBehavior = new CEnemyBehavior(&CEngine::GetInstance()->GetActiveScene().FindObjectOfType<CPlayerControllerComponent>()->GameObject());
	//myEnemy->AddComponent<CAIBehaviorComponent>(*myEnemy, enemyBehavior);
	//myEnemy->myTransform->Position({ -2.0f, 0.0f, 6.0f });
	//myEnemy->AddComponent<CCircleColliderComponent>(*myEnemy, 0.5f, ECollisionLayer::ENEMY, static_cast<int>(ECollisionLayer::PLAYER));
	//CEngine::GetInstance()->GetActiveScene().AddInstance(myEnemy);
	//myEnemy->Awake();

	//NO TOUCHY UNLESS BOSS TEST
	//myTestBoss = new CGameObject();
	//CBossBehavior* bossBehavior = new CBossBehavior(&CEngine::GetInstance()->GetActiveScene().FindObjectOfType<CPlayerControllerComponent>()->GameObject());
	//myTestBoss->myTransform->Position({ -2.0f, 0.0f, 6.0f });
	//myTestBoss->AddComponent<CCircleColliderComponent>(*myTestBoss, 0.5f, ECollisionLayer::BOSS, static_cast<int>(ECollisionLayer::PLAYER));
	//myTestBoss->AddComponent<CModelComponent>(*myTestBoss, "Assets/Graphics/Skeletons/CH_E_Boss_SK.fbx");
	//myTestBoss->AddComponent<CStatsComponent>(*myTestBoss, 10.0f, 10.0f, 3.0f, 3.0f, 20.0f, 15.0f);
	//myTestBoss->AddComponent<CAIBehaviorComponent>(*myTestBoss, bossBehavior);

	//myTestBoss->AddComponent<CNavMeshComponent>(*myTestBoss);

	//std::pair<EAbilityType, unsigned int> ab1 = { EAbilityType::BossAbility1, 1 };
	//std::pair<EAbilityType, unsigned int> ab2 = { EAbilityType::BossAbility2, 1 };
	//std::pair<EAbilityType, unsigned int> ab3 = { EAbilityType::BossAbility3, 1 };
	//std::vector<std::pair<EAbilityType, unsigned int>> abs;
	//abs.emplace_back(ab1);
	//abs.emplace_back(ab2);
	//abs.emplace_back(ab3);
	//CAbilityComponent* ac = myTestBoss->AddComponent<CAbilityComponent>(*myTestBoss, abs);
	//ac->Awake();

	//CEngine::GetInstance()->GetActiveScene().AddInstance(myTestBoss);
	//myTestBoss->Awake();
}

void CInGameState::Start()
{
	for (auto& gameObject : CEngine::GetInstance()->GetActiveScene().myGameObjects)
	{
		gameObject->Start();
	}
}

void CInGameState::Update()
{
	CCollisionManager::GetInstance()->Update();

	myCanvas->Update();
	myTokenPool->GetInstance()->Update();

	for (auto& gameObject : CEngine::GetInstance()->GetActiveScene().myGameObjects)
	{
		gameObject->Update();

	}
	
	myColliderPusher->EnemiesPushOutEnemies();
	myColliderPusher->PlayerPushOutEnemies();

	mySelection->FindSelectedEnemy();
	//static SDamagePopupData damage;
	//damage.myDamage = 32.0f;
	//damage.myHitType = Random(0, 4);
	//if (Input::GetInstance()->IsKeyPressed('U')) {
	//	CMainSingleton::PopupTextService().SpawnPopup(EPopupType::Damage, &damage);
	//}

	//static std::string tutorial = "Press Space to Continue";
	//if (Input::GetInstance()->IsKeyPressed('I')) {
	//	CMainSingleton::PopupTextService().SpawnPopup(EPopupType::Tutorial, tutorial);
	//}

	//static std::string warning = "You require more Mana";
	//if (Input::GetInstance()->IsKeyPressed('O')) {
	//	CMainSingleton::PopupTextService().SpawnPopup(EPopupType::Warning, warning);
	//}

	//static std::string text1 = "Skill 1";
	//if (Input::GetInstance()->IsKeyPressed('J')) {
	//	CMainSingleton::PopupTextService().SpawnPopup(EPopupType::Info, text1);
	//}

	//static std::string text2 = "Skill 2";
	//if (Input::GetInstance()->IsKeyPressed('K')) {
	//	CMainSingleton::PopupTextService().SpawnPopup(EPopupType::Info, text2);
	//}

	//static std::string text3 = "Skill 3";
	//if (Input::GetInstance()->IsKeyPressed('L')) {
	//	CMainSingleton::PopupTextService().SpawnPopup(EPopupType::Info, text3);
	//}
}

void CInGameState::ReceiveEvent(const EInputEvent aEvent)
{
	if (this == myStateStack.GetTop()) {
		switch (aEvent) {
		case IInputObserver::EInputEvent::PauseGame:
			myStateStack.PushState(new CPauseState(myStateStack));
			myStateStack.Awake();
			myStateStack.Start();
			break;
		default:
			break;
		}
	}
}

void CInGameState::MakeSceneActive()
{
	CEngine::GetInstance()->SetActiveScene(myActiveScene);
}
