#include "pch.h"
#include "DirectX11Framework.hpp"

#include "WindowHandler.hpp"

//#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")

DirectX11Framework::DirectX11Framework()
{
	mySwapChain			= nullptr;
	myDevice			= nullptr;
	myDeviceContext		= nullptr;
	myBackBuffer		= nullptr;
	myDepthBuffer		= nullptr;
}

DirectX11Framework::~DirectX11Framework()
{
	mySwapChain			= nullptr;
	myDevice			= nullptr;
	myDeviceContext		= nullptr;
	myBackBuffer		= nullptr;
	myDepthBuffer		= nullptr;
}

bool DirectX11Framework::Init(WindowHandler* aWindowHandler)
{
	HRESULT hResult;
	DXGI_SWAP_CHAIN_DESC swapChainDescription = {};
	swapChainDescription.BufferCount		= 1;
	swapChainDescription.BufferDesc.Format	= DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDescription.BufferUsage		= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDescription.OutputWindow		= aWindowHandler->GetWindowhandle();
	swapChainDescription.SampleDesc.Count	= 1;
	swapChainDescription.SampleDesc.Quality	= 0;
	swapChainDescription.Windowed			= true;
	//swapChainDescription.SwapEffect			= DXGI_SWAP_EFFECT_DISCARD;

	hResult = D3D11CreateDeviceAndSwapChain(  /*IDXGIAdapter*/nullptr, D3D_DRIVER_TYPE_HARDWARE
											, /*HMODULE Software*/nullptr, /*UINT Flags*/D3D11_CREATE_DEVICE_DEBUG, /*D3D_FEATURE_LEVEL*/nullptr
											, /*UINT FeatureLevel*/0, /*UINT SDKVersion*/D3D11_SDK_VERSION
											, &swapChainDescription, &mySwapChain, &myDevice, /*D3D_FEATURE_LEVEL* */nullptr, &myDeviceContext);
	swapChainDescription.SampleDesc.Count;
	
	if(FAILED(hResult))
	{
		// ?
		return false;
	}

	ID3D11Texture2D* backBufferTexture = nullptr;
	hResult = mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTexture);
	if(FAILED(hResult))
	{
		return false;
	}

	hResult = myDevice->CreateRenderTargetView(backBufferTexture, nullptr, &myBackBuffer);
	if(FAILED(hResult))
	{
		return false;
	}
	hResult = backBufferTexture->Release();
	if(FAILED(hResult))
	{
		return false;
	}

	//Temporary
	// Depth Buffer
	ID3D11Texture2D* depthBufferTexture = nullptr;
	D3D11_TEXTURE2D_DESC depthBufferDescription = {0};
	depthBufferDescription.Width				= static_cast<unsigned int>(aWindowHandler->GetWidth());
	depthBufferDescription.Height				= static_cast<unsigned int>(aWindowHandler->GetHeight());
	depthBufferDescription.ArraySize			= 1;
	depthBufferDescription.Format				= DXGI_FORMAT_D32_FLOAT;
	depthBufferDescription.SampleDesc.Count		= 1;
	depthBufferDescription.SampleDesc.Quality	= 0;
	//depthBufferDescription.Usage				= D3D11_USAGE_DEFAULT;//? IDK what this does, deperate attempt to solve ERROR #388
	depthBufferDescription.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	depthBufferDescription.MipLevels			= 1;
	hResult = myDevice->CreateTexture2D(&depthBufferDescription, nullptr, &depthBufferTexture);
	if (FAILED(hResult))
	{
		return false;
	}

	hResult = myDevice->CreateDepthStencilView(depthBufferTexture, nullptr/*&depthStencilDescription*/, &myDepthBuffer);
	if (FAILED(hResult))
	{
		return false;
	}

	// Unnecessary with LH coord sys...
		//D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		//// Depth test parameters
		//depthStencilDesc.DepthEnable	= true;
		//depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		//depthStencilDesc.DepthFunc		= D3D11_COMPARISON_LESS;
		//// Stencil test parameters
		//depthStencilDesc.StencilEnable		= true;
		//depthStencilDesc.StencilReadMask	= 0xFF;
		//depthStencilDesc.StencilWriteMask	= 0xFF;
		//// Stencil operations if pixel is front-facing
		//depthStencilDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
		//depthStencilDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_INCR;
		//depthStencilDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
		//depthStencilDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;
		//// Stencil operations if pixel is back-facing
		//depthStencilDesc.BackFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
		//depthStencilDesc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_DECR;
		//depthStencilDesc.BackFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
		//depthStencilDesc.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;
		//// Create depth stencil state
		//ID3D11DepthStencilState* depthStencilState = nullptr;
		//myDevice->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
	// !Unnecessary with LH coord sys

	// !Depth Buffer
	myDeviceContext->OMSetRenderTargets(1, &myBackBuffer, myDepthBuffer);// If ERROR #388 Graphics Debugger crashes here
	//Unnecesary	//myDeviceContext->OMSetDepthStencilState(depthStencilState, 1);
	// Viewport
	D3D11_VIEWPORT viewport = {0};
	viewport.TopLeftX	= 0.0f;
	viewport.TopLeftY	= 0.0f;
	viewport.Width		= static_cast<float>(aWindowHandler->GetWidth());
	viewport.Height		= static_cast<float>(aWindowHandler->GetHeight());
	viewport.MinDepth	= 0.0f;
	viewport.MaxDepth	= 1.0f;
	myDeviceContext->RSSetViewports(1, &viewport);
	// !Viewport
	//!Temporary

	return true;
}

void DirectX11Framework::BeginFrame(std::array<float, 4> aClearColor)
{
	myDeviceContext->ClearRenderTargetView(myBackBuffer, &aClearColor[0]);
	myDeviceContext->ClearDepthStencilView(myDepthBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DirectX11Framework::EndFrame()
{
	mySwapChain->Present(1, 0);
}

IDXGISwapChain* DirectX11Framework::GetSwapChain()
{
	return mySwapChain;
}

ID3D11Device* DirectX11Framework::GetDevice()
{
	return myDevice;
}

ID3D11DeviceContext* DirectX11Framework::GetDeviceContext()
{
	return myDeviceContext;
}

ID3D11RenderTargetView* DirectX11Framework::GetBackBuffer()
{
	return myBackBuffer;
}
