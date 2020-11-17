#include "WinInclude.h"
#include "Engine.h"
#include "Input.h"

#include <SimpleMath.h>
#include <Scene.h>
#include <CameraFactory.h>
#include <Camera.h>
#include "../Engine/Model.h"
//#include "Model.h"// Reminder. Gives directX Model.h not Engine Model.h
#include <ModelFactory.h>

#include <GameObject.h>
#include <TransformComponent.h>
#include <ModelComponent.h>
#include <AnimationComponent.h>
#include <CameraComponent.h>

#include <LightFactory.h>
#include <EnvironmentLight.h>
#include <Line.h>
#include <LineInstance.h>
#include <LineFactory.h>
#include <Timer.h>
#include "LevelLoader.h"

#include <Animation.h>

#include <filesystem>
#include <iostream>
#include <map>

#include "helpers.h"
#include "loadAssetsFromDirectory.h"
#include "spriteViewer.h"

using namespace CommonUtilities;
namespace SM = DirectX::SimpleMath;
namespace MW = ModelViewer;

#define USE_CONSOLE_COMMAND
void InitConsole()
{
#pragma warning( push )
#pragma warning( disable : 4996 )
#pragma warning( disable : 6031 )
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
#pragma warning( pop )
}
void CloseConsole()
{
#pragma warning( push )
#pragma warning( disable : 4996 )
	fclose(stdin); 
	fclose(stdout);
	fclose(stderr);
#pragma warning( pop )
}

CGameObject* InitModels(const std::string& aModelPath)
{
	CScene* scene = CScene::GetInstance();

	CGameObject* gameobject = new CGameObject();

	gameobject->AddComponent<CModelComponent>(CModelComponent(*gameobject, aModelPath));
	gameobject->myTransform->Position({ 0.0f,0.0f,0.0f });
	
	scene->AddInstance(gameobject);

	return gameobject;
}

// Reminder: Vem tar hand om delete av CModel? CModelFactory verkar inte ta hand om det och inte CModelInstance?

void Update(std::vector<std::string>& aModelFilePathList, CGameObject* aCurrentGameObject,CGameObject* aCamera)
{
	float dt = CTimer::Dt();

	float cameraMoveSpeed = 500.0f;
	if (Input::GetInstance()->IsKeyDown(VK_UP)) {		aCamera->myTransform->MoveLocal({ 0.0f, 0.0f, cameraMoveSpeed * dt}); }
	if (Input::GetInstance()->IsKeyDown(VK_DOWN)) {		aCamera->myTransform->MoveLocal({ 0.0f, 0.0f, -cameraMoveSpeed * dt }); }
	if (Input::GetInstance()->IsKeyDown(VK_RIGHT)) {	aCamera->myTransform->MoveLocal({ cameraMoveSpeed * dt, 0.0f, 0.0f }); }
	if (Input::GetInstance()->IsKeyDown(VK_LEFT)) {		aCamera->myTransform->MoveLocal({ -cameraMoveSpeed * dt, 0.0f, 0.0f }); }

	// Rotation functions
	float rotationSpeed = 1.0f;
	// X axis
	if (Input::GetInstance()->IsKeyDown('R')){	aCurrentGameObject->GetComponent<CTransformComponent>()->Rotate({ rotationSpeed * dt,0.0f,0.0f });	}
	if (Input::GetInstance()->IsKeyDown('F')){	aCurrentGameObject->GetComponent<CTransformComponent>()->Rotate({ -rotationSpeed * dt,0.0f,0.0f }); }									   
	// Y axis								   
	if (Input::GetInstance()->IsKeyDown('T')){	aCurrentGameObject->GetComponent<CTransformComponent>()->Rotate({ 0.0f, rotationSpeed * dt,0.0f });	 }
	if (Input::GetInstance()->IsKeyDown('G')){	aCurrentGameObject->GetComponent<CTransformComponent>()->Rotate({ 0.0f, -rotationSpeed * dt,0.0f }); }									   
	// Z axis								   
	if (Input::GetInstance()->IsKeyDown('Y')) { aCurrentGameObject->GetComponent<CTransformComponent>()->Rotate({ 0.0f,0.0f,rotationSpeed * dt });	}
	if (Input::GetInstance()->IsKeyDown('H')){	aCurrentGameObject->GetComponent<CTransformComponent>()->Rotate({ 0.0f,0.0f,-rotationSpeed * dt }); }

	// ! Rotation Functions 

	// Zoom/ move Camera functions
	float moveSpeed = 5.0f;
	// X axis
	if (Input::GetInstance()->IsKeyDown('A')){	aCurrentGameObject->GetComponent<CTransformComponent>()->Move({ moveSpeed * dt, 0.0f, 0.0f });	}
	if (Input::GetInstance()->IsKeyDown('D')){	aCurrentGameObject->GetComponent<CTransformComponent>()->Move({ -moveSpeed * dt, 0.0f, 0.0f });	}
	// Y axis
	if (Input::GetInstance()->IsKeyDown('Q')){	aCurrentGameObject->GetComponent<CTransformComponent>()->Move({ 0.0f, moveSpeed * dt, 0.0f });	}
	if (Input::GetInstance()->IsKeyDown('E')){	aCurrentGameObject->GetComponent<CTransformComponent>()->Move({ 0.0f, -moveSpeed * dt, 0.0f }); }
	// Z axis
	if (Input::GetInstance()->IsKeyDown('W')){	aCurrentGameObject->GetComponent<CTransformComponent>()->Move({ 0.0f, 0.0f, -moveSpeed * dt }); }
	if (Input::GetInstance()->IsKeyDown('S')){	aCurrentGameObject->GetComponent<CTransformComponent>()->Move({ 0.0f, 0.0f, moveSpeed * dt });	}
	// ! Zoom/ move functions

	// Reset functions
	if (Input::GetInstance()->IsKeyDown('K'))
	{
		aCurrentGameObject->GetComponent<CTransformComponent>()->Transform({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	}
	if (Input::GetInstance()->IsKeyDown('C'))
	{
		aCamera->myTransform->Rotation({ 33.f,-45.f,0.f });
		aCamera->myTransform->Position({ 3.0f,4.0f,-3.5f });
	}
	// ! Reset function

	if (Input::GetInstance()->IsKeyPressed(VK_ESCAPE))
	{
		SetForegroundWindow(GetConsoleWindow());

		size_t loadModelNumber = aModelFilePathList.size();
		std::cout << "Which model do you wish to load Give a number between: 0 and " << aModelFilePathList.size() - 1 << "\nL> ";
		std::cin >> loadModelNumber;
		while (!MW::CheckForIncorrectModelNumber(loadModelNumber, aModelFilePathList.size()))
		{
			std::cin.clear();
			std::cout << "Try again: Which model do you wish to load Give a number between: 0 and " << aModelFilePathList.size() - 1 << std::endl;
			std::cin >> loadModelNumber;
		}
		
		std::cout << "Loading model number: " << loadModelNumber << " : " << aModelFilePathList[loadModelNumber] << std::endl;

		aCurrentGameObject->GetComponent<CModelComponent>()->SetModel(aModelFilePathList[loadModelNumber]);
		aCurrentGameObject->GetComponent<CTransformComponent>()->Transform({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });

		std::cout << "\nInstructions" << std::endl 
			<< "   Main Window: Press 'ESC' and then return to this Console." << std::endl
			<< "   Console: Enter the desired models number." << std::endl;

	}
}

CGameObject* InitAnimation(const std::string& aFilePath)
{
	CGameObject* gameObject = new CGameObject();

	gameObject->AddComponent<CModelComponent>(CModelComponent(*gameObject, aFilePath));
	gameObject->myTransform->Position({ 0.0f,0.0f,0.0f });

	if (MW::GetSuffixFromString(aFilePath) == "_SK")
	{
		const size_t lastSlashIndex		= aFilePath.find_last_of("\\/");
		const std::string folderPath	= aFilePath.substr(0, lastSlashIndex + 1);

		std::filesystem::path p(folderPath);
		std::filesystem::directory_iterator start(p);
		std::filesystem::directory_iterator end;

		std::vector<std::string> somePathsToAnimations;
		for (auto it = start; it != end; ++it)
		{
			if (it->path().extension() == ".fbx")
			{
				const std::string filePath = it->path().filename().string();
				if (MW::GetSuffixFromString(filePath) == "_AN")
				{
					somePathsToAnimations.emplace_back(folderPath + filePath);
				}
			}
		}

		CAnimationComponent* animComp = gameObject->AddComponent<CAnimationComponent>(*gameObject);
		animComp->GetMyAnimation()->Init(aFilePath.c_str(), somePathsToAnimations);
		animComp->SetBlend(0, 1, 1.0f);
		animComp->Awake();
	}

	CScene::GetInstance()->AddInstance(gameObject);

	return gameObject;
}
bool ChangeModel(CGameObject* aCurrentGameObject, std::vector<std::string>& aModelFilePathList)
{
	SetForegroundWindow(GetConsoleWindow());

	size_t loadModelNumber = aModelFilePathList.size();
	std::cout << "Which model do you wish to load Give a number between: 0 and " << aModelFilePathList.size() - 1 << "\nL> ";
	std::cin >> loadModelNumber;
	while (!MW::CheckForIncorrectModelNumber(loadModelNumber, aModelFilePathList.size()))
	{
		std::cin.clear();
		std::cout << "Try again: Which model do you wish to load Give a number between: 0 and " << aModelFilePathList.size() - 1 << std::endl;
		std::cin >> loadModelNumber;
	}

	std::cout << "Loading model number: " << loadModelNumber << " : " << aModelFilePathList[loadModelNumber] << std::endl;

	aCurrentGameObject->GetComponent<CModelComponent>()->SetModel(aModelFilePathList[loadModelNumber]);
	aCurrentGameObject->GetComponent<CTransformComponent>()->Transform({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });

	if (MW::GetSuffixFromString(aModelFilePathList[loadModelNumber]) == "_SK")
	{
		const size_t lastSlashIndex		= aModelFilePathList[loadModelNumber].find_last_of("\\/");
		const std::string folderPath	= aModelFilePathList[loadModelNumber].substr(0, lastSlashIndex + 1);

		std::filesystem::path p(folderPath);
		std::filesystem::directory_iterator start(p);
		std::filesystem::directory_iterator end;

		std::vector<std::string> somePathsToAnimations;
		for (auto& it = start; it != end; ++it)
		{
			if (it->path().extension() == ".fbx")
			{
				const std::string filePath = it->path().filename().string();
				if (MW::GetSuffixFromString(filePath) == "_AN")
				{
					somePathsToAnimations.emplace_back(folderPath + filePath);
				}
			}
		}

		if (!aCurrentGameObject->GetComponent<CAnimationComponent>())
		{
			CAnimationComponent* animComp = aCurrentGameObject->AddComponent<CAnimationComponent>(*aCurrentGameObject);
			animComp->GetMyAnimation()->Init(aModelFilePathList[loadModelNumber].c_str(), somePathsToAnimations);
			animComp->Awake();
		}
		else
		{
			aCurrentGameObject->GetComponent<CAnimationComponent>()->ReplaceAnimation(aModelFilePathList[loadModelNumber].c_str(), somePathsToAnimations);
		}
	}

	std::cout << "\nInstructions" << std::endl 
		<< "   Main Window: Press 'ESC' and then return to this Console." << std::endl
		<< "   Console: Enter the desired models number." << std::endl;

	return true;
}
void UpdateAnimationTest(CGameObject* aCurrentGameObject,CGameObject* /*aCamera*/, std::vector<std::string>& aModelFilePathList)
{
	const auto animComp = aCurrentGameObject->GetComponent<CAnimationComponent>();
	if (animComp)
	{
		if (Input::GetInstance()->IsKeyPressed('0')) { aCurrentGameObject->GetComponent<CAnimationComponent>()->PlayAnimation(0); }
		if (Input::GetInstance()->IsKeyPressed('1')) { aCurrentGameObject->GetComponent<CAnimationComponent>()->PlayAnimation(1); }
		if (Input::GetInstance()->IsKeyPressed('2')) { aCurrentGameObject->GetComponent<CAnimationComponent>()->PlayAnimation(2); }

		aCurrentGameObject->GetComponent<CAnimationComponent>()->Update();

		if (animComp->Enabled())
		{}
	}

	if (Input::GetInstance()->IsKeyPressed(VK_ESCAPE))
	{
		if (!ChangeModel(aCurrentGameObject, aModelFilePathList))
		{
			// oh no. How could this even happen :o?
		}
	}
}

//////////////////////////////////// MAIN STARTS HERE ///////////////////////////////////////////////////////////////////

#define RUNNING_ANIMATIONS_TEST

#define ASSET_ROOT "Assets"
#define ASSET_ROOT_ANIMATION_TEST "Assets/3D/Datadriven_Animation_Test"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	hInstance;
	hPrevInstance;
	lpCmdLine;
	nShowCmd;

#ifdef USE_CONSOLE_COMMAND
	InitConsole();
#endif

	CWindowHandler::SWindowData windowData;
	windowData.myX		= 100;
	windowData.myY		= 100;
	windowData.myWidth	= 1280;
	windowData.myHeight = 720;
	
	CEngine engine;
	bool shouldRun = engine.Init(windowData);
	if (!shouldRun)
		return 1;

// CAMERA
	CGameObject* camera = new CGameObject();
	/*CCameraComponent* camComp = */camera->AddComponent<CCameraComponent>(*camera);
	//CCamera* camera = CCameraFactory::GetInstance()->CreateCamera(65.0f, 5000.0f);
		camera->myTransform->Rotation({ 33.f,-45.f,0.f });
		camera->myTransform->Position({ 1.5f,2.0f,-1.5f });
		//CScene::GetInstance()->AddInstance(camera);
		CScene::GetInstance()->SetMainCamera(camera->GetComponent<CCameraComponent>());
// ENV LIGHT
	CEnvironmentLight* environmentLight = CLightFactory::GetInstance()->CreateEnvironmentLight("Yokohama2.dds");
		environmentLight->SetDirection(SM::Vector3(0, -1, -1));
		environmentLight->SetColor(SM::Vector3(1.0f, 1.0f, 1.0f));
		environmentLight->SetIntensity(1.0f);
		CScene::GetInstance()->AddInstance(environmentLight);
// GRID
	CLineInstance* grid = new CLineInstance();
		grid->Init(CLineFactory::GetInstance()->CreateGrid({ 0.33f,0.33f,0.33f, 1.0f }));
		grid->SetPosition({ 0.0f,-0.01f,0.0f });
		CScene::GetInstance()->AddInstance(grid);
// AXIS ORIGIN MARKER
	CLineInstance* origin = new CLineInstance();
		origin->Init(CLineFactory::GetInstance()->CreateAxisMarker());
		origin->SetPosition({ 0.0f,0.01f,0.0f });
		CScene::GetInstance()->AddInstance(origin);

	CGameObject* currentGameObject = nullptr;

#ifdef RUNNING_ANIMATIONS_TEST
	std::vector<std::string> filePaths;
	MW::LoadModelPaths(ASSET_ROOT_ANIMATION_TEST, filePaths);

	currentGameObject = InitAnimation(filePaths[0]);

#else
	std::vector<std::string> filePaths;
	MW::LoadModelPaths(ASSET_ROOT, filePaths);
	currentGameObject = InitModels(filePaths[0]);

#endif // ! RUNNING_ANIMATIONS
	int counter = 0;
	for (auto& str : filePaths)
	{
		const size_t lastSlashIndex = str.find_last_of("\\/");
		std::string modelName		= str.substr(lastSlashIndex + 1, str.size() - lastSlashIndex - 5);
		std::cout << "Number: " << counter << "\t: " << modelName << std::endl;
		++counter;
	}
	std::cout << "\nInstructions" << std::endl
		<< "   Main Window (Iron Wrought, where you can see a model): Press 'ESC' and then return to this window (Console)." << std::endl
		<< "   Console: Enter the desired models number." << std::endl << std::endl
		<< "CONTROLS:\n"
		<< "   Arrow Keys...........Move Camera\n"
		<< "   W/A/S/D/Q/E..........Move Model\n"
		<< "   R/F..................Rotate Model around its X axis\n"
		<< "   T/G..................Rotate Model around its Y axis\n"
		<< "   Y/H..................Rotate Model around its Z axis\n"
		<< "   K....................Reset Model\n"
		<< "   C....................Reset Camera\n"
		<< "   I....................Show InGame - HUD (Group 4)\n"
		<< std::endl;

	bool showUI = false;
	
	MSG windowMessage = { 0 };
	while (shouldRun)
	{
		while (PeekMessage(&windowMessage, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&windowMessage);
			DispatchMessage(&windowMessage);
	
			if (windowMessage.message == WM_QUIT)
			{
				shouldRun = false;
			}
		}
	
		engine.BeginFrame();
#ifdef RUNNING_ANIMATIONS_TEST
		UpdateAnimationTest(currentGameObject, camera, filePaths);
#else
		Update(filePaths, currentGameObject, camera);
#endif // ! RUNNING_ANIMATIONS_TEST
		if (showUI)
		{
			SpriteViewer::Update();
		}
		else
		{
			if (Input::GetInstance()->IsKeyPressed('I'))
			{
				SpriteViewer::Init();
				showUI = true;
			}
		}

		engine.RenderFrame();
		engine.EndFrame();
		Input::GetInstance()->update();
	}

	delete currentGameObject;
	currentGameObject = nullptr;

	delete environmentLight;
	environmentLight = nullptr;

	delete grid;
	grid = nullptr;

	delete origin;
	origin = nullptr;
	

#ifdef USE_CONSOLE_COMMAND
	CloseConsole();
#endif

	return 0;
}
