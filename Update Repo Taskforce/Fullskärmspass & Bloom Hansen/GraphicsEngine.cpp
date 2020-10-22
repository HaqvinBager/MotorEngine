#include "stdafx.h"
#include "GraphicsEngine.h"
#include "WindowHandler.h"
#include <array>
#include <vector>

CGraphicsEngine::CGraphicsEngine() {
	myScene = CScene::GetInstance();
}

CGraphicsEngine::~CGraphicsEngine() {
}

bool CGraphicsEngine::Init(CWindowHandler::SWindowData aWindowData) {
	if (!myWindowHandler.Init(aWindowData)) {
		return false;
	}

	if (!myFramework.Init(&myWindowHandler)) {
		return false;
	}

	if (!myForwardRenderer.Init(&myFramework)) {
		return false;
	}

	myRenderManager.Init(&myFramework, &myWindowHandler);

	return true;
}

CDirectX11Framework* CGraphicsEngine::GetFramework() {
	return &myFramework;
}

CWindowHandler* CGraphicsEngine::GetWindowHandler() {
	return &myWindowHandler;
}

void CGraphicsEngine::BeginFrame() {
	std::array<float, 4> clearcolor = { 0.5f, 0.5f, 0.5f, 1.0f };
	myFramework.BeginFrame(clearcolor);
}

void CGraphicsEngine::EndFrame() {
	myFramework.EndFrame();
}

void CGraphicsEngine::RenderFrame() {
	bool bruhRender = false;

	if (bruhRender) {
		CEnvironmentLight* environmentLight = myScene->GetEnvironmentLight();
		CCamera* mainCamera = myScene->GetMainCamera();
		std::vector<CModelInstance*> modelsToRender = myScene->CullModels(mainCamera);
		std::vector<std::pair<unsigned int, std::array<CPointLight*, 8>>> modelPointLights;
		for (CModelInstance* instance : modelsToRender) {
			modelPointLights.push_back(myScene->CullLights(instance));
		}

		myForwardRenderer.Render(environmentLight, modelPointLights, mainCamera, modelsToRender);
	} else {
		myRenderManager.Render();
	}
}
