#include "stdafx.h"
#include "BootUpState.h"
#include "Scene.h"
#include "SpriteInstance.h"
#include "SpriteFactory.h"
#include "GameObject.h"
#include "CameraComponent.h"
#include "CameraControllerComponent.h"
#include "TransformComponent.h"
#include "EnviromentLightComponent.h"
#include "JsonReader.h"

CBootUpState::CBootUpState(CStateStack& aStateStack, const CStateStack::EState aState)
	: CState(aStateStack, aState)
	, myScene(nullptr)
	, myTimer(0.0f)
	, myLogoDisplayDuration(4.0f)
	, myFadeOutStart(3.0f)
	, myLogoToRender(0)
{
}

CBootUpState::~CBootUpState()
{
}

void CBootUpState::Awake()
{
	myScene = new CScene();
}

void CBootUpState::Start()
{
	rapidjson::Document document = CJsonReader::LoadDocument("Json/SplashSettings.json");

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

	myLogos.emplace_back(new CSpriteInstance(*myScene, true));
	myLogos.back()->Init(CSpriteFactory::GetInstance()->GetSprite(document["TGA Logo Path"].GetString()));
	myLogos.back()->SetShouldRender(true);

	myLogos.emplace_back(new CSpriteInstance(*myScene, true));
	myLogos.back()->Init(CSpriteFactory::GetInstance()->GetSprite(document["Group Logo Path"].GetString()));
	myLogos.back()->SetShouldRender(false);

	CEngine::GetInstance()->AddScene(myState, myScene);
	CEngine::GetInstance()->SetActiveScene(myState);

	CTimer::Mark();
	myTimer = 0.0f;
	myLogoToRender = 0;
}

void CBootUpState::Stop()
{
	myScene->DestroySprites();
	delete myScene;
	myScene = nullptr;
}

void CBootUpState::Update()
{
	myTimer += CTimer::Dt();
	std::cout << myTimer << std::endl;

	if (myTimer > myFadeOutStart)
	{ 
		auto color = myLogos[myLogoToRender]->GetColor();
		color.x = 1.0f - (myTimer - myFadeOutStart) / (myLogoDisplayDuration - myFadeOutStart);
		color.y = 1.0f - (myTimer - myFadeOutStart) / (myLogoDisplayDuration - myFadeOutStart);
		color.z = 1.0f - (myTimer - myFadeOutStart) / (myLogoDisplayDuration - myFadeOutStart);
		myLogos[myLogoToRender]->SetColor(color);
	}

	if (myTimer > myLogoDisplayDuration) {
		myTimer = 0.0f;
		
		myLogos[myLogoToRender]->SetShouldRender(false);
		myLogoToRender++;

		if (myLogoToRender >= myLogos.size())
		{
			myStateStack.PopTopAndPush(CStateStack::EState::MainMenu);
			return;
		}

		myLogos[myLogoToRender]->SetShouldRender(true);
	}
}

void CBootUpState::ReceiveEvent(const EInputEvent /*anEvent*/)
{
}

void CBootUpState::Receive(const SStringMessage& /*aMessage*/)
{
}
