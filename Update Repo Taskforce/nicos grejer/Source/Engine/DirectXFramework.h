#pragma once
#include <array>
//#include <Windows.h>
#include "WinInclude.h"
//#include <d3d11.h>
#include <wrl.h>

#include "FullscreenTexture.h"

class CWindowHandler;
struct ID3D11Device;
struct ID3D11DeviceContext;

//class CFullscreenTexture;

class CDirectXFramework
{
public:
	CDirectXFramework();
	~CDirectXFramework();

	void BeginFrame(std::array<float, 4> aClearColor);
	void EndFrame();

	bool Init(CWindowHandler* aWindowHandler);

	bool ResizeBackBufferTexture();

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetContext() const;
	ID3D11Texture2D* GetBackbufferTexture() const;

private:
	Microsoft::WRL::ComPtr<struct IDXGISwapChain> mySwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device> myDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> myContext;
	Microsoft::WRL::ComPtr<struct ID3D11RenderTargetView> myBackBuffer;
	Microsoft::WRL::ComPtr<struct ID3D11DepthStencilView> myDepthBuffer;

	CFullscreenTexture myBackBufferTest;
	CFullscreenTexture myDepthBufferTest;
};

