#include "stdafx.h"
#include "FullscreenTextureFactory.h"
#include "DirectXFramework.h"
#include "FullscreenTexture.h"

CFullscreenTextureFactory::CFullscreenTextureFactory()
{
	myFramework = nullptr;
}

CFullscreenTextureFactory::~CFullscreenTextureFactory()
{
	myFramework = nullptr;
}

bool CFullscreenTextureFactory::Init(CDirectXFramework* aFramework)
{
	myFramework = aFramework;
	if (!myFramework) {
		return false;
	}
	return true;
}

CFullscreenTexture CFullscreenTextureFactory::CreateTexture(DirectX::SimpleMath::Vector2 aSize, DXGI_FORMAT aFormat)
{
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

	ID3D11Texture2D* texture = nullptr;
	result = myFramework->GetDevice()->CreateTexture2D(&textureDesc, nullptr, &texture);
	if (FAILED(result)) {
		/*return*/;
	}

	CFullscreenTexture 	returnTexture = CreateTexture(texture);

	//returnTexture = CreateTexture(texture);

	ID3D11ShaderResourceView* shaderResource;
	//if (texture != NULL) {
		result = myFramework->GetDevice()->CreateShaderResourceView(texture, nullptr, &shaderResource);
		if (FAILED(result)) {
			/*return*/;
		}
	//}

	returnTexture.myShaderResource = shaderResource;
	return returnTexture;
}

CFullscreenTexture CFullscreenTextureFactory::CreateTexture(ID3D11Texture2D* aTexture)
{
	HRESULT result;

	ID3D11RenderTargetView* renderTarget;
	result = myFramework->GetDevice()->CreateRenderTargetView(aTexture, nullptr, &renderTarget);
	if (FAILED(result)) {
		/*return*/;
	}

	D3D11_VIEWPORT* viewport = nullptr;
	if (aTexture) 
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		aTexture->GetDesc(&textureDesc);
		viewport = new D3D11_VIEWPORT({ 0.0f, 0.0f, static_cast<float>(textureDesc.Width), static_cast<float>(textureDesc.Height), 0.0f, 1.0f });
	}

	CFullscreenTexture returnTexture;
	returnTexture.myContext = myFramework->GetContext();
	returnTexture.myTexture = aTexture;
	returnTexture.myRenderTarget = renderTarget;
	returnTexture.myViewport = viewport;
	return returnTexture;
}

CFullscreenTexture CFullscreenTextureFactory::CreateDepth(UINT aWidth, UINT aHeight, DXGI_FORMAT aFormat)
{
	HRESULT result;

	D3D11_TEXTURE2D_DESC textureDesc = { 0 };
	textureDesc.Width = static_cast<unsigned int>(aWidth);
	textureDesc.Height = static_cast<unsigned int>(aHeight);
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
	if (FAILED(result)) 
	{
		/*return*/;
	}

	ID3D11DepthStencilView* depth;
	//if (texture != NULL) {
		result = myFramework->GetDevice()->CreateDepthStencilView(texture, nullptr, &depth);
		if (FAILED(result))
		{
			/*return*/;
		}
	//}

	D3D11_VIEWPORT* viewport = new D3D11_VIEWPORT({ 0.0f, 0.0f, (float)aWidth, (float)aHeight, 0.0f, 1.0f });

	CFullscreenTexture returnDepth;
	returnDepth.myContext = myFramework->GetContext();
	returnDepth.myTexture = texture;
	returnDepth.myDepth = depth;
	returnDepth.myViewport = viewport;
	return returnDepth;
}
