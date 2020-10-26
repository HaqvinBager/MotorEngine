#include "stdafx.h"
#include "LoadLevelState.h"
#include "InGameState.h"
#include "StateStack.h"
#include "Scene.h"
#include "LevelLoader.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "ModelComponent.h"
#include "ModelFactory.h"
#include "Model.h"
#include "CameraFactory.h"
#include "Camera.h"
#include "LightFactory.h"
#include "EnvironmentLight.h"
#include <CUnityLoader.h> //In order to know the size of ObjectData

CLoadLevelState::CLoadLevelState(CStateStack& aStateStack) : CState(aStateStack) {

}

CLoadLevelState::~CLoadLevelState()
{
}

void CLoadLevelState::Awake()
{
	//Push Load Level State for Level (Saved somewhere)
	CLevelLoader levelLoader;// = //std::make_unique<CLevelLoader>();
	levelLoader.Init();
	levelLoader.LoadNewLevel("Levels/SampleScene");

	CGameObject* player = new CGameObject();
	player->AddComponent<CTransformComponent>(*player);
	auto model = player->AddComponent<CModelComponent>(*player);

	CModel* playerModel = CModelFactory::GetInstance()->GetModelPBR("Assets/3D/Character/MainCharacter/CH_PL_Daughter_01_19G4_1_19.fbx");
	model->SetMyModel(playerModel);

	CCamera* camera = CCameraFactory::GetInstance()->CreateCamera(65.0f);

	CEnvironmentLight* environmentLight = CLightFactory::GetInstance()->CreateEnvironmentLight("Yokohama2.dds");

	CScene::GetInstance()->AddInstance(environmentLight);
	CScene::GetInstance()->AddInstance(player);
	CScene::GetInstance()->AddInstance(camera);
	CScene::GetInstance()->SetMainCamera(camera);
	//When Load Level is Complete -> LoadLevelState will Pop itself and then this Awake will continue.


}

void CLoadLevelState::Start() {

	myStateStack.PushState(new CInGameState(myStateStack));
	myStateStack.Awake();
	myStateStack.Start();
}

void CLoadLevelState::Update()
{
}
