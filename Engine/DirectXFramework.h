#pragma once
#include "WinInclude.h"
#include <array>
#include <wrl.h>

class CWindowHandler;
struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;

class CDirectXFramework {
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
	Microsoft::WRL::ComPtr<IDXGISwapChain> mySwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device> myDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> myContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> myBackBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> myDepthBuffer;
};