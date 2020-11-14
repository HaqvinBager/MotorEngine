#include "stdafx.h"
#include "RenderManager.h"
#include "DirectXFramework.h"
#include "Scene.h"
#include "LineInstance.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "ModelFactory.h"
#include "CameraComponent.h"


CRenderManager::CRenderManager() : myScene(*CScene::GetInstance())
{
}

CRenderManager::~CRenderManager()
{
}

bool CRenderManager::Init(CDirectXFramework* aFramework, CWindowHandler* aWindowHandler)
{
	if (!myForwardRenderer.Init(aFramework))
	{
		return false;
	}

	if (!myFullscreenRenderer.Init(aFramework))
	{
		return false;
	}

	if (!myFullscreenTextureFactory.Init(aFramework))
	{
		return false;
	}

	if (!myParticleRenderer.Init(aFramework))
	{
		return false;
	}

	if (!myRenderStateManager.Init(aFramework))
	{
		return false;
	}

	if (!myVFXRenderer.Init(aFramework))
	{
		return false;
	}

	if (!mySpriteRenderer.Init(aFramework))
	{
		return false;
	}

	if (!myTextRenderer.Init(aFramework))
	{
		return false;
	}

	ID3D11Texture2D* backbufferTexture = aFramework->GetBackbufferTexture();
	if (!backbufferTexture)
	{
		return false;
	}
	myBackbuffer = myFullscreenTextureFactory.CreateTexture(backbufferTexture);
	myIntermediateDepth = myFullscreenTextureFactory.CreateDepth({ (float)aWindowHandler->GetWidth(), (float)aWindowHandler->GetHeight() }, DXGI_FORMAT_D24_UNORM_S8_UINT);

	myIntermediateTexture = myFullscreenTextureFactory.CreateTexture({ (float)aWindowHandler->GetWidth(), (float)aWindowHandler->GetHeight() }, DXGI_FORMAT_R8G8B8A8_UNORM);
	myLuminanceTexture = myFullscreenTextureFactory.CreateTexture({ (float)aWindowHandler->GetWidth(), (float)aWindowHandler->GetHeight() }, DXGI_FORMAT_R8G8B8A8_UNORM);
	myHalfSizeTexture = myFullscreenTextureFactory.CreateTexture({ (float)aWindowHandler->GetWidth() / 2.0f, (float)aWindowHandler->GetHeight() / 2.0f }, DXGI_FORMAT_R8G8B8A8_UNORM);
	myQuaterSizeTexture = myFullscreenTextureFactory.CreateTexture({ (float)aWindowHandler->GetWidth() / 4.0f, (float)aWindowHandler->GetHeight() / 4.0f }, DXGI_FORMAT_R8G8B8A8_UNORM);
	myBlurTexture1 = myFullscreenTextureFactory.CreateTexture({ (float)aWindowHandler->GetWidth(), (float)aWindowHandler->GetHeight() }, DXGI_FORMAT_R8G8B8A8_UNORM);
	myBlurTexture2 = myFullscreenTextureFactory.CreateTexture({ (float)aWindowHandler->GetWidth(), (float)aWindowHandler->GetHeight() }, DXGI_FORMAT_R8G8B8A8_UNORM);
	myVignetteTexture = myFullscreenTextureFactory.CreateTexture({ (float)aWindowHandler->GetWidth(), (float)aWindowHandler->GetHeight() }, DXGI_FORMAT_R8G8B8A8_UNORM);

	return true;
}

void CRenderManager::Render()
{
	myRenderStateManager.SetAllDefault();
	myBackbuffer.ClearTexture({ 0.5f,0.5f,0.5f,1.0f });
	myIntermediateTexture.ClearTexture({ 0.5f,0.5f,0.5f,1.0f });
	myIntermediateDepth.ClearDepth();

	myIntermediateTexture.SetAsActiveTarget(&myIntermediateDepth);

	CEnvironmentLight* environmentlight = myScene.GetEnvironmentLight();
	CCameraComponent* maincamera = myScene.GetMainCamera();
	//std::vector<CModelComponent*> modelsToRender = myScene.CullGameObjects(maincamera);
	std::vector<CGameObject*> gameObjects = myScene.CullGameObjects(maincamera);
	std::vector<std::pair<unsigned int, std::array<CPointLight*, 8>>> pointlights;
	for (CGameObject* instance : gameObjects)
	{
		pointlights.push_back(myScene.CullLights(instance));
	}
	myForwardRenderer.Render(environmentlight, pointlights, maincamera, gameObjects);

	auto modelToOutline = myScene.GetModelToOutline();
	if (modelToOutline) {
		pointlights.emplace_back(myScene.CullLights(modelToOutline));
		std::vector<CGameObject*> interimVector;
		interimVector.emplace_back(modelToOutline);
		myRenderStateManager.SetDepthStencilState(CRenderStateManager::DepthStencilStates::DEPTHSTENCILSTATE_STENCILWRITE, 0xFF);
		myForwardRenderer.Render(environmentlight, pointlights, maincamera, interimVector);

		modelToOutline->GetComponent<CTransformComponent>()->SetOutlineScale();
		myRenderStateManager.SetDepthStencilState(CRenderStateManager::DepthStencilStates::DEPTHSTENCILSTATE_STENCILMASK, 0xFF);
		myForwardRenderer.RenderOutline(maincamera, modelToOutline, CModelFactory::GetInstance()->GetOutlineModelSubset());
		modelToOutline->GetComponent<CTransformComponent>()->ResetScale();
	}

	const std::vector<CLineInstance*>& lineInstances = myScene.CullLineInstances();
	const std::vector<SLineTime>& lines = myScene.CullLines();

	myForwardRenderer.RenderLines(maincamera, lines);
	myForwardRenderer.RenderLineInstances(maincamera, lineInstances);


	myRenderStateManager.SetBlendState(CRenderStateManager::BlendStates::BLENDSTATE_ALPHABLEND);
	myRenderStateManager.SetDepthStencilState(CRenderStateManager::DepthStencilStates::DEPTHSTENCILSTATE_ONLYREAD);

	myVFXRenderer.Render(maincamera, gameObjects);

	myParticleRenderer.Render(maincamera, gameObjects);

	//std::vector<CSpriteInstance*> sprites = myScene.CullSprites();
	//mySpriteRenderer.Render(sprites);

	//std::vector<CAnimatedUIElement*> animatedUIElements = myScene.CullAnimatedUI();
	//mySpriteRenderer.Render(animatedUIElements);

	myRenderStateManager.SetBlendState(CRenderStateManager::BlendStates::BLENDSTATE_DISABLE);
	myRenderStateManager.SetDepthStencilState(CRenderStateManager::DepthStencilStates::DEPTHSTENCILSTATE_DEFAULT);


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

	myVignetteTexture.SetAsActiveTarget();
	myIntermediateTexture.SetAsResourceOnSlot(0);
	myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::FULLSCREENSHADER_VIGNETTE);

	myBackbuffer.SetAsActiveTarget();
	myVignetteTexture.SetAsResourceOnSlot(0);
	myHalfSizeTexture.SetAsResourceOnSlot(1);
	myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::FULLSCREENSHADER_BLOOM);


	myRenderStateManager.SetBlendState(CRenderStateManager::BlendStates::BLENDSTATE_ALPHABLEND);
	myRenderStateManager.SetDepthStencilState(CRenderStateManager::DepthStencilStates::DEPTHSTENCILSTATE_ONLYREAD);

	std::vector<CSpriteInstance*> sprites = myScene.CullSprites();
	mySpriteRenderer.Render(sprites);

	std::vector<CSpriteInstance*> animatedUIFrames;
	std::vector<CAnimatedUIElement*> animatedUIElements = myScene.CullAnimatedUI(animatedUIFrames);
	mySpriteRenderer.Render(animatedUIElements);
	mySpriteRenderer.Render(animatedUIFrames);

	myRenderStateManager.SetBlendState(CRenderStateManager::BlendStates::BLENDSTATE_DISABLE);
	myRenderStateManager.SetDepthStencilState(CRenderStateManager::DepthStencilStates::DEPTHSTENCILSTATE_DEFAULT);

	std::vector<CTextInstance*> textsToRender = myScene.GetTexts();
	myTextRenderer.Render(textsToRender);
}
