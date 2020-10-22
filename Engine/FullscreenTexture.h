#pragma once

struct ID3D11DeviceContext;
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11ShaderResourceView;
struct D3D11_VIEWPORT;
class CFullscreenTextureFactory;

namespace SM = DirectX::SimpleMath;

class CFullscreenTexture {
public:
	CFullscreenTexture();
	~CFullscreenTexture();
	void ClearTexture(SM::Vector4 aClearColor = { 0.0f, 0.0f, 0.0f, 0.0f });
	void ClearDepth(float aClearDepth = 1.0f, unsigned int aClearStencil = 0);
	void SetAsActiveTarget(CFullscreenTexture* aDepth = nullptr);
	void SetAsResourceOnSlot(unsigned int aSlot);

	friend CFullscreenTextureFactory;

private:
	ID3D11DeviceContext* myContext;
	ID3D11Texture2D* myTexture;

	union {
		ID3D11RenderTargetView* myRenderTarget;
		ID3D11DepthStencilView* myDepth;
	};

	ID3D11ShaderResourceView* myShaderResource;
	D3D11_VIEWPORT* myViewport;
};

