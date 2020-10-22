#pragma once
#include "ForwardRenderer.h"
#include "FullscreenRenderer.h"
#include "FullscreenTexture.h"
#include "FullscreenTextureFactory.h"
#include "ParticleRenderer.h"
#include "RenderStateManager.h"

class CDirectXFramework;
class CWindowHandler;
class CScene;

class CRenderManager {
public:
	CRenderManager();
	~CRenderManager();
	bool Init(CDirectXFramework* aFramework, CWindowHandler* aWindowHandler);
	void Render();

private:
	CScene& myScene;
	CRenderStateManager myRenderStateManager;
	CForwardRenderer myForwardRenderer;
	CFullscreenRenderer myFullscreenRenderer;
	CParticleRenderer myParticleRenderer;
	

	CFullscreenTextureFactory myFullscreenTextureFactory;
	CFullscreenTexture myBackbuffer;
	CFullscreenTexture myIntermediateTexture;
	CFullscreenTexture myIntermediateDepth;
	CFullscreenTexture myLuminanceTexture;
	CFullscreenTexture myHalfSizeTexture;
	CFullscreenTexture myQuaterSizeTexture;
	CFullscreenTexture myBlurTexture1;
	CFullscreenTexture myBlurTexture2;
};

