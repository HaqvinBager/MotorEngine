#pragma once
#include "WindowHandler.h"
#include "DirectXFramework.h"

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
class CInputMapper;

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



private:
	
	CWindowHandler* myWindowHandler;
	CDirectXFramework* myFramework;
	CForwardRenderer* myForwardRenderer;
	CRenderManager* myRenderManager;
	CScene* myScene;
	CTimer* myTimer;

	CModelFactory* myModelFactory;
	CCameraFactory* myCameraFactory;
	CLightFactory* myLightFactory;
	CParticleFactory* myParticleFactory;
	CVFXFactory* myVFXFactory;
	CLineFactory* myLineFactory;
	CSpriteFactory* mySpriteFactory;
	CInputMapper* myInputMapper;
};