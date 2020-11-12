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
class CTextInstance;
class CCameraComponent;
struct SLineTime;

class CScene {
	friend class CEngine;
	friend class CInGameState;
	friend class CMenuState;
public:

	static CScene* GetInstance();
	bool Init();
	void SetMainCamera(CCameraComponent* aCamera);
	CCameraComponent* GetMainCamera();
	CEnvironmentLight* GetEnvironmentLight();
	std::vector<CGameObject*> CullGameObjects(CCameraComponent* aMainCamera);
	std::pair<unsigned int, std::array<CPointLight*, 8>> CullLights(CGameObject* aGameObject);
	std::vector<CParticleInstance*> CullParticles(CCameraComponent* aMainCamera);
	std::vector<CVFXInstance*> CullVFX(CCameraComponent* aMainCamera);
	const std::vector<SLineTime>& CullLines() const;
	const std::vector<CLineInstance*>& CullLineInstances() const;
	std::vector<CSpriteInstance*> CullSprites(CCameraComponent* aMainCamera);
	std::vector<CTextInstance*> GetTexts();

	CGameObject* GetModelToOutline() const { return myModelToOutline; }

	bool AddInstance(CCamera* aCamera);
	bool AddInstance(CEnvironmentLight* anEnvironmentLight);
	bool AddInstance(CPointLight* aPointLight);
	bool AddInstance(CGameObject* aGameObject);
	bool AddInstance(CParticleInstance* aParticleInstance);
	bool AddInstance(CVFXInstance* aVFXInstance);
	bool AddInstance(CLineInstance* aLineInstance);
	bool AddInstance(CSpriteInstance* aSprite);
	bool AddInstance(CTextInstance* aText);

	bool RemoveInstance(CGameObject* aGameObject);
	bool RemoveInstance(CPointLight* aPointLight);

	bool ClearScene();
	bool ClearSprites();

	void SetModelToOutline(CGameObject* aGameObject);

private:
	CScene();
	~CScene();

private:
	std::vector<CGameObject*> myGameObjects;
	CCameraComponent* myMainCamera;

	//Ev Remove
	std::vector<CCamera*> myCameras;
	std::vector<CEnvironmentLight*> myEnvironmentLights;
	std::vector<CPointLight*> myPointLights;
	std::vector<CParticleInstance*> myParticles;
	std::vector<CVFXInstance*> myVFXInstances;
	std::vector<CLineInstance*> myLineInstances;
	std::vector<CSpriteInstance*> mySprites;
	std::vector<CTextInstance*> myTexts;
	CCollisionManager* myCollisionManager;
	static CScene* ourInstance;

	CGameObject* myModelToOutline;
};