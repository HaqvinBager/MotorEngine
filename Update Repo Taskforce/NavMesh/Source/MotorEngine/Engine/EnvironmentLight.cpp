#include "stdafx.h"
#include "EnvironmentLight.h"
#include "DirectXFramework.h"
#include <SimpleMath.h>
#include <DDSTextureLoader.h>

CEnvironmentLight::CEnvironmentLight()
{
}

CEnvironmentLight::~CEnvironmentLight()
{

}

bool CEnvironmentLight::Init(CDirectXFramework* aFramework, std::string aFilePath)
{
	HRESULT result;
	wchar_t* cubeMapPathWide = new wchar_t[aFilePath.length() + 1];
	std::copy(aFilePath.begin(), aFilePath.end(), cubeMapPathWide);
	cubeMapPathWide[aFilePath.length()] = 0;

	result = DirectX::CreateDDSTextureFromFile(aFramework->GetDevice(), cubeMapPathWide, nullptr, &cubeShaderResourceView);
	if (FAILED(result))
	{
		return false;
	}

	delete[] cubeMapPathWide;

	ID3D11Resource* cubeResource;
	cubeShaderResourceView->GetResource(&cubeResource);
	ID3D11Texture2D* cubeTexture = reinterpret_cast<ID3D11Texture2D*>(cubeResource);
	D3D11_TEXTURE2D_DESC cubeDescription = { 0 };
	cubeTexture->GetDesc(&cubeDescription);
	myMipLevels = cubeDescription.MipLevels;

	return true;
}

ID3D11ShaderResourceView* const* CEnvironmentLight::GetCubeMap()
{
	return &cubeShaderResourceView;
}

unsigned int CEnvironmentLight::GetMipLevels()
{
	return myMipLevels;
}
