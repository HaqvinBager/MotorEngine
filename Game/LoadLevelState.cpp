#include "stdafx.h"
#include "LoadLevelState.h"
#include "InGameState.h"
#include "StateStack.h"
#include "Scene.h"
#include "Engine.h"
#include "JsonReader.h"
#include "GameObject.h"

using namespace rapidjson;
CLoadLevelState::CLoadLevelState(CStateStack& aStateStack) : CState(aStateStack)
{
	SaveLevelNames();
	myState = CStateStack::EStates::LoadLevel;
	myActiveScene = CEngine::GetInstance()->ScenesSize();
}

CLoadLevelState::~CLoadLevelState()
{
	CEngine::GetInstance()->PopBackScene();
}

void CLoadLevelState::Awake()
{



	unsigned int loadSceneIndex = Load(ELevel::LoadScreen);
	CEngine::GetInstance()->SetActiveScene(loadSceneIndex);

	Document latestExportedLevelDoc = CJsonReader::LoadDocument("Levels/DebugLevel.json");
	int levelIndex = latestExportedLevelDoc["LevelIndex"].GetInt();

	//Start Loading the ELevel::<Level> on a seperate thread.
	myLoadLevelFuture = std::async(std::launch::async, &CLoadLevelState::Load, this, static_cast<ELevel>(levelIndex));

	for (auto& gameObject : CEngine::GetInstance()->GetActiveScene().GetActiveGameObjects())
	{
		gameObject->Awake();
	}
}

void CLoadLevelState::Start()
{
	for (auto& gameObject : CEngine::GetInstance()->GetActiveScene().GetActiveGameObjects())
	{
		gameObject->Start();
	}
}

void CLoadLevelState::Update()
{
	//When the Thread loading the ELevell::<Level> level is complete this will be true.
	if (myLoadLevelFuture._Is_ready())
	{
		//myLoadedLevelFuture returnType is the same as the CLoadLevelState::Load return type.
		//The value it will get is the Scene index in which the SceneLoaded will use in CEngine::myScenes
		CEngine::GetInstance()->SetActiveScene(myLoadLevelFuture.get());
		myStateStack.PushState(new CInGameState(myStateStack));
		myStateStack.Awake();
		myStateStack.Start();
	}

	for (auto& gameObject : CEngine::GetInstance()->GetActiveScene().GetActiveGameObjects())
	{
		gameObject->Update();
	}
}

unsigned int CLoadLevelState::Load(const ELevel aLevel)
{
	if (mySceneReader.OpenBin(BinPath(aLevel)))
	{
		SaveModelPaths(aLevel);

		if (aLevel == ELevel::LoadScreen) //LoadScreen uses a different Type (Which kind of Data it will Load from Unity) 
		{
			SLoadScreenData& data = mySceneReader.ReadLoadScreenData();
			CScene* loadScreenScene = new CScene();
			myUnityFactory.FillScene(data, BinModelPaths(aLevel), *loadScreenScene);
			return CEngine::GetInstance()->AddScene(loadScreenScene);
		}
		else //All other Scenes are regarded as "InGame" scenes. And will have to contain at least a Camera, Directional Light & Player (player is currently "utkommenterad", Fix Monday)
		{

			SInGameData& data = mySceneReader.ReadInGameData();
			CScene* inGameScene = new CScene();

			std::string navMeshPath = myLevelNames[static_cast<int>(aLevel)] + "_ExportedNavMesh.obj";
			inGameScene->InitNavMesh(navMeshPath);

			myUnityFactory.FillScene(data, BinModelPaths(aLevel), *inGameScene);
			return CEngine::GetInstance()->AddScene(inGameScene);
		}
	}
	return 0;
}

void CLoadLevelState::SaveLevelNames()
{
	Document levelSelectDocument = CJsonReader::LoadDocument("Levels/LevelSelect.json");
	int count = 0;
	for (auto& levelNameObject : levelSelectDocument["Levels"].GetArray())
	{
		std::string levelBinPath = "Levels/";
		std::string levelName = levelBinPath + levelNameObject["LevelName"].GetString();
		myLevelNames.emplace_back(levelName);
		levelBinPath.append(levelNameObject["LevelName"].GetString());
		levelBinPath.append("_bin.bin");
		myBinPaths.emplace_back(levelBinPath);

		std::string modelsPath = levelName + "_bin_modelPaths.json";
		myModelJsonFileMap[static_cast<ELevel>(count)] = modelsPath;
		count++;
	}
}

void CLoadLevelState::SaveModelPaths(const ELevel aLevel)
{
	Document modelPathDocument = CJsonReader::LoadDocument(myModelJsonFileMap[aLevel]);
	for (auto& modelPath : modelPathDocument["ModelPaths"].GetArray())
	{
		myBinModelPaths[aLevel].emplace_back(modelPath["Path"].GetString());
	}
}

std::string& CLoadLevelState::BinPath(const ELevel aLevel)
{
	return myBinPaths[static_cast<int>(aLevel)];
}

std::vector<std::string>& CLoadLevelState::BinModelPaths(const ELevel aLevel)
{
	return myBinModelPaths[aLevel];
}

void CLoadLevelState::MakeSceneActive() {
	CEngine::GetInstance()->SetActiveScene(myActiveScene);

}