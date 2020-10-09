#pragma once
#include "WindowHandler.h"
#include "DirectXFramework.h"

class CWindowHandler;
class CDirextXFramework;
class CForwardRenderer;
class CTimer;
class CModelFactory;
class CCameraFactory;
class CLightFactory;
class CScene;

class CEngine
{
	friend class CForwardRenderer;
	friend class CModelFactory;
	friend class CLightFactory;
public:
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
	CScene* myScene;
	CTimer* myTimer;

	CModelFactory* myModelFactory;
	CCameraFactory* myCameraFactory;
	CLightFactory* myLightFactory;
};