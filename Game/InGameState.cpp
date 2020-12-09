#include "stdafx.h"
#include "InGameState.h"

#include "Scene.h"
#include "PauseState.h"

#include "GameObject.h"
#include "AnimationComponent.h"
#include "TransformComponent.h"
#include "CircleColliderComponent.h"
#include "RectangleColliderComponent.h"
#include "TriangleColliderComponent.h"
#include "PlayerControllerComponent.h"
#include "ModelComponent.h"
#include "CameraComponent.h"
#include "AIBehaviorComponent.h"
#include "TransformComponent.h"
#include "AIBehaviorComponent.h"
#include "AbilityComponent.h"
#include "StatsComponent.h"

#include "Timer.h"
#include "Engine.h"
#include "PostMaster.h"
#include "WindowHandler.h"
#include "PopupTextService.h"
#include "MainSingleton.h"
#include "DialogueSystem.h"
#include "InputMapper.h"


#include "Model.h"
#include "Animation.h"
#include "SpriteInstance.h"
#include "SpriteFactory.h"
#include "Canvas.h"
#include "AnimatedUIElement.h"
#include "TokenPool.h"
#include "EnemyBehavior.h"
#include "RandomNumberGenerator.h"
//testing
#include "AIBehaviorComponent.h"
#include "BossBehavior.h"
#include "NavMeshComponent.h"
#include "CameraComponent.h"
#include "Scene.h"
#include "MouseSelection.h"
#include "ColliderPushManager.h"

#include <iostream>



CInGameState::CInGameState(CStateStack& aStateStack, const CStateStack::EState aState)
	: CState(aStateStack, aState)
	, myCanvas(nullptr)
	, myTokenPool(nullptr)
	, myColliderPusher(nullptr)
	, myExitLevel(false)
{}

CInGameState::~CInGameState()
{
	delete mySelection;
	mySelection = nullptr;

	delete myColliderPusher;
	myColliderPusher = nullptr;
}

void CInGameState::Awake()
{
	mySelection = new CMouseSelection();
	myColliderPusher = new CColliderPushManager();
}

#include "animationLoader.h" //only for boss test
void CInGameState::Start()
{
	myExitLevel = false;
	CMainSingleton::DialogueSystem().Enabled(true);
	CMainSingleton::PostMaster().Subscribe("Dungeon", this);
	CMainSingleton::PostMaster().Subscribe("Gardens", this);
	CMainSingleton::PostMaster().Subscribe("Castle", this);
	CMainSingleton::PostMaster().Subscribe("BossRoom", this);
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::PauseGame, this);

	CEngine::GetInstance()->SetActiveScene(myState);

	myCanvas = new CCanvas();
	myCanvas->Init("Json/UI_InGame_Description.json", CEngine::GetInstance()->GetActiveScene());

	myTokenPool = new CTokenPool(4, 1.0f);// todo: fix reset

	//NO TOUCHY UNLESS BOSS TEST
	//myTestBoss = new CGameObject();
	//CBossBehavior* bossBehavior = new CBossBehavior(&CEngine::GetInstance()->GetActiveScene().FindObjectOfType<CPlayerControllerComponent>()->GameObject());
	//myTestBoss->myTransform->Position({ -2.0f, 0.0f, 6.0f });
	//myTestBoss->AddComponent<CCircleColliderComponent>(*myTestBoss, 0.5f, ECollisionLayer::BOSS, static_cast<int>(ECollisionLayer::PLAYER));
	//myTestBoss->AddComponent<CModelComponent>(*myTestBoss, "Assets/Graphics/Animations/CH_E_Boss_SK/CH_E_Boss_SK.fbx");

	//AddAnimationsToGameObject(*myTestBoss, "Assets/Graphics/Animations/CH_E_Boss_SK/CH_E_Boss_SK.fbx", EAnimatedObject::Boss);

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
	//myTestBoss->AddComponent<CAbilityComponent>(*myTestBoss, abs);

	//CEngine::GetInstance()->GetActiveScene().AddInstance(myTestBoss);
	//NO TOUCHY UNLESS BOSS TEST

	std::vector<CGameObject*>& gameObjects = CEngine::GetInstance()->GetActiveScene().myGameObjects;
	size_t currentSize = gameObjects.size();
	for (size_t i = 0; i < currentSize; ++i)
	{
		if (gameObjects[i])
		{
			gameObjects[i]->Awake();
		}
	}

	////Late awake
	size_t newSize = gameObjects.size();
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

	for (auto& gameObject : CEngine::GetInstance()->GetActiveScene().myGameObjects)
	{
		gameObject->Start();
	}
}

void CInGameState::Stop()
{
	CInputMapper::GetInstance()->RemoveObserver(IInputObserver::EInputEvent::PauseGame, this);
	CMainSingleton::CollisionManager().ClearColliders();

	delete myTokenPool;
	myTokenPool = nullptr;

	delete myCanvas;
	myCanvas = nullptr;

	CMainSingleton::PostMaster().Unsubscribe("Dungeon", this);
	CMainSingleton::PostMaster().Unsubscribe("Gardens", this);
	CMainSingleton::PostMaster().Unsubscribe("Castle", this);
	CMainSingleton::PostMaster().Unsubscribe("BossRoom", this);
}

void CInGameState::Update()
{
	CMainSingleton::CollisionManager().Update();

	myCanvas->Update();
	myTokenPool->GetInstance()->Update();

	for (auto& gameObject : CEngine::GetInstance()->GetActiveScene().myGameObjects)
	{
		gameObject->Update();
	}

	myColliderPusher->EnemiesPushOutEnemies();
	//myColliderPusher->PlayerPushOutEnemies();

	mySelection->FindSelectedEnemy();

	static SDamagePopupData damage;
	damage.myDamage = 32.0f;
	damage.myHitType = Random(0, 4);
	if (Input::GetInstance()->IsKeyPressed('U')) {
		CMainSingleton::PopupTextService().SpawnPopup(EPopupType::Damage, &damage);
	}

	static std::string tutorial = "Press Space to Continue";
	if (Input::GetInstance()->IsKeyPressed('I')) {
		CMainSingleton::PopupTextService().SpawnPopup(EPopupType::Tutorial, tutorial);
	}

	static std::string warning = "You require more Mana";
	if (Input::GetInstance()->IsKeyPressed('O')) {
		CMainSingleton::PopupTextService().SpawnPopup(EPopupType::Warning, warning);
	}

	static std::string text1 = "Skill 1";
	if (Input::GetInstance()->IsKeyPressed('J')) {
		CMainSingleton::PopupTextService().SpawnPopup(EPopupType::Info, text1);
	}

	static std::string text2 = "Skill 2";
	if (Input::GetInstance()->IsKeyPressed('K')) {
		CMainSingleton::PopupTextService().SpawnPopup(EPopupType::Info, text2);
	}

	static std::string text3 = "Skill 3";
	if (Input::GetInstance()->IsKeyPressed('L')) {
		CMainSingleton::PopupTextService().SpawnPopup(EPopupType::Info, text3);
	}

	if (Input::GetInstance()->IsKeyPressed('7'))
	{
		int aSceneIndex = 0;
		CMainSingleton::PostMaster().Send({ EMessageType::LoadDialogue, &aSceneIndex });
	}

	if (Input::GetInstance()->IsKeyPressed('8'))
	{
		int aSceneIndex = 1;
		CMainSingleton::PostMaster().Send({ EMessageType::LoadDialogue, &aSceneIndex });
	}

	if (Input::GetInstance()->IsKeyPressed('9'))
	{
		int aSceneIndex = 2;
		CMainSingleton::PostMaster().Send({ EMessageType::LoadDialogue, &aSceneIndex });
	}

	if (Input::GetInstance()->IsKeyPressed('0'))
	{
		CMainSingleton::PostMaster().Send({ EMessageType::IntroStarted, NULL });
	}

	if (myExitLevel)
	{
		myExitLevel = false;
		CEngine::GetInstance()->SetRenderScene(false);
		CMainSingleton::CollisionManager().ClearColliders();
		CMainSingleton::DialogueSystem().Enabled(false);

		myStateStack.PopTopAndPush(CStateStack::EState::LoadLevel);
	}

}

void CInGameState::ReceiveEvent(const EInputEvent aEvent)
{
	if (this == myStateStack.GetTop()) {
		switch (aEvent) {
		case IInputObserver::EInputEvent::PauseGame:
			if (CMainSingleton::DialogueSystem().Enabled()) {
				CMainSingleton::DialogueSystem().Enabled(false);
			} else {
				myStateStack.PushState(CStateStack::EState::PauseMenu);
			}
			break;
		default:
			break;
		}
	}
}

void CInGameState::Receive(const SStringMessage& /*aMessage*/)
{
	myExitLevel = true;
}
