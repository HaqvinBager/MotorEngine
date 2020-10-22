#include "stdafx.h"
#include "ForwardRenderer.h"
#include <d3d11.h>
#include "DirectX11Framework.h"
#include "ModelFactory.h"
#include "Camera.h"
#include "EnvironmentLight.h"
#include "PointLight.h"

CForwardRenderer::CForwardRenderer() : myContext(nullptr), myFrameBuffer(nullptr), myObjectBuffer(nullptr) {
}

CForwardRenderer::~CForwardRenderer() {
	myContext = nullptr;
	myFrameBuffer = nullptr;
	myObjectBuffer = nullptr;
}

bool CForwardRenderer::Init(CDirectX11Framework* aFramework) {
	if (!aFramework) {
		return false;
	}

	myContext = aFramework->GetContext();
	if (!myContext) {
		return false;
	}

	ID3D11Device* device = aFramework->GetDevice();
	if (!device) {
		return false;
	}

	HRESULT result;
	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	bufferDescription.ByteWidth = sizeof(SFrameBufferData);
	result = device->CreateBuffer(&bufferDescription, nullptr, &myFrameBuffer);
	if (FAILED(result)) {
		return false;
	}

	bufferDescription.ByteWidth = sizeof(SObjectBufferData);
	result = device->CreateBuffer(&bufferDescription, nullptr, &myObjectBuffer);
	if (FAILED(result)) {
		return false;
	}

	return true;
}

void CForwardRenderer::Render(CEnvironmentLight* aEnvirontmentLight, std::vector<std::pair<unsigned int, std::array<CPointLight*, 8>>> aModelPointLightList, CCamera* aCamera, std::vector<CModelInstance*> &aModelList) {
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE bufferData;

	myFrameBufferData.myCameraPosition = (SM::Vector4)aCamera->GetTransform().Translation();
	myFrameBufferData.myCameraPosition.w = 1;
	myFrameBufferData.myToCamera = aCamera->GetTransform().Invert();
	myFrameBufferData.myToProjection = aCamera->GetProjection();
	myFrameBufferData.myDirectionalLightDirection = aEnvirontmentLight->GetDirection();
	myFrameBufferData.myDirectionalLightColor = aEnvirontmentLight->GetColor();
	myFrameBufferData.myEnvironmentLightMipCount = aEnvirontmentLight->GetMipCount();

	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = myContext->Map(myFrameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	if (FAILED(result)) {
		return;
	}

	memcpy(bufferData.pData, &myFrameBufferData, sizeof(SFrameBufferData));
	myContext->Unmap(myFrameBuffer, 0);
	myContext->VSSetConstantBuffers(0, 1, &myFrameBuffer);
	myContext->PSSetConstantBuffers(0, 1, &myFrameBuffer);
	myContext->PSSetConstantBuffers(1, 1, &myObjectBuffer);
	myContext->PSSetShaderResources(0, 1, aEnvirontmentLight->GetCubemap());

	int modelIndex = 0;
	for (CModelInstance* instance : aModelList) {
		CModel* model = instance->GetModel();
		CModel::SModelData modelData = model->GetModelData();

		myObjectBufferData.myToWorld = instance->GetTransform();
		for (int i = 0; i < aModelPointLightList[modelIndex].first; ++i) {
			SM::Vector3 position = aModelPointLightList[modelIndex].second[i]->GetPosition();
			SM::Vector3 color = aModelPointLightList[modelIndex].second[i]->GetColor();
			myObjectBufferData.myPointLights[i].myPositionAndIntensity = { position.x, position.y, position.z, aModelPointLightList[modelIndex].second[i]->GetIntensity() };
			myObjectBufferData.myPointLights[i].myColorAndRange = { color.x, color.y, color.z, aModelPointLightList[modelIndex].second[i]->GetRange() };
		}
		myObjectBufferData.myNumberOfUsedPointLights = aModelPointLightList[modelIndex].first;

		++modelIndex;

		ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
		result = myContext->Map(myObjectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);

		if (FAILED(result)) {
			return;
		}

		memcpy(bufferData.pData, &myObjectBufferData, sizeof(SObjectBufferData));
		myContext->Unmap(myObjectBuffer, 0);

		myContext->IASetPrimitiveTopology(modelData.myPrimitiveTopology);
		myContext->IASetInputLayout(modelData.myInputLayout);

		myContext->IASetVertexBuffers(0, 1, &modelData.myVertexBuffer, &modelData.myStride, &modelData.myOffset);
		myContext->IASetIndexBuffer(modelData.myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		myContext->VSSetConstantBuffers(1, 1, &myObjectBuffer);
		myContext->VSSetShader(modelData.myVertexShader, nullptr, 0);

		myContext->PSSetShaderResources(1, 3, &modelData.myTexture[0]);
		myContext->PSSetShader(modelData.myPixelShader, nullptr, 0);
		myContext->PSSetSamplers(0, 1, &modelData.mySampler);

		myContext->DrawIndexed(modelData.myNumberOfIndices, 0, 0);

		ID3D11ShaderResourceView* clearShaderResourceView = nullptr;
		myContext->PSSetShaderResources(0, 1, &clearShaderResourceView);
	}
}
