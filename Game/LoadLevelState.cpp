#include "stdafx.h"
#include "LoadLevelState.h"

#include "InGameState.h"
#include "StateStack.h"
#include "Scene.h"
#include "Engine.h"
#include "JsonReader.h"
#include "GameObject.h"
#include "PostMaster.h"
#include "MainSingleton.h"

#include "CollisionEventComponent.h"

using namespace rapidjson;
CLoadLevelState::CLoadLevelState(CStateStack& aStateStack, const CStateStack::EState aState)
	: CState(aStateStack, aState)
	, myLevelToLoad(ELevel::Dungeon)
{}

CLoadLevelState::~CLoadLevelState()
{
	CMainSingleton::PostMaster().Unsubscribe("Dungeon", this);
	CMainSingleton::PostMaster().Unsubscribe("Gardens", this);
	CMainSingleton::PostMaster().Unsubscribe("Castle", this);
	CMainSingleton::PostMaster().Unsubscribe("BossRoom", this);
}

void CLoadLevelState::Awake()
{
	SaveLevelNames();
	CMainSingleton::PostMaster().Subscribe("Dungeon", this);
	CMainSingleton::PostMaster().Subscribe("Gardens", this);
	CMainSingleton::PostMaster().Subscribe("Castle", this);
	CMainSingleton::PostMaster().Subscribe("BossRoom", this);
}

void CLoadLevelState::Start()
{
	CEngine::GetInstance()->ClearModelFactory();

	CEngine::GetInstance()->SetActiveScene(Load(ELevel::LoadScreen));
	CEngine::GetInstance()->SetRenderScene(true);

	// Only use this for testing. Use myLevelToLoad for correct level to level loading. Its data is updated on Level Load Events
	/*Document latestExportedLevelDoc = CJsonReader::LoadDocument("Levels/DebugLevel.json");
	int levelIndex = latestExportedLevelDoc["LevelIndex"].GetInt();*/
	//levelIndex;

	//Start Loading the ELevel::<Level> on a seperate thread.
	myLoadLevelFuture = std::async(std::launch::async, &CLoadLevelState::Load, this, /*static_cast<ELevel>(levelIndex)*/myLevelToLoad);
	//myLoadLevelFuture = std::async(std::launch::async, &CLoadLevelState::Load, this, myLevelToLoad);


	for (auto& gameObject : CEngine::GetInstance()->GetActiveScene().GetActiveGameObjects())
	{
		gameObject->Awake();
	}

	for (auto& gameObject : CEngine::GetInstance()->GetActiveScene().GetActiveGameObjects())
	{
		gameObject->Start();
	}
}

void CLoadLevelState::Stop()
{

}

void CLoadLevelState::Update()
{
	//When the Thread loading the ELevell::<Level> level is complete this will be true.
	if (myLoadLevelFuture._Is_ready())
	{
		//myLoadedLevelFuture returnType is the same as the CLoadLevelState::Load return type.
		//The value it will get is the Scene index in which the SceneLoaded will use in CEngine::myScenes
		//myActiveScene = ;
		myLoadLevelFuture.get();
		//CEngine::GetInstance()->SetActiveScene();
		myStateStack.PopTopAndPush(CStateStack::EState::InGame);
	}
	else
	{
		for (auto& gameObject : CEngine::GetInstance()->GetActiveScene().GetActiveGameObjects())
		{
			gameObject->Update();
		}
	}

}

void CLoadLevelState::Receive(const SStringMessage& aMessage)
{
	std::string level = "Levels/";
	level.append(aMessage.myMessageType);
	ELevel eLevel = ELevel::NavTest;
	for (int i = 0; i < myLevelNames.size(); ++i)
	{
		if (myLevelNames[i] == level)
		{
			eLevel = static_cast<ELevel>(i);
			break;
		}
	}
	myLevelToLoad = eLevel;
}

///
///	Engine::AddScene(enum EState, CScene* scene)
/// {
///		if(sceneMap contains EState)
///			delete sceneMap[EState]
///
///		sceneMap[EState] = scene;
///
///	Engine::ClearScene(enum EState)
/// {
///		if(sceneMap containts EState)
///			sceneMap[EState]->ClearScene();
///
/// Engine::SetActiveScene(int aEState)
/// {
///		EState state = static_cast<EState>(aEState)
///		if(sceneMap containts EState)
///			myActiveState = EState
///
/// CScene* Engine::GetActiveScene()
/// {
///		return sceneMap[myActiveState];
///
///
/// Future: CScene => CState

const CStateStack::EState CLoadLevelState::Load(const ELevel aLevel)
{
	if (mySceneReader.OpenBin(BinPath(aLevel)))
	{
		SaveModelPaths(aLevel);

		if (aLevel == ELevel::LoadScreen) //LoadScreen uses a different Type (Which kind of Data it will Load from Unity)
		{
			SLoadScreenData& data = mySceneReader.ReadLoadScreenData();
			CScene* loadScreenScene = new CScene();// myLoadScreenScene
			myUnityFactory.FillScene(data, BinModelPaths(aLevel), *loadScreenScene);

			std::cout << "Adding Loading Screen" << std::endl;
			return CEngine::GetInstance()->AddScene(CStateStack::EState::LoadLevel, loadScreenScene);
		}
		else //All other Scenes are regarded as "InGame" scenes. And will have to contain at least a Camera, Directional Light & Player (player is currently "utkommenterad", Fix Monday)
		{

			SInGameData& data = mySceneReader.ReadInGameData();
			CScene* inGameScene = new CScene();

			std::string navMeshPath = myLevelNames[static_cast<int>(aLevel)] + "_ExportedNavMesh.obj";
			inGameScene->InitNavMesh(navMeshPath);

			myUnityFactory.FillScene(data, BinModelPaths(aLevel), *inGameScene);
			//std::cout << "Adding Loading Screen" << std::endl;
			return CEngine::GetInstance()->AddScene(CStateStack::EState::InGame, inGameScene);
		}
	}
	return CStateStack::EState::NoState;
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
