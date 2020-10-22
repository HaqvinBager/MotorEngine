#include "pch.h"
#include "Game.hpp"
//DXEngine
#include "Engine.hpp"
#include "Model.hpp"
#include "ModelInstance.hpp"
#include "ModelFactory.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "CameraFactory.hpp"
#include "EnvironmentLight.hpp"
#include "DirectionalLight.hpp"
#include "LightFactory.hpp"

#include "Line.hpp"
#include "LineInstance.hpp"
#include "LineFactory.hpp"

// CommonUtilities
#include "InputHandler.hpp"
#include "DL_Debug.h"
#include "UtilityTimer.hpp"
using namespace DL_Debug;

namespace MS = ModelSystem;
namespace SM = DirectX::SimpleMath;

Game::Game()
	: myCamera(nullptr)
	, myEnvironmentLight(nullptr)
	, myDirectionalLight(nullptr)
	, myGrid(nullptr)
{}
Game::~Game(){
	for (size_t i = 0; i < myModels.size(); ++i)
	{
		delete myModels[i];
		myModels[i] = nullptr;
	}
	for (size_t i = 0; i < myLines.size(); ++i)
	{
		delete myLines[i];
		myLines[i] = nullptr;
	}

	// CameraFactory holds container with all cameras and deletes them upon destruction

	delete myEnvironmentLight;
	myEnvironmentLight = nullptr;

	delete myDirectionalLight;
	myDirectionalLight = nullptr;

	delete myGrid;
	myGrid = nullptr;
}

bool Game::Init()
{
	Scene& scene = Scene::GetInstance();
	
	myCamera = CameraFactory::GetInstance().CreateCamera(65.0f);// 90 FoV Horizontal is 65 in Vertical
	myCamera->SetTransform(SM::Vector3(0.0f, 3.0f, -5.0f), SM::Vector3(0.0f, 0.0f, 0.0f));
	scene.AddInstance(myCamera);
	scene.SetMainCamera(myCamera);
	
	myEnvironmentLight = LightFactory::GetInstance().CreateEnvironmentLight(L"Textures/Yokohama2_1.dds");
	myEnvironmentLight->SetColor({ 1.0f, 0.95f, 0.92f, 0.0f });
	scene.AddInstance(myEnvironmentLight);
	scene.SetEnvironmentLight(myEnvironmentLight);
	
	myDirectionalLight = LightFactory::GetInstance().CreateDirectionalLight({ 0.2f,1.0f,-1.0f }, {1.0f,0.5f,0.0f,1.0f});
	scene.AddInstance(myDirectionalLight);
	scene.SetMainDirectionalLight(myDirectionalLight);

	ModelSystem::ModelFactory& modelFactory = ModelSystem::ModelFactory::GetInstance();
	CommonUtilities::UtilityTimer timer;
	MS::Model* model = modelFactory.LoadModel("Models/Characters/Enemy Robot/enemy_robot_idle.fbx");
	timer.Update();
	DL_GAME_LOG("Models/Characters/Enemy Robot/enemy_robot_idle.fbx took %f ms to load.", timer.GetTotalTime())
	if (!model)
	{
		assert(false && "No model was loaded!");
		return false;
	}
	MS::ModelInstance* modelInstance = new ModelSystem::ModelInstance();
	modelInstance->Init(model);
	modelInstance->SetTransform(SM::Vector3(0.0f, 0.0f, 0.0f), SM::Vector3(0.0f, 0.0f, 0.0f));
	scene.AddInstance(modelInstance);
	myModels.emplace_back(modelInstance);


	Line* line = LineFactory::GetInstance().CreateLine({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f });
	if (!line)
	{
		assert(false && "No line was loaded!");
		return false;
	}
	myLines.emplace_back(new LineInstance());
	myLines.back()->Init(line);
	//myLines.back()->SetTransform(SM::Vector3(0.0f, 0.0f, 0.0f), SM::Vector3(0.0f, 0.0f, 0.0f));
	scene.AddInstance(myLines.back());

	line = LineFactory::GetInstance().CreateLine({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f });
	if (!line)
	{
		assert(false && "No line was loaded!");
		return false;
	}
	myLines.emplace_back(new LineInstance());
	myLines.back()->Init(line);
	//myLines.back()->SetTransform(SM::Vector3(0.0f, 0.0f, 0.0f), SM::Vector3(0.0f, 0.0f, 0.0f));
	scene.AddInstance(myLines.back());

	line = LineFactory::GetInstance().CreateLine({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f });
	if (!line)
	{
		assert(false && "No line was loaded!");
		return false;
	}
	myLines.emplace_back(new LineInstance());
	myLines.back()->Init(line);
	//myLines.back()->SetTransform(SM::Vector3(0.0f, 0.0f, 0.0f), SM::Vector3(0.0f, 0.0f, 0.0f));
	scene.AddInstance(myLines.back());

	myGrid = new LineInstance();
	myGrid->Init(LineFactory::GetInstance().CreateGrid({ 0.3f,0.3f,0.3f ,1.0f}));
	myGrid->SetPosition({ 0.0f,0.0f,0.0f});
	scene.AddInstance(myGrid);

	return true;	
}
#include <iostream>
bool Game::Update(const float dt, CommonUtilities::InputHandler& aInputHandler)
{
	aInputHandler.Update();

	float camera_move = 10.0f;
	float camera_rot_speed = 33.0f;

	if (aInputHandler.IsKeyDown('W'))
	{
		myCamera->Move(SM::Vector3(0, 0, camera_move * dt));
	}
	if (aInputHandler.IsKeyDown('S'))
	{
		myCamera->Move(SM::Vector3(0, 0, -camera_move * dt));
	}

	if (aInputHandler.IsKeyDown('A'))
	{
		myCamera->Move(SM::Vector3(-camera_move * dt, 0, 0));
	}
	if (aInputHandler.IsKeyDown('D'))
	{
		myCamera->Move(SM::Vector3(camera_move * dt, 0, 0));
	}

	if (aInputHandler.IsKeyDown(VK_SPACE))
	{
		myCamera->Move(SM::Vector3(0, camera_move * dt, 0));
	}
	if (aInputHandler.IsKeyDown('C'))
	{
		myCamera->Move(SM::Vector3(0, -camera_move * dt, 0));
	}

	if (aInputHandler.IsKeyDown('Q'))
	{
		myCamera->Rotate(SM::Vector3(0, -camera_rot_speed * dt, 0));
	}
	if (aInputHandler.IsKeyDown('E'))
	{
		myCamera->Rotate(SM::Vector3(0, camera_rot_speed * dt, 0));
	}

	if (aInputHandler.IsKeyDown('U'))
	{
		myModels[0]->Rotate({ 0, 33.0f * dt, 0 });
	}

	if (aInputHandler.IsKeyDown('L'))
	{
		myDirectionalLight->Rotate({33.f * dt, 33.f * dt, 0});	
	}
	if (aInputHandler.IsKeyDown('O'))
	{
		myDirectionalLight->SetColorAndIntensity({1.0f,0.5f,0.0f, myDirectionalLight->GetColor().w + dt * 10.0f});	
	}
	if (aInputHandler.IsKeyDown('P'))
	{
		myDirectionalLight->SetColorAndIntensity({1.0f,0.5f,0.0f,myDirectionalLight->GetColor().w - dt * 10.0f});	
	}

	if (aInputHandler.IsKeyDown(VK_UP))
	{	
		myLines.back()->Move({0, 0.33f * dt, 0});	
	}
	if (aInputHandler.IsKeyDown(VK_DOWN))
	{
		myLines.back()->Move({0, -0.33f * dt, 0});	
	}
	if (aInputHandler.IsKeyDown(VK_LEFT))
	{
		myLines.back()->Rotate({0, -33.f * dt, 0});	
	}
	if (aInputHandler.IsKeyDown(VK_RIGHT))
	{	
		myLines.back()->Rotate({0, 33.f * dt, 0});	
	}

	if (aInputHandler.IsKeyDown('7'))
	{
		myGrid->Rotate({ 33.f * dt, 0.0f, 0.0f });
	}
	if (aInputHandler.IsKeyDown('8'))
	{
		myGrid->Rotate({ 0.0f, 33.f * dt, 0.0f });
	}
	if (aInputHandler.IsKeyDown('9'))
	{
		myGrid->Rotate({ 0.0f, 0.0f, 33.f * dt });
	}
	//if (aInputHandler.IsKeyDown('N'))
	//{	
	//	myLines.back()->GetLine()->Set({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f });
	//}
	//if (aInputHandler.IsKeyDown('M'))
	//{	
	//	myLines.back()->GetLine()->Set({ 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.5f, 0.0f, 1.0f, 1.0f });
	//}

	return true;
}

void Game::InitcallBack(){}
void Game::UpdateCallBack(){}