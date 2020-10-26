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
#include <ModelInstance.h>
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

/// Notes for SG feedback session:
/// Ask them if they want to be able to load several models. I.e be able to have Sword and King side by side
///		About controls: move camera? move with mouse?
///		Manip light?
/// /// Remember: on initial model positon some models are not facing the camera, might not be visible but they are there. 
///		Or might be positioned to high/ low. So try to move the object. 
///		ex: nr31 EN_P_L3Painting_01_19G4_01_19.fbx, is facing away from the camera.
/// 
/// EN_W_WallDoor_01_19G4_01_19.fbx causes crash might be due to fbx version

std::size_t number_of_files_in_directory(std::filesystem::path path)
{//https://stackoverflow.com/questions/41304891/how-to-count-the-number-of-files-in-a-directory-using-standard/41305019
	using std::filesystem::directory_iterator;
	return std::distance(directory_iterator(path), directory_iterator{});
}
void LoadModelPaths(const std::string& aStartFolderPath, std::vector<std::string>& aFBXFilePaths)
{
	struct SFileInfo// Used for directories.
	{
		short myNumItems = 0;//rename to myNumItems
		std::string myFullPath;
	};

	std::filesystem::path p(aStartFolderPath);

	std::filesystem::recursive_directory_iterator start(p);
	std::filesystem::recursive_directory_iterator end;

	std::map<std::string, SFileInfo> folders;
	std::vector<std::string> prevFolders;// Keys to use for std::map folders.
	/// <prevFolders>
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
#ifdef _DEBUG
		//std::cout << "Current: " << filePath << std::endl;
		//std::cout << std::endl;
		//std::string nextDepthSpacing = " ";
		//for (int d = 0; d < depth + 1; ++d)
		//{
		//	std::cout << nextDepthSpacing <<"prevFolders[" << d << "] = " << prevFolders[d] << std::endl;
		//	std::cout << nextDepthSpacing << "folders[prevFolders[" << d << "]].myFullPath = " << folders[prevFolders[d]].myFullPath << std::endl;
		//	std::cout << nextDepthSpacing << "folders[prevFolders[" << d << "]].myNumItems = " << folders[prevFolders[d]].myNumItems << std::endl;
		//	nextDepthSpacing.append(" ");
		//}
		//std::cout << std::endl;
#endif// ! _DEBUG

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
#ifdef _DEBUG
				//std::cout << "--DEPTH    Reducing depth to " << depth << std::endl;
#endif // ! _DEBUG
			}

			folderPath = folders[prevFolders[depth]].myFullPath;

#ifdef _DEBUG
			//std::cout << "FOLDER ENDS" << std::endl;
			//std::cout << " Changing to folder: " << folderPath << std::endl << std::endl;
#endif // ! _DEBUG

			folders[prevFolders[depth]].myNumItems -= 1;
			if (folders[prevFolders[depth]].myNumItems <= 0)
			{
				if (depth != 0)
				{
					--depth;
					prevFolders.pop_back();
#ifdef _DEBUG
					//std::cout << "--DEPTH    Reducing depth, again; to " << depth << std::endl;
#endif // ! _DEBUG
				}

				folderPath = folders[prevFolders[depth]].myFullPath;
#ifdef _DEBUG
				//std::cout << "FOLDER ENDS" << std::endl;
				//std::cout << " Changing to folder: " << folderPath << std::endl << std::endl;
#endif // ! _DEBUG
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
#ifdef _DEBUG
			//std::cout << "L> Found directory: " << filePath << std::endl /*<< std::endl*/;
#endif // ! _DEBUG
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
#ifdef _DEBUG
					std::cout	<< "      Found FBX: \n        " << aFBXFilePaths.back() << std::endl;
#endif // ! _DEBUG
				}
			}
		}
#ifdef _DEBUG
		//std::cout << "__________________________________________________________________________________________" << std::endl;
#endif // ! _DEBUG
	}
}

CModelInstance* InitModels(const std::string& aModelPath/*, CCamera* aCamera*/)
{
	CScene* scene = CScene::GetInstance();

	CCamera* camera = CCameraFactory::GetInstance()->CreateCamera(65.0f, 5000.0f);
	camera->SetPosition({ 0,1.0f,-5.0f });
	scene->AddInstance(camera);
	scene->SetMainCamera(camera);

	CLightFactory* lightFactory = CLightFactory::GetInstance();
	CEnvironmentLight* environmentLight = lightFactory->CreateEnvironmentLight("Yokohama2.dds");
	environmentLight->SetDirection(SM::Vector3(1, 0, 0));
	//environmentLight->SetColor(SM::Vector3(1.0f, 0.0f, 0.0f));
	environmentLight->SetColor(SM::Vector3(1.0f, 1.0f, 1.0f));

	scene->AddInstance(environmentLight);

	CModelInstance* model = CModelFactory::GetInstance()->CreateModel(aModelPath, { 1.0f, 1.0f, 1.0f });
	//CModelInstance* model = CModelFactory::GetInstance()->CreateModel("Model/Chest/Particle_Chest.fbx", { 1.0f, 1.0f, 1.0f });
	model->SetPosition({ 0.0f, 0.0f, .0f });

	scene->AddInstance(model);

	return model;	
}

// Reminder: Vem tar hand om delete av CModel? CModelFactory verkar inte ta hand om det och inte CModelInstace?

bool CheckForIncorrectModelNumber(const size_t& aLoadModelNumber, const size_t& aMax)
{
	return (static_cast<int>(aLoadModelNumber) > -1 && aLoadModelNumber < aMax);
}
std::string CheckForGroupNumber(short& aNumber)
{// Obsolete as of 
	std::string path = "";
	std::cin.clear();
	std::cout << "Enter group number - 3 or 4:";
	//std::cin >> aNumber;
	aNumber = 4;
	if (aNumber == 4 || aNumber == 3)
	{
		if (aNumber == 3)
		{
			path = "Model";
		}
		else if (aNumber == 4)
		{
			path = "Assets";
		}
	}
	else
	{
		std::cout << " ! Input not supported!" << std::endl;
		path = CheckForGroupNumber(aNumber);
	}

	return std::move(path);
}

void Update(std::vector<std::string>& aModelFilePathList, CModelInstance* aCurrentModelInstance/*,CCamera* aCamera*/)
{
	// Rotation functions
	float rotationSpeed = 1.0f;
	float dt = CTimer::Dt();
	// X axis
	if (Input::GetInstance()->IsKeyDown('R'))
	{
		aCurrentModelInstance->Rotate({ rotationSpeed * dt,0.0f,0.0f });
	}

	if (Input::GetInstance()->IsKeyDown('F'))
	{
		aCurrentModelInstance->Rotate({ -rotationSpeed * dt,0.0f,0.0f });
	}

	// Y axis
	if (Input::GetInstance()->IsKeyDown('T'))
	{
		aCurrentModelInstance->Rotate({ 0.0f, rotationSpeed * dt,0.0f });
	}

	if (Input::GetInstance()->IsKeyDown('G'))
	{
		aCurrentModelInstance->Rotate({ 0.0f, -rotationSpeed * dt,0.0f });
	}

	// Z axis
	if (Input::GetInstance()->IsKeyDown('Y'))
	{
		aCurrentModelInstance->Rotate({ 0.0f,0.0f,rotationSpeed * dt });
	}
	if (Input::GetInstance()->IsKeyDown('H'))
	{
		aCurrentModelInstance->Rotate({ 0.0f,0.0f,-rotationSpeed * dt });
	}

	// ! Rotation Functions 

	// Zoom/ move Camera functions
	float moveSpeed = 50.0f;
	// X axis

	if (Input::GetInstance()->IsKeyDown('A'))
	{
		aCurrentModelInstance->Move({ moveSpeed * dt, 0.0f, 0.0f });
	}

	if (Input::GetInstance()->IsKeyDown('D'))
	{
		aCurrentModelInstance->Move({ -moveSpeed * dt, 0.0f, 0.0f });
	}

	// Y axis
	if (Input::GetInstance()->IsKeyDown('Q'))
	{
		aCurrentModelInstance->Move({ 0.0f, moveSpeed * dt, 0.0f });
	}

	if (Input::GetInstance()->IsKeyDown('E'))
	{
		aCurrentModelInstance->Move({ 0.0f, -moveSpeed * dt, 0.0f });
	}

	// Z axis

	if (Input::GetInstance()->IsKeyDown('W'))
	{
		aCurrentModelInstance->Move({ 0.0f, 0.0f, -moveSpeed * dt });
	}

	if (Input::GetInstance()->IsKeyDown('S'))
	{
		aCurrentModelInstance->Move({ 0.0f, 0.0f, moveSpeed * dt });
	}
	// ! Zoom/ move functions

	// Reset function

	if (Input::GetInstance()->IsKeyDown('K'))
	{
		aCurrentModelInstance->SetTransform({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
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
		aCurrentModelInstance->SetModel(CModelFactory::GetInstance()->CreateModel(aModelFilePathList[loadModelNumber], { 1.0f, 1.0f, 1.0f })->GetModel());	
		aCurrentModelInstance->SetTransform({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
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

	CLineInstance* grid = new CLineInstance();
	grid->Init(CLineFactory::GetInstance()->CreateGrid({ 0.33f,0.33f,0.33f, 1.0f }));
	CScene::GetInstance()->AddInstance(grid);

	//short groupNumber = 0;
	std::string root = "Assets";//CheckForGroupNumber(groupNumber);

	std::vector<std::string> filePaths;
	LoadModelPaths(root, filePaths);

	//CCamera* camera = nullptr;
	CModelInstance* currentModel = nullptr;
	currentModel = InitModels(filePaths[0]);
	
	//std::cout << "Models in " << root << std::endl;
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
		<< "   Console: Enter the desired models number." << std::endl;
	
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
		Update(filePaths, currentModel/*, camera*/);
		engine.RenderFrame();
		engine.EndFrame();
		Input::GetInstance()->update();
	}

	delete currentModel;
	currentModel = nullptr;

	delete grid;
	grid = nullptr;
	

#ifdef USE_CONSOLE_COMMAND
	CloseConsole();
#endif

	return 0;
}

// Pseudo code for finding all the fbx:es in a root folder.
/*
	struct FileInfo
		short mySize
		string myFullPath

	map<string, FileInfo> folders
	vector<string> prevFolders
	int depth = 0

	prevFolders.emplace_back(aFolder)

	fileInfo.mySize = start.DirectorySize
	fileInfo.myFullPath = aFolder
	folders.emplace(prevFolders[depth], fileInfo)

	string folderPath = aFolder;
	for(iterator)
		string file = iterator.name
		if(iterator is directory)
			depth++
			prevFolder.emplace_back(file)

			folderPath.append(/ + file)

			FileInfo.mySize = iterator.DirectorySize
			FileInfo.myFullPath = folderPath
			folders.emplace(file, FileInfo)

		else
			fileExtension = search for ., get substr from . to end of string
			if(fileExtension == FBX)
				save to modellist .emplace_back(folders[prevFolder[depth]].myFullPath + / + file)

		if(folders[prevFolder[depth]].mySize > -1)
			folders[prevFolder[depth]].mySize -= 1
			if(folders[prevFolder[depth]].mySize == -1)
				--depth
				prevFolder.pop_back()
				folderPath = prevFolder[depth]
*/
