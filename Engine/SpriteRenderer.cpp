#include "stdafx.h"
#include "SpriteRenderer.h"
#include "Sprite.h"
#include "SpriteInstance.h"

CSpriteRenderer::CSpriteRenderer() : myContext(nullptr), myObjectBuffer()
{
}

CSpriteRenderer::~CSpriteRenderer()
{
}

bool CSpriteRenderer::Init(CDirectXFramework* aFramework)
{
	if (!aFramework) {
		return false;
	}

	myContext = aFramework->GetContext();
	if (!myContext) {
		return false;
	}

    ID3D11Device* device = aFramework->GetDevice();
    if (!device) {
        return false;
    }

    D3D11_BUFFER_DESC bufferDescription = { 0 };
    bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
    bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    bufferDescription.ByteWidth = sizeof(SObjectBufferData);
    ENGINE_HR_BOOL_MESSAGE(device->CreateBuffer(&bufferDescription, nullptr, &myObjectBuffer), "Object Buffer could not be created.");

	return true;
}

void CSpriteRenderer::Render(std::vector<CSpriteInstance*>& aSpriteList)
{
    for (unsigned int instanceIndex = 0; instanceIndex < aSpriteList.size(); ++instanceIndex)
    {
        CSpriteInstance* instance = aSpriteList[instanceIndex];
        CSprite* sprite = instance->GetSprite();

        myObjectBufferData.myPosition = instance->GetPosition();
        myObjectBufferData.myColor = instance->GetColor();
        myObjectBufferData.myUVRect = instance->GetUVRect();
        myObjectBufferData.mySize = instance->GetSize();

        BindBuffer(myObjectBuffer, myObjectBufferData, "Object Buffer");

        CSprite::SSpriteData spriteData = sprite->GetSpriteData();

        myContext->IASetPrimitiveTopology(spriteData.myPrimitiveTopology);
        myContext->IASetInputLayout(nullptr);
        myContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
        myContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
        
        myContext->VSSetConstantBuffers(0, 1, &myObjectBuffer);
        myContext->VSSetShader(spriteData.myVertexShader, nullptr, 0);

        myContext->GSSetShader(spriteData.myGeometryShader, nullptr, 0);

        myContext->PSSetSamplers(0, 1, &spriteData.mySampler);
        myContext->PSSetConstantBuffers(0, 1, &myObjectBuffer);
        myContext->PSSetShaderResources(0, 1, &spriteData.myTexture);
        myContext->PSSetShader(spriteData.myPixelShader, nullptr, 0);

        myContext->Draw(3, 0);

        //Reset Resources
        ID3D11ShaderResourceView* nullView = NULL;
        myContext->PSSetShaderResources(0, 1, &nullView);

        myContext->GSSetShader(nullptr, nullptr, 0);
    }
}
