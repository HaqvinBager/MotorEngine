#include "stdafx.h"
#include "TitleScreenState.h"
#include "Scene.h"
#include "SpriteInstance.h"
#include "SpriteFactory.h"
#include "GameObject.h"
#include "CameraComponent.h"
#include "CameraControllerComponent.h"
#include "TransformComponent.h"
#include "EnviromentLightComponent.h"
#include "JsonReader.h"
#include "Input.h"

#include "MainSingleton.h"
#include "PostMaster.h"

CTitleScreenState::CTitleScreenState(CStateStack& aStateStack, const CStateStack::EState aState)
	: CState(aStateStack, aState)
	, myScene(nullptr)
	, myBackground(nullptr)
	, myTitleScreenActive(false)
{
}

CTitleScreenState::~CTitleScreenState()
{
}

void CTitleScreenState::Awake()
{
	myScene = new CScene();
}

void CTitleScreenState::Start()
{
	CMainSingleton::PostMaster().Send({ EMessageType::PlayMainMenuMusic, nullptr });

	rapidjson::Document document = CJsonReader::LoadDocument("Json/TitleScreenSettings.json");

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

	if (!document.HasParseError()) {
		if (document.HasMember("Background Path"))
		{
			myBackground = new CSpriteInstance(*myScene, true);
			myBackground->Init(CSpriteFactory::GetInstance()->GetSprite(document["Background Path"].GetString()));

			myTitleScreenActive = true;
		}
	}

	CEngine::GetInstance()->AddScene(myState, myScene);
	CEngine::GetInstance()->SetActiveScene(myState);

	CTimer::Mark();
}

void CTitleScreenState::Stop()
{
	myScene->DestroySprites();
	myScene = nullptr;
	//delete myScene;
	//myScene = nullptr;
}

void CTitleScreenState::Update()
{
	if (!myTitleScreenActive)
	{
		myStateStack.PopTopAndPush(CStateStack::EState::MainMenu);
		return;
	}

	if (Input::GetInstance()->IsKeyPressed(VK_RETURN)
		|| Input::GetInstance()->IsKeyPressed(VK_SPACE)
		|| Input::GetInstance()->IsKeyPressed(VK_ESCAPE)
		|| Input::GetInstance()->IsMousePressed(Input::MouseButton::Left)
		|| Input::GetInstance()->IsMousePressed(Input::MouseButton::Right)
		)
	{
		myStateStack.PopTopAndPush(CStateStack::EState::MainMenu);
		return;
	}
}

void CTitleScreenState::ReceiveEvent(const EInputEvent /*anEvent*/)
{
}

void CTitleScreenState::Receive(const SStringMessage& /*aMessage*/)
{
}
