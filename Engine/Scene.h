#pragma once
#include <vector>

class CModelInstance;
class CCamera;
class CEnvironmentLight;

class CScene {

	friend class CEngine;

public:

	static CScene* GetInstance();
	bool Init();
	void SetMainCamera(CCamera* aCamera);
	CCamera* GetMainCamera();
	CEnvironmentLight* GetEnvironmentLight();
	std::vector<CModelInstance*> CullModels(CCamera* aMainCamera);

	bool AddInstance(CModelInstance* aModel);
	bool AddInstance(CCamera* aCamera);
	bool AddInstance(CEnvironmentLight* anEnvironmentLight);

	bool ClearInstances();
private:
	CScene();
	~CScene();

private:
	CCamera* myMainCamera;
	std::vector<CCamera*> myCameras;
	std::vector<CModelInstance*> myModelInstances;
	std::vector<CEnvironmentLight*> myEnvironmentLights;
	static CScene* ourInstance;
};