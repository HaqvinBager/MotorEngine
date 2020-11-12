#pragma once
#include <vector>
#include "SimpleMath.h"
#include "ModelMath.h"
#include <wrl.h>


class CModelComponent;
class CEngine;
class CCamera;
class CEnvironmentLight;
class CPointLight;
class CGameObject;
class CLineInstance;
class CModel;
class CCameraComponent;
struct SLineTime;

class CForwardRenderer
{
public:
	CForwardRenderer();
	~CForwardRenderer();

	bool Init(CDirectXFramework* aFramework);
	void Render(CEnvironmentLight* anEnvironmentLight, std::vector<std::pair<unsigned int, std::array<CPointLight*, 8>>> aModelPointLightList, CCameraComponent* aCamera, std::vector<CGameObject*>& aGameObjectList);
	void RenderLines(CCameraComponent* aCamera, const std::vector<SLineTime>& aLineList);
	void RenderOutline(CCameraComponent* aCamera, CGameObject* aModelInstance, CModel* someOutlineModelData);
	void RenderLineInstances(CCameraComponent* aCamera, const std::vector<CLineInstance*>& aLineList);

private:
	template<class T>
	void BindBuffer(ID3D11Buffer* aBuffer, T& someBufferData, std::string aBufferType)
	{
		D3D11_MAPPED_SUBRESOURCE bufferData;
		ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
		std::string errorMessage = aBufferType + " could not be bound.";
		ENGINE_HR_MESSAGE(myContext->Map(aBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData), errorMessage.c_str());

		memcpy(bufferData.pData, &someBufferData, sizeof(T));
		myContext->Unmap(aBuffer, 0);
	}

private:
	struct SFrameBufferData
	{
		DirectX::SimpleMath::Matrix myToCamera;
		DirectX::SimpleMath::Matrix myToProjection;
		DirectX::SimpleMath::Vector4 myCameraPosition;
		DirectX::SimpleMath::Vector4 myDirectionalLightDirection;
		DirectX::SimpleMath::Vector4 myDirectionalLightColor;
	} myFrameBufferData;

	static_assert((sizeof(SFrameBufferData) % 16) == 0, "CB size not padded correctly");

	struct SObjectBufferData
	{
		DirectX::SimpleMath::Matrix myToWorld; //4 bytes * 16 (64 bytes)

		struct SPointLightBufferData {
			DirectX::SimpleMath::Vector4 myPositionAndIntensity;
			DirectX::SimpleMath::Vector4 myColorAndRange;
		} myPointLights[8];

		unsigned int myNumberOfUsedPointLights;
		unsigned int myPadding[3];
	} myObjectBufferData;

	static_assert((sizeof(SObjectBufferData) % 16) == 0, "CB size not padded correctly");


	struct SBoneBufferData {
		SlimMatrix44 myBones[64];
	} myBoneBufferData;

	static_assert((sizeof(SBoneBufferData) % 16) == 0, "CB size not padded correctly");

private:

	ID3D11DeviceContext* myContext;
	ID3D11Buffer* myFrameBuffer;
	ID3D11Buffer* myObjectBuffer;
	ID3D11Buffer* myBoneBuffer;


};

//struct SObjectLightBufferData
//{
//	DirectX::SimpleMath::Matrix myToWorld; //4 bytes * 1
//	DirectX::SimpleMath::Vector3 myPosition; //4 bytes *
//	float myRange;							//4 bytes (1
//	DirectX::SimpleMath::Vector4 myColorAndIntensity; //
//	float myNumberOfUsedPointLights; //4 bytes
//	float padding[3]; // 4 * 3 Bytes (12)
//} myLightBufferData;
