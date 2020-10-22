#include "stdafx.h"
#include "Game.h"
#include "Scene.h"

#include "Engine.h"
#include "CameraFactory.h"
#include "ModelFactory.h"
#include "SimpleMath.h"
#include "Camera.h"
#include "ModelInstance.h"
#include "LightFactory.h"
#include "EnvironmentLight.h"
#include "PointLight.h"
#include "Timer.h"
#include "ParticleFactory.h"
#include "ParticleInstance.h"
#include "Input.h"
#include "VFXFactory.h"
#include "VFXInstance.h"

#ifdef _DEBUG
#pragma comment(lib, "Engine_Debug.lib")
#endif // _DEBUG
#ifdef NDEBUG
#pragma comment(lib, "Engine_Release.lib")
#endif // NDEBUG

namespace SM = DirectX::SimpleMath;
using namespace CommonUtilities;

CGame::CGame()
{}

CGame::~CGame()
{}

void CGame::Init()
{
	CScene* scene = CScene::GetInstance();
	CCameraFactory* cameraFactory = CCameraFactory::GetInstance();
	CModelFactory* modelFactory = CModelFactory::GetInstance();

	CCamera* camera = cameraFactory->CreateCamera(65.0f);
	camera->SetPosition(SM::Vector3(0.0f, 1.0f, -10.0f));
	scene->AddInstance(camera);
	scene->SetMainCamera(camera);

	//Light Factory
	CLightFactory* lightFactory = CLightFactory::GetInstance();
	
	//Environment Light
	CEnvironmentLight* environmentLight = lightFactory->CreateEnvironmentLight("Yokohama2.dds");
	environmentLight->SetDirection(SM::Vector3(0.0f, 1.0f, -1.0f));
	environmentLight->SetColor(SM::Vector3(0.8f, 0.8f, 0.8f));
	scene->AddInstance(environmentLight);
	//End Environment Light

	//Point Lights
	//constexpr UINT numberOfPointLights = 4;
	//constexpr float lightStep = 3.5;
	//constexpr float commonRange = 40.0f;
	//constexpr float commonIntensity = 50.0f;

	//std::vector<SM::Vector3> pointLightPositions;
	//pointLightPositions.emplace_back(SM::Vector3{ -2.0f*lightStep, 0.5f * lightStep, -4.0f });
	//pointLightPositions.emplace_back(SM::Vector3{ -lightStep, 0.75f * lightStep, -2.0f });
	//pointLightPositions.emplace_back(SM::Vector3{ 0.0f, 0.5f * lightStep, -4.0f });
	//pointLightPositions.emplace_back(SM::Vector3{ lightStep, 0.75f * lightStep, -4.0f });

	//std::vector<SM::Vector4> pointLightColorsAndIntensities;
	//pointLightColorsAndIntensities.emplace_back(SM::Vector4{ 1.0f, 0.0f, 0.0f, commonIntensity });
	//pointLightColorsAndIntensities.emplace_back(SM::Vector4{ 0.0f, 1.0f, 0.0f, commonIntensity });
	//pointLightColorsAndIntensities.emplace_back(SM::Vector4{ 0.0f, 0.0f, 1.0f, commonIntensity });
	//pointLightColorsAndIntensities.emplace_back(SM::Vector4{ 1.0f, 0.0f, 1.0f, commonIntensity });

	//for (UINT i = 0; i < numberOfPointLights; ++i) {
	//	CPointLight* newPointLight = lightFactory->CreatePointLight();
	//	newPointLight->SetPosition(pointLightPositions[i]);
	//	newPointLight->SetColor({ 
	//		pointLightColorsAndIntensities[i].x, 
	//		pointLightColorsAndIntensities[i].y , 
	//		pointLightColorsAndIntensities[i].z 
	//		});
	//	newPointLight->SetRange(commonRange);
	//	newPointLight->SetIntensity(pointLightColorsAndIntensities[i].w);
	//	scene->AddInstance(newPointLight);
	//}
	//End Point Lights

	//Models
	CModel* cubeModel = modelFactory->GetCube();
	CModel* model = modelFactory->GetModel("Models/Chest/Particle_Chest.fbx");
	if (!model)
	{
		model = modelFactory->LoadModel("");
		if (!model)
		{
			return;
		}
	}

	constexpr size_t numberOfInstances = 6;
	constexpr float step = 3.0f;
	myInstances.resize(numberOfInstances + 3);

	std::vector<SM::Vector3> positions;
	//Cubes
	positions.emplace_back(SM::Vector3{  0.0f, -step, 10.0f });
	positions.emplace_back(SM::Vector3{  0.0f,  step, 10.0f });
	positions.emplace_back(SM::Vector3{  step,  0.0f, 10.0f + -step });
	positions.emplace_back(SM::Vector3{ -step,  0.0f, 10.0f +  step });
	positions.emplace_back(SM::Vector3{  step,  0.0f, 10.0f +  step });
	positions.emplace_back(SM::Vector3{ -step,  0.0f, 10.0f + -step });
	
	// Model
	positions.emplace_back(SM::Vector3{ 0.0f, -1.0f, 0.0f });
	positions.emplace_back(SM::Vector3{ -10.0f, -1.0f, 0.0f });
	positions.emplace_back(SM::Vector3{ 10.0f, -1.0f, 0.0f });

	for (int i = 0; i < numberOfInstances; ++i) {
		myInstances[i] = new CModelInstance();
		myInstances[i]->Init(cubeModel);
		myInstances[i]->SetPosition(positions[i]);
		scene->AddInstance(myInstances[i]);
	}
	// END CUBES

	// MODELUS
	constexpr int numberOfModels = 3;
	for (int i = 0; i < numberOfModels; ++i) {
		myInstances[numberOfInstances + i] = new CModelInstance();
		myInstances[numberOfInstances + i]->Init(model);
		myInstances[numberOfInstances + i]->SetScale(0.025f);
		myInstances[numberOfInstances + i]->SetPosition(positions[numberOfInstances + i]);
		//scene->SetModelToOutline(myInstances[numberOfInstances + i]);
		scene->AddInstance(myInstances[numberOfInstances + i]);
	}
	// END MODELUS

	//Start Particles
	CParticleFactory* particleFactory = CParticleFactory::GetInstance();
	std::string particlePath = "ParticleData_SmokeEmitter.json";
	CParticle* particle = particleFactory->GetParticle(particlePath);
	if (!particle) {
		particle = particleFactory->LoadParticle(particlePath);
		if (!particle) {
			return;
		}
	}
	CParticleInstance* particleInstance = new CParticleInstance();
	particleInstance->Init(particle);
	particleInstance->SetPosition({0.0f, -1.0f, -2.0f});
	particleInstance->SetRotation({ 0.0f, 0.0f, 0.0f });
	scene->AddInstance(particleInstance);
	//End Particles

	//Start VFX
	CVFXFactory* vfxFactory = CVFXFactory::GetInstance();
	std::string vfxPath = "VFX/Fog_test.fbx";
	std::string vfxDataPath = "VFXData_FogWall.json";
	CVFXBase* vfxBase = vfxFactory->GetVFXBase(vfxPath, vfxDataPath);
	if(!vfxBase) {
		return;
	}
	CVFXInstance* vfxInstance = new CVFXInstance();
	vfxInstance->Init(vfxBase);
	vfxInstance->SetScale(0.025f);
	vfxInstance->SetTransform({ 0.0f, -3.0f, 10.0f }, { -90.0f, 0.0f, 0.0f });
	scene->AddInstance(vfxInstance);
	myVFX.emplace_back(vfxInstance);
	//End VFX
}

void CGame::Update(const float dt)
{
	CCamera* aCamera = CScene::GetInstance()->GetMainCamera();

	float speed = 10.0f;
	float rotationSpeedFactor = 0.25f;

	if (Input::GetInstance()->IsKeyDown('E'))
	{
		aCamera->Rotate({ 0.0f, speed * rotationSpeedFactor * dt, 0.0f });
	}
	if (Input::GetInstance()->IsKeyDown('Q'))
	{
		aCamera->Rotate({ 0.0f, -speed * rotationSpeedFactor * dt, 0.0f });
	}

	auto camera = CScene::GetInstance()->GetMainCamera();
	SM::Vector3 camera_movement_input(0, 0, 0);
	camera_movement_input.z = Input::GetInstance()->IsKeyDown('W') ? speed : camera_movement_input.z;
	camera_movement_input.x = Input::GetInstance()->IsKeyDown('A') ? -speed : camera_movement_input.x;
	camera_movement_input.z = Input::GetInstance()->IsKeyDown('S') ? -speed : camera_movement_input.z;
	camera_movement_input.x = Input::GetInstance()->IsKeyDown('D') ? speed : camera_movement_input.x;
	camera_movement_input.y = Input::GetInstance()->IsKeyDown('R') ? speed : camera_movement_input.y;
	camera_movement_input.y = Input::GetInstance()->IsKeyDown('F') ? -speed : camera_movement_input.y;
	camera->Move(camera_movement_input * dt);

	if (Input::GetInstance()->IsMouseDown(Input::MouseButton::Left))
	{
		SM::Vector2 mouse_movement_input(0, 0);
		mouse_movement_input.x = (float)Input::GetInstance()->MouseDeltaX();
		mouse_movement_input.y = (float)Input::GetInstance()->MouseDeltaY();
		camera->Rotate({ -mouse_movement_input.y * rotationSpeedFactor * dt, -mouse_movement_input.x * rotationSpeedFactor * dt, 0.0f });
	}

	static int index = 6;
	if (Input::GetInstance()->IsKeyPressed(VK_SPACE)) {
		CScene::GetInstance()->SetModelToOutline(CScene::GetInstance()->CullModels(camera)[index]);
		index++;
		if(index > 7) {
			index = 6;
		}
	}

	DirectX::SimpleMath::Vector3 rotateBy = { 0.0f, 2.0f, 0.0f };
	rotateBy *= dt;

	static DirectX::SimpleMath::Vector2 scrollAmount = {0, 0};
	static DirectX::SimpleMath::Vector2 scrollAmount2 = {0, 0};
	scrollAmount += {0.15f * dt, 0.15f * dt};
	scrollAmount2 += {0.15f * dt, 0.15f * dt};

	////Rotate light
	//float time = CTimer::Time() * 3.0f;
	//constexpr float radius = 5.0f;
	////CScene::GetInstance()->GetEnvironmentLight()->SetDirection({ sinf(time), 0, cosf(time) });
	//CScene::GetInstance()->GetPointLights()[0]->SetPosition({ radius *sinf(time), 0, radius *cosf(time) });
	////End rotate light

	//for (int i = 0; i < myInstances.size(); ++i) {
	//	//myInstances[i]->Rotate(rotateBy);
	//	myInstances[i]->Scroll(scrollAmount, scrollAmount2);
	//}

	//for (auto& instance : myVFX) {
	//	//instance->Rotate(rotateBy);
	//	instance->Scroll(scrollAmount, scrollAmount2);
	//}
}
