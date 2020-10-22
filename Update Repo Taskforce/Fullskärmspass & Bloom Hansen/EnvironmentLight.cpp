#include "stdafx.h"
#include "EnvironmentLight.h"
#include "..\DirectXTK\Inc\DDSTextureLoader.h"

CEnvironmentLight::CEnvironmentLight() : myCubemap(nullptr), myMipCount(0) {
}

CEnvironmentLight::~CEnvironmentLight() {
}

bool CEnvironmentLight::Init(std::string aFilePath, ID3D11Device* aDevice) {
	HRESULT result;
	std::wstring cubemapFilePath(aFilePath.begin(), aFilePath.end());
	result = DirectX::CreateDDSTextureFromFile(aDevice, cubemapFilePath.c_str(), nullptr, &myCubemap);
	if (FAILED(result)) {
		return false;
	}

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

void CEnvironmentLight::SetDirection(SM::Vector3 aDirection) {
	myDirection.x = aDirection.x;
	myDirection.y = aDirection.y;
	myDirection.z = aDirection.z;
	myDirection.Normalize();
}

void CEnvironmentLight::SetColor(SM::Vector3 aColor) {
	myColor.x = aColor.x;
	myColor.y = aColor.y;
	myColor.z = aColor.z;
}

ID3D11ShaderResourceView* const* CEnvironmentLight::GetCubemap() {
	return &myCubemap;
}

unsigned int CEnvironmentLight::GetMipCount() const {
	return myMipCount;
}

SM::Vector4 CEnvironmentLight::GetDirection() const {
	return myDirection;
}

SM::Vector4 CEnvironmentLight::GetColor() const {
	return myColor;
}
