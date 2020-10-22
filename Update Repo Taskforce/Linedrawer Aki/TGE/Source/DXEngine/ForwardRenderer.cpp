#include "pch.h"
#include "ForwardRenderer.hpp"

#include "DirectX11Framework.hpp"
#include "Model.hpp"
#include "ModelInstance.hpp"
#include "Camera.hpp"
#include "EnvironmentLight.hpp"
#include "DirectionalLight.hpp"
#include "Line.hpp"
#include "LineInstance.hpp"

namespace Rendering
{
	ForwardRenderer::ForwardRenderer()
		: myDeviceContext(nullptr)
		, myFrameBuffer(nullptr)
		, myObjectBuffer(nullptr)
	{}
	ForwardRenderer::~ForwardRenderer()
	{}
	bool ForwardRenderer::Init( DirectX11Framework* aFramework )
	{
		if( !aFramework )
		{
			assert(aFramework == nullptr && "DirectX11Framework* was null!");
			return false;
		}

		myDeviceContext = aFramework->GetDeviceContext();
		if( !myDeviceContext )
		{
			assert(aFramework == nullptr && "DirectX11Framework* was null!");
			return false;
		}

		ID3D11Device* device = aFramework->GetDevice();
		if (!device)
		{
			return false;
		}

		HRESULT hResult;

		D3D11_BUFFER_DESC bufferDescription = {0};
		bufferDescription.Usage				= D3D11_USAGE_DYNAMIC;
		bufferDescription.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
		bufferDescription.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
		
		bufferDescription.ByteWidth	= sizeof(SFrameBufferData);
		hResult = device->CreateBuffer(&bufferDescription, nullptr, &myFrameBuffer);
		if (FAILED(hResult))
		{
			return false;
		}

		bufferDescription.ByteWidth = sizeof(SObjectBufferData);
		hResult = device->CreateBuffer(&bufferDescription, nullptr, &myObjectBuffer);
		if (FAILED(hResult))
		{
			return false;
		}

		return true;
	}
	void ForwardRenderer::Render(EnvironmentLight* aEnvironmentLight, DirectionalLight* aDirectionalLight, Camera* aCamera, std::vector<ModelSystem::ModelInstance*>& aModelList )
	{
		HRESULT hResult;
		D3D11_MAPPED_SUBRESOURCE bufferData;

		namespace SM = DirectX::SimpleMath;
		myFrameBufferData.myToCamera = aCamera->GetTransform().Invert();
		myFrameBufferData.myToProjection = aCamera->GetProjection();

		myFrameBufferData.myDirectionalLightColor = aDirectionalLight->GetColor();
		//if (aEnvironmentLight->GetDirection().Length() != 1.0f)
		//{
		//	assert(false && "EnvironmentLight's direction's length != 1");
		//	return;
		//}
		myFrameBufferData.myDirectionalLightDirection = aDirectionalLight->GetDirection();

		ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
		hResult = myDeviceContext->Map(myFrameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
		if (FAILED(hResult))
		{
			assert(false && "Failed to map myFrameBuffer!");
			return;
		}
		memcpy(bufferData.pData, &myFrameBufferData, sizeof(SFrameBufferData));
		myDeviceContext->Unmap(myFrameBuffer, 0);
		myDeviceContext->VSSetConstantBuffers(0, 1, &myFrameBuffer);
		myDeviceContext->PSSetConstantBuffers(0, 1, &myFrameBuffer);
		myDeviceContext->PSSetShaderResources(0, 1, aEnvironmentLight->GetCubeMap());


		for( ModelSystem::ModelInstance* instance : aModelList )
		{
			//ModelSystem::Model* model				 = instance->GetModel();
			ModelSystem::Model::SModelData modelData = instance->GetModel()->GetModelData();

			myObjectBufferData.myToWorld = instance->GetTransform();
			ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
			hResult = myDeviceContext->Map(myObjectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
			if (FAILED(hResult))
			{
				assert(false && "Failed to map myObjectBuffer!");
				return;
			}
			memcpy(bufferData.pData, &myObjectBufferData, sizeof(SObjectBufferData));
			myDeviceContext->Unmap(myObjectBuffer, 0);

			myDeviceContext->IASetPrimitiveTopology(modelData.myPrimitiveTopology);
			myDeviceContext->IASetInputLayout(modelData.myInputLayout);
			myDeviceContext->IASetVertexBuffers(0, 1, &modelData.myVertexBuffer, &modelData.myStride, &modelData.myOffset);
			myDeviceContext->IASetIndexBuffer(modelData.myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

			myDeviceContext->VSSetConstantBuffers(1, 1, &myObjectBuffer);
			myDeviceContext->VSSetShader(modelData.myVertexShader, nullptr, 0);
			//myDeviceContext->PSSetShaderResources(0, 1, &modelData.myTexture);
			myDeviceContext->PSSetShaderResources(1, 6, modelData.myTextures.data());
			myDeviceContext->PSSetShader(modelData.myPixelShader, nullptr, 0);

			myDeviceContext->DrawIndexed(modelData.myNumberOfIndices, 0, 0);
			//myDeviceContext->Draw(modelData.myNumberOfVertices, 0);
		}
	}
	void ForwardRenderer::RenderLines(Camera* aCamera, std::vector<LineInstance*>& aLineList)
	{
		HRESULT hResult;
		D3D11_MAPPED_SUBRESOURCE bufferData;

		namespace SM = DirectX::SimpleMath;
		myFrameBufferData.myToCamera		= aCamera->GetTransform().Invert();
		myFrameBufferData.myToProjection	= aCamera->GetProjection();

		ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
		hResult = myDeviceContext->Map(myFrameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
		if (FAILED(hResult))
		{
			assert(false && "Failed to map myFrameBuffer!");
			return;
		}
		memcpy(bufferData.pData, &myFrameBufferData, sizeof(SFrameBufferData));
		myDeviceContext->Unmap(myFrameBuffer, 0);
		myDeviceContext->VSSetConstantBuffers(0, 1, &myFrameBuffer);
		myDeviceContext->PSSetConstantBuffers(0, 1, &myFrameBuffer);

		for( LineInstance* instance : aLineList )
		{
			Line::SLineData lineData = instance->GetLine()->GetLineData();

			myObjectBufferData.myToWorld = instance->GetTransform();
			ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
			hResult = myDeviceContext->Map(myObjectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
			if (FAILED(hResult))
			{
				assert(false && "Failed to map myObjectBuffer!");
				return;
			}
			memcpy(bufferData.pData, &myObjectBufferData, sizeof(SObjectBufferData));
			myDeviceContext->Unmap(myObjectBuffer, 0);

			myDeviceContext->IASetPrimitiveTopology(lineData.myPrimitiveTopology);
			myDeviceContext->IASetInputLayout(lineData.myInputLayout);
			myDeviceContext->IASetVertexBuffers(0, 1, &lineData.myVertexBuffer, &lineData.myStride, &lineData.myOffset);
			myDeviceContext->IASetIndexBuffer(lineData.myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

			myDeviceContext->VSSetConstantBuffers(1, 1, &myObjectBuffer);
			myDeviceContext->VSSetShader(lineData.myVertexShader, nullptr, 0);

			myDeviceContext->PSSetShader(lineData.myPixelShader, nullptr, 0);

			//myDeviceContext->DrawIndexed(lineData.myNumberOfIndices, 0, 0);
			myDeviceContext->Draw(lineData.myNumberOfVertices, 0);
		}
	}
}
