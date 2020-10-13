#pragma once
#include <vector>

class CModelInstance;
class CCamera;
class CEnvironmentLight;
class CGameObject;

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

	bool AddInstance(CModelInstance* aModel);
	bool AddInstance(CCamera* aCamera);
	bool AddInstance(CEnvironmentLight* anEnvironmentLight);
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
	std::vector<CGameObject*> myGameObjects;
	static CScene* ourInstance;
};