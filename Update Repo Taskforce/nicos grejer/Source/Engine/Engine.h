#pragma once
#include "WindowHandler.h"
#include "DirectXFramework.h"

class CWindowHandler;
class CDirextXFramework;
class CScene;
class CTimer;
class CModelFactory;
class CCameraFactory;
class CLightFactory;
class CRenderManager;
class CParticleFactory;
class CVFXFactory;

class CEngine
{
	friend class CModelFactory;
	friend class CLightFactory;
	friend class CForwardRenderer;
	friend class CVFXFactory;
public:
	CEngine();
	~CEngine();

	bool Init(CWindowHandler::SWindowData& someWindowData);

	float BeginFrame();
	void RenderFrame();
	void EndFrame();

public:
	CWindowHandler* GetWindowHandler() const { return myWindowHandler; }

private:
	CWindowHandler* myWindowHandler;
	CDirectXFramework* myFramework;
	CScene* myScene;
	CTimer* myEngineTimer;
	CModelFactory* myModelFactory;
	CCameraFactory* myCameraFactory;
	CLightFactory* myLightFactory;
	CRenderManager* myRenderManager;
	CParticleFactory* myParticleFactory;
	CVFXFactory* myVFXFactory;
};