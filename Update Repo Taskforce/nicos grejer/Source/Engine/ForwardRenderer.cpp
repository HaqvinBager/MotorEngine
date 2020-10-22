#include "stdafx.h"
#include "ForwardRenderer.h"
#include "DirectXFramework.h"
#include "ModelInstance.h"
#include "Model.h"
#include "Camera.h"
#include "EnvironmentLight.h"
#include "PointLight.h"

namespace SM = DirectX::SimpleMath;

CForwardRenderer::CForwardRenderer() : myContext(nullptr), myFrameBuffer(), myObjectBuffer(), myTextureScrollingBuffer()
{
	myContext = nullptr;
}

CForwardRenderer::~CForwardRenderer()
{
	myContext = nullptr;
}

bool CForwardRenderer::Init(CDirectXFramework* aFramework)
{
	myContext = aFramework->GetContext();
	if (!myContext)
	{
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
	if (FAILED(result))
	{
		return false;
	}

	bufferDescription.ByteWidth = sizeof(SObjectBufferData);
	result = device->CreateBuffer(&bufferDescription, nullptr, &myObjectBuffer);
	if (FAILED(result))
	{
		return false;
	}

	bufferDescription.ByteWidth = sizeof(STextureScrollingData);
	result = device->CreateBuffer(&bufferDescription, nullptr, &myTextureScrollingBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void CForwardRenderer::Render(CEnvironmentLight* anEnvironmentLight, std::vector<std::pair<unsigned int, std::array<CPointLight*, 8>>> somePointLights, CCamera* aCamera, std::vector<CModelInstance*>& aModelList)
{
	myFrameBufferData.myCameraPosition = aCamera->GetPosition();
	myFrameBufferData.myToCamera = aCamera->GetTransform().Invert();
	myFrameBufferData.myToProjection = aCamera->GetProjection();
	myFrameBufferData.myDirectionalLightDirection = anEnvironmentLight->GetDirection();
	myFrameBufferData.myDirectionalLightColor = anEnvironmentLight->GetColor();
	//myFrameBufferData.myEnvironmentLightMipCount = anEnvironmentLight->GetMipCount();

	BindBuffer(myFrameBuffer, myFrameBufferData, "Frame Buffer");

	myContext->VSSetConstantBuffers(0, 1, &myFrameBuffer);
	myContext->PSSetConstantBuffers(0, 1, &myFrameBuffer);
	ID3D11ShaderResourceView* environmentLightShaderResource = anEnvironmentLight->GetCubemap();
	myContext->PSSetShaderResources(0, 1, &environmentLightShaderResource);

	UINT lightIndex = 0;
	for (CModelInstance* instance : aModelList)
	{
		CModel* model = instance->GetModel();
		CModel::SModelData modelData = model->GetModelData();

		myObjectBufferData.myToWorld = instance->GetTransform();
		myObjectBufferData.myNumberOfUsedPointLights = somePointLights[lightIndex].first;

		for (unsigned int i = 0; i < myObjectBufferData.myNumberOfUsedPointLights; ++i) {
			CPointLight* light = somePointLights[lightIndex].second[i];
			myObjectBufferData.myPointLights[i].myPosition = light->GetPosition();
			SM::Vector3 color = light->GetColor();
			myObjectBufferData.myPointLights[i].myColor = DirectX::SimpleMath::Vector4(color.x, color.y, color.z, light->GetIntensity());
			myObjectBufferData.myPointLights[i].myRange = light->GetRange();
		}
		++lightIndex;
		
		BindBuffer(myObjectBuffer, myObjectBufferData, "Object Buffer");

		myTextureScrollingData.scroll = instance->GetTextureScroll();
		myTextureScrollingData.scroll2 = instance->GetTextureScroll2();

		BindBuffer(myTextureScrollingBuffer, myTextureScrollingData, "Texture Scrolling Buffer");

		myContext->IASetPrimitiveTopology(modelData.myPrimitiveTopology);
		myContext->IASetInputLayout(modelData.myInputLayout);

		myContext->IASetVertexBuffers(0, 1, &modelData.myVertexBuffer, &modelData.myStride, &modelData.myOffset);
		myContext->IASetIndexBuffer(modelData.myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		myContext->VSSetConstantBuffers(1, 1, &myObjectBuffer);
		myContext->VSSetShader(modelData.myVertexShader, nullptr, 0);

		myContext->GSSetShader(nullptr, nullptr, 0);

		myContext->PSSetConstantBuffers(1, 1, &myObjectBuffer);
		myContext->PSSetConstantBuffers(2, 1, &myTextureScrollingBuffer);
		myContext->PSSetShaderResources(1, 3, &modelData.myTexture[0]);
		myContext->PSSetShader(modelData.myPixelShader, nullptr, 0);

		myContext->PSSetSamplers(0, 1, &modelData.mySampler);

		myContext->DrawIndexed(modelData.myNumberOfIndices, 0, 0);
	}
}

void CForwardRenderer::RenderOutline(CCamera* aCamera, CModelInstance* aModelInstance, CModel* someOutlineModelData)
{
	if (!someOutlineModelData) {
		return;
	}

	myFrameBufferData.myCameraPosition = aCamera->GetPosition();
	myFrameBufferData.myToCamera = aCamera->GetTransform().Invert();
	myFrameBufferData.myToProjection = aCamera->GetProjection();

	BindBuffer(myFrameBuffer, myFrameBufferData, "Frame Buffer");

	myContext->VSSetConstantBuffers(0, 1, &myFrameBuffer);
	myContext->PSSetConstantBuffers(0, 1, &myFrameBuffer);

	CModel* model = aModelInstance->GetModel();
	CModel::SModelData modelData = model->GetModelData();
	CModel::SModelData outlineModelData = someOutlineModelData->GetModelData();

	myObjectBufferData.myToWorld = aModelInstance->GetTransform();

	BindBuffer(myObjectBuffer, myObjectBufferData, "Object Buffer");

	myTextureScrollingData.scroll = aModelInstance->GetTextureScroll();
	myTextureScrollingData.scroll2 = aModelInstance->GetTextureScroll2();

	BindBuffer(myTextureScrollingBuffer, myTextureScrollingData, "Texture Scrolling Buffer");

	myContext->IASetPrimitiveTopology(modelData.myPrimitiveTopology);
	myContext->IASetInputLayout(modelData.myInputLayout);

	myContext->IASetVertexBuffers(0, 1, &modelData.myVertexBuffer, &modelData.myStride, &modelData.myOffset);
	myContext->IASetIndexBuffer(modelData.myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	myContext->VSSetConstantBuffers(1, 1, &myObjectBuffer);
	myContext->VSSetShader(outlineModelData.myVertexShader, nullptr, 0);

	myContext->PSSetConstantBuffers(1, 1, &myObjectBuffer);
	myContext->PSSetConstantBuffers(2, 1, &myTextureScrollingBuffer);
	myContext->PSSetShaderResources(1, 3, &modelData.myTexture[0]);
	myContext->PSSetShader(outlineModelData.myPixelShader, nullptr, 0);

	myContext->PSSetSamplers(0, 1, &modelData.mySampler);

	myContext->DrawIndexed(modelData.myNumberOfIndices, 0, 0);
	
}
