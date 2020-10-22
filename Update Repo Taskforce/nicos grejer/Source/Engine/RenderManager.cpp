#include "stdafx.h"
#include "RenderManager.h"
#include "DirectXFramework.h"
#include "Scene.h"
#include "ModelFactory.h"
#include "ModelInstance.h"

CRenderManager::CRenderManager(CScene& aScene) : myScene(aScene)
{
}

CRenderManager::~CRenderManager()
{
}

bool CRenderManager::Init(CDirectXFramework* aFramework, CWindowHandler* aWindowHandler)
{
    if (!myForwardRenderer.Init(aFramework)) {
        return false;
    }

    if (!myRenderStateManager.Init(aFramework)) {
        return false;
    }

    if (!myFullscreenRenderer.Init(aFramework)) {
        return false;
    }

    if (!myFullscreenTextureFactory.Init(aFramework)) {
        return false;
    }

    if (!myParticleRenderer.Init(aFramework)) {
        return false;
    }

    if (!myVFXRenderer.Init(aFramework)) {
        return false;
    }

    ID3D11Texture2D* backbufferTexture = aFramework->GetBackbufferTexture();
    if (!backbufferTexture) {
        return false;
    }

    myBackbuffer =          myFullscreenTextureFactory.CreateTexture(backbufferTexture);
    myIntermediateTexture = myFullscreenTextureFactory.CreateTexture({ (float)aWindowHandler->GetWidth(), (float)aWindowHandler->GetHeight() }, DXGI_FORMAT_R8G8B8A8_UNORM);
    myLuminanceTexture =    myFullscreenTextureFactory.CreateTexture({ (float)aWindowHandler->GetWidth(), (float)aWindowHandler->GetHeight() }, DXGI_FORMAT_R8G8B8A8_UNORM);
    myHalfsizeTexture =     myFullscreenTextureFactory.CreateTexture({ (float)aWindowHandler->GetWidth() / 2.0f, (float)aWindowHandler->GetHeight() / 2.0f }, DXGI_FORMAT_R8G8B8A8_UNORM);
    myQuartersizeTexture =  myFullscreenTextureFactory.CreateTexture({ (float)aWindowHandler->GetWidth() / 4.0f, (float)aWindowHandler->GetHeight() / 4.0f }, DXGI_FORMAT_R8G8B8A8_UNORM);
    myBlurTexture1 =        myFullscreenTextureFactory.CreateTexture({ (float)aWindowHandler->GetWidth(), (float)aWindowHandler->GetHeight() }, DXGI_FORMAT_R8G8B8A8_UNORM);
    myBlurTexture2 =        myFullscreenTextureFactory.CreateTexture({ (float)aWindowHandler->GetWidth(), (float)aWindowHandler->GetHeight() }, DXGI_FORMAT_R8G8B8A8_UNORM);
    myVignetteTexture =     myFullscreenTextureFactory.CreateTexture({ (float)aWindowHandler->GetWidth(), (float)aWindowHandler->GetHeight() }, DXGI_FORMAT_R8G8B8A8_UNORM);

    myIntermediateDepth =   myFullscreenTextureFactory.CreateDepth(aWindowHandler->GetWidth(), aWindowHandler->GetHeight(), DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT);
    return true;
}

void CRenderManager::Render()
{
    myRenderStateManager.SetAllDefault();
    myBackbuffer.ClearTexture({0.5f, 0.5f, 0.5f, 1.0f});
    myIntermediateTexture.ClearTexture({ 0.5f, 0.5f, 0.5f, 1.0f });
    myIntermediateDepth.ClearDepth();

    myIntermediateTexture.SetAsActiveTarget(&myIntermediateDepth);

    CEnvironmentLight* environmentLight = myScene.GetEnvironmentLight();
    CCamera* mainCamera = myScene.GetMainCamera();
    std::vector<CModelInstance*> modelsToRender = myScene.CullModels(mainCamera);
    std::vector<std::pair<unsigned int, std::array<CPointLight*, 8>>> pointLights;
    for (CModelInstance* instance : modelsToRender) 
    {
        pointLights.emplace_back(myScene.CullLights(instance));
    }
    myForwardRenderer.Render(environmentLight, pointLights, mainCamera, modelsToRender);

    CModelInstance* modelToOutline = myScene.GetModelToOutline(); 
    pointLights.emplace_back(myScene.CullLights(modelToOutline));
    std::vector<CModelInstance*> interimVector;
    if (modelToOutline) {
        interimVector.emplace_back(modelToOutline);
        myRenderStateManager.SetDepthStencilState(CRenderStateManager::DepthStencilStates::DEPTHSTENCILSTATE_STENCILWRITE, 0xFF);
        myForwardRenderer.Render(environmentLight, pointLights, mainCamera, interimVector);

        modelToOutline->SetOutlineScale();
        myRenderStateManager.SetDepthStencilState(CRenderStateManager::DepthStencilStates::DEPTHSTENCILSTATE_STENCILMASK, 0xFF);
        myForwardRenderer.RenderOutline(mainCamera, modelToOutline, CModelFactory::GetInstance()->GetOutlineModelSubset());
        modelToOutline->ResetScale();
    }

    myRenderStateManager.SetBlendState(CRenderStateManager::BlendStates::BLENDSTATE_ALPHABLEND);
    myRenderStateManager.SetDepthStencilState(CRenderStateManager::DepthStencilStates::DEPTHSTENCILSTATE_ONLYREAD);
    std::vector<CVFXInstance*> vfxInstancesToRender = myScene.CullVFX(mainCamera);
    myVFXRenderer.Render(mainCamera, vfxInstancesToRender);
    std::vector<CParticleInstance*> particlesToRender = myScene.CullParticles(mainCamera);
    myParticleRenderer.Render(mainCamera, particlesToRender);
    myRenderStateManager.SetBlendState(CRenderStateManager::BlendStates::BLENDSTATE_DISABLE);
    myRenderStateManager.SetDepthStencilState(CRenderStateManager::DepthStencilStates::DEPTHSTENCILSTATE_DEFAULT);

    myLuminanceTexture.SetAsActiveTarget();
    myIntermediateTexture.SetAsResourceOnSlot(0);
    myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::FULLSCREENSHADER_LUMINANCE);

    myHalfsizeTexture.SetAsActiveTarget();
    myLuminanceTexture.SetAsResourceOnSlot(0);
    myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::FULLSCREENSHADER_COPY);

    myQuartersizeTexture.SetAsActiveTarget();
    myHalfsizeTexture.SetAsResourceOnSlot(0);
    myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::FULLSCREENSHADER_COPY);

    myBlurTexture1.SetAsActiveTarget();
    myQuartersizeTexture.SetAsResourceOnSlot(0);
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

    myQuartersizeTexture.SetAsActiveTarget();
    myBlurTexture1.SetAsResourceOnSlot(0);
    myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::FULLSCREENSHADER_COPY);

    myHalfsizeTexture.SetAsActiveTarget();
    myQuartersizeTexture.SetAsResourceOnSlot(0);
    myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::FULLSCREENSHADER_COPY);

    myVignetteTexture.SetAsActiveTarget();
    myIntermediateTexture.SetAsResourceOnSlot(0);
    myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::FULLSCREENSHADER_VIGNETTE);

    myBackbuffer.SetAsActiveTarget();
    myVignetteTexture.SetAsResourceOnSlot(0);
    myHalfsizeTexture.SetAsResourceOnSlot(1);
    myFullscreenRenderer.Render(CFullscreenRenderer::FullscreenShader::FULLSCREENSHADER_BLOOM);
}
