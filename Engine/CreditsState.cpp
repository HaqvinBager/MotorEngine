#include "stdafx.h"
#include "CreditsState.h"
#include "Engine.h"
#include "Scene.h"
#include "SpriteInstance.h"
#include "SpriteFactory.h"
#include "Canvas.h"
#include "GameObject.h"
#include "CameraComponent.h"
#include "CameraControllerComponent.h"
#include "TransformComponent.h"
#include "EnviromentLightComponent.h"
#include "JsonReader.h"
#include "MainSingleton.h"

CCreditsState::CCreditsState(CStateStack& aStateStack, const CStateStack::EState aState)
	: CState(aStateStack, aState)
	, myScene(nullptr)
	, myBackground(nullptr)
	, myCanvas(nullptr)
{
}

CCreditsState::~CCreditsState()
{
}

void CCreditsState::Awake()
{

}

void CCreditsState::Start()
{
	myScene = new CScene();

	CGameObject* camera = new CGameObject(0);
	camera->AddComponent<CCameraComponent>(*camera, 70.0f);
	camera->AddComponent<CCameraControllerComponent>(*camera, 25.0f);
	camera->myTransform->Position({ 0.0f, 0.0f, 0.0f });
	camera->myTransform->Rotation({ 0.0f, 0.0f, 0.0f });
	myScene->AddInstance(camera);
	myScene->SetMainCamera(camera->GetComponent<CCameraComponent>());

	CGameObject* envLight = new CGameObject(1);
	envLight->AddComponent<CEnviromentLightComponent>(*envLight);
	myScene->AddInstance(envLight);
	myScene->SetEnvironmentLight(envLight->GetComponent<CEnviromentLightComponent>()->GetEnviromentLight());

	myCanvas = new CCanvas();
	myCanvas->Init("Json/UI_Credits_Description.json", *myScene);

	CMainSingleton::PostMaster().Subscribe(EMessageType::MainMenu, this);

	CEngine::GetInstance()->AddScene(myState, myScene);
	CEngine::GetInstance()->SetActiveScene(myState);
}

void CCreditsState::Stop()
{
	if (myScene) {
		CMainSingleton::PostMaster().Unsubscribe(EMessageType::MainMenu, this);
		delete myCanvas;
		myCanvas = nullptr;
		//delete myScene;
		//myScene = nullptr;
	}
	CEngine::GetInstance()->RemoveScene(myState);
}

void CCreditsState::Update()
{
	myCanvas->Update();

	//if (Input::GetInstance()->IsKeyPressed('1'))
	//{
	//	CEngine::GetInstance()->SetResolution({ 1280.0f, 720.0f });
	//	myStateStack.PopTopAndPush(CStateStack::EState::Credits);
	//}
	//if (Input::GetInstance()->IsKeyPressed('2'))
	//{
	//	CEngine::GetInstance()->SetResolution({ 1600.0f, 900.0f });
	//	myStateStack.PopTopAndPush(CStateStack::EState::Credits);
	//}
	//if (Input::GetInstance()->IsKeyPressed('3'))
	//{
	//	CEngine::GetInstance()->SetResolution({ 1920.0f, 1080.0f });
	//	myStateStack.PopTopAndPush(CStateStack::EState::Credits);
	//}
}

void CCreditsState::Receive(const SMessage& aMessage)
{
	switch (aMessage.myMessageType)
	{
	case EMessageType::MainMenu:
		{
			myStateStack.PopUntil(CStateStack::EState::MainMenu);
		} break;
	default:
		break;
	}
}
