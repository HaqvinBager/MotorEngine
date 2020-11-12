#pragma once
#include <vector>
#include "SimpleMath.h"

struct ID3D11DeviceContext;
struct ID3D11Buffer;
class CDirectXFramework;
class CCamera;
class CVFXInstance;
class CGameObject;
class CCameraComponent;

class CVFXRenderer {
public:
	CVFXRenderer();
	~CVFXRenderer();

	bool Init(CDirectXFramework* aFramework);

	void Render(CCameraComponent* aCamera, std::vector<CGameObject*>& aGameObjectList);

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
	struct SFrameBufferData {
		DirectX::SimpleMath::Matrix myToCamera;
		DirectX::SimpleMath::Matrix myToProjection;
	} myFrameBufferData;

	struct SObjectBufferData {
		DirectX::SimpleMath::Matrix myToWorld;
	} myObjectBufferData;

	struct STextureScrollingData {
		DirectX::SimpleMath::Vector2 scrollSpeed1;
		DirectX::SimpleMath::Vector2 scrollSpeed2;
		DirectX::SimpleMath::Vector2 scrollSpeed3;
		DirectX::SimpleMath::Vector2 scrollSpeed4;

		float uvScale1;
		float uvScale2;
		float uvScale3;
		float uvScale4;
		float scrollTimer;
		float opacityStrength;
		DirectX::SimpleMath::Vector2 padding;
	} myTextureScrollingData;

private:
	ID3D11DeviceContext* myContext;
	ID3D11Buffer* myFrameBuffer;
	ID3D11Buffer* myObjectBuffer;
	ID3D11Buffer* myTextureScrollingBuffer;

	float myTime;
};

