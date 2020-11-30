#include "stdafx.h"
#include "FullscreenRenderer.h"
#include "DirectXFramework.h"
#include <fstream>

CFullscreenRenderer::CFullscreenRenderer() : myContext(nullptr), myVertexShader(nullptr), myPixelShaders() {
}

CFullscreenRenderer::~CFullscreenRenderer() {
}

bool CFullscreenRenderer::Init(CDirectXFramework* aFramework) {
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
	vsFile.open("Shaders/FullscreenVertexShader.cso", std::ios::binary);
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	ID3D11VertexShader* vertexShader;
	result = device->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
	if (FAILED(result)) {
		return false;
	}
	vsFile.close();
	myVertexShader = vertexShader;

	std::array<std::string, static_cast<size_t>(FullscreenShader::FULLSCREENSHADER_COUNT)> filepaths;
	filepaths[static_cast<size_t>(FullscreenShader::FULLSCREENSHADER_COPY)] = "Shaders/FullscreenPixelShader_Copy.cso";
	filepaths[static_cast<size_t>(FullscreenShader::FULLSCREENSHADER_LUMINANCE)] = "Shaders/FullscreenPixelShader_Luminance.cso";
	filepaths[static_cast<size_t>(FullscreenShader::FULLSCREENSHADER_GAUSSIANHORIZONTAL)] = "Shaders/FullscreenPixelShader_GaussianBlurHorizontal.cso";
	filepaths[static_cast<size_t>(FullscreenShader::FULLSCREENSHADER_GAUSSIANVERTICAL)] = "Shaders/FullscreenPixelShader_GaussianBlurVertical.cso";
	filepaths[static_cast<size_t>(FullscreenShader::FULLSCREENSHADER_BLOOM)] = "Shaders/FullscreenPixelShader_Bloom.cso";
	filepaths[static_cast<size_t>(FullscreenShader::FULLSCREENSHADER_VIGNETTE)] = "Shaders/FullscreenPixelShader_Vignette.cso";

	for (UINT i = 0; i < static_cast<size_t>(FullscreenShader::FULLSCREENSHADER_COUNT); i++) {
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

void CFullscreenRenderer::Render(FullscreenShader anEffect) {
	myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	myContext->IASetInputLayout(nullptr);
	myContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	myContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

	myContext->VSSetShader(myVertexShader, nullptr, 0);
	myContext->PSSetShader(myPixelShaders[static_cast<size_t>(anEffect)], nullptr, 0);
	myContext->PSSetSamplers(0, 1, &mySampler);

	myContext->Draw(3, 0);

	ID3D11ShaderResourceView* nullView = NULL;
	myContext->PSSetShaderResources(0, 1, &nullView);
	myContext->PSSetShaderResources(1, 1, &nullView);
	myContext->PSSetShaderResources(2, 1, &nullView);
	myContext->PSSetShaderResources(3, 1, &nullView);
	myContext->PSSetShaderResources(4, 1, &nullView);
	myContext->PSSetShaderResources(5, 1, &nullView);
	myContext->PSSetShaderResources(6, 1, &nullView);
	myContext->PSSetShaderResources(7, 1, &nullView);
	myContext->PSSetShaderResources(8, 1, &nullView);

	myContext->GSSetShader(nullptr, nullptr, 0);
}
