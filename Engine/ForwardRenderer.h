#pragma once
#include <vector>
#include "SimpleMath.h"
#include "ModelMath.h"
#include <wrl.h>


class CModelInstance;
class CEngine;
class CCamera;
class CEnvironmentLight;
class CPointLight;

class CForwardRenderer
{
public:
	CForwardRenderer();
	~CForwardRenderer();

	bool Init(CEngine& anEngine);
	void Render(CEnvironmentLight* anEnvironmentLight, CCamera* aCamera, std::vector<CModelInstance*>& aModelList);

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