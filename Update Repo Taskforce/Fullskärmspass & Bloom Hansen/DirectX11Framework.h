#pragma once
#include <d3d11.h>
#include <array>
#include "WindowHandler.h"

//----- TEST -----
//#include "FullscreenTexture.h"
//----- TEST -----

class CDirectX11Framework {
public:
	CDirectX11Framework();
	~CDirectX11Framework();
	bool Init(CWindowHandler* aWindowHandler);
	void BeginFrame(std::array<float, 4> aClearColor);
	void EndFrame();

	ID3D11DeviceContext* GetContext() const;
	ID3D11Device* GetDevice() const;
	ID3D11Texture2D* GetBackbufferTexture() const;

	IDXGISwapChain* mySwapChain;
	ID3D11Device* myDevice;
	ID3D11DeviceContext* myContext;
	ID3D11RenderTargetView* myBackBuffer;
	ID3D11DepthStencilView* myDepthBuffer;
	ID3D11Texture2D* myBackbufferTexture;
	//----- TEST -----
	//CFullscreenTexture myBackBuffer;
	//CFullscreenTexture myDepthBuffer;
	//----- TEST -----
};