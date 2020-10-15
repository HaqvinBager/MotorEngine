#include "stdafx.h"
#include "ForwardRenderer.h"
#include "Engine.h"
#include "ModelInstance.h"
#include "Model.h"
#include "Camera.h"
#include "EnvironmentLight.h"
#include "ModelMath.h"
#include "GameObject.h"
#include "ModelComponent.h"
#include "AnimationComponent.h"
#include "TransformComponent.h"

namespace SM = DirectX::SimpleMath;

CForwardRenderer::CForwardRenderer() : myContext(nullptr), myFrameBuffer(nullptr), myFrameBufferData(), myObjectBuffer(nullptr), myObjectBufferData(), myBoneBuffer(nullptr), myBoneBufferData() {

}

CForwardRenderer::~CForwardRenderer() {
	myContext = nullptr;
}

bool CForwardRenderer::Init(CEngine& anEngine) {
	if (!anEngine.myFramework)
		return false;

	myContext = anEngine.myFramework->GetContext();
	if (!myContext) {
		return false;
	}

	ID3D11Device* device = anEngine.myFramework->GetDevice();
	if (!device)
		return false;

	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDescription.MiscFlags = 0;
	bufferDescription.ByteWidth = static_cast<UINT>(sizeof(SFrameBufferData) + (16 - (sizeof(SFrameBufferData) % 16)));
	bufferDescription.StructureByteStride = 0;

	ENGINE_HR_BOOL_MESSAGE(device->CreateBuffer(&bufferDescription, nullptr, &myFrameBuffer), "Frame Buffer could not be created.");

	bufferDescription.ByteWidth = static_cast<UINT>(sizeof(SObjectBufferData) + (16 - (sizeof(SObjectBufferData) % 16)));
	ENGINE_HR_BOOL_MESSAGE(device->CreateBuffer(&bufferDescription, nullptr, &myObjectBuffer), "Object Buffer could not be created.");

	bufferDescription.ByteWidth = static_cast<UINT>(sizeof(SBoneBufferData) + (16 - (sizeof(SBoneBufferData) % 16)));
	ENGINE_HR_BOOL_MESSAGE(device->CreateBuffer(&bufferDescription, nullptr, &myBoneBuffer), "Bone Buffer could not be created.");

	return true;
}

void CForwardRenderer::Render(CEnvironmentLight* anEnvironmentLight, CCamera* aCamera, std::vector<CModelInstance*>& aModelList, std::vector<CGameObject*>& aGameObjectList)
{
	D3D11_MAPPED_SUBRESOURCE bufferData;
	myFrameBufferData.myToCamera = aCamera->GetTransform().Invert();
	myFrameBufferData.myToProjection = aCamera->GetProjection();
	myFrameBufferData.myCameraPosition = aCamera->GetPosition();
	myFrameBufferData.myDirectionalLightDirection = anEnvironmentLight->GetDirection();
	myFrameBufferData.myDirectionalLightColor = anEnvironmentLight->GetColor();

	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
	ENGINE_HR_MESSAGE(myContext->Map(myFrameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData), "Frame Buffer could not be mapped.");

	memcpy(bufferData.pData, &myFrameBufferData, sizeof(SFrameBufferData));
	myContext->Unmap(myFrameBuffer, 0);
	myContext->VSSetConstantBuffers(0, 1, &myFrameBuffer);
	myContext->PSSetConstantBuffers(0, 1, &myFrameBuffer);
	myContext->PSSetShaderResources(0, 1, anEnvironmentLight->GetCubeMap());
	
	// MODELCOMPONENT
	for (CGameObject* gameobject : aGameObjectList)
	{
		//Added this if Check because not all GameObjects Must have a CModelComponent.
		//Refactoring suggestion: Have CModelComponents be "created" in some kind of Factory.
		//This factory will make sure to hold all CModelComponent Data in a Cache friendly array <3
		//Big hype!
		if (gameobject->GetComponent<CModelComponent>() == nullptr)
			continue;

		CModel* model = gameobject->GetComponent<CModelComponent>()->GetMyModel()->GetModel();

		CModel::SModelData modelData = model->GetModelData();

		myObjectBufferData.myToWorld = gameobject->GetComponent<CTransformComponent>()->Transform();

		D3D11_MAPPED_SUBRESOURCE PSbufferData;
		ZeroMemory(&PSbufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
		ENGINE_HR_MESSAGE(myContext->Map(myObjectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &PSbufferData), "Object Buffer could not be mapped.");

		CopyMemory(PSbufferData.pData, &myObjectBufferData, sizeof(SObjectBufferData));
		myContext->Unmap(myObjectBuffer, 0);

		if (gameobject->GetComponent<CAnimationComponent>() != nullptr) {
			memcpy(myBoneBufferData.myBones, gameobject->GetComponent<CAnimationComponent>()->GetBones().data(), sizeof(SlimMatrix44) * 64);

			D3D11_MAPPED_SUBRESOURCE VSBufferData;
			ZeroMemory(&VSBufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
			ENGINE_HR_MESSAGE(myContext->Map(myBoneBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &VSBufferData), "Bone Buffer could not be mapped.");
			CopyMemory(VSBufferData.pData, &myBoneBufferData, sizeof(SBoneBufferData));
			myContext->Unmap(myBoneBuffer, 0);

			myContext->VSSetConstantBuffers(2, 1, &myBoneBuffer);
		}



		myContext->IASetPrimitiveTopology(modelData.myPrimitiveTopology);
		myContext->IASetInputLayout(modelData.myInputLayout);
		myContext->IASetVertexBuffers(0, 1, &modelData.myVertexBuffer, &modelData.myStride, &modelData.myOffset);
		myContext->IASetIndexBuffer(modelData.myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		myContext->VSSetConstantBuffers(1, 1, &myObjectBuffer);
		myContext->VSSetShader(modelData.myVertexShader, nullptr, 0);

		myContext->PSSetConstantBuffers(1, 1, &myObjectBuffer);
		myContext->PSSetShaderResources(1, 3, &modelData.myTexture[0]);
		myContext->PSSetSamplers(0, 1, &modelData.mySamplerState);
		myContext->PSSetShader(modelData.myPixelShader, nullptr, 0);

		myContext->DrawIndexed(modelData.myNumberOfIndicies, 0, 0);

	}
	// MODELCOMPONENT END


	for (CModelInstance* instance : aModelList) {
		CModel* model = instance->GetModel();
		CModel::SModelData modelData = model->GetModelData();

		myObjectBufferData.myToWorld = instance->GetTransform();

		D3D11_MAPPED_SUBRESOURCE PSbufferData;
		ZeroMemory(&PSbufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
		ENGINE_HR_MESSAGE(myContext->Map(myObjectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &PSbufferData), "Object Buffer could not be mapped.");

		CopyMemory(PSbufferData.pData, &myObjectBufferData, sizeof(SObjectBufferData));
		myContext->Unmap(myObjectBuffer, 0);

		memcpy(myBoneBufferData.myBones, instance->GetBones().data(), sizeof(SlimMatrix44) * 64);

		D3D11_MAPPED_SUBRESOURCE VSBufferData;
		ZeroMemory(&VSBufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
		ENGINE_HR_MESSAGE(myContext->Map(myBoneBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &VSBufferData), "Bone Buffer could not be mapped.");

		CopyMemory(VSBufferData.pData, &myBoneBufferData, sizeof(SBoneBufferData));
		myContext->Unmap(myBoneBuffer, 0);

		myContext->IASetPrimitiveTopology(modelData.myPrimitiveTopology);
		myContext->IASetInputLayout(modelData.myInputLayout);
		myContext->IASetVertexBuffers(0, 1, &modelData.myVertexBuffer, &modelData.myStride, &modelData.myOffset);
		myContext->IASetIndexBuffer(modelData.myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		myContext->VSSetConstantBuffers(1, 1, &myObjectBuffer);
		myContext->VSSetConstantBuffers(2, 1, &myBoneBuffer);
		myContext->VSSetShader(modelData.myVertexShader, nullptr, 0);

		myContext->PSSetConstantBuffers(1, 1, &myObjectBuffer);
		myContext->PSSetShaderResources(1, 3, &modelData.myTexture[0]);
		myContext->PSSetSamplers(0, 1, &modelData.mySamplerState);
		myContext->PSSetShader(modelData.myPixelShader, nullptr, 0);

		myContext->DrawIndexed(modelData.myNumberOfIndicies, 0, 0);
	}
}