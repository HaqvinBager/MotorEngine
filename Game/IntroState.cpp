#include "stdafx.h"
#include "IntroState.h"
#include "Scene.h"
#include "SpriteInstance.h"
#include "SpriteFactory.h"
#include "GameObject.h"
#include "CameraComponent.h"
#include "CameraControllerComponent.h"
#include "TransformComponent.h"
#include "EnviromentLightComponent.h"
#include "JsonReader.h"
#include "MainSingleton.h"
#include "PopupTextService.h"

CIntroState::CIntroState(CStateStack& aStateStack, const CStateStack::EState aState)
	: CState(aStateStack, aState)
	, myScene(nullptr)
	, myBackground(nullptr)
	, myFeedbackTimer(0.0f)
	, myFeedbackThreshold(10.0f)
	, myHasShownTutorial(false)
	, myWillShowTutorial(true)
	, myIntroStateActive(false)
{
}

CIntroState::~CIntroState()
{
}

void CIntroState::Awake()
{
	myScene = new CScene();
}

void CIntroState::Start()
{
	rapidjson::Document document = CJsonReader::LoadDocument("Json/IntroStateSettings.json");

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

	if (!document.HasParseError()) {
		if (document.HasMember("Background Path"))
		{
			myBackground = new CSpriteInstance(*myScene, true);
			myBackground->Init(CSpriteFactory::GetInstance()->GetSprite(document["Background Path"].GetString()));
			myBackground->SetSize({ 15.1f, 8.5f });

			myIntroStateActive = true;
			CMainSingleton::PostMaster().Send({ EMessageType::IntroStarted, NULL });
		}
	}

	CEngine::GetInstance()->AddScene(myState, myScene);
	CEngine::GetInstance()->SetActiveScene(myState);

	CMainSingleton::PostMaster().Subscribe(EMessageType::StopDialogue, this);

	CTimer::Mark();
}

void CIntroState::Stop()
{
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::StopDialogue, this);
	myScene->DestroySprites();
	delete myScene;
	myScene = nullptr;
}

void CIntroState::Update()
{
	if (!myIntroStateActive)
	{
		myStateStack.PopTopAndPush(CStateStack::EState::LoadLevel);
		return;
	}

	myFeedbackTimer += CTimer::Dt();
	
	if ((myFeedbackTimer > myFeedbackThreshold) && myWillShowTutorial)
	{
		myHasShownTutorial = true;
		myFeedbackTimer = 0.0f;
		CMainSingleton::PopupTextService().SpawnPopup(EPopupType::Tutorial, "Press Space to Continue, or Escape to Skip");
	}

	if (Input::GetInstance()->IsKeyPressed(VK_SPACE)) {
		myFeedbackTimer = 0.0f;
		if (myHasShownTutorial)
		{
			myWillShowTutorial = false;
		}
	}
}

void CIntroState::Receive(const SMessage& aMessage)
{
	switch (aMessage.myMessageType)
	{
	case EMessageType::StopDialogue:
		myStateStack.PopTopAndPush(CStateStack::EState::LoadLevel);
		break;
	default:
		break;
	}
}


