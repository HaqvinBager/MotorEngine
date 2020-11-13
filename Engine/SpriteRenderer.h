#pragma once
#include <vector>
#include "SimpleMath.h"

struct ID3D11DeviceContext;
class CDirectXFramework;
class CSpriteInstance;
class CAnimatedUIElement;

class CSpriteRenderer
{
public:
	CSpriteRenderer();
	~CSpriteRenderer();

	bool Init(CDirectXFramework* aFramework);

	void Render(std::vector<CSpriteInstance*>& aSpriteList);
	void Render(std::vector<CAnimatedUIElement*>& someAnimatedElements);

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
	ID3D11Device* myDevice;
	ID3D11DeviceContext* myContext;
	ID3D11Buffer* myObjectBuffer;
	ID3D11Buffer* myTextureScrollingBuffer;
	ID3D11VertexShader* mySpriteVertexShader;
	ID3D11GeometryShader* mySpriteGeometryShader;
	ID3D11PixelShader* mySpritePixelShader;
	ID3D11PixelShader* myAnimatedUIPixelShader;
};