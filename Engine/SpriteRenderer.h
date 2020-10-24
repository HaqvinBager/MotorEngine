#pragma once
#include <vector>
#include "SimpleMath.h"

struct ID3D11DeviceContext;
class CDirectXFramework;
class CSpriteInstance;

class CSpriteRenderer
{
public:
	CSpriteRenderer();
	~CSpriteRenderer();

	bool Init(CDirectXFramework* aFramework);

	void Render(std::vector<CSpriteInstance*>& aSpriteList);

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
	struct SObjectBufferData
	{
		DirectX::SimpleMath::Vector4 myPosition;
		DirectX::SimpleMath::Vector4 myColor;
		DirectX::SimpleMath::Vector4 myUVRect;
		DirectX::SimpleMath::Vector2 mySize;
		float padding[2];
	} myObjectBufferData;

private:
	ID3D11DeviceContext* myContext;
	ID3D11Buffer* myObjectBuffer;
};