#include "stdafx.h"
#include "RenderManager.h"
#include "DirectXFramework.h"
#include "Scene.h"
#include "LineInstance.h"
#include "ModelFactory.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "ModelComponent.h"
#include "InstancedModelComponent.h"
#include "MainSingleton.h"
#include "PopupTextService.h"
#include "Engine.h"
#include "Scene.h"

#include <algorithm>

CRenderManager::CRenderManager() /*: myScene(*CScene::GetInstance())*/
	: myUseBloom(true)
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
	myIntermediateDepth = myFullscreenTextureFactory.CreateDepth(aWindowHandler->GetResolution(), DXGI_FORMAT_D24_UNORM_S8_UINT);

	myIntermediateTexture = myFullscreenTextureFactory.CreateTexture(aWindowHandler->GetResolution(), DXGI_FORMAT_R8G8B8A8_UNORM);
	myLuminanceTexture = myFullscreenTextureFactory.CreateTexture(aWindowHandler->GetResolution(), DXGI_FORMAT_R8G8B8A8_UNORM);
	myHalfSizeTexture = myFullscreenTextureFactory.CreateTexture(aWindowHandler->GetResolution() / 2.0f, DXGI_FORMAT_R8G8B8A8_UNORM);
	myQuaterSizeTexture = myFullscreenTextureFactory.CreateTexture(aWindowHandler->GetResolution() / 4.0f, DXGI_FORMAT_R8G8B8A8_UNORM);
	myBlurTexture1 = myFullscreenTextureFactory.CreateTexture(aWindowHandler->GetResolution(), DXGI_FORMAT_R8G8B8A8_UNORM);
	myBlurTexture2 = myFullscreenTextureFactory.CreateTexture(aWindowHandler->GetResolution(), DXGI_FORMAT_R8G8B8A8_UNORM);
	myVignetteTexture = myFullscreenTextureFactory.CreateTexture(aWindowHandler->GetResolution(), DXGI_FORMAT_R8G8B8A8_UNORM);

	return true;
}

void CRenderManager::Render(CScene& aScene)
{
	//if (CScene::GetInstance()->Ready() == false)
	//	return;

	if (Input::GetInstance()->IsKeyPressed(VK_F6))
	{
		myUseBloom = myForwardRenderer.ToggleRenderPass();
	}

	myRenderStateManager.SetAllDefault();
	myBackbuffer.ClearTexture({0.1f,0.1f,0.1f,1.0f});
	myIntermediateTexture.ClearTexture({0.1f,0.1f,0.1f,1.0f});
	myIntermediateDepth.ClearDepth();

	myIntermediateTexture.SetAsActiveTarget(&myIntermediateDepth);

	CEnvironmentLight* environmentlight = aScene.GetEnvironmentLight();
	CCameraComponent* maincamera = aScene.GetMainCamera();
	//std::vector<CModelComponent*> modelsToRender = myScene.CullGameObjects(maincamera);
	std::vector<CGameObject*> gameObjects = aScene.CullGameObjects(maincamera);
	std::vector<CGameObject*> instancedGameObjects;
	std::vector<CGameObject*> instancedGameObjectsWithAlpha;
	std::vector<std::pair<unsigned int, std::array<CPointLight*, 8>>> pointlights;
	
	std::vector<int> indicesOfOutlineModels;
	for (unsigned int i = 0; i < gameObjects.size(); ++i)
	{
		auto instance = gameObjects[i];
		for (auto gameObjectToOutline : aScene.GetModelsToOutline()) {
			if (instance == gameObjectToOutline) {
				indicesOfOutlineModels.emplace_back(i);
			}
		}

		if (instance->GetComponent<CModelComponent>()) {
			pointlights.emplace_back(aScene.CullLights(instance));
		} else if (instance->GetComponent<CInstancedModelComponent>()) {
			pointlights.emplace_back(aScene.CullLights(instance));
			if (instance->GetComponent<CInstancedModelComponent>()->RenderWithAlpha())
			{
				instancedGameObjectsWithAlpha.emplace_back(instance);
				continue;
			}
			instancedGameObjects.emplace_back(instance);
		}
	}

	std::sort(indicesOfOutlineModels.begin(), indicesOfOutlineModels.end(), [](UINT a, UINT b) { return a > b; });

	for (auto index : indicesOfOutlineModels)
	{
		std::swap(gameObjects[index], gameObjects.back());
		gameObjects.pop_back();
	}

	myForwardRenderer.Render(environmentlight, pointlights, maincamera, gameObjects);
	myForwardRenderer.InstancedRender(environmentlight, pointlights, maincamera, instancedGameObjects);
		
	for (auto modelToOutline : aScene.GetModelsToOutline()) {
		std::vector<CGameObject*> interimVector;
		if (modelToOutline) {
			pointlights.emplace_back(aScene.CullLights(modelToOutline));
			interimVector.emplace_back(modelToOutline);
			myRenderStateManager.SetDepthStencilState(CRenderStateManager::DepthStencilStates::DEPTHSTENCILSTATE_STENCILWRITE, 0xFF);

			myForwardRenderer.Render(environmentlight, pointlights, maincamera, interimVector);
			
			if (modelToOutline != aScene.GetPlayer()) {
				modelToOutline->GetComponent<CTransformComponent>()->SetOutlineScale();
			}

			myRenderStateManager.SetDepthStencilState(CRenderStateManager::DepthStencilStates::DEPTHSTENCILSTATE_STENCILMASK, 0xFF);

			if (modelToOutline != aScene.GetPlayer()) {
				myForwardRenderer.RenderOutline(maincamera, modelToOutline, CModelFactory::GetInstance()->GetOutlineModelSubset(), { 1.0f, 0.0f, 0.0f, 1.0f });
				modelToOutline->GetComponent<CTransformComponent>()->ResetScale();
			}
			else {
				myForwardRenderer.RenderOutline(maincamera, modelToOutline, CModelFactory::GetInstance()->GetOutlineModelSubset(), { 25.0f / 255.0f, 200.0f / 255.0f, 208.0f / 255.0f, 1.0f });
			}
		}
	}

	const std::vector<CLineInstance*>& lineInstances = aScene.CullLineInstances();
	const std::vector<SLineTime>& lines = aScene.CullLines();

	myForwardRenderer.RenderLines(maincamera, lines);
	myForwardRenderer.RenderLineInstances(maincamera, lineInstances);


	myRenderStateManager.SetBlendState(CRenderStateManager::BlendStates::BLENDSTATE_ALPHABLEND);
	myRenderStateManager.SetDepthStencilState(CRenderStateManager::DepthStencilStates::DEPTHSTENCILSTATE_ONLYREAD);

	myForwardRenderer.InstancedRender(environmentlight, pointlights, maincamera, instancedGameObjectsWithAlpha);

	myVFXRenderer.Render(maincamera, gameObjects);

	myParticleRenderer.Render(maincamera, gameObjects);

	//std::vector<CSpriteInstance*> sprites = myScene.CullSprites();
	//mySpriteRenderer.Render(sprites);

	//std::vector<CAnimatedUIElement*> animatedUIElements = myScene.CullAnimatedUI();
	//mySpriteRenderer.Render(animatedUIElements);

	myRenderStateManager.SetBlendState(CRenderStateManager::BlendStates::BLENDSTATE_DISABLE);
	myRenderStateManager.SetDepthStencilState(CRenderStateManager::DepthStencilStates::DEPTHSTENCILSTATE_DEFAULT);

	// Hope this works!
	myUseBloom ? RenderBloom() : RenderWithoutBloom();

	myRenderStateManager.SetBlendState(CRenderStateManager::BlendStates::BLENDSTATE_ALPHABLEND);
	myRenderStateManager.SetDepthStencilState(CRenderStateManager::DepthStencilStates::DEPTHSTENCILSTATE_ONLYREAD);

	std::vector<CSpriteInstance*> sprites = aScene.CullSprites();
	CMainSingleton::PopupTextService().EmplaceSprites(sprites);
	CMainSingleton::DialogueSystem().EmplaceSprites(sprites);
	mySpriteRenderer.Render(sprites);

	std::vector<CSpriteInstance*> animatedUIFrames;
	std::vector<CAnimatedUIElement*> animatedUIElements = aScene.CullAnimatedUI(animatedUIFrames);
	CEngine::GetInstance()->GetActiveScene().GetMainCamera()->EmplaceSprites(animatedUIFrames);
	mySpriteRenderer.Render(animatedUIElements);
	mySpriteRenderer.Render(animatedUIFrames);

	myRenderStateManager.SetBlendState(CRenderStateManager::BlendStates::BLENDSTATE_DISABLE);
	myRenderStateManager.SetDepthStencilState(CRenderStateManager::DepthStencilStates::DEPTHSTENCILSTATE_DEFAULT);

	std::vector<CTextInstance*> textsToRender = aScene.GetTexts();
	CMainSingleton::PopupTextService().EmplaceTexts(textsToRender);
	CMainSingleton::DialogueSystem().EmplaceTexts(textsToRender);
	myTextRenderer.Render(textsToRender);
}

void CRenderManager::RenderBloom()
{
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
}

void CRenderManager::RenderWithoutBloom()
{
	myBackbuffer.SetAsActiveTarget();
	myIntermediateTexture.SetAsResourceOnSlot(0);
	myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::FULLSCREENSHADER_VIGNETTE);
}
