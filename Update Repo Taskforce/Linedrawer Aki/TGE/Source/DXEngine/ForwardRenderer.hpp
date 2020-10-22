#pragma once

#include "SimpleMath.h"
#include <vector>

class DirectX11Framework;
class Camera;
class EnvironmentLight;
class DirectionalLight;
namespace ModelSystem
{
	class Model;
	class ModelInstance;
}
class Line;
class LineInstance;

struct ID3D11DeviceContext;
struct ID3D11Buffer;

namespace Rendering
{
	class ForwardRenderer
	{
	public:
		ForwardRenderer();
		~ForwardRenderer();

		bool Init(DirectX11Framework* aFramework);
		void Render(EnvironmentLight* aEnvironmentLight, DirectionalLight* aDirectionalLight, Camera* aCamera, std::vector<ModelSystem::ModelInstance*>& aModelList);
		void RenderLines(Camera* aCamera, std::vector<LineInstance*>& aLineList);

	private:
		struct SFrameBufferData
		{
			DirectX::SimpleMath::Matrix myToCamera;
			DirectX::SimpleMath::Matrix myToProjection;

			DirectX::SimpleMath::Vector4 myDirectionalLightColor;
			DirectX::SimpleMath::Vector4 myDirectionalLightDirection;
		} myFrameBufferData;

		struct SObjectBufferData
		{
			DirectX::SimpleMath::Matrix myToWorld;
			struct SPointLightBufferData
			{
				DirectX::SimpleMath::Vector4 myPosition;
				DirectX::SimpleMath::Vector4 myColor;
				float myRange;
				//float myPadding[3];
			}myPointLights[8];
			unsigned int myNumberOfUsedPointLights;
			unsigned int myPointLightPadding[3];

		} myObjectBufferData;
	
	private:
		ID3D11DeviceContext* myDeviceContext;
		ID3D11Buffer*		 myFrameBuffer;
		ID3D11Buffer*		 myObjectBuffer;
	};
}