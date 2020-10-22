#include "stdafx.h"
#include "FullscreenRenderer.h"
#include "DirectX11Framework.h"
#include <fstream>

CFullscreenRenderer::CFullscreenRenderer() : myContext(nullptr), myVertexShader(nullptr), myPixelShaders() {
}

CFullscreenRenderer::~CFullscreenRenderer() {
}

bool CFullscreenRenderer::Init(CDirectX11Framework* aFramework) {
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

	std::array<std::string, FullscreenShader::FULLSCREENSHADER_COUNT> filepaths;
	filepaths[FullscreenShader::FULLSCREENSHADER_COPY] = "FullscreenPixelShader_Copy.cso";
	filepaths[FullscreenShader::FULLSCREENSHADER_LUMINANCE] = "FullscreenPixelShader_Luminance.cso";
	filepaths[FullscreenShader::FULLSCREENSHADER_GAUSSIANHORIZONTAL] = "FullscreenPixelShader_GaussianBlurHorizontal.cso";
	filepaths[FullscreenShader::FULLSCREENSHADER_GAUSSIANVERTICAL] = "FullscreenPixelShader_GaussianBlurVertical.cso";
	filepaths[FullscreenShader::FULLSCREENSHADER_BLOOM] = "FullscreenPixelShader_Bloom.cso";

	for (UINT i = 0; i < FullscreenShader::FULLSCREENSHADER_COUNT; i++) {
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

	return true;
}

void CFullscreenRenderer::Render(FullscreenShader aEffect) {
	myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	myContext->IASetInputLayout(nullptr);
	myContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	myContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

	myContext->VSSetShader(myVertexShader, nullptr, 0);
	myContext->PSSetShader(myPixelShaders[aEffect], nullptr, 0);

	myContext->Draw(3, 0);

	ID3D11ShaderResourceView* clearShaderResourceView = nullptr;
	myContext->PSSetShaderResources(0, 1, &clearShaderResourceView);
}
