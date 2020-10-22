#include "stdafx.h"
#include "RenderManager.h"
#include "DirectX11Framework.h"
#include "Scene.h"

CRenderManager::CRenderManager() : myScene(*CScene::GetInstance()) {
}

CRenderManager::~CRenderManager() {
}

bool CRenderManager::Init(CDirectX11Framework* aFramework, CWindowHandler* aWindowHandler) {
	if (!myForwardRenderer.Init(aFramework)) {
		return false;
	}

	if (!myFullscreenRenderer.Init(aFramework)) {
		return false;
	}

	if (!myFullscreenTextureFactory.Init(aFramework)) {
		return false;
	}

	ID3D11Texture2D* backbufferTexture = aFramework->GetBackbufferTexture();
	if (!backbufferTexture) {
		return false;
	}
	myBackbuffer = myFullscreenTextureFactory.CreateTexture(backbufferTexture);
	myIntermediateDepth = myFullscreenTextureFactory.CreateDepth({ (float)aWindowHandler->GetWidth(), (float)aWindowHandler->GetHeight() }, DXGI_FORMAT_D32_FLOAT);

	myIntermediateTexture = myFullscreenTextureFactory.CreateTexture({ (float)aWindowHandler->GetWidth(), (float)aWindowHandler->GetHeight() }, DXGI_FORMAT_R8G8B8A8_UNORM);
	myLuminanceTexture = myFullscreenTextureFactory.CreateTexture({ (float)aWindowHandler->GetWidth(), (float)aWindowHandler->GetHeight() }, DXGI_FORMAT_R8G8B8A8_UNORM);
	myHalfSizeTexture = myFullscreenTextureFactory.CreateTexture({ (float)aWindowHandler->GetWidth() / 2.0f, (float)aWindowHandler->GetHeight() / 2.0f }, DXGI_FORMAT_R8G8B8A8_UNORM);
	myQuaterSizeTexture = myFullscreenTextureFactory.CreateTexture({ (float)aWindowHandler->GetWidth() / 4.0f, (float)aWindowHandler->GetHeight() / 4.0f }, DXGI_FORMAT_R8G8B8A8_UNORM);
	myBlurTexture1 = myFullscreenTextureFactory.CreateTexture({ (float)aWindowHandler->GetWidth(), (float)aWindowHandler->GetHeight() }, DXGI_FORMAT_R8G8B8A8_UNORM);
	myBlurTexture2 = myFullscreenTextureFactory.CreateTexture({ (float)aWindowHandler->GetWidth(), (float)aWindowHandler->GetHeight() }, DXGI_FORMAT_R8G8B8A8_UNORM);

	return true;
}

void CRenderManager::Render() {
	myBackbuffer.ClearTexture();
	myIntermediateTexture.ClearTexture();
	myIntermediateDepth.ClearDepth();

	myIntermediateTexture.SetAsActiveTarget(&myIntermediateDepth);

	CEnvironmentLight* environmentlight = myScene.GetEnvironmentLight();
	CCamera* maincamera = myScene.GetMainCamera();
	std::vector<CModelInstance*> modelsToRender = myScene.CullModels(maincamera);
	std::vector<std::pair<unsigned int, std::array<CPointLight*, 8>>> pointlights;
	for (CModelInstance* instance : modelsToRender) {
		pointlights.push_back(myScene.CullLights(instance));
	}
	myForwardRenderer.Render(environmentlight, pointlights, maincamera, modelsToRender);

	myLuminanceTexture.SetAsActiveTarget();
	myIntermediateTexture.SetAsResourceOnSlot(0);
	myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::FULLSCREENSHADER_LUMINANCE);

	myHalfSizeTexture.SetAsActiveTarget();
	myLuminanceTexture.SetAsResourceOnSlot(0);
	myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::FULLSCREENSHADER_COPY);

	myQuaterSizeTexture.SetAsActiveTarget();
	myHalfSizeTexture.SetAsResourceOnSlot(0);
	myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::FULLSCREENSHADER_COPY);

	myBlurTexture1.SetAsActiveTarget();
	myQuaterSizeTexture.SetAsResourceOnSlot(0);
	myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::FULLSCREENSHADER_COPY);

	myBlurTexture2.SetAsActiveTarget();
	myBlurTexture1.SetAsResourceOnSlot(0);
	myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::FULLSCREENSHADER_GAUSSIANHORIZONTAL);

	myBlurTexture1.SetAsActiveTarget();
	myBlurTexture2.SetAsResourceOnSlot(0);
	myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::FULLSCREENSHADER_GAUSSIANVERTICAL);

	myBlurTexture2.SetAsActiveTarget();
	myBlurTexture1.SetAsResourceOnSlot(0);
	myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::FULLSCREENSHADER_GAUSSIANHORIZONTAL);

	myBlurTexture1.SetAsActiveTarget();
	myBlurTexture2.SetAsResourceOnSlot(0);
	myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::FULLSCREENSHADER_GAUSSIANVERTICAL);

	myQuaterSizeTexture.SetAsActiveTarget();
	myBlurTexture1.SetAsResourceOnSlot(0);
	myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::FULLSCREENSHADER_COPY);

	myHalfSizeTexture.SetAsActiveTarget();
	myQuaterSizeTexture.SetAsResourceOnSlot(0);
	myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::FULLSCREENSHADER_COPY);

	myBackbuffer.SetAsActiveTarget();
	myIntermediateTexture.SetAsResourceOnSlot(0);
	myHalfSizeTexture.SetAsResourceOnSlot(1);
	myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::FULLSCREENSHADER_BLOOM);
}
