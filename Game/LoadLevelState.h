#pragma once
#include "State.h"
#include <future>
#include "SceneReader.h"
#include "UnityFactory.h"
#include "Observer.h"

/// <summary>
/// This Enum is mapped to the Levels that are inside the LevelSelect.Json file (Important that they are in the same order as in the json file)
/// </summary>
enum class ELevel
{
	//LoadScreen,
	//Dungeon,
	//Gardens,
	//Castle,
	//NavTest,
	//BossRoom	
	
	LoadScreen,
	Level1,
	Level2,
	Level3,
	Level4,
	Level5,
	Level6,
	Level7,
	Level8,
	Level9
};

class CScene;

class CLoadLevelState : public CState, public IStringObserver
{
public:
	CLoadLevelState(CStateStack& aStateStack, const CStateStack::EState aState = CStateStack::EState::LoadLevel);
	~CLoadLevelState() override;

	void Start() override;
	void Stop() override;
	void Update() override;
	void Receive(const SStringMessage& aMessage) override;

private:
	void Awake() override;

	/// <summary>
	/// Returns the Index this Scene will use in CEngine::myScenes
	/// </summary>
	const CStateStack::EState Load(const ELevel aLevel);

	void SaveLevelNames();
	void SaveModelPaths(const ELevel aLevel);
	std::string& BinPath(const ELevel aLevel);
	std::vector<std::string>& BinModelPaths(const ELevel aLevel);

	ELevel myLevelToLoad;
	

	CSceneReader mySceneReader;
	CUnityFactory myUnityFactory;

	std::future<CStateStack::EState> myLoadLevelFuture;
	std::vector<std::string> myLevelNames;
	std::vector<std::string> myBinPaths;
	std::unordered_map<ELevel, std::string> myModelJsonFileMap;
	std::unordered_map<ELevel, std::vector<std::string>> myBinModelPaths;
};