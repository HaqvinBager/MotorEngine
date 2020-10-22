#include "stdafx.h"
#include "DirectX11Framework.h"

//----- TEST -----
//#include "FullscreenTextureFactory.h"
//----- TEST -----

CDirectX11Framework::CDirectX11Framework() {
    mySwapChain = nullptr;
    myDevice = nullptr;
    myContext = nullptr;
    myBackbufferTexture = nullptr;
}

CDirectX11Framework::~CDirectX11Framework() {
    mySwapChain = nullptr;
    myDevice = nullptr;
    myContext = nullptr;
    myBackbufferTexture = nullptr;
}

bool CDirectX11Framework::Init(CWindowHandler* aWindowHandler) {
    HRESULT result;
    DXGI_SWAP_CHAIN_DESC swapchainDescription = {};
    swapchainDescription.BufferCount = 1;
    swapchainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapchainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchainDescription.OutputWindow = aWindowHandler->GetWindowHandle();
    swapchainDescription.SampleDesc.Count = 1;
    swapchainDescription.Windowed = true;

    result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG, nullptr, 0, D3D11_SDK_VERSION,
        &swapchainDescription, &mySwapChain, &myDevice, nullptr, &myContext);

    if (FAILED(result)) {
        return false;
    }

    //ID3D11Texture2D* myBackbufferTexture;
    result = mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&myBackbufferTexture);

    if (FAILED(result)) {
        return false;
    }

    //----- TEST -----
    //CFullscreenTextureFactory* fullscreenTextureFactory = new CFullscreenTextureFactory();
    //fullscreenTextureFactory->Init(this);

    //myBackBuffer = fullscreenTextureFactory->CreateTexture(backbufferTexture);

    //myDepthBuffer = fullscreenTextureFactory->CreateDepth(
    //    { (float)aWindowHandler->GetWidth(), (float)aWindowHandler->GetHeight() }, DXGI_FORMAT_D32_FLOAT);

    //myBackBuffer.SetAsActiveTarget(&myDepthBuffer);
    //----- TEST -----

    result = myDevice->CreateRenderTargetView(myBackbufferTexture, nullptr, &myBackBuffer);

    if (FAILED(result)) {
        return false;
    }

    result = myBackbufferTexture->Release();

    if (FAILED(result)) {
        return false;
    }

    //TEMP
    ID3D11Texture2D* depthbufferTexture;
    D3D11_TEXTURE2D_DESC depthbufferDescription = { 0 };
    depthbufferDescription.Width = static_cast<unsigned int>(aWindowHandler->GetWidth());
    depthbufferDescription.Height = static_cast<unsigned int>(aWindowHandler->GetHeight());
    depthbufferDescription.ArraySize = 1;
    depthbufferDescription.Format = DXGI_FORMAT_D32_FLOAT;
    depthbufferDescription.SampleDesc.Count = 1;
    depthbufferDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    result = myDevice->CreateTexture2D(&depthbufferDescription, nullptr, &depthbufferTexture);

    if (FAILED(result)) {
        return false;
    }

    result = myDevice->CreateDepthStencilView(depthbufferTexture, nullptr, &myDepthBuffer);

    if (FAILED(result)) {
        return false;
    }

    myContext-> OMSetRenderTargets(1, &myBackBuffer, myDepthBuffer);

    D3D11_VIEWPORT viewport = { 0 };
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = static_cast<float>(aWindowHandler->GetWidth());
    viewport.Height = static_cast<float>(aWindowHandler->GetHeight());
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    myContext->RSSetViewports(1, &viewport);
    //TEMP
    
    return true;
}

void CDirectX11Framework::BeginFrame(std::array<float, 4> aClearColor) {
    myContext->ClearRenderTargetView(myBackBuffer, &aClearColor[0]);
    myContext->ClearDepthStencilView(myDepthBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    //----- TEST -----
    //myBackBuffer.ClearTexture();
    //myDepthBuffer.ClearDepth();
    //----- TEST -----
}

void CDirectX11Framework::EndFrame() {
    mySwapChain->Present(1, 0);
}

ID3D11DeviceContext* CDirectX11Framework::GetContext() const {
    return myContext;
}

ID3D11Device* CDirectX11Framework::GetDevice() const {
    return myDevice;
}

ID3D11Texture2D* CDirectX11Framework::GetBackbufferTexture() const {
    return myBackbufferTexture;
}
