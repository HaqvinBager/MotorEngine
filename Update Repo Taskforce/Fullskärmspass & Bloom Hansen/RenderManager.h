#pragma once
#include "ForwardRenderer.h"
#include "FullscreenRenderer.h"
#include "FullscreenTexture.h"
#include "FullscreenTextureFactory.h"

class CDirectX11Framework;
class CWindowHandler;
class CScene;

class CRenderManager {
public:
	CRenderManager();
	~CRenderManager();
	bool Init(CDirectX11Framework* aFramework, CWindowHandler* aWindowHandler);
	void Render();

private:
	CScene& myScene;
	CForwardRenderer myForwardRenderer;
	CFullscreenRenderer myFullscreenRenderer;

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

