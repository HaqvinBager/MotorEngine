#include "stdafx.h"
#include "FullscreenRenderer.h"
#include "DirectXFramework.h"

#include <fstream>

CFullscreenRenderer::CFullscreenRenderer()
{
    myContext = nullptr;
    myVertexShader = nullptr;
}

CFullscreenRenderer::~CFullscreenRenderer()
{
    myContext = nullptr;
    myVertexShader = nullptr;
}

bool CFullscreenRenderer::Init(CDirectXFramework* aFramework)
{
    if (!aFramework) {
        return false;
    }

    myContext = aFramework->GetContext();
    if (!myContext) {
        return false;
    }

    ID3D11Device* device = aFramework->GetDevice();
    HRESULT result;

    std::ifstream vsFile;
    vsFile.open("FullscreenVertexShader.cso", std::ios::binary);
    std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
    ID3D11VertexShader* vertexShader;
    result = device->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
    if (FAILED(result)) {
        return false;
    }
    vsFile.close();
    myVertexShader = vertexShader;

    std::array<std::string, (size_t)FullscreenShader::FULLSCREENSHADER_COUNT> filepaths;
    filepaths[(size_t)FullscreenShader::FULLSCREENSHADER_COPY] = "FullscreenPixelShader_Copy.cso";
    filepaths[(size_t)FullscreenShader::FULLSCREENSHADER_LUMINANCE] = "FullscreenPixelShader_Luminance.cso";
    filepaths[(size_t)FullscreenShader::FULLSCREENSHADER_GAUSSIANHORIZONTAL] = "FullscreenPixelShader_GaussianBlurHorizontal.cso";
    filepaths[(size_t)FullscreenShader::FULLSCREENSHADER_GAUSSIANVERTICAL] = "FullscreenPixelShader_GaussianBlurVertical.cso";
    filepaths[(size_t)FullscreenShader::FULLSCREENSHADER_BLOOM] = "FullscreenPixelShader_Bloom.cso";
    filepaths[(size_t)FullscreenShader::FULLSCREENSHADER_VIGNETTE] = "FullscreenPixelShader_Vignette.cso";
    
    for (unsigned int i = 0; i < (size_t)FullscreenShader::FULLSCREENSHADER_COUNT; ++i) {
        std::ifstream psFile;
        psFile.open(filepaths[i], std::ios::binary);
        std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
        ID3D11PixelShader* pixelShader;
        result = device->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);
        if (FAILED(result)) {
            return false;
        }
        psFile.close();
        myPixelShaders[i] = pixelShader;
    }

    //Start Sampler
    ID3D11SamplerState* sampler;
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    ENGINE_HR_MESSAGE(device->CreateSamplerState(&samplerDesc, &sampler), "Sampler could not be created.");
    mySampler = sampler;
    //End Sampler

    return true;
}

void CFullscreenRenderer::Render(FullscreenShader anEffect)
{
    myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    myContext->IASetInputLayout(nullptr);
    myContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
    myContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

    myContext->VSSetShader(myVertexShader, nullptr, 0);
    myContext->PSSetSamplers(0, 1, &mySampler);
    myContext->PSSetShader(myPixelShaders[(size_t)anEffect], nullptr, 0);

    myContext->Draw(3, 0);

    //Reset PS Resources
    ID3D11ShaderResourceView* nullView = NULL;
    myContext->PSSetShaderResources(0, 1, &nullView);
    myContext->PSSetShaderResources(1, 1, &nullView);
}
