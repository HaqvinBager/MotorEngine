#pragma once
#include "ForwardRenderer.h"
#include "FullscreenRenderer.h"
#include "FullscreenTexture.h"
#include "FullscreenTextureFactory.h"
#include "ParticleRenderer.h"
#include "VFXRenderer.h"
#include "RenderStateManager.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"

class CDirectXFramework;
class CWindowHandler;
class CScene;

class CRenderManager {
public:
	CRenderManager();
	~CRenderManager();
	bool Init(CDirectXFramework* aFramework, CWindowHandler* aWindowHandler);
	void Render(CScene& aScene);

private:
	void RenderBloom();
	void RenderWithoutBloom();

private:
	//CScene& myScene;
	CRenderStateManager myRenderStateManager;
	CForwardRenderer myForwardRenderer;
	CFullscreenRenderer myFullscreenRenderer;
	CParticleRenderer myParticleRenderer;
	CVFXRenderer myVFXRenderer;
	CSpriteRenderer mySpriteRenderer;
	CTextRenderer myTextRenderer;

	CFullscreenTextureFactory myFullscreenTextureFactory;
	CFullscreenTexture myBackbuffer;
	CFullscreenTexture myIntermediateTexture;
	CFullscreenTexture myIntermediateDepth;
	CFullscreenTexture myLuminanceTexture;
	CFullscreenTexture myHalfSizeTexture;
	CFullscreenTexture myQuaterSizeTexture;
	CFullscreenTexture myBlurTexture1;
	CFullscreenTexture myBlurTexture2;
	CFullscreenTexture myVignetteTexture;

	bool myUseBloom;
};