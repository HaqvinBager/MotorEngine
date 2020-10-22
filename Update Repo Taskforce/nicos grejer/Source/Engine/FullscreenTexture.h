#pragma once
#include "SimpleMath.h"
#include <d3d11.h>

class CFullscreenTextureFactory;

class CFullscreenTexture
{
	friend CFullscreenTextureFactory;
public:
	CFullscreenTexture();
	~CFullscreenTexture();

	void ClearTexture(DirectX::SimpleMath::Vector4 aClearColor = DirectX::SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f));
	void ClearDepth(float aClearDepth = 1.0f, unsigned int aClearStencil = 0);
	void SetAsActiveTarget(CFullscreenTexture* aDepth = nullptr);
	void SetAsResourceOnSlot(unsigned int aSlot);

private:
	ID3D11DeviceContext* myContext;
	ID3D11Texture2D* myTexture;
	union 
	{
		ID3D11RenderTargetView* myRenderTarget;
		ID3D11DepthStencilView* myDepth;
	};
	ID3D11ShaderResourceView* myShaderResource;
	D3D11_VIEWPORT* myViewport;
};

