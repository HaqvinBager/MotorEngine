#pragma once
#include "..\DirectXTK\Inc\SimpleMath.h"

struct ID3D11Texture2D;
class CDirectX11Framework;
class CFullscreenTexture;

namespace SM = DirectX::SimpleMath;

class CFullscreenTextureFactory {
public:
	CFullscreenTextureFactory();
	~CFullscreenTextureFactory();
	bool Init(CDirectX11Framework* aFramework);
	CFullscreenTexture CreateTexture(SM::Vector2 aSize, DXGI_FORMAT aFormat);
	CFullscreenTexture CreateTexture(ID3D11Texture2D* aTexture);
	CFullscreenTexture CreateDepth(SM::Vector2 aSize, DXGI_FORMAT aFormat);

private:
	CDirectX11Framework* myFramework;
};

