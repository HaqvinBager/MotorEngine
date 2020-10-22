#pragma once
#include <vector>
#include "ModelInstance.h"
#include "..\DirectXTK\Inc\SimpleMath.h"

namespace SM = DirectX::SimpleMath;

struct ID3D11DeviceContext;
struct ID3D11Buffer;
class CDirectX11Framework;
class CCamera;
class CEnvironmentLight;
class CPointLight;

class CForwardRenderer {
public:
	CForwardRenderer();
	~CForwardRenderer();
	bool Init(CDirectX11Framework* aFramework);
	void Render(CEnvironmentLight* aEnvirontmentLight, std::vector<std::pair<unsigned int, std::array<CPointLight*, 8>>> aModelPointLightList, CCamera* aCamera, std::vector<CModelInstance*> &aModelList);

private:
	struct SFrameBufferData {
		SM::Vector4 myCameraPosition;
		SM::Matrix myToCamera;
		SM::Matrix myToProjection;
		SM::Vector4 myDirectionalLightDirection;
		SM::Vector4 myDirectionalLightColor;
		unsigned int myEnvironmentLightMipCount;
		unsigned int trash[3];
	} myFrameBufferData;

	struct SObjectBufferData {
		SM::Matrix myToWorld;

		struct SPointLightBufferData {
			SM::Vector4 myPositionAndIntensity;
			SM::Vector4 myColorAndRange;
		} myPointLights[8];

		unsigned int myNumberOfUsedPointLights;
		unsigned int myPadding[3];
	} myObjectBufferData;

	ID3D11DeviceContext* myContext;
	ID3D11Buffer* myFrameBuffer;
	ID3D11Buffer* myObjectBuffer;
};

