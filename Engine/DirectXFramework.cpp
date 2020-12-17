#include "DirectXFramework.h"
#include "WindowHandler.h"
#include <d3d11.h>

CDirectXFramework::CDirectXFramework()
{
	mySwapChain = nullptr;
	myDevice = nullptr;
	myContext = nullptr;
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

void CDirectXFramework::EndFrame()
{
	mySwapChain->Present(0, 0);
}

bool CDirectXFramework::Init(CWindowHandler* aWindowHandler)
{
	if (!aWindowHandler)
	{
		return false;
	}

#if _DEBUG
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
#else
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
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapchainDesc,
		&mySwapChain,
		&myDevice,
		nullptr,
		&myContext), "Failed to created Device and Swap Chain.");
#endif

	return true;
}

void CDirectXFramework::ToggleFullscreenState(bool aSetFullscreen)
{
	mySwapChain->SetFullscreenState(aSetFullscreen, NULL);
}

bool CDirectXFramework::ResizeBackBufferTexture()
{

	//HRESULT result;
	//ID3D11Texture2D* backbufferTexture;
	//result = mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backbufferTexture);
	//if (FAILED(result))
	//{
	//	return false;
	//}
	//return true;

	//ENGINE_HR_BOOL_MESSAGE(mySwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0), "Could not resize buffers!");

	//ID3D11Texture2D* pBuffer = nullptr;
	//ENGINE_HR_BOOL_MESSAGE(mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer), "Could not resize buffers!");

	//if (!pBuffer)
	//	return false;

	//ENGINE_HR_BOOL_MESSAGE(myDevice->CreateRenderTargetView(pBuffer, NULL, &myBackBuffer), "Could not resize buffers!");

	//pBuffer->Release();
	return true;
}

ID3D11Device* CDirectXFramework::GetDevice() const {
	return myDevice.Get();
}

ID3D11DeviceContext* CDirectXFramework::GetContext() const {
	return myContext.Get();
}

ID3D11Texture2D* CDirectXFramework::GetBackbufferTexture() const {
	ID3D11Texture2D* backbufferTexture;
	ENGINE_HR_MESSAGE(mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backbufferTexture), "Failed to Get Buffer");
	return backbufferTexture;
}
