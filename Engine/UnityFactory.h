#pragma once
#include "SceneData.h"


class CScene;
class CGameObject;
struct SNavMesh;

class CUnityFactory
{
public:
	CUnityFactory();
	~CUnityFactory();

	bool FillScene(const SLoadScreenData& aData, const std::vector<std::string>& aBinModelPaths, CScene& aScene);
	bool FillScene(const SInGameData& aData, const std::vector<std::string>& aBinModelPaths, CScene& aScene);

	//std::vector<CGameObject*> CreateGameObjects(const SInGameData& aData);
	//std::vector<CGameObject*> CreateGameObjects(const SLoadScreenData& aData, const std::vector<std::string>& aBinModelPaths);

private:
	CGameObject* CreateGameObject(const SCameraData& aData);
	CGameObject* CreateGameObject(const SDirectionalLightData& aData);
	//CGameObject* CreateGameObject(const SPointLightData& aData);

	CGameObject* CreateGameObject(const SGameObjectData& aData, const std::string& aModelPath);
	CGameObject* CreateGameObject(const SPlayerData& aData, const std::string& aModelPath);
};
