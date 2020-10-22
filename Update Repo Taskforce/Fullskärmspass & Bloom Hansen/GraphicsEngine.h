#pragma once
#include "WindowHandler.h"
#include "DirectX11Framework.h"
#include "Scene.h"
#include "ForwardRenderer.h"

#include "RenderManager.h"

class CGraphicsEngine {
public:
	CGraphicsEngine();
	~CGraphicsEngine();
	bool Init(CWindowHandler::SWindowData aWindowData);
	CDirectX11Framework* GetFramework();
	CWindowHandler* GetWindowHandler();
	void BeginFrame();
	void EndFrame();
	void RenderFrame();

private:
	CWindowHandler myWindowHandler;
	CDirectX11Framework myFramework;
	CForwardRenderer myForwardRenderer;
	CRenderManager myRenderManager;
	CScene* myScene;
};