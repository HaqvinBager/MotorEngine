#include "stdafx.h"
#include "LevelSelectState.h"
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
#include "rapidjson\document.h"
#include "rapidjson\istreamwrapper.h"

using namespace rapidjson;

CLevelSelectState::CLevelSelectState(CStateStack& aStateStack, const CStateStack::EState aState) :
	CState(aStateStack, aState),
	myScene(nullptr),
	myBackground(nullptr),
	myCanvas(nullptr),
	myGrupp(0)
{
}

CLevelSelectState::~CLevelSelectState()
{
}

void CLevelSelectState::Awake()
{
	std::ifstream inputStream("Json/Grupp.json");
	IStreamWrapper inputWrapper(inputStream);
	Document document;
	document.ParseStream(inputWrapper);
	myGrupp = document["Grupp"].GetInt();
}

void CLevelSelectState::Start()
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
	myCanvas->Init("Json/UI_LevelSelect_Description.json", *myScene);

	CMainSingleton::PostMaster().Subscribe(EMessageType::MainMenu, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::LoadLevel1, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::LoadLevel2, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::LoadLevel3, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::LoadLevel4, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::LoadLevel5, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::LoadLevel6, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::LoadLevel7, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::LoadLevel8, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::LoadLevel9, this);

	CEngine::GetInstance()->AddScene(myState, myScene);
	CEngine::GetInstance()->SetActiveScene(myState);
}

void CLevelSelectState::Stop()
{
	if (myScene)
	{
		CMainSingleton::PostMaster().Unsubscribe(EMessageType::MainMenu, this);
		CMainSingleton::PostMaster().Unsubscribe(EMessageType::LoadLevel1, this);
		CMainSingleton::PostMaster().Unsubscribe(EMessageType::LoadLevel2, this);
		CMainSingleton::PostMaster().Unsubscribe(EMessageType::LoadLevel3, this);
		CMainSingleton::PostMaster().Unsubscribe(EMessageType::LoadLevel4, this);
		CMainSingleton::PostMaster().Unsubscribe(EMessageType::LoadLevel5, this);
		CMainSingleton::PostMaster().Unsubscribe(EMessageType::LoadLevel6, this);
		CMainSingleton::PostMaster().Unsubscribe(EMessageType::LoadLevel7, this);
		CMainSingleton::PostMaster().Unsubscribe(EMessageType::LoadLevel8, this);
		CMainSingleton::PostMaster().Unsubscribe(EMessageType::LoadLevel9, this);
		delete myCanvas;
		myCanvas = nullptr;
	}
}

void CLevelSelectState::Update()
{
	myCanvas->Update();
}

void CLevelSelectState::Receive(const SMessage& aMessage)
{
	if (myGrupp == 3)
	{
		switch (aMessage.myMessageType)
		{
		case EMessageType::MainMenu:
		{
			myStateStack.PopUntil(CStateStack::EState::MainMenu);
		} break;
		case EMessageType::LoadLevel1:
		{
			SStringMessage stringMessage = { "Level_1_1", nullptr };
			CMainSingleton::PostMaster().Send(stringMessage);
			myStateStack.PushState(CStateStack::EState::LoadLevel);
		} break;
		case EMessageType::LoadLevel2:
		{
			SStringMessage stringMessage = { "Level_2_1", nullptr };
			CMainSingleton::PostMaster().Send(stringMessage);
			myStateStack.PushState(CStateStack::EState::LoadLevel);
		} break;
		case EMessageType::LoadLevel3:
		{
			SStringMessage stringMessage = { "DiabloLevel_Andres_Dungeon_2", nullptr };
			CMainSingleton::PostMaster().Send(stringMessage);
			myStateStack.PushState(CStateStack::EState::LoadLevel);
		} break;
		case EMessageType::LoadLevel4:
		{
			SStringMessage stringMessage = { "Level4", nullptr };
			CMainSingleton::PostMaster().Send(stringMessage);
			myStateStack.PushState(CStateStack::EState::LoadLevel);
		} break;
		case EMessageType::LoadLevel5:
		{
			SStringMessage stringMessage = { "Level5", nullptr };
			CMainSingleton::PostMaster().Send(stringMessage);
			myStateStack.PushState(CStateStack::EState::LoadLevel);
		} break;
		case EMessageType::LoadLevel6:
		{
			SStringMessage stringMessage = { "Level6", nullptr };
			CMainSingleton::PostMaster().Send(stringMessage);
			myStateStack.PushState(CStateStack::EState::LoadLevel);
		} break;
		case EMessageType::LoadLevel7:
		{
			SStringMessage stringMessage = { "Level7", nullptr };
			CMainSingleton::PostMaster().Send(stringMessage);
			myStateStack.PushState(CStateStack::EState::LoadLevel);
		} break;
		case EMessageType::LoadLevel8:
		{
			SStringMessage stringMessage = { "Level8", nullptr };
			CMainSingleton::PostMaster().Send(stringMessage);
			myStateStack.PushState(CStateStack::EState::LoadLevel);
		} break;
		case EMessageType::LoadLevel9:
		{
			SStringMessage stringMessage = { "Level9", nullptr };
			CMainSingleton::PostMaster().Send(stringMessage);
			myStateStack.PushState(CStateStack::EState::LoadLevel);
		} break;
		default:
			break;
		}
	}
	else if(myGrupp == 4)
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


}
