#pragma once
//#include <d3d11.h>
#include <array>

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

class WindowHandler;

class DirectX11Framework
{
public:
	DirectX11Framework();
	~DirectX11Framework();

	bool Init(WindowHandler* aWindowHandler);
	void BeginFrame(std::array<float, 4> aClearColor);
	void EndFrame();

	IDXGISwapChain*			GetSwapChain();
	ID3D11Device*			GetDevice();
	ID3D11DeviceContext*	GetDeviceContext();
	ID3D11RenderTargetView* GetBackBuffer();

private:
	IDXGISwapChain*			mySwapChain;
	ID3D11Device*			myDevice;
	ID3D11DeviceContext*	myDeviceContext;
	ID3D11RenderTargetView* myBackBuffer;
	ID3D11DepthStencilView* myDepthBuffer;
};

