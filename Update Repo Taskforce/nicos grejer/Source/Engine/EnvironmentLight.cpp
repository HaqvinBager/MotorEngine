#include "stdafx.h"
#include "DirectXFramework.h"
#include "EnvironmentLight.h"
#include "DDSTextureLoader.h"

CEnvironmentLight::CEnvironmentLight()
{
    myCubemap = nullptr;
}

CEnvironmentLight::~CEnvironmentLight()
{
    myCubemap = nullptr;
}

bool CEnvironmentLight::Init(CDirectXFramework* aFramework, std::string aFilePath)
{
    HRESULT result;
    wchar_t* wideString = new wchar_t[aFilePath.length() + 1];
    std::copy(aFilePath.begin(), aFilePath.end(), wideString);
    wideString[aFilePath.length()] = 0;

    result = DirectX::CreateDDSTextureFromFile(aFramework->GetDevice(), wideString, nullptr, &myCubemap);
    if (FAILED(result)) {
        return false;
    }
    delete[] wideString;

    ID3D11Resource* cubeResource;
    myCubemap->GetResource(&cubeResource);
    ID3D11Texture2D* cubeTexture = reinterpret_cast<ID3D11Texture2D*>(cubeResource);
    D3D11_TEXTURE2D_DESC cubeDescription;
    cubeTexture->GetDesc(&cubeDescription);

    myMipCount = cubeDescription.MipLevels;

    myDirection = { 0.0f, 0.0f, 0.0f, 0.0f };
    myColor = { 0.0f, 0.0f, 0.0f, 1.0f };

    return true;
}
