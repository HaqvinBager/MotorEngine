#include "DirectXFramework.h"
#include "WindowHandler.h"
#include <d3d11.h>

CDirectXFramework::CDirectXFramework()
{
	mySwapChain = nullptr;
	myDevice = nullptr;
	myContext = nullptr;
	myBackBuffer = nullptr;


}

CDirectXFramework::~CDirectXFramework()
{
	int a = 0;
	a += 2;
	BOOL bFullscreen;
	mySwapChain->GetFullscreenState(&bFullscreen, nullptr);
	// if fullscreen true
	if (bFullscreen != 0)
	{
		mySwapChain->SetFullscreenState(FALSE, NULL);
	}
}

void CDirectXFramework::BeginFrame(std::array<float, 4> aClearColor)
{
	myContext->ClearRenderTargetView(myBackBuffer.Get(), &aClearColor[0]);
	myContext->ClearDepthStencilView(myDepthBuffer.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void CDirectXFramework::EndFrame()
{
	mySwapChain->Present(1, 0);
}

bool CDirectXFramework::Init(CWindowHandler* aWindowHandler)
{
	if (!aWindowHandler)
	{
		return false;
	}

	HRESULT result;


	DXGI_SWAP_CHAIN_DESC swapchainDesc = {};
	swapchainDesc.BufferCount = 1;
	swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.OutputWindow = aWindowHandler->GetWindowHandle();
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.Windowed = true;
	ENGINE_HR_MESSAGE(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapchainDesc,
		&mySwapChain,
		&myDevice,
		nullptr,
		&myContext), "Failed to created Device and Swap Chain.");

	ID3D11Texture2D* backbufferTexture;
	ENGINE_HR_MESSAGE(mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backbufferTexture), "Failed to Get Buffer");
	ENGINE_HR_MESSAGE(myDevice->CreateRenderTargetView(backbufferTexture, nullptr, &myBackBuffer), "Failed to create RendertargetView");
	ENGINE_HR_MESSAGE(backbufferTexture->Release(), "Failed to release backbuffer");

	ID3D11Texture2D* depthBufferTexture;
	D3D11_TEXTURE2D_DESC depthBufferDescription = { 0 };
	depthBufferDescription.Width = static_cast<unsigned int>(aWindowHandler->GetWidth());
	depthBufferDescription.Height = static_cast<unsigned int>(aWindowHandler->GetHeight());
	depthBufferDescription.ArraySize = 1;
	depthBufferDescription.Format = DXGI_FORMAT_D32_FLOAT;
	depthBufferDescription.SampleDesc.Count = 1;
	depthBufferDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	result = myDevice->CreateTexture2D(&depthBufferDescription, nullptr, &depthBufferTexture);
	if (FAILED(result))
		return false;

	result = myDevice->CreateDepthStencilView(depthBufferTexture, nullptr, &myDepthBuffer);
	if (FAILED(result))
		return false;

	myContext->OMSetRenderTargets(1, myBackBuffer.GetAddressOf(), myDepthBuffer.Get());
	D3D11_VIEWPORT viewport = { 0 };

	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(aWindowHandler->GetWidth());
	viewport.Height = static_cast<float>(aWindowHandler->GetHeight());
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	myContext->RSSetViewports(1, &viewport);

	return true;
}

bool CDirectXFramework::ResizeBackBufferTexture()
{
	// Also create new rendertargetview?

	HRESULT result;
	ID3D11Texture2D* backbufferTexture;
	result = mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backbufferTexture);
	if (FAILED(result))
	{
		return false;
	}
	return true;
}

ID3D11Device* CDirectXFramework::GetDevice() const {
	return myDevice.Get();
}

ID3D11DeviceContext* CDirectXFramework::GetContext() const {
	return myContext.Get();
}
