#include "stdafx.h"
#include "MenuState.h"

#include "SpriteInstance.h"
#include "SpriteFactory.h"
#include "Scene.h"
#include "CameraFactory.h"
#include "Canvas.h"
#include "StateStack.h"
#include "../../Game/LoadLevelState.h"
#include "Engine.h"
#include "GameObject.h"
#include "CameraComponent.h"
#include "CameraControllerComponent.h"
#include "EnviromentLightComponent.h"
#include "TransformComponent.h"
#include "Button.h"
#include "PostMaster.h"
#include "MainSingleton.h"

CMenuState::CMenuState(CStateStack& aStateStack, const CStateStack::EState aState) 
	: CState(aStateStack, aState)
	, myCanvas(nullptr)
	, myScene(nullptr)
{}

CMenuState::~CMenuState() 
{
	delete myScene;
	myScene = nullptr;

	delete myCanvas;
	myCanvas = nullptr;
}

void CMenuState::Awake() 
{
	myScene = new CScene();

	CGameObject* camera = new CGameObject();
	camera->AddComponent<CCameraComponent>(*camera, 70.0f);
	camera->AddComponent<CCameraControllerComponent>(*camera, 25.0f);
	camera->myTransform->Position({ 0.0f, 0.0f, 0.0f });
	camera->myTransform->Rotation({ 0.0f, 0.0f, 0.0f });
	myScene->AddInstance(camera);
	myScene->SetMainCamera(camera->GetComponent<CCameraComponent>());

	CGameObject* envLight = new CGameObject();
	envLight->AddComponent<CEnviromentLightComponent>(*envLight);
	myScene->AddInstance(envLight);
	myScene->SetEnvironmentLight(envLight->GetComponent<CEnviromentLightComponent>()->GetEnviromentLight());

	myCanvas = new CCanvas();
	myCanvas->Init("Json/UI_MainMenu_Description.json", *myScene);

	for (auto buttons : myCanvas->GetButtons())
	{
		for (auto messageType : buttons->GetMessagesToSend())
			CMainSingleton::PostMaster().Subscribe(messageType, this);
	}

	CEngine::GetInstance()->AddScene(myState, myScene);
}

void CMenuState::Start() 
{
	CEngine::GetInstance()->SetActiveScene(myState);
}

void CMenuState::Stop()
{
	for (auto buttons : myCanvas->GetButtons())
	{
		for (auto messageType : buttons->GetMessagesToSend())
			CMainSingleton::PostMaster().Unsubscribe(messageType, this);
	}
}

void CMenuState::Update() {
	myCanvas->Update();
}

void CMenuState::Receive(const SMessage &aMessage) {
	if (this == myStateStack.GetTop()) {
		switch (aMessage.myMessageType) {
		case EMessageType::StartGame:
		{
			myStateStack.PushState(CStateStack::EState::IntroState);
		} break;
		case EMessageType::Quit:
		{
			myStateStack.PopState();
		} break;
		}
	}
}