#include "stdafx.h"
#include "FullscreenTextureFactory.h"
#include "FullscreenTexture.h"
#include <d3d11.h>
#include "DirectX11Framework.h"

CFullscreenTextureFactory::CFullscreenTextureFactory() : myFramework(nullptr) {
}

CFullscreenTextureFactory::~CFullscreenTextureFactory() {
}

bool CFullscreenTextureFactory::Init(CDirectX11Framework* aFramework) {
	myFramework = aFramework;
	return true;
}

CFullscreenTexture CFullscreenTextureFactory::CreateTexture(SM::Vector2 aSize, DXGI_FORMAT aFormat) {
	HRESULT result;

	D3D11_TEXTURE2D_DESC textureDesc = { 0 };
	textureDesc.Width = static_cast<unsigned int>(aSize.x);
	textureDesc.Height = static_cast<unsigned int>(aSize.y);
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = aFormat;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	ID3D11Texture2D* texture;
	result = myFramework->GetDevice()->CreateTexture2D(&textureDesc, nullptr, &texture);
	if (FAILED(result)) {
		//return;
	}

	CFullscreenTexture returnTexture;
	returnTexture = CreateTexture(texture);

	ID3D11ShaderResourceView* shaderResource;
	result = myFramework->GetDevice()->CreateShaderResourceView(texture, nullptr, &shaderResource);
	if (FAILED(result)) {
		//return;
	}

	returnTexture.myShaderResource = shaderResource;
	return returnTexture;
}

CFullscreenTexture CFullscreenTextureFactory::CreateTexture(ID3D11Texture2D* aTexture) {
	HRESULT result;

	ID3D11RenderTargetView* renderTarget;
	result = myFramework->GetDevice()->CreateRenderTargetView(aTexture, nullptr, &renderTarget);
	if (FAILED(result)) {
		//return;
	}

	D3D11_VIEWPORT* viewport = nullptr;
	if (aTexture) {
		D3D11_TEXTURE2D_DESC textureDescription;
		aTexture->GetDesc(&textureDescription);
		viewport = new D3D11_VIEWPORT({ 0.0f, 0.0f, static_cast<float>(textureDescription.Width), static_cast<float>(textureDescription.Height), 0.0f, 1.0f });
	}

	CFullscreenTexture returnTexture;
	returnTexture.myContext = myFramework->GetContext();
	returnTexture.myTexture = aTexture;
	returnTexture.myRenderTarget = renderTarget;
	returnTexture.myViewport = viewport;
	return returnTexture;
}

CFullscreenTexture CFullscreenTextureFactory::CreateDepth(SM::Vector2 aSize, DXGI_FORMAT aFormat) {
	HRESULT result;

	D3D11_TEXTURE2D_DESC textureDesc = { 0 };
	textureDesc.Width = static_cast<unsigned int>(aSize.x);
	textureDesc.Height = static_cast<unsigned int>(aSize.y);
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = aFormat;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	ID3D11Texture2D* texture;
	result = myFramework->GetDevice()->CreateTexture2D(&textureDesc, nullptr, &texture);
	if (FAILED(result)) {
		//return;
	}

	ID3D11DepthStencilView* depth;
	result = myFramework->GetDevice()->CreateDepthStencilView(texture, nullptr, &depth);
	if (FAILED(result)) {
		//return;
	}

	D3D11_VIEWPORT* viewport = new D3D11_VIEWPORT({ 0.0f, 0.0f, aSize.x, aSize.y, 0.0f, 1.0f });

	CFullscreenTexture returnDepth;
	returnDepth.myContext = myFramework->GetContext();
	returnDepth.myTexture = texture;
	returnDepth.myDepth = depth;
	returnDepth.myViewport = viewport;
	return returnDepth;
}
