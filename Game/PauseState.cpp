#include "stdafx.h"
#include "PauseState.h"

#include "Canvas.h"
#include "MainSingleton.h"
#include "Scene.h"
#include "Engine.h"
#include "TransformComponent.h"
#include "EnviromentLightComponent.h"
#include "CameraControllerComponent.h"
#include "CameraComponent.h"

CPauseState::CPauseState(CStateStack& aStateStack, const CStateStack::EState aState) 
	: CState(aStateStack, aState)
	, myCanvas(nullptr) 
	, myScene(nullptr)
{}

CPauseState::~CPauseState() 
{
	delete myScene;
	myScene = nullptr;
}

void CPauseState::Awake() 
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
	myCanvas->Init("Json/UI_PauseMenu_Description.json", *myScene);

	CEngine::GetInstance()->AddScene(myState, myScene);
}

void CPauseState::Start()  
{
	CEngine::GetInstance()->SetActiveScene(myState);
	CMainSingleton::PostMaster().Subscribe(EMessageType::MainMenu, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::Resume, this);
}

void CPauseState::Stop()
{
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::MainMenu,this);
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::Resume,this);
}

void CPauseState::Update() 
{
	myCanvas->Update();
}

#include <iostream>
void CPauseState::Receive(const SMessage& aMessage) {
	if (this == myStateStack.GetTop()) {
		switch (aMessage.myMessageType) {
		case EMessageType::MainMenu:
			myStateStack.PopUntil(CStateStack::EState::MainMenu);
			break;
		case EMessageType::Resume:
			std::cout << "Should not pop yet" << std::endl;
			myStateStack.PopState();
			break;
		default:
			break;
		}
	}
}