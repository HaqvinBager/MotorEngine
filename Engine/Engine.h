#pragma once
#include "WindowHandler.h"
#include "DirectXFramework.h"
#include <string>
#include <vector>

class CWindowHandler;
class CDirextXFramework;
class CTimer;
class CModelFactory;
class CCameraFactory;
class CLightFactory;
class CScene;
class CRenderManager;
class CParticleFactory;
class CSpriteFactory;
class CTextFactory;
class CInputMapper;
class CDebug;
class CEnemyFactory;
class CMainSingleton;
class CForwardRenderer;
class CVFXFactory;
class CLineFactory;

class CEngine
{
	friend class CForwardRenderer;
	friend class CModelFactory;
	friend class CVFXFactory;
	friend class CLightFactory;
public:
	friend class CLineFactory;
	CEngine();
	~CEngine();
	void Destroy();
	bool Init(CWindowHandler::SWindowData& someWindowData);
	float BeginFrame();
	void RenderFrame();
	void EndFrame();
	CWindowHandler* GetWindowHandler();
	void InitWindowsImaging();
	void CrashWithScreenShot(std::wstring& aSubPath);

	static CEngine* GetInstance();

	unsigned int AddScene(CScene* aScene);
	void SetActiveScene(int sceneIndex);
	void SetActiveScene(CScene* aScene);
	CScene& GetActiveScene();

private:
	static CEngine* ourInstance;
	
	CWindowHandler* myWindowHandler;
	CDirectXFramework* myFramework;
	CForwardRenderer* myForwardRenderer;
	CRenderManager* myRenderManager;
	CScene* myScene;
	CTimer* myTimer;
	CDebug* myDebug;

	unsigned int myActiveScene;
	std::vector<CScene*> myScenes;

	CModelFactory* myModelFactory;
	CCameraFactory* myCameraFactory;
	CLightFactory* myLightFactory;
	CParticleFactory* myParticleFactory;
	CVFXFactory* myVFXFactory;
	CLineFactory* myLineFactory;
	CSpriteFactory* mySpriteFactory;
	CTextFactory* myTextFactory;
	CInputMapper* myInputMapper;
	CEnemyFactory* myEnemyFactory;
	CMainSingleton* myMainSingleton;
};