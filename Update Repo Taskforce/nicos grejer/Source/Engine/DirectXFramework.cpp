#include "DirectXFramework.h"
#include "WindowHandler.h"
#include <d3d11.h>
#include "FullscreenTextureFactory.h"

CDirectXFramework::CDirectXFramework()
{
    mySwapChain = nullptr;
    myDevice = nullptr;
    myContext = nullptr;
    myBackBuffer = nullptr;
}

CDirectXFramework::~CDirectXFramework()
{
    BOOL bFullscreen;
    mySwapChain->GetFullscreenState(&bFullscreen, nullptr);
    // if fullscreen true
    if (bFullscreen != 0)
    {
        mySwapChain->SetFullscreenState(FALSE, NULL);
    }

    // Using ComPtr
    //mySwapChain = nullptr;
    //myDevice = nullptr;
    //myContext = nullptr;
    //myBackBuffer = nullptr;
}

void CDirectXFramework::BeginFrame(std::array<float, 4> /*aClearColor*/)
{}

void CDirectXFramework::EndFrame()
{
    mySwapChain->Present(1, 0);
}

bool CDirectXFramework::Init(CWindowHandler* aWindowHandler)
{
    HRESULT result;
    DXGI_SWAP_CHAIN_DESC swapchainDesc = {};
    swapchainDesc.BufferCount = 1;
    swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchainDesc.OutputWindow = aWindowHandler->GetWindowHandle();
    swapchainDesc.SampleDesc.Count = 1;
    //swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    swapchainDesc.Windowed = true;

    result = D3D11CreateDeviceAndSwapChain(
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
        &myContext);

    if (FAILED(result))
    {
        return false;
    }

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

ID3D11Device* CDirectXFramework::GetDevice() const
{
    return myDevice.Get();
}

ID3D11DeviceContext* CDirectXFramework::GetContext() const
{
    return myContext.Get();
}

ID3D11Texture2D* CDirectXFramework::GetBackbufferTexture() const
{
    ID3D11Texture2D* backbufferTexture;
    HRESULT result = mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backbufferTexture);
    if (FAILED(result))
    {
        return nullptr;
    }
    return backbufferTexture;
}
