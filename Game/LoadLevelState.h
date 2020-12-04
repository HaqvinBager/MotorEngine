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
	LoadScreen,
	Dungeon,
	Gardens,
	Castle,
	NavTest
};


class CLoadLevelState : public CState, public IObserver {
public:
	CLoadLevelState(CStateStack& aStateStack, const CStateStack::EState aState = CStateStack::EState::LoadLevel);
	~CLoadLevelState() override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void MakeSceneActive() override;

	void Receive(const SMessage& aMessage) override;
private:

	/// <summary>
	/// Returns the Index this Scene will use in CEngine::myScenes
	/// </summary>
	unsigned int Load(const ELevel aLevel);

	void SaveLevelNames();
	void SaveModelPaths(const ELevel aLevel);
	std::string& BinPath(const ELevel aLevel);
	std::vector<std::string>& BinModelPaths(const ELevel aLevel);

	CSceneReader mySceneReader;
	CUnityFactory myUnityFactory;

	std::future<unsigned int> myLoadLevelFuture;
	std::vector<std::string> myLevelNames;
	std::vector<std::string> myBinPaths;
	std::unordered_map<ELevel, std::string> myModelJsonFileMap;
	std::unordered_map<ELevel, std::vector<std::string>> myBinModelPaths;
};