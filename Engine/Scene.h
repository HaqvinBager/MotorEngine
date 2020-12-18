#pragma once
#include <vector>
#include "GameObject.h"
#include "Component.h"
#include "SpriteInstance.h"
#include "EngineDefines.h"

class CModelComponent;
class CCamera;
class CEnvironmentLight;
class CCollisionManager;
class CPointLight;
class CParticleInstance;
class CVFXInstance;
class CLineInstance;
//class CSpriteInstance;
class CAnimatedUIElement;
class CTextInstance;
class CCameraComponent;
struct SLineTime;
struct SNavMesh;
class CModel;
class IAIBehavior;
class CInstancedModelComponent;

typedef std::pair<unsigned int, std::array<CPointLight*, LIGHTCOUNT>> LightPair;

class CScene {
	friend class CEngine;
	friend class CInGameState;
	friend class CMenuState;
public:

	CScene();
	~CScene();

	//static CScene* GetInstance();
	bool Init();
	bool InitNavMesh(std::string aPath);
	void SetMainCamera(CCameraComponent* aCamera);


	//This will run every 5-10 frames (It doesn't need to run all the time anyway!)
	void UpdateLightsNearestPlayer();

	CCameraComponent* GetMainCamera();
	CEnvironmentLight* GetEnvironmentLight();
	SNavMesh* GetNavMesh();
	std::vector<CGameObject*> CullGameObjects(CCameraComponent* aMainCamera);
	std::pair<unsigned int, std::array<CPointLight*, LIGHTCOUNT>> CullLights(CGameObject* aGameObject);
	LightPair CullLightInstanced(CInstancedModelComponent* aModelType);

	std::vector<CParticleInstance*> CullParticles(CCameraComponent* aMainCamera);
	std::vector<CVFXInstance*> CullVFX(CCameraComponent* aMainCamera);
	const std::vector<SLineTime>& CullLines() const;
	const std::vector<CLineInstance*>& CullLineInstances() const;
	std::vector<CSpriteInstance*> CullSprites();
	std::vector<CAnimatedUIElement*> CullAnimatedUI(std::vector<CSpriteInstance*>& someFramesToReturn);
	std::vector<CTextInstance*> GetTexts();

	std::vector<CGameObject*> GetModelsToOutline() const { return myModelsToOutline; }
	std::vector<CPointLight*> GetLightsNearestPlayer() { return myLightsSortedNearestPlayer;  }

	std::vector<CPointLight*>& GetPointLights() { return myPointLights; }

	bool AddInstances(std::vector<CGameObject*>& someGameObjects);
	bool SetEnvironmentLight(CEnvironmentLight* anEnvironmentLight);
	bool AddInstance(CPointLight* aPointLight);
	bool AddInstance(CGameObject* aGameObject);
	bool AddInstance(CParticleInstance* aParticleInstance);
	bool AddInstance(CVFXInstance* aVFXInstance);
	bool AddInstance(CLineInstance* aLineInstance);
	bool AddInstance(CSpriteInstance* aSprite);
	bool AddInstance(CAnimatedUIElement* anAnimatedUIElement);
	bool RemoveInstance(CAnimatedUIElement* anAnimatedUIElement);
	bool AddInstance(CTextInstance* aText);
	bool AddEnemies(CGameObject* aEnemy);
	bool AddBoss(CGameObject* aBoss);
	bool AddDestructible(CGameObject* aDestructible);
	bool AddPlayer(CGameObject* aPlayer);

	std::vector<CGameObject*> GetEnemies() { return myEnemies; }
	std::vector<CGameObject*> GetDestructibles() { return myDestructibles; }
	CGameObject* GetPlayer() { return myPlayer; }
	CGameObject* GetBoss() { return myBoss; }


	bool RemoveInstance(CGameObject* aGameObject);
	bool RemoveInstance(CPointLight* aPointLight);

	bool DestroyGameObjects();
	bool DestroySprites();
	bool DestroyPointLights();
	bool DestroyParticles();
	bool DestroyVFXInstances();
	bool DestroyLineInstances();
	bool DestroyAnimatedUIElement();
	bool DestroyTextInstances();

	void SetPlayerToOutline(CGameObject* aPlayer);
	void SetEnemyToOutline(CGameObject* anEnemy);

	const bool Ready() const { return myIsReadyToRender; }
	void Ready(bool aReady) { myIsReadyToRender = aReady; }

	void SetShouldRenderLineInstance(const bool aShouldRender);

	void TakeOwnershipOfAIBehavior(IAIBehavior* aBehavior);

	const std::vector<CGameObject*>& GetActiveGameObjects() const { return myGameObjects; }

	template <class T>
	T* FindObjectOfType() {
		for (auto& gameObject : myGameObjects) {
			if (gameObject->GetComponent<T>() != nullptr) {
				return gameObject->GetComponent<T>();
			}
		}
		return nullptr;
	}

private:
	struct NearestPlayerComparer {
		DirectX::SimpleMath::Vector3 myPos;
		bool operator()(const CPointLight* a, const CPointLight* b) const;
	} ourNearestPlayerComparer;

	std::vector<CGameObject*> myGameObjects;
	CCameraComponent* myMainCamera;
	CEnvironmentLight* myEnvironmentLight;

	//Ev Remove // Ev? / Aki
	//std::vector<CCamera*> myCameras;
	//std::vector<CEnvironmentLight*> myEnvironmentLights;
	std::vector<CPointLight*> myLightsSortedNearestPlayer;
	std::vector<CPointLight*> myPointLights;
	std::vector<CParticleInstance*> myParticles;
	std::vector<CVFXInstance*> myVFXInstances;
	std::vector<CLineInstance*> myLineInstances;
	std::vector<CAnimatedUIElement*> myAnimatedUIElements;
	std::vector<CTextInstance*> myTexts;
	std::unordered_map<ERenderOrder, std::vector<CSpriteInstance*>> mySpriteInstances;
	std::vector<CGameObject*> myEnemies;
	std::vector<CGameObject*> myDestructibles;
	CGameObject* myPlayer;
	CGameObject* myBoss;
	IAIBehavior* myEnemyBehavior;

	//CCollisionManager* myCollisionManager;

	SNavMesh* myNavMesh;
	CLineInstance* myNavMeshGrid;

	bool myIsReadyToRender;

	//static CScene* ourInstance;

	std::vector<CGameObject*> myModelsToOutline;

#ifdef  _DEBUG
private:
	bool myShouldRenderLineInstance;
#endif //  _DEBUG
};
