#include "stdafx.h"
#include "RenderStateManager.h"

CRenderStateManager::CRenderStateManager()
{
    myContext = nullptr;
    myBlendStates;
    myDepthStencilStates;
    myRasterizerStates;
    mySamplerStates;
}

CRenderStateManager::~CRenderStateManager()
{
    myContext = nullptr;
}

bool CRenderStateManager::Init(CDirectXFramework* aFramework)
{
    myContext = aFramework->GetContext();
    ID3D11Device* device = aFramework->GetDevice();

    if (!myContext) {
        return false;
    }

    if (!device) {
        return false;
    }

    if (!CreateBlendStates(device)) {
        return false;
    }

    if (!CreateDepthStencilStates(device))
    {
        return false;
    }

    if (!CreateRasterizerStates(device)) 
    {
        return false;
    }
    
    if (!CreateSamplerStates(device))
    {
        return false;
    }

    return true;
}

void CRenderStateManager::SetBlendState(BlendStates aBlendState)
{
    std::array<float, 4> blendFactors = { 0.5f, 0.5f, 0.5f, 0.5f };
    myContext->OMSetBlendState(myBlendStates[(size_t)aBlendState], blendFactors.data(), 0xFFFFFFFFu);
}

void CRenderStateManager::SetDepthStencilState(DepthStencilStates aDepthStencilState, UINT aStencilRef)
{
    myContext->OMSetDepthStencilState(myDepthStencilStates[(size_t)aDepthStencilState], aStencilRef);
}

void CRenderStateManager::SetRasterizerState(RasterizerStates aRasterizerState)
{
    myContext->RSSetState(myRasterizerStates[(size_t)aRasterizerState]);
}

void CRenderStateManager::SetSamplerState(SamplerStates aSamplerState)
{
    myContext->VSSetSamplers(0, 1, &mySamplerStates[(size_t)aSamplerState]);
    myContext->PSSetSamplers(0, 1, &mySamplerStates[(size_t)aSamplerState]);
}

void CRenderStateManager::SetAllStates(BlendStates aBlendState, DepthStencilStates aDepthStencilState, RasterizerStates aRasterizerState, SamplerStates aSamplerState)
{
    SetBlendState(aBlendState);
    SetDepthStencilState(aDepthStencilState);
    SetRasterizerState(aRasterizerState);
    SetSamplerState(aSamplerState);
}

void CRenderStateManager::SetAllDefault()
{
    SetBlendState(BlendStates::BLENDSTATE_DISABLE);
    SetDepthStencilState(DepthStencilStates::DEPTHSTENCILSTATE_DEFAULT);
    SetRasterizerState(RasterizerStates::RASTERIZERSTATE_DEFAULT);
    SetSamplerState(SamplerStates::SAMPLERSTATE_TRILINEAR);
}

void CRenderStateManager::Release()
{
    for (unsigned int i = 0; i < static_cast<unsigned int>(BlendStates::BLENDSTATE_COUNT); ++i)
    {
        myBlendStates[i]->Release();
    }
    
    for (unsigned int i = 0; i < static_cast<unsigned int>(DepthStencilStates::DEPTHSTENCILSTATE_COUNT); ++i)
    {
        myDepthStencilStates[i]->Release();
    }

    for (unsigned int i = 0; i < static_cast<unsigned int>(RasterizerStates::RASTERIZERSTATE_COUNT); ++i)
    {
        myRasterizerStates[i]->Release();
    }

    for (unsigned int i = 0; i < static_cast<unsigned int>(SamplerStates::SAMPLERSTATE_COUNT); ++i)
    {
        mySamplerStates[i]->Release();
    }
}

bool CRenderStateManager::CreateBlendStates(ID3D11Device* aDevice)
{
    D3D11_BLEND_DESC alphaBlendDesc = { 0 };
    alphaBlendDesc.RenderTarget[0].BlendEnable = true;
    alphaBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    alphaBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    alphaBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    alphaBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    alphaBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    alphaBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
    alphaBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    D3D11_BLEND_DESC additiveBlendDesc = { 0 };
    additiveBlendDesc.RenderTarget[0].BlendEnable = true;
    additiveBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    additiveBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    additiveBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    additiveBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    additiveBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    additiveBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
    additiveBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    ID3D11BlendState* alphaBlendState;
    ENGINE_HR_MESSAGE(aDevice->CreateBlendState(&alphaBlendDesc, &alphaBlendState), "Alpha Blend State could not be created.");

    ID3D11BlendState* additiveBlendState;
    ENGINE_HR_MESSAGE(aDevice->CreateBlendState(&additiveBlendDesc, &additiveBlendState), "Additive Blend State could not be created");

    myBlendStates[(size_t)BlendStates::BLENDSTATE_DISABLE] = nullptr;
    myBlendStates[(size_t)BlendStates::BLENDSTATE_ALPHABLEND] = alphaBlendState;
    myBlendStates[(size_t)BlendStates::BLENDSTATE_ADDITIVEBLEND] = additiveBlendState;

    return true;
}

bool CRenderStateManager::CreateDepthStencilStates(ID3D11Device* aDevice)
{
    D3D11_DEPTH_STENCIL_DESC onlyreadDepthDesc = { 0 };
    onlyreadDepthDesc.DepthEnable = true;
    onlyreadDepthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    onlyreadDepthDesc.DepthFunc = D3D11_COMPARISON_LESS;
    onlyreadDepthDesc.StencilEnable = false;

    D3D11_DEPTH_STENCIL_DESC stencilWriteDesc = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };
    stencilWriteDesc.DepthEnable = true;
    stencilWriteDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    stencilWriteDesc.DepthFunc = D3D11_COMPARISON_LESS;
    stencilWriteDesc.StencilEnable = TRUE;
    stencilWriteDesc.StencilReadMask = 0xFF;
    stencilWriteDesc.StencilWriteMask = 0xFF;
    stencilWriteDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    stencilWriteDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    stencilWriteDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
    stencilWriteDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    D3D11_DEPTH_STENCIL_DESC stencilMaskDesc = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };
    stencilMaskDesc.DepthEnable = FALSE;
    //stencilMaskDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    stencilMaskDesc.StencilEnable = TRUE;
    stencilMaskDesc.StencilReadMask = 0xFF;
    stencilMaskDesc.StencilWriteMask = 0xFF;
    stencilMaskDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    stencilMaskDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    stencilMaskDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;

    ID3D11DepthStencilState* onlyreadDepthStencilState;
    ENGINE_HR_MESSAGE(aDevice->CreateDepthStencilState(&onlyreadDepthDesc, &onlyreadDepthStencilState), "OnlyRead Depth Stencil State could not be created.");

    ID3D11DepthStencilState* writeDepthStencilState;
    ENGINE_HR_MESSAGE(aDevice->CreateDepthStencilState(&stencilWriteDesc, &writeDepthStencilState), "Write Stencil State could not be created.");
    
    ID3D11DepthStencilState* maskDepthStencilState;
    ENGINE_HR_MESSAGE(aDevice->CreateDepthStencilState(&stencilMaskDesc, &maskDepthStencilState), "Mask Stencil State could not be created.");

    myDepthStencilStates[(size_t)DepthStencilStates::DEPTHSTENCILSTATE_DEFAULT] = nullptr;
    myDepthStencilStates[(size_t)DepthStencilStates::DEPTHSTENCILSTATE_ONLYREAD] = onlyreadDepthStencilState;
    myDepthStencilStates[(size_t)DepthStencilStates::DEPTHSTENCILSTATE_STENCILWRITE] = writeDepthStencilState;
    myDepthStencilStates[(size_t)DepthStencilStates::DEPTHSTENCILSTATE_STENCILMASK] = maskDepthStencilState;

    return true;
}

bool CRenderStateManager::CreateRasterizerStates(ID3D11Device* aDevice)
{
    D3D11_RASTERIZER_DESC wireframeRasterizerDesc = {};
    wireframeRasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
    wireframeRasterizerDesc.CullMode = D3D11_CULL_BACK;
    wireframeRasterizerDesc.DepthClipEnable = true;

    ID3D11RasterizerState* wireframeRasterizerState;
    ENGINE_HR_MESSAGE(aDevice->CreateRasterizerState(&wireframeRasterizerDesc, &wireframeRasterizerState), "Wireframe Rasterizer State could not be created.");

    myRasterizerStates[(size_t)RasterizerStates::RASTERIZERSTATE_DEFAULT] = nullptr;
    myRasterizerStates[(size_t)RasterizerStates::RASTERIZERSTATE_WIREFRAME] = wireframeRasterizerState;

    return true;
}

bool CRenderStateManager::CreateSamplerStates(ID3D11Device* aDevice)
{
    D3D11_SAMPLER_DESC pointSamplerDesc = {};
    pointSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    pointSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    pointSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    pointSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    pointSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    pointSamplerDesc.MinLOD = -FLT_MAX;
    pointSamplerDesc.MaxLOD = FLT_MAX;

    D3D11_SAMPLER_DESC wrapSamplerDesc = {};
    wrapSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    wrapSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    wrapSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    wrapSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    wrapSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    wrapSamplerDesc.MinLOD = -FLT_MAX;
    wrapSamplerDesc.MaxLOD = FLT_MAX;

    ID3D11SamplerState* pointSamplerState;
    ENGINE_HR_MESSAGE(aDevice->CreateSamplerState(&pointSamplerDesc, &pointSamplerState), "Point Sampler could not be created.");

    ID3D11SamplerState* wrapSamplerState;
    ENGINE_HR_MESSAGE(aDevice->CreateSamplerState(&wrapSamplerDesc, &wrapSamplerState), "Wrap Sampler could not be created.");

    mySamplerStates[(size_t)SamplerStates::SAMPLERSTATE_TRILINEAR] = nullptr;
    mySamplerStates[(size_t)SamplerStates::SAMPLERSTATE_POINT] = pointSamplerState;
    mySamplerStates[(size_t)SamplerStates::SAMPLERSTATE_WRAP] = wrapSamplerState;

    return true;
}
