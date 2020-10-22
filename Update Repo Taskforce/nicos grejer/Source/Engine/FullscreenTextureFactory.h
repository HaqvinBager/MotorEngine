#pragma once
#include "SimpleMath.h" 

class CDirectXFramework;
class CFullscreenTexture;

class CFullscreenTextureFactory
{
public:
	CFullscreenTextureFactory();
	~CFullscreenTextureFactory();

	bool Init(CDirectXFramework* aFramework);

	CFullscreenTexture CreateTexture(DirectX::SimpleMath::Vector2 aSize, DXGI_FORMAT aFormat);
	CFullscreenTexture CreateTexture(ID3D11Texture2D* aTexture);
	CFullscreenTexture CreateDepth(UINT aWidth, UINT aHeight, DXGI_FORMAT aFormat);

private:
	CDirectXFramework* myFramework;
};

