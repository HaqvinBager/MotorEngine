#pragma once
#include <vector>
#include "GameObject.h"
#include "Component.h"
#include "SpriteInstance.h"

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
class CEnemyBehavior;

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
	CCameraComponent* GetMainCamera();
	CEnvironmentLight* GetEnvironmentLight();
	SNavMesh* GetNavMesh();
	std::vector<CGameObject*> CullGameObjects(CCameraComponent* aMainCamera);
	std::pair<unsigned int, std::array<CPointLight*, 8>> CullLights(CGameObject* aGameObject);
	std::vector<CParticleInstance*> CullParticles(CCameraComponent* aMainCamera);
	std::vector<CVFXInstance*> CullVFX(CCameraComponent* aMainCamera);
	const std::vector<SLineTime>& CullLines() const;
	const std::vector<CLineInstance*>& CullLineInstances() const;
	std::vector<CSpriteInstance*> CullSprites();
	std::vector<CAnimatedUIElement*> CullAnimatedUI(std::vector<CSpriteInstance*>& someFramesToReturn);
	std::vector<CTextInstance*> GetTexts();

	std::vector<CGameObject*> GetModelsToOutline() const { return myModelsToOutline; }

	bool AddInstances(std::vector<CGameObject*>& someGameObjects);
	bool AddInstance(CCamera* aCamera);
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
	bool AddPlayer(CGameObject* aPlayer);

	std::vector<CGameObject*> GetEnemies() { return myEnemies; }
	CGameObject* GetPlayer() { return myPlayer; }

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

	void TakeOwnershipOfAIBehavior(CEnemyBehavior* aBehavior);

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
	std::vector<CGameObject*> myGameObjects;
	CCameraComponent* myMainCamera;
	CEnvironmentLight* myEnvironmentLight;

	//Ev Remove // Ev? / Aki
	//std::vector<CCamera*> myCameras;
	//std::vector<CEnvironmentLight*> myEnvironmentLights;
	std::vector<CPointLight*> myPointLights;
	std::vector<CParticleInstance*> myParticles;
	std::vector<CVFXInstance*> myVFXInstances;
	std::vector<CLineInstance*> myLineInstances;
	std::vector<CAnimatedUIElement*> myAnimatedUIElements;
	std::vector<CTextInstance*> myTexts;
	std::unordered_map<ERenderOrder, std::vector<CSpriteInstance*>> mySpriteInstances;
	std::vector<CGameObject*> myEnemies;
	CGameObject* myPlayer;
	CEnemyBehavior* myEnemyBehavior;

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
