#pragma once
#include <vector>

class CModelInstance;
class CCamera;
class CEnvironmentLight;
class CGameObject;
class CCollisionManager;
class CPointLight;
class CParticleInstance;
class CVFXInstance;
class CLineInstance;
class CSpriteInstance;

class CScene {
	friend class CEngine;
	friend class CInGameState;
public:

	static CScene* GetInstance();
	bool Init();
	void SetMainCamera(CCamera* aCamera);
	CCamera* GetMainCamera();
	CEnvironmentLight* GetEnvironmentLight();
	std::vector<CModelInstance*> CullModels(CCamera* aMainCamera);
	std::vector<CGameObject*> CullGameObjects(CCamera* aMainCamera);
	std::pair<unsigned int, std::array<CPointLight*, 8>> CullLights(CModelInstance* aModelInstance);
	std::vector<CParticleInstance*> CullParticles(CCamera* aMainCamera);
	std::vector<CVFXInstance*> CullVFX(CCamera* aMainCamera);
	std::vector<CLineInstance*> CullLines(CCamera* aMainCamera);
	std::vector<CSpriteInstance*> CullSprites(CCamera* aMainCamera);

	bool AddInstance(CModelInstance* aModel);
	bool AddInstance(CCamera* aCamera);
	bool AddInstance(CEnvironmentLight* anEnvironmentLight);
	bool AddInstance(CPointLight* aPointLight);
	bool AddInstance(CGameObject* aGameObject);
	bool AddInstance(CParticleInstance* aParticleInstance);
	bool AddInstance(CVFXInstance* aVFXInstance);
	bool AddInstance(CLineInstance* aLineInstance);
	bool AddInstance(CSpriteInstance* aSprite);

	bool ClearInstances();

private:

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
	std::vector<CParticleInstance*> myParticles;
	std::vector<CVFXInstance*> myVFXInstances;
	std::vector<CLineInstance*> myLineInstances;
	std::vector<CSpriteInstance*> mySprites;
	CCollisionManager* myCollisionManager;
	static CScene* ourInstance;
};