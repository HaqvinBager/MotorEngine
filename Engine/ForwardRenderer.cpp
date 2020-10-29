#include "stdafx.h"
#include "ForwardRenderer.h"
#include "Engine.h"
#include "Model.h"
#include "Camera.h"
#include "EnvironmentLight.h"
#include "ModelMath.h"
#include "GameObject.h"
#include "ModelComponent.h"
#include "AnimationComponent.h"
#include "TransformComponent.h"
#include "PointLight.h"
#include "Line.h"
#include "LineInstance.h"
#include "Debug.h"

namespace SM = DirectX::SimpleMath;

CForwardRenderer::CForwardRenderer() : myContext(nullptr), myFrameBuffer(nullptr), myFrameBufferData(), myObjectBuffer(nullptr), myObjectBufferData(), myBoneBuffer(nullptr), myBoneBufferData() {

}

CForwardRenderer::~CForwardRenderer() {
	myContext = nullptr;
}

bool CForwardRenderer::Init(CDirectXFramework* aFramework) {
	if (!aFramework)
		return false;

	myContext = aFramework->GetContext();
	if (!myContext) {
		return false;
	}

	ID3D11Device* device = aFramework->GetDevice();
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

void CForwardRenderer::Render(CEnvironmentLight* anEnvironmentLight, std::vector<std::pair<unsigned int, std::array<CPointLight*, 8>>> aModelPointLightList, CCamera* aCamera, std::vector<CGameObject*>& aGameObjectList)
{
	myFrameBufferData.myToCamera = aCamera->GetTransform().Invert();
	myFrameBufferData.myToProjection = aCamera->GetProjection();
	myFrameBufferData.myCameraPosition = aCamera->GetPosition();
	myFrameBufferData.myDirectionalLightDirection = anEnvironmentLight->GetDirection();
	myFrameBufferData.myDirectionalLightColor = anEnvironmentLight->GetColor();

	BindBuffer(myFrameBuffer, myFrameBufferData, "Frame Buffer");

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

		if (gameobject->GetComponent<CModelComponent>()->GetMyModel() == nullptr)
			continue;

		CModel* model = gameobject->GetComponent<CModelComponent>()->GetMyModel();

		CModel::SModelData modelData = model->GetModelData();

		myObjectBufferData.myToWorld = gameobject->GetComponent<CTransformComponent>()->Transform();

		BindBuffer(myObjectBuffer, myObjectBufferData, "Object Buffer");

		if (gameobject->GetComponent<CAnimationComponent>() != nullptr) {
			memcpy(myBoneBufferData.myBones, gameobject->GetComponent<CAnimationComponent>()->GetBones().data(), sizeof(SlimMatrix44) * 64);

			BindBuffer(myBoneBuffer, myBoneBufferData, "Bone Buffer");

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

	/*int modelIndex = 0;
	for (CModelInstance* instance : aModelList) {
		CModel* model = instance->GetModel();
		CModel::SModelData modelData = model->GetModelData();

		myObjectBufferData.myToWorld = instance->GetTransform();

		for (unsigned int i = 0; i < aModelPointLightList[modelIndex].first; ++i) {
			SM::Vector3 position = aModelPointLightList[modelIndex].second[i]->GetPosition();
			SM::Vector3 color = aModelPointLightList[modelIndex].second[i]->GetColor();
			myObjectBufferData.myPointLights[i].myPositionAndIntensity = { position.x, position.y, position.z, aModelPointLightList[modelIndex].second[i]->GetIntensity() };
			myObjectBufferData.myPointLights[i].myColorAndRange = { color.x, color.y, color.z, aModelPointLightList[modelIndex].second[i]->GetRange() };
		}
		myObjectBufferData.myNumberOfUsedPointLights = aModelPointLightList[modelIndex].first;

		++modelIndex;

		BindBuffer(myObjectBuffer, myObjectBufferData, "Object Buffer");

		memcpy(myBoneBufferData.myBones, instance->GetBones().data(), sizeof(SlimMatrix44) * 64);

		BindBuffer(myBoneBuffer, myBoneBufferData, "Bone Buffer");

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
	}*/
}

void CForwardRenderer::RenderLines(CCamera* aCamera, const std::vector<SLineTime>& aLineList) {

	namespace SM = DirectX::SimpleMath;
	myFrameBufferData.myToCamera = aCamera->GetTransform().Invert();
	myFrameBufferData.myToProjection = aCamera->GetProjection();

	BindBuffer(myFrameBuffer, myFrameBufferData, "Frame Buffer");

	myContext->VSSetConstantBuffers(0, 1, &myFrameBuffer);
	myContext->PSSetConstantBuffers(0, 1, &myFrameBuffer);

	for (const SLineTime& instance : aLineList)
	{
		CLine::SLineData lineData = instance.myLine.GetLine()->GetLineData();

		myObjectBufferData.myToWorld = instance.myLine.GetTransform();

		BindBuffer(myObjectBuffer, myObjectBufferData, "Object Buffer");

		myContext->IASetPrimitiveTopology(lineData.myPrimitiveTopology);
		myContext->IASetInputLayout(lineData.myInputLayout);
 		myContext->IASetVertexBuffers(0, 1, &lineData.myVertexBuffer, &lineData.myStride, &lineData.myOffset);
		myContext->IASetIndexBuffer(lineData.myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		myContext->VSSetConstantBuffers(1, 1, &myObjectBuffer);
		myContext->VSSetShader(lineData.myVertexShader, nullptr, 0);

		myContext->PSSetShader(lineData.myPixelShader, nullptr, 0);

		//myContext->DrawIndexed(lineData.myNumberOfIndices, 0, 0);
		myContext->Draw(lineData.myNumberOfVertices, 0);
	}
}
