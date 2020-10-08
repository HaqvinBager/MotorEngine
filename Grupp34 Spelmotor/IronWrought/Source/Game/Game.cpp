#include "stdafx.h"
#include "Game.h"
#include <Input.h>
#include <Timer.h>
#include <Scene.h>
#include <SimpleMath.h>
#include <Animation.h>

#include <CameraFactory.h>
#include <Camera.h>

#include <Model.h>
#include <ModelFactory.h>
#include <ModelInstance.h>
#include <LightFactory.h>
#include <EnvironmentLight.h>

#include "LevelLoader.h"


#ifdef _DEBUG
#pragma comment(lib, "Engine_Debug.lib")
#pragma comment(lib, "UnityImporterApp_Debug.lib")
#endif // _DEBUG
#ifdef NDEBUG
#pragma comment(lib, "Engine_Release.lib")
#endif // NDEBUG

namespace SM = DirectX::SimpleMath;
using namespace CommonUtilities;

CGame::CGame() 
	: myLevelLoader(new CLevelLoader())
{
}

CGame::~CGame()
{
}

void CGame::Init()
{
	myLevelLoader->Init();
	//myLevelLoader->LoadNewLevel("Levels/SampleScene_exportedLevelASCII.txt");
	CScene* scene = CScene::GetInstance();
	CLightFactory* lightFactory = CLightFactory::GetInstance();

	CCamera* camera = CCameraFactory::GetInstance()->CreateCamera(65.0f);
	
	CModelInstance* cube = new CModelInstance();	
	CModel* cubeModel = CModelFactory::GetInstance()->GetCube();
	cube->Init(cubeModel);

	CModelInstance* model = CModelFactory::GetInstance()->CreateModel("Model/Chest/Particle_Chest.fbx", { 0.025f, 0.025f, 0.025f });
	model->SetPosition({ 12.5f, 0.0f, 15.0f });

	CEnvironmentLight* environmentLight = lightFactory->CreateEnvironmentLight("Yokohama2.dds");
	environmentLight->SetDirection(SM::Vector3(0, 1, -1));
	environmentLight->SetColor(SM::Vector3(0.8f, 0.8f, 0.8f));

	scene->AddInstance(camera);
	scene->AddInstance(environmentLight);

	scene->AddInstance(cube);
	scene->AddInstance(model);
	scene->SetMainCamera(camera);
}

void CGame::Update()
{
	UpdateCamera();
	
	if (Input::GetInstance()->IsKeyPressed('1')) {
		myLevelLoader->LoadNewLevel("Levels/Dungeon_exportedLevelASCII.txt");
	}
	if (Input::GetInstance()->IsKeyPressed('2')) {
		myLevelLoader->LoadNewLevel("Levels/Gardens_exportedLevelASCII.txt");
	}
	if (Input::GetInstance()->IsKeyPressed('3')) {
		myLevelLoader->LoadNewLevel("Levels/Castle_exportedLevelASCII.txt");
	}
	if (Input::GetInstance()->IsKeyPressed('4')) {
		myLevelLoader->LoadNewLevel("Levels/SampleScene_exportedLevelASCII.txt");
	}
	if (Input::GetInstance()->IsKeyPressed('5')) {
		myLevelLoader->LoadNewLevel("Levels/SampleScene_exportedLevelASCII_Smily.txt");
	}
}

void CGame::UpdateCamera()
{
	const float dt = CTimer::Dt();
	float cameraMoveSpeed = 20.0f;
	float verticalMoveSpeedModifier = 0.5f;
	SM::Vector3 camera_movement_input(0, 0, 0);
	camera_movement_input.z = Input::GetInstance()->IsKeyDown('W') ? cameraMoveSpeed : camera_movement_input.z;
	camera_movement_input.z = Input::GetInstance()->IsKeyDown('S') ? -cameraMoveSpeed : camera_movement_input.z;
	camera_movement_input.x = Input::GetInstance()->IsKeyDown('D') ? cameraMoveSpeed : camera_movement_input.x;
	camera_movement_input.x = Input::GetInstance()->IsKeyDown('A') ? -cameraMoveSpeed : camera_movement_input.x;
	camera_movement_input.y = Input::GetInstance()->IsKeyDown('R') ? cameraMoveSpeed * verticalMoveSpeedModifier : camera_movement_input.y;
	camera_movement_input.y = Input::GetInstance()->IsKeyDown('F') ? -cameraMoveSpeed * verticalMoveSpeedModifier : camera_movement_input.y;

	float rotationSpeed = 5.0f;
	float camera_rotation_input = 0;
	camera_rotation_input = Input::GetInstance()->IsKeyDown('Q') ? -rotationSpeed : camera_rotation_input;
	camera_rotation_input = Input::GetInstance()->IsKeyDown('E') ? rotationSpeed : camera_rotation_input;

	CCamera* camera = CScene::GetInstance()->GetMainCamera();
	camera->Move(camera_movement_input * dt);
	camera->Rotate({ 0, camera_rotation_input * dt, 0 });
}