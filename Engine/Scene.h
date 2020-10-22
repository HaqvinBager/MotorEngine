#pragma once
#include <vector>

class CModelInstance;
class CCamera;
class CEnvironmentLight;
class CGameObject;
class CCollisionManager;
class CPointLight;

class CScene {

	friend class CEngine;

public:

	static CScene* GetInstance();
	bool Init();
	void SetMainCamera(CCamera* aCamera);
	CCamera* GetMainCamera();
	CEnvironmentLight* GetEnvironmentLight();
	std::vector<CModelInstance*> CullModels(CCamera* aMainCamera);
	std::vector<CGameObject*> CullGameObjects(CCamera* aMainCamera);
	std::pair<unsigned int, std::array<CPointLight*, 8>> CullLights(CModelInstance* aModelInstance);

	bool AddInstance(CModelInstance* aModel);
	bool AddInstance(CCamera* aCamera);
	bool AddInstance(CEnvironmentLight* anEnvironmentLight);
	bool AddInstance(CPointLight* aPointLight);
	bool AddInstance(CGameObject* aGameObject);


	bool ClearInstances();
private:
	CScene();
	~CScene();

private:
	CCamera* myMainCamera;
	std::vector<CCamera*> myCameras;
	std::vector<CModelInstance*> myModelInstances;
	std::vector<CEnvironmentLight*> myEnvironmentLights;
	std::vector<CPointLight*> myPointLights;
	std::vector<CGameObject*> myGameObjects;
	CCollisionManager* myCollisionManager;
	static CScene* ourInstance;
};