#include "WinInclude.h"
#include "Engine.h"
#include "Input.h"

#include <SimpleMath.h>
#include <Scene.h>
#include <CameraFactory.h>
#include <Camera.h>
#include <Model.h>
#include <ModelFactory.h>
#include <ModelInstance.h>
#include <LightFactory.h>
#include <EnvironmentLight.h>
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


void CheckFolder(const std::string& aFolder)
{
	std::filesystem::path p(aFolder.c_str());
	std::filesystem::directory_iterator start(p);
	std::filesystem::directory_iterator end;

	for (auto i = start; i != end; ++i)
	{
		std::string filePath = i->path().filename().string();
		const size_t checkForDot = filePath.find(".");
		if (checkForDot == std::string::npos)
		{
			std::cout << filePath << " is a folder" << std::endl;
			continue;
		}
		std::cout << filePath << std::endl;
	}
}
std::size_t number_of_files_in_directory(std::filesystem::path path)
{//https://stackoverflow.com/questions/41304891/how-to-count-the-number-of-files-in-a-directory-using-standard/41305019
	using std::filesystem::directory_iterator;
	return std::distance(directory_iterator(path), directory_iterator{});
}


void LoadModels(const std::string& aFolderPath, std::vector<std::string>& aFilePaths)
{
	struct SFileInfo {

		short mySize = 0;
		std::string myFullPath;
	};

	std::filesystem::path p(aFolderPath);
	{
		std::filesystem::recursive_directory_iterator start(p);
		std::filesystem::recursive_directory_iterator end;

		std::map<std::string, SFileInfo> folders;
		std::vector<std::string> prevFolders;

		int depth = 0;

		std::string folderPath			= aFolderPath;
		std::map<std::string, short> foldersWithSize;

		prevFolders.emplace_back(aFolderPath);

		SFileInfo fileInfo;
		fileInfo.mySize			= static_cast<short>( number_of_files_in_directory(start->path()));
		fileInfo.myFullPath		= aFolderPath;

		folders.emplace(prevFolders[depth], fileInfo);

		for (auto i = start; i != end; ++i)
		{
			std::string filePath = i->path().filename().string();

			if (i->is_directory())
			{
				++depth;
				prevFolders.emplace_back(filePath);

				folderPath.append("/" + filePath);

				fileInfo.mySize		= static_cast<short>(number_of_files_in_directory(i->path()));
				fileInfo.myFullPath = folderPath;

				folders.emplace(filePath, fileInfo);
				std::cout << "Found directory: " << filePath << std::endl << std::endl;
			}
			else
			{
				const size_t checkForDot = filePath.find(".");
				std::string fileExtension = filePath.substr(checkForDot, ((4 + checkForDot) < filePath.length() ? 4 : filePath.length() - 1 ));
				// todo filesystem::path has a function to check if it is a directory.
				if (fileExtension == ".fbx")
				{
					aFilePaths.emplace_back(folders[prevFolders[depth]].myFullPath + "/" + filePath);
					std::cout	<< "Found FBX: \n"
								<< "aFilePaths.back() : " << aFilePaths.back() << std::endl;
				}
			}
			
			if (folders[prevFolders[depth]].mySize > -1)
			{
				folders[prevFolders[depth]].mySize -= 1;
				if (folders[prevFolders[depth]].mySize == -1)
				{
					--depth;
					prevFolders.pop_back();
					
					folderPath = folders[prevFolders[depth]].myFullPath;
					
					folders[prevFolders[depth]].mySize -= 1;
					if (folders[prevFolders[depth]].mySize == -1)
					{
						--depth;
						prevFolders.pop_back();

						folderPath = folders[prevFolders[depth]].myFullPath;
					}

					/*std::cout << "FOLDER ENDS" << std::endl;

					std::cout << "Changing to folder: " << folderPath << std::endl << std::endl;*/
				}
			}
			
			
		}
	}
}

CModelInstance* InitModels(const std::string& /*aModelPath*/)
{
	CScene* scene = CScene::GetInstance();

	CCamera* camera = CCameraFactory::GetInstance()->CreateCamera(65.0f, 5000.0f);
	camera->SetPosition({ 0,0,-5.0f });
	scene->AddInstance(camera);
	scene->SetMainCamera(camera);

	CLightFactory* lightFactory = CLightFactory::GetInstance();
	CEnvironmentLight* environmentLight = lightFactory->CreateEnvironmentLight("Yokohama2.dds");
	environmentLight->SetDirection(SM::Vector3(0, 0, 1));
	environmentLight->SetColor(SM::Vector3(1.0f, 1.0f, 1.0f));
	scene->AddInstance(environmentLight);

	//CModel* cubeModel = CModelFactory::GetInstance()->GetCube();
	//CModelInstance* cube = new CModelInstance();
	//cube->Init(cubeModel);
	//scene->AddInstance(cube);

	//CModelInstance* model = CModelFactory::GetInstance()->CreateModel(aModelPath, { 1.0f, 1.0f, 1.0f });
	CModelInstance* model = CModelFactory::GetInstance()->CreateModel("Model/Chest/Particle_Chest.fbx", { 1.0f, 1.0f, 1.0f });
	model->SetPosition({ 0.0f, 0.0f, .0f });

	scene->AddInstance(model);

	return model;
	
}

void Update(std::vector<std::string>& aModelFilePathList, CModelInstance* aCurrentModelInstance, int& aCurrentModelPathIndex)
{
	if (Input::GetInstance()->IsKeyDown(VK_UP))
	{
		if (aCurrentModelPathIndex > 0)
		{
			--aCurrentModelPathIndex;
			delete aCurrentModelInstance;
			aCurrentModelInstance = nullptr;
			aCurrentModelInstance = CModelFactory::GetInstance()->CreateModel(aModelFilePathList[aCurrentModelPathIndex], { 1.0f, 1.0f, 1.0f });
		}
	}

	if (Input::GetInstance()->IsKeyDown(VK_DOWN))
	{
		if (aCurrentModelPathIndex < aModelFilePathList.size())
		{
			++aCurrentModelPathIndex;
			delete aCurrentModelInstance;
			aCurrentModelInstance = nullptr;
			aCurrentModelInstance = CModelFactory::GetInstance()->CreateModel(aModelFilePathList[aCurrentModelPathIndex], { 1.0f, 1.0f, 1.0f });
		}
	}
}

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
	windowData.myX = 100;
	windowData.myY = 100;
	windowData.myWidth = 1280;
	windowData.myHeight = 720;
	
	CEngine engine;
	bool shouldRun = engine.Init(windowData);
	if (!shouldRun)
		return 1;

	std::vector<std::string> filePaths;
	CModelInstance* currentModel = nullptr;
	LoadModels("Model", filePaths);
	int currentModelPathIndex = 1;// 71 total
	currentModel = InitModels(filePaths[currentModelPathIndex]);

	

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
		Update(filePaths, currentModel,currentModelPathIndex);
		engine.RenderFrame();
		engine.EndFrame();
		Input::GetInstance()->update();
	}

#ifdef USE_CONSOLE_COMMAND
	CloseConsole();
#endif

	return 0;
}

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
