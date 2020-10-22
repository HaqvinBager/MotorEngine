#pragma once
#include "ForwardRenderer.h"
#include "RenderStateManager.h"
#include "FullscreenRenderer.h"
#include "ParticleRenderer.h"
#include "VFXRenderer.h"
#include "FullscreenTextureFactory.h"
#include "FullscreenTexture.h"

class CDirectXFramework;
class CScene;

class CRenderManager
{
public:
	CRenderManager(CScene& aScene);
	~CRenderManager();

	bool Init(CDirectXFramework* aFramework, CWindowHandler* aWindowHandler);

	void Render();

private:
	CScene& myScene;
	CRenderStateManager myRenderStateManager;
	CForwardRenderer myForwardRenderer;
	CFullscreenRenderer myFullscreenRenderer;
	CParticleRenderer myParticleRenderer;
	CVFXRenderer myVFXRenderer;

	CFullscreenTextureFactory myFullscreenTextureFactory;
	CFullscreenTexture myBackbuffer;
	CFullscreenTexture myIntermediateTexture;
	CFullscreenTexture myIntermediateDepth;
	CFullscreenTexture myLuminanceTexture;
	CFullscreenTexture myHalfsizeTexture;
	CFullscreenTexture myQuartersizeTexture;
	CFullscreenTexture myBlurTexture1;
	CFullscreenTexture myBlurTexture2;
	CFullscreenTexture myVignetteTexture;
};

