#pragma once
#include <vector>

class CModelComponent;
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
	std::vector<CGameObject*> CullGameObjects(CCamera* aMainCamera);
	std::pair<unsigned int, std::array<CPointLight*, 8>> CullLights(CGameObject* aGameObject);
	std::vector<CParticleInstance*> CullParticles(CCamera* aMainCamera);
	std::vector<CVFXInstance*> CullVFX(CCamera* aMainCamera);
	const std::vector<CLineInstance>& CullLines(CCamera* aMainCamera) const;
	std::vector<CSpriteInstance*> CullSprites(CCamera* aMainCamera);

	bool AddInstance(CCamera* aCamera);
	bool AddInstance(CEnvironmentLight* anEnvironmentLight);
	bool AddInstance(CPointLight* aPointLight);
	bool AddInstance(CGameObject* aGameObject);
	bool AddInstance(CParticleInstance* aParticleInstance);
	bool AddInstance(CVFXInstance* aVFXInstance);
	bool AddInstance(CLineInstance* aLineInstance);
	bool AddInstance(CSpriteInstance* aSprite);

	bool ClearScene();
private:
	CScene();
	~CScene();

private:
	std::vector<CGameObject*> myGameObjects;
	CCamera* myMainCamera;

	//Ev Remove
	std::vector<CCamera*> myCameras;
	std::vector<CEnvironmentLight*> myEnvironmentLights;
	std::vector<CPointLight*> myPointLights;
	std::vector<CParticleInstance*> myParticles;
	std::vector<CVFXInstance*> myVFXInstances;
	std::vector<CLineInstance*> myLineInstances;
	std::vector<CSpriteInstance*> mySprites;
	CCollisionManager* myCollisionManager;
	static CScene* ourInstance;
};