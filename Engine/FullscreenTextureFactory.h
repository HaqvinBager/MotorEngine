#pragma once

struct ID3D11Texture2D;
class CDirectXFramework;
class CFullscreenTexture;

namespace SM = DirectX::SimpleMath;

class CFullscreenTextureFactory {
public:
	CFullscreenTextureFactory();
	~CFullscreenTextureFactory();
	bool Init(CDirectXFramework* aFramework);
	CFullscreenTexture CreateTexture(SM::Vector2 aSize, DXGI_FORMAT aFormat);
	CFullscreenTexture CreateTexture(ID3D11Texture2D* aTexture);
	CFullscreenTexture CreateDepth(SM::Vector2 aSize, DXGI_FORMAT aFormat);

private:
	CDirectXFramework* myFramework;
};

