#pragma once
#include "WindowHandler.h"
#include "DirectXFramework.h"

#include "StateStack.h"

#include <string>
#include <vector>
#include <unordered_map>

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
class CAudioManager;

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
	bool Init(CWindowHandler::SWindowData& someWindowData);
	float BeginFrame();
	void RenderFrame();
	void EndFrame();
	CWindowHandler* GetWindowHandler();
	void InitWindowsImaging();
	void CrashWithScreenShot(std::wstring& aSubPath);

	static CEngine* GetInstance();
	
	const CStateStack::EState AddScene(const CStateStack::EState aState, CScene* aScene);
	void SetActiveScene(const CStateStack::EState aState);
	CScene& GetActiveScene();

	//void PopBackScene();
	//void SetActiveScene(CScene* aScene);
	
	//unsigned int ScenesSize();

private:
	static CEngine* ourInstance;
	
	CWindowHandler* myWindowHandler;
	CDirectXFramework* myFramework;
	CForwardRenderer* myForwardRenderer;
	CRenderManager* myRenderManager;
	CScene* myScene;
	CTimer* myTimer;
	CDebug* myDebug;

	//unsigned int myActiveScene;
	CStateStack::EState myActiveState;
	//std::vector<CScene*> myScenes;
	std::unordered_map<CStateStack::EState, CScene*> mySceneMap;
	CScene* myInGameScene;

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
	CAudioManager* myAudioManager;
};