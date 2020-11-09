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

using namespace CommonUtilities;
namespace SM = DirectX::SimpleMath;

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

std::size_t number_of_files_in_directory(std::filesystem::path path)
{//https://stackoverflow.com/questions/41304891/how-to-count-the-number-of-files-in-a-directory-using-standard/41305019
	using std::filesystem::directory_iterator;
	return std::distance(directory_iterator(path), directory_iterator{});
}
void LoadModelPaths(const std::string& aStartFolderPath, std::vector<std::string>& aFBXFilePaths)
{
	struct SFileInfo// Used for directories.
	{
		short myNumItems = 0;
		std::string myFullPath;
	};

	std::filesystem::path p(aStartFolderPath);

	std::filesystem::recursive_directory_iterator start(p);
	std::filesystem::recursive_directory_iterator end;

	std::map<std::string, SFileInfo> folders;
	std::vector<std::string> prevFolders;// Keys to use for std::map folders.

	/// Description of how <prevFolders> works

	///		prevFolders[depth]
	///		"Assets" depth = 0
	///			"3D" depth = 1
	///				"Character" depth = 2
	///					"Boss" depth = 3
	///						1 file(s) when all have been checked go up 1 depth.
	///					"MainCharacter" depth = 3
	///  
	/// Example =>	we are on depth 3, "Boss":
	///		folders[prevFolders[depth]].myFullPath = "Assets/3D/Character/Boss"
	///		folders[prevFolders[depth]].mySize = 1
	///		If we find an FBX inside of ../Boss: then aFBXFilePath = folders[prevFolders[depth]].myFullPath + / +"fbxFileName"
	///		(so when size = 0 we go up 1 depth)
	/// 
	/// </prevFolders>

	int depth = 0;// Key for prevFolders

	std::string folderPath = aStartFolderPath;
	std::map<std::string, short> foldersWithSize;

	prevFolders.emplace_back(aStartFolderPath);

	SFileInfo fileInfo;
	fileInfo.myNumItems		= static_cast<short>( number_of_files_in_directory(start->path()));
	fileInfo.myFullPath		= aStartFolderPath;

	folders.emplace(prevFolders[depth], fileInfo);

	for (auto i = start; i != end; ++i)
	{
		std::string filePath = i->path().filename().string();

		if (folders[prevFolders[depth]].myNumItems > 0)
		{
			folders[prevFolders[depth]].myNumItems -= 1;
		}
		else if (folders[prevFolders[depth]].myNumItems == 0)
		{
			if (depth != 0)
			{
				--depth;
				prevFolders.pop_back();

			}
			folderPath = folders[prevFolders[depth]].myFullPath;

			folders[prevFolders[depth]].myNumItems -= 1;
			if (folders[prevFolders[depth]].myNumItems <= 0)
			{
				if (depth != 0)
				{
					--depth;
					prevFolders.pop_back();
				}

				folderPath = folders[prevFolders[depth]].myFullPath;
			}
		}

		if (i->is_directory())
		{
			++depth;
			prevFolders.emplace_back(filePath);

			folderPath.append("/" + filePath);

			fileInfo.myNumItems	= static_cast<short>(number_of_files_in_directory(i->path()));
			fileInfo.myFullPath = folderPath;

			folders.emplace(filePath, fileInfo);
		}
		else
		{
			const size_t checkForDot = filePath.find(".");
			std::string fileExtension = filePath.substr(checkForDot, ((4 + checkForDot) < filePath.length() ? 4 : filePath.length() - 1 ));
			// todo filesystem::path has a function to check if it is a directory.
			if (fileExtension == ".fbx")
			{
				//ex: CH_NPC_Boss_Attack_AN.fbx 7 from last
				std::string suffix = filePath.substr(filePath.length() - 7, filePath.length() - 5);
				if (suffix != "_AN.fbx")
				{
					aFBXFilePaths.emplace_back(folders[prevFolders[depth]].myFullPath + "/" + filePath);
				}
			}
		}
	}
}

CGameObject* InitModels(const std::string& aModelPath)
{
	CScene* scene = CScene::GetInstance();

	CLightFactory* lightFactory = CLightFactory::GetInstance();
	CEnvironmentLight* environmentLight = lightFactory->CreateEnvironmentLight("Yokohama2.dds");
	environmentLight->SetDirection(SM::Vector3(0, 0, -1));
	environmentLight->SetColor(SM::Vector3(1.0f, 1.0f, 1.0f));
	//environmentLight->SetColor(SM::Vector3(0.0f, 0.0f, 1.0f));

	environmentLight->SetIntensity(1.0f);


	scene->AddInstance(environmentLight);

	CGameObject* gameobject = new CGameObject();

	gameobject->AddComponent<CModelComponent>(CModelComponent(*gameobject, aModelPath));
	gameobject->myTransform->Position({ 0.0f,0.0f,0.0f });
	
	scene->AddInstance(gameobject);

	return gameobject;
}

// Reminder: Vem tar hand om delete av CModel? CModelFactory verkar inte ta hand om det och inte CModelInstance?

bool CheckForIncorrectModelNumber(const size_t& aLoadModelNumber, const size_t& aMax)
{
	return (static_cast<int>(aLoadModelNumber) > -1 && aLoadModelNumber < aMax);
}


void Update(std::vector<std::string>& aModelFilePathList, CGameObject* aCurrentGameObject,CCamera* aCamera)
{
	float dt = CTimer::Dt();

	float cameraMoveSpeed = 5.0f;
	if (Input::GetInstance()->IsKeyDown(VK_UP))
	{
		aCamera->Move({ 0.0f, 0.0f, cameraMoveSpeed * dt });
	}
	if (Input::GetInstance()->IsKeyDown(VK_DOWN))
	{
		aCamera->Move({ 0.0f, 0.0f, -cameraMoveSpeed * dt });
	}
	if (Input::GetInstance()->IsKeyDown(VK_RIGHT))
	{
		aCamera->Move({ cameraMoveSpeed * dt, 0.0f, 0.0f });
	}
	if (Input::GetInstance()->IsKeyDown(VK_LEFT))
	{
		aCamera->Move({ -cameraMoveSpeed * dt, 0.0f, 0.0f });
	}

	// Rotation functions
	float rotationSpeed = 1.0f;
	// X axis
	if (Input::GetInstance()->IsKeyDown('R'))
	{
		aCurrentGameObject->GetComponent<CTransformComponent>()->Rotate({ rotationSpeed * dt,0.0f,0.0f });
	}

	if (Input::GetInstance()->IsKeyDown('F'))
	{
		aCurrentGameObject->GetComponent<CTransformComponent>()->Rotate({ -rotationSpeed * dt,0.0f,0.0f });
	}

	// Y axis
	if (Input::GetInstance()->IsKeyDown('T'))
	{
		aCurrentGameObject->GetComponent<CTransformComponent>()->Rotate({ 0.0f, rotationSpeed * dt,0.0f });
	}

	if (Input::GetInstance()->IsKeyDown('G'))
	{
		aCurrentGameObject->GetComponent<CTransformComponent>()->Rotate({ 0.0f, -rotationSpeed * dt,0.0f });
	}

	// Z axis
	if (Input::GetInstance()->IsKeyDown('Y'))
	{
		aCurrentGameObject->GetComponent<CTransformComponent>()->Rotate({ 0.0f,0.0f,rotationSpeed * dt });
	}
	if (Input::GetInstance()->IsKeyDown('H'))
	{
		aCurrentGameObject->GetComponent<CTransformComponent>()->Rotate({ 0.0f,0.0f,-rotationSpeed * dt });
	}

	// ! Rotation Functions 

	// Zoom/ move Camera functions
	float moveSpeed = 5.0f;
	// X axis

	if (Input::GetInstance()->IsKeyDown('A'))
	{
		aCurrentGameObject->GetComponent<CTransformComponent>()->Move({ moveSpeed * dt, 0.0f, 0.0f });
	}

	if (Input::GetInstance()->IsKeyDown('D'))
	{
		aCurrentGameObject->GetComponent<CTransformComponent>()->Move({ -moveSpeed * dt, 0.0f, 0.0f });
	}

	// Y axis
	if (Input::GetInstance()->IsKeyDown('Q'))
	{
		aCurrentGameObject->GetComponent<CTransformComponent>()->Move({ 0.0f, moveSpeed * dt, 0.0f });
	}

	if (Input::GetInstance()->IsKeyDown('E'))
	{
		aCurrentGameObject->GetComponent<CTransformComponent>()->Move({ 0.0f, -moveSpeed * dt, 0.0f });
	}

	// Z axis

	if (Input::GetInstance()->IsKeyDown('W'))
	{
		aCurrentGameObject->GetComponent<CTransformComponent>()->Move({ 0.0f, 0.0f, -moveSpeed * dt });
	}

	if (Input::GetInstance()->IsKeyDown('S'))
	{
		aCurrentGameObject->GetComponent<CTransformComponent>()->Move({ 0.0f, 0.0f, moveSpeed * dt });
	}
	// ! Zoom/ move functions

	// Reset function

	if (Input::GetInstance()->IsKeyDown('K'))
	{
		aCurrentGameObject->GetComponent<CTransformComponent>()->Transform({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	}
	if (Input::GetInstance()->IsKeyDown('C'))
	{
		aCamera->SetRotation({ 33.f,-45.f,0.f });
		aCamera->SetPosition({ 3.0f,4.0f,-3.5f });
	}

	// ! Reset function

	if (Input::GetInstance()->IsKeyPressed(VK_ESCAPE))
	{
		SetForegroundWindow(GetConsoleWindow());

		size_t loadModelNumber = aModelFilePathList.size();
		std::cout << "Which model do you wish to load Give a number between: 0 and " << aModelFilePathList.size() - 1 << "\nL> ";
		std::cin >> loadModelNumber;
		while (!CheckForIncorrectModelNumber(loadModelNumber, aModelFilePathList.size()))
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

//////////////////////////////////// MAIN STARTS HERE ///////////////////////////////////////////////////////////////////

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

	CScene* scene = CScene::GetInstance();

	CCamera* camera = CCameraFactory::GetInstance()->CreateCamera(65.0f, 5000.0f);
	camera->SetRotation({ 33.f,-45.f,0.f });
	camera->SetPosition({ 3.0f,4.0f,-3.5f });
	scene->AddInstance(camera);
	scene->SetMainCamera(camera);

	CLineInstance* grid = new CLineInstance();
	grid->Init(CLineFactory::GetInstance()->CreateGrid({ 0.33f,0.33f,0.33f, 1.0f }));
	grid->SetPosition({ 0.0f,-0.01f,0.0f });
	CScene::GetInstance()->AddInstance(grid);

	CLineInstance* origin = new CLineInstance();
	origin->Init(CLineFactory::GetInstance()->CreateAxisMarker());
	origin->SetPosition({ 0.0f,0.01f,0.0f });
	CScene::GetInstance()->AddInstance(origin);

	std::string root = "Assets";

	std::vector<std::string> filePaths;
	LoadModelPaths(root, filePaths);

	CGameObject* currentGameObject = nullptr;
	currentGameObject = InitModels(filePaths[96]);
	
	int counter = 0;
	for (auto& str : filePaths)
	{
		const size_t last_slash_idx = str.find_last_of("\\/");
		std::string modelName		= str.substr(last_slash_idx + 1, str.size() - last_slash_idx - 5);
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
		<< std::endl;
	
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
		Update(filePaths, currentGameObject, camera);

		engine.RenderFrame();
		engine.EndFrame();
		Input::GetInstance()->update();
	}

	delete currentGameObject;
	currentGameObject = nullptr;

	delete grid;
	grid = nullptr;

	delete origin;
	origin = nullptr;
	

#ifdef USE_CONSOLE_COMMAND
	CloseConsole();
#endif

	return 0;
}
