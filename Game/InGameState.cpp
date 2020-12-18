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
	: CState(aStateStack, aState),
	myCanvas(nullptr),
	myTokenPool(nullptr),
	myColliderPusher(nullptr),
	myExitLevel(false)
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

//#include "animationLoader.h" //only for boss test
void CInGameState::Start()
{
	CInputMapper::GetInstance()->ClearObserverList(IInputObserver::EInputEvent::Ability1);
	CInputMapper::GetInstance()->ClearObserverList(IInputObserver::EInputEvent::Ability2);
	CInputMapper::GetInstance()->ClearObserverList(IInputObserver::EInputEvent::Ability3);
	CInputMapper::GetInstance()->ClearObserverList(IInputObserver::EInputEvent::AttackClick);

	myExitLevel = false;
	CMainSingleton::PostMaster().Subscribe("Dungeon", this);
	CMainSingleton::PostMaster().Subscribe("Gardens", this);
	CMainSingleton::PostMaster().Subscribe("Castle", this);
	CMainSingleton::PostMaster().Subscribe("BossRoom", this);
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::PauseGame, this);

	CMainSingleton::PostMaster().Subscribe(EMessageType::BossDied, this);

	CEngine::GetInstance()->SetActiveScene(myState);

	myCanvas = new CCanvas();
	myCanvas->Init("Json/UI_InGame_Description.json", CEngine::GetInstance()->GetActiveScene());

	myTokenPool = new CTokenPool(4, 1.0f);// todo: fix reset

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

	for (auto& gameObject : CEngine::GetInstance()->GetActiveScene().myGameObjects)
	{
		gameObject->Start();
	}

	CEngine::GetInstance()->GetActiveScene().SetPlayerToOutline(CEngine::GetInstance()->GetActiveScene().GetPlayer());
	CEngine::GetInstance()->GetActiveScene().GetMainCamera()->Fade(true);
}

void CInGameState::Stop()
{
	CInputMapper::GetInstance()->RemoveObserver(IInputObserver::EInputEvent::PauseGame, this);
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::BossDied, this);
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

	CEngine::GetInstance()->GetActiveScene().SetEnemyToOutline(mySelection->FindSelectedEnemy());

	if (CEngine::GetInstance()->GetActiveScene().GetBoss() != nullptr) {
		CEngine::GetInstance()->GetActiveScene().SetEnemyToOutline(mySelection->FindSelectedBoss());
	}

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

	//if (Input::GetInstance()->IsKeyPressed('7'))
	//{
	//	int aSceneIndex = 0;
	//	CMainSingleton::PostMaster().Send({ EMessageType::LoadDialogue, &aSceneIndex });
	//}

	//if (Input::GetInstance()->IsKeyPressed('8'))
	//{
	//	int aSceneIndex = 1;
	//	CMainSingleton::PostMaster().Send({ EMessageType::LoadDialogue, &aSceneIndex });
	//}

	//if (Input::GetInstance()->IsKeyPressed('9'))
	//{
	//	int aSceneIndex = 2;
	//	CMainSingleton::PostMaster().Send({ EMessageType::LoadDialogue, &aSceneIndex });
	//}

	//if (Input::GetInstance()->IsKeyPressed('0'))
	//{
	//	CMainSingleton::PostMaster().Send({ EMessageType::IntroStarted, NULL });
	//}

	//if (Input::GetInstance()->IsKeyPressed('T'))
	//{
	//	CEngine::GetInstance()->GetActiveScene().GetMainCamera()->SetTrauma(0.65f);
	//}

	//if (Input::GetInstance()->IsKeyPressed('Y'))
	//{
	//	CEngine::GetInstance()->GetActiveScene().GetMainCamera()->SetTrauma(0.85f);
	//}

	if (myExitLevel)
	{
		myExitLevel = false;
		CEngine::GetInstance()->SetRenderScene(false);
		CMainSingleton::CollisionManager().ClearColliders();
		CMainSingleton::PostMaster().SendLate({ EMessageType::StopDialogue, 0 });
		CMainSingleton::DialogueSystem().ExitDialogue();

		myStateStack.PopTopAndPush(CStateStack::EState::LoadLevel);
	}
}

void CInGameState::ReceiveEvent(const EInputEvent aEvent)
{
	if (this == myStateStack.GetTop()) {
		switch (aEvent) {
		case IInputObserver::EInputEvent::PauseGame:
			if (CMainSingleton::DialogueSystem().Active()) {
				CMainSingleton::PostMaster().SendLate({ EMessageType::StopDialogue, 0 });
				CMainSingleton::DialogueSystem().ExitDialogue();
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

void CInGameState::Receive(const SMessage& aMessage)
{
	switch (aMessage.myMessageType)
	{
		case EMessageType::BossDied:
			myStateStack.PopTopAndPush(CStateStack::EState::Credits);
		break;

		default:break;
	}
}