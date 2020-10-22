#include "stdafx.h"
#include "FullscreenTexture.h"
#include "FullscreenTextureFactory.h"

CFullscreenTexture::CFullscreenTexture()
{
	myContext = nullptr;
	myTexture = nullptr;
	myShaderResource = nullptr;
	myViewport = nullptr;
}

CFullscreenTexture::~CFullscreenTexture()
{
	myContext = nullptr;
	myTexture = nullptr;
	myShaderResource = nullptr;
	myViewport = nullptr;
}

void CFullscreenTexture::ClearTexture(DirectX::SimpleMath::Vector4 aClearColor)
{
	myContext->ClearRenderTargetView(myRenderTarget, &aClearColor.x);
}

void CFullscreenTexture::ClearDepth(float /*aClearDepth*/, unsigned int /*aClearStencil*/)
{
	myContext->ClearDepthStencilView(myDepth, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void CFullscreenTexture::SetAsActiveTarget(CFullscreenTexture* aDepth)
{
	if (aDepth) 
	{
		myContext->OMSetRenderTargets(1, &myRenderTarget, aDepth->myDepth);
	}
	else
	{
		myContext->OMSetRenderTargets(1, &myRenderTarget, nullptr);
	}
	myContext->RSSetViewports(1, myViewport);
}

void CFullscreenTexture::SetAsResourceOnSlot(unsigned int aSlot)
{
	myContext->PSSetShaderResources(aSlot, 1, &myShaderResource);
}
