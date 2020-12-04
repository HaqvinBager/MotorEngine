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

#include <Animation.h>
#include <Canvas.h>

#include <filesystem>
#include <iostream>
#include <map>

#include "helpers.h"
#include "loadAssetsFromDirectory.h"
#include "spriteViewer.h"

//using namespace CommonUtilities;
namespace SM = DirectX::SimpleMath;
namespace MW = ModelViewer;

//#define VFX

#ifdef VFX
#include "../Game/AbilityComponent.h"//  Group 4 stuff might break for group 3!
	#ifdef _DEBUG
	#pragma comment (lib, "../../../Lib/Game_Debug.lib")
	#else
	#pragma comment (lib, "../../../Lib/Game_Release.lib")
	#endif
#endif

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

#define CAMERA_DEFAULT_ROT { 33.f,-45.f,0.f }
#define CAMERA_DEFAULT_POS { 1.75f,2.0f,-1.5f }

#ifdef VFX
CGameObject* InitVFX(CScene& aScene)
{
	CGameObject* go = new CGameObject();

	std::pair<EAbilityType, unsigned int> abilPair = { EAbilityType::AbilityTest, 1u };
	std::vector<std::pair<EAbilityType, unsigned int>> vecPair;
	vecPair.emplace_back(abilPair);
	vecPair.shrink_to_fit();

	go->AddComponent<CAbilityComponent>(*go, vecPair);
	//go->Awake();
	//go->Start();
	//for (auto& obj : aScene.GetActiveGameObjects())
	//{
	//	obj->Awake();
	//}
	aScene.AddInstance(go);

	const std::vector<CGameObject*>& gameObjects = aScene.GetActiveGameObjects();
	size_t currentSize = gameObjects.size();

	for (size_t i = 0; i < currentSize; ++i)
	{
		if (gameObjects[i])
		{
			gameObjects[i]->Awake();
		}

	}

	size_t newSize = gameObjects.size();

	//Late awake
	for (size_t j = currentSize; j < newSize; ++j) 
	{
		if (gameObjects[j])
		{
			gameObjects[j]->Awake();
		}
	}

	go->GetComponent<CAbilityComponent>()->UseAbility(EAbilityType::AbilityTest, { 0.f,0.f,0.f });

	return go;
}
void UpdateVFX(CGameObject* aCurrentGameObject,CGameObject* /*aCamera*/, CScene* aScene)
{
	for (auto& obj : aScene->GetActiveGameObjects())
	{
		obj->Update();
	}
	if (Input::GetInstance()->IsKeyPressed('V'))
	{
		aCurrentGameObject->GetComponent<CAbilityComponent>()->UseAbility(EAbilityType::AbilityTest, { 0.f,0.f,0.f });
	}
}
#endif

#pragma region TRANSFORM FUNCTIONS
void UpdateCamera(CGameObject& aCamera, const float dt)
{
	float cameraMoveSpeed = 500.0f;
	if (Input::GetInstance()->IsKeyDown(VK_UP)) {		aCamera.myTransform->MoveLocal({ 0.0f, 0.0f, cameraMoveSpeed * dt}); }
	if (Input::GetInstance()->IsKeyDown(VK_DOWN)) {		aCamera.myTransform->MoveLocal({ 0.0f, 0.0f, -cameraMoveSpeed * dt }); }
	if (Input::GetInstance()->IsKeyDown(VK_RIGHT)) {	aCamera.myTransform->MoveLocal({ cameraMoveSpeed * dt, 0.0f, 0.0f }); }
	if (Input::GetInstance()->IsKeyDown(VK_LEFT)) {		aCamera.myTransform->MoveLocal({ -cameraMoveSpeed * dt, 0.0f, 0.0f }); }

	if (Input::GetInstance()->IsKeyDown('C'))
	{
		aCamera.myTransform->Rotation(CAMERA_DEFAULT_ROT);
		aCamera.myTransform->Position(CAMERA_DEFAULT_POS);
	}
}
void RotateModel(CGameObject& aCurrentGameObject, const float dt)
{
	float rotationSpeed = 1.0f;
	// X axis
	if (Input::GetInstance()->IsKeyDown('R')){	aCurrentGameObject.GetComponent<CTransformComponent>()->Rotate({ rotationSpeed * dt,0.0f,0.0f });	}
	if (Input::GetInstance()->IsKeyDown('F')){	aCurrentGameObject.GetComponent<CTransformComponent>()->Rotate({ -rotationSpeed * dt,0.0f,0.0f }); }									   
	// Y axis								   
	if (Input::GetInstance()->IsKeyDown('T')){	aCurrentGameObject.GetComponent<CTransformComponent>()->Rotate({ 0.0f, rotationSpeed * dt,0.0f });	 }
	if (Input::GetInstance()->IsKeyDown('G')){	aCurrentGameObject.GetComponent<CTransformComponent>()->Rotate({ 0.0f, -rotationSpeed * dt,0.0f }); }									   
	// Z axis								   
	if (Input::GetInstance()->IsKeyDown('Y')) { aCurrentGameObject.GetComponent<CTransformComponent>()->Rotate({ 0.0f,0.0f,rotationSpeed * dt });	}
	if (Input::GetInstance()->IsKeyDown('H')){	aCurrentGameObject.GetComponent<CTransformComponent>()->Rotate({ 0.0f,0.0f,-rotationSpeed * dt }); }
}
void MoveModel(CGameObject& aCurrentGameObject, const float dt)
{
	float moveSpeed = 5.0f;
	// X axis
	if (Input::GetInstance()->IsKeyDown('A')){	aCurrentGameObject.GetComponent<CTransformComponent>()->Move({ moveSpeed * dt, 0.0f, 0.0f });	}
	if (Input::GetInstance()->IsKeyDown('D')){	aCurrentGameObject.GetComponent<CTransformComponent>()->Move({ -moveSpeed * dt, 0.0f, 0.0f });	}
	// Y axis
	if (Input::GetInstance()->IsKeyDown('Q')){	aCurrentGameObject.GetComponent<CTransformComponent>()->Move({ 0.0f, moveSpeed * dt, 0.0f });	}
	if (Input::GetInstance()->IsKeyDown('E')){	aCurrentGameObject.GetComponent<CTransformComponent>()->Move({ 0.0f, -moveSpeed * dt, 0.0f }); }
	// Z axis
	if (Input::GetInstance()->IsKeyDown('W')){	aCurrentGameObject.GetComponent<CTransformComponent>()->Move({ 0.0f, 0.0f, -moveSpeed * dt }); }
	if (Input::GetInstance()->IsKeyDown('S')){	aCurrentGameObject.GetComponent<CTransformComponent>()->Move({ 0.0f, 0.0f, moveSpeed * dt });	}
}
void ResetModelTransform(CGameObject& aCurrentGameObject)
{
	if (Input::GetInstance()->IsKeyDown('K'))
	{
		aCurrentGameObject.GetComponent<CTransformComponent>()->Transform({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	}
}
#pragma endregion ! TRANSFORM FUNCTIONS

#pragma region MODELS 
CGameObject* InitModels(const std::string& aModelPath, CScene& aScene)
{
	//CScene* scene = CScene::GetInstance();

	CGameObject* gameobject = new CGameObject();

	gameobject->AddComponent<CModelComponent>(CModelComponent(*gameobject, aModelPath));
	gameobject->myTransform->Position({ 0.0f,0.0f,0.0f });
	aScene.AddInstance(gameobject);

	return gameobject;
}
void ChangeModel(CGameObject* aCurrentGameObject, const std::vector<std::string>& aModelFilePathList)
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
void UpdateModel(const std::vector<std::string>& aModelFilePathList, CGameObject* aCurrentGameObject,CGameObject* aCamera)
{
	float dt = CTimer::Dt();

	UpdateCamera(*aCamera, dt);
	RotateModel(*aCurrentGameObject, dt);
	MoveModel(*aCurrentGameObject, dt);
	ResetModelTransform(*aCurrentGameObject);

	if (Input::GetInstance()->IsKeyPressed(VK_ESCAPE))
	{
		ChangeModel(aCurrentGameObject, aModelFilePathList);
	}
}
#pragma endregion ! MODELS

#pragma region ANIMATIONS
CGameObject* InitAnimation(const std::string& aFilePath, CScene& aScene)
{
	CGameObject* gameObject = new CGameObject();

	gameObject->AddComponent<CModelComponent>(CModelComponent(*gameObject, aFilePath));
	gameObject->myTransform->Position({ 0.0f,0.0f,0.0f });

	if (MW::GetSuffixFromString(aFilePath) == "_SK")
	{
		std::vector<std::string> somePathsToAnimations = MW::Get_ANFiles(aFilePath);
#ifndef VFX
		CAnimationComponent* animComp = gameObject->AddComponent<CAnimationComponent>(*gameObject, aFilePath, somePathsToAnimations);
		animComp->Awake();
#else
		gameObject->AddComponent<CAnimationComponent>(*gameObject, aFilePath, somePathsToAnimations);
#endif
	}

	aScene.AddInstance(gameObject);

	return gameObject;
}
bool ChangeAnimationModel(CGameObject* aCurrentGameObject, const std::vector<std::string>& aModelFilePathList)
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

	if (MW::Has_SKSuffix(aModelFilePathList[loadModelNumber]))
	{
		std::vector<std::string> somePathsToAnimations = MW::Get_ANFiles(aModelFilePathList[loadModelNumber]);

		if (!aCurrentGameObject->GetComponent<CAnimationComponent>())
		{
			//animComp->GetMyAnimation()->Init(aModelFilePathList[loadModelNumber].c_str(), somePathsToAnimations);
#ifndef VFX
			CAnimationComponent* animComp = aCurrentGameObject->AddComponent<CAnimationComponent>(*aCurrentGameObject, aModelFilePathList[loadModelNumber].c_str(), somePathsToAnimations);
			animComp->Awake();
#else
			aCurrentGameObject->AddComponent<CAnimationComponent>(*aCurrentGameObject, aModelFilePathList[loadModelNumber].c_str(), somePathsToAnimations);
#endif
		}
		else
		{
			aCurrentGameObject->GetComponent<CAnimationComponent>()->ReplaceAnimation(aModelFilePathList[loadModelNumber].c_str(), somePathsToAnimations);
			aCurrentGameObject->GetComponent<CAnimationComponent>()->Awake();
		}
	}

	std::cout << "\nInstructions" << std::endl 
		<< "   Main Window: Press 'ESC' and then return to this Console." << std::endl
		<< "   Console: Enter the desired models number." << std::endl;

	return true;
}
bool IsLessThan(const size_t aIsLess, const size_t aThanThis)
{
	return (aIsLess < aThanThis);
}
void UpdateAnimation(CGameObject* aCurrentGameObject, CGameObject* aCamera, const std::vector<std::string>& aModelFilePathList)
{
	const auto animComp = aCurrentGameObject->GetComponent<CAnimationComponent>();
	if (animComp)
	{
		const size_t nrOfAnims = animComp->GetMyAnimation()->GetNrOfAnimations();

		if (Input::GetInstance()->IsKeyPressed('0')) { if(IsLessThan(0, nrOfAnims)) aCurrentGameObject->GetComponent<CAnimationComponent>()->PlayAnimation(0, true); }
		if (Input::GetInstance()->IsKeyPressed('1')) { if(IsLessThan(1, nrOfAnims)) aCurrentGameObject->GetComponent<CAnimationComponent>()->PlayAnimation(1, true); }
		if (Input::GetInstance()->IsKeyPressed('2')) { if(IsLessThan(2, nrOfAnims)) aCurrentGameObject->GetComponent<CAnimationComponent>()->PlayAnimation(2, true); }
		if (Input::GetInstance()->IsKeyPressed('3')) { if(IsLessThan(3, nrOfAnims)) aCurrentGameObject->GetComponent<CAnimationComponent>()->PlayAnimation(3, true); }
		if (Input::GetInstance()->IsKeyPressed('4')) { if(IsLessThan(4, nrOfAnims)) aCurrentGameObject->GetComponent<CAnimationComponent>()->PlayAnimation(4, true); }
		if (Input::GetInstance()->IsKeyPressed('5')) { if(IsLessThan(5, nrOfAnims)) aCurrentGameObject->GetComponent<CAnimationComponent>()->PlayAnimation(5, true); }
		if (Input::GetInstance()->IsKeyPressed('6')) { if(IsLessThan(6, nrOfAnims)) aCurrentGameObject->GetComponent<CAnimationComponent>()->PlayAnimation(6, true); }
		if (Input::GetInstance()->IsKeyPressed('7')) { if(IsLessThan(7, nrOfAnims)) aCurrentGameObject->GetComponent<CAnimationComponent>()->PlayAnimation(7, true); }
		if (Input::GetInstance()->IsKeyPressed('8')) { if(IsLessThan(8, nrOfAnims)) aCurrentGameObject->GetComponent<CAnimationComponent>()->PlayAnimation(8, true); }
		if (Input::GetInstance()->IsKeyPressed('9')) { if(IsLessThan(9, nrOfAnims)) aCurrentGameObject->GetComponent<CAnimationComponent>()->PlayAnimation(9, true); }

#ifndef VFX
		aCurrentGameObject->GetComponent<CAnimationComponent>()->Update();
#endif
	}

	const float dt = CTimer::Dt();
	UpdateCamera(*aCamera, dt);
	RotateModel(*aCurrentGameObject, dt);
	MoveModel(*aCurrentGameObject, dt);
	ResetModelTransform(*aCurrentGameObject);

	if (Input::GetInstance()->IsKeyPressed(VK_ESCAPE))
	{
		if (!ChangeAnimationModel(aCurrentGameObject, aModelFilePathList))
		{
			// oh no. How could this even happen :o?
		}
	}
}
#pragma endregion ! ANIMATIONS

/*
*	If loading animations outside of Model viewer:
*		Load model as normal
*		Check if filepath has suffix _SK
*			Get _AN files from the filepath directory
*			Add animComp, pass _AN files to the component.
*		
*	Connecting _SK models _AN files to StringID
*	tbd
*
*	PlayAnimations: Use index (0-n), Use AnimStringID? 
*/

//////////////////////////////////// MAIN STARTS HERE ///////////////////////////////////////////////////////////////////
#define ASSET_ROOT "Assets/3D"
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

	CScene* scene = new CScene();

// CAMERA
	CGameObject* camera = new CGameObject();
		camera->AddComponent<CCameraComponent>(*camera, 65.0f);
		camera->myTransform->Rotation(CAMERA_DEFAULT_ROT);
		camera->myTransform->Position(CAMERA_DEFAULT_POS);
		//CScene::GetInstance()->AddInstance(camera);
		scene->SetMainCamera(camera->GetComponent<CCameraComponent>());
// ENV LIGHT
	//CEnvironmentLight* environmentLight = CLightFactory::GetInstance()->CreateEnvironmentLight("Assets/Cubemaps/cubemap_BGRA8.dds");
	//CEnvironmentLight* environmentLight = CLightFactory::GetInstance()->CreateEnvironmentLight("Assets/Cubemaps/Yokohama2_color.dds");
	//CEnvironmentLight* environmentLight = CLightFactory::GetInstance()->CreateEnvironmentLight("Assets/Cubemaps/Yokohama2.dds");
	//CEnvironmentLight* environmentLight = CLightFactory::GetInstance()->CreateEnvironmentLight("Assets/Cubemaps/Cube_lv1.dds");
	//CEnvironmentLight* environmentLight = CLightFactory::GetInstance()->CreateEnvironmentLight("Assets/Cubemaps/HCross_BGRA8_4Mips.dds");
	CEnvironmentLight* environmentLight = CLightFactory::GetInstance()->CreateEnvironmentLight("Assets/Cubemaps/c.dds");
		environmentLight->SetDirection(SM::Vector3(0, 0, -1));
		environmentLight->SetColor(SM::Vector3(1.0f, 1.0f, 1.0f));
		environmentLight->SetIntensity(1.0f);
		scene->SetEnvironmentLight(environmentLight);
// GRID
	CLineInstance* grid = new CLineInstance();
		grid->Init(CLineFactory::GetInstance()->CreateGrid({ 0.33f,0.33f,0.33f, 1.0f }));
		grid->SetPosition({ 0.0f,-0.01f,0.0f });
		scene->AddInstance(grid);
// AXIS ORIGIN MARKER
	CLineInstance* origin = new CLineInstance();
		origin->Init(CLineFactory::GetInstance()->CreateAxisMarker());
		//origin->Init(CLineFactory::GetInstance()->CreateSquareXZ(1.f));
		//origin->Init(CLineFactory::GetInstance()->CreateTriangleXZ(10.f,5.f));
		origin->SetPosition({ 0.0f,0.02f,0.0f });
		//origin->SetRotation({ 0.0f,90.f,0 });
		origin->SetScale(1.2f);
		scene->AddInstance(origin);

	CEngine::GetInstance()->AddScene(scene);
	CEngine::GetInstance()->SetActiveScene(0);
	CGameObject* currentGameObject = nullptr;

	CCanvas* canvas = nullptr; 

	bool viewAnimations = true;
	/*SetForegroundWindow(GetConsoleWindow());
	std::cout << "Show Models or Animations? M for Models, A for Animations " << std::endl
	<< "   Show Models: any model that does not have a rig." << std::endl
	<< "   Show Animations: any model that has a rig." << std::endl
	<< "   Animated models: add _SK at the end of the skinned fbx and" << std::endl
	<< "   _AN on every animation file. Textures also need _SK at the end."<< std::endl
	<< "   Ex: CH_01_Boss_SK.fbx    CH_01_Boss@Walk01_AN.fbx    CH_01_Boss_SK_D.dds \n M / A:" << std::endl;

	char input = 'o';
	std::cin >> input;
	while (input != 'A' && input != 'M')
	{
		std::cin.clear();
		std::cout << "Show Models or Animations? M for Models, A for Animations\n M / A: " << std::endl;
		std::cin >> input;
	}
	viewAnimations = (input == 'A');*/

	std::vector<std::string> filePaths;
#ifdef VFX
	CGameObject* vfxObject = nullptr;
#endif
	if (viewAnimations)
	{
		MW::LoadModelPaths(ASSET_ROOT, filePaths, true);
		currentGameObject = InitAnimation(filePaths[0], *scene);
	}
	else
	{
		MW::LoadModelPaths(ASSET_ROOT, filePaths);
		currentGameObject = InitModels(filePaths[0], *scene);
	}
#ifdef VFX
	vfxObject = InitVFX(*scene);
#endif // VFX

	int counter = 0;
	for (auto& str : filePaths)
	{
		//std::cout << "fullpath " << counter << " " << str << std::endl;

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
		<< "   1/2/3/4/5/6/7/8/9/0..Play Animations for SK model\n"
		<< "   \n"
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
#ifdef VFX
		UpdateVFX(vfxObject, camera, scene);
#endif
		if (viewAnimations)
		{
			UpdateAnimation(currentGameObject, camera, filePaths);
		}
		else
		{
			UpdateModel(filePaths, currentGameObject, camera);
		}

		if (Input::GetInstance()->IsKeyPressed('I') && !showUI)
		{
			canvas = SpriteViewer::Init();
			showUI = true;
		}
		SpriteViewer::Update(*canvas, showUI);

		engine.RenderFrame();
		engine.EndFrame();
		Input::GetInstance()->update();
	}

	delete scene;
	scene = nullptr;

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
