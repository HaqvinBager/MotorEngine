#include "stdafx.h"
#include "SpriteRenderer.h"
#include "Sprite.h"
#include "SpriteInstance.h"
#include "AnimatedUIElement.h"
#include "DDSTextureLoader.h"

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

    myDevice = aFramework->GetDevice();
    if (!myDevice) {
        return false;
    }

    D3D11_BUFFER_DESC bufferDescription = { 0 };
    bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
    bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    bufferDescription.ByteWidth = sizeof(SObjectBufferData);
    ENGINE_HR_BOOL_MESSAGE(myDevice->CreateBuffer(&bufferDescription, nullptr, &myObjectBuffer), "Object Buffer could not be created.");

    bufferDescription.ByteWidth = sizeof(SObjectBufferData);
    ENGINE_HR_BOOL_MESSAGE(myDevice->CreateBuffer(&bufferDescription, nullptr, &myTextureScrollingBuffer), "Texture Scrolling Buffer could not be created.");

    std::ifstream vsFile;
    vsFile.open("SpriteVertexShader.cso", std::ios::binary);
    std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
    ID3D11VertexShader* vertexShader;
    ENGINE_HR_BOOL_MESSAGE(myDevice->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader), "Vertex Shader could not be created.");
    vsFile.close();
    mySpriteVertexShader = vertexShader;

    std::ifstream gsFile;
    gsFile.open("SpriteGeometryShader.cso", std::ios::binary);
    std::string gsData = { std::istreambuf_iterator<char>(gsFile), std::istreambuf_iterator<char>() };
    ID3D11GeometryShader* geometryShader;
    ENGINE_HR_BOOL_MESSAGE(myDevice->CreateGeometryShader(gsData.data(), gsData.size(), nullptr, &geometryShader), "Geometry Shader could not be created.");
    gsFile.close();
    mySpriteGeometryShader = geometryShader;

    std::ifstream psFile;
    psFile.open("SpritePixelShader.cso", std::ios::binary);
    std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
    ID3D11PixelShader* pixelShader;
    ENGINE_HR_BOOL_MESSAGE(myDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader), "Pixel Shader could not be created.");
    psFile.close();
    mySpritePixelShader = pixelShader;

    psFile;
    psFile.open("SpriteVFXTextureBlendingPixelShader.cso", std::ios::binary);
    psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
    ID3D11PixelShader* pixelShader2;
    ENGINE_HR_BOOL_MESSAGE(myDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader2), "Pixel Shader could not be created.");
    psFile.close();
    myAnimatedUIPixelShader = pixelShader2;

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
        myContext->VSSetShader(mySpriteVertexShader, nullptr, 0);

        myContext->GSSetShader(mySpriteGeometryShader, nullptr, 0);

        myContext->PSSetSamplers(0, 1, &spriteData.mySampler);
        myContext->PSSetConstantBuffers(0, 1, &myObjectBuffer);
        myContext->PSSetShaderResources(0, 1, &spriteData.myTexture);
        myContext->PSSetShader(mySpritePixelShader, nullptr, 0);

        myContext->Draw(3, 0);

        //Reset Resources
        ID3D11ShaderResourceView* nullView = NULL;
        myContext->PSSetShaderResources(0, 1, &nullView);

        myContext->GSSetShader(nullptr, nullptr, 0);
    }
}

void CSpriteRenderer::Render(std::vector<CAnimatedUIElement*>& someAnimatedElements) {
    for (unsigned int instanceIndex = 0; instanceIndex < someAnimatedElements.size(); ++instanceIndex)
    {
        ID3D11ShaderResourceView* texture1 = nullptr;
        ID3D11ShaderResourceView* texture2 = nullptr;
        ID3D11ShaderResourceView* texture3 = nullptr;
        ID3D11ShaderResourceView* texture4 = nullptr;

        DirectX::CreateDDSTextureFromFile(myDevice, someAnimatedElements[instanceIndex]->myTexturePaths[0].c_str(), nullptr, &texture1);
        DirectX::CreateDDSTextureFromFile(myDevice, someAnimatedElements[instanceIndex]->myTexturePaths[1].c_str(), nullptr, &texture2);
        DirectX::CreateDDSTextureFromFile(myDevice, someAnimatedElements[instanceIndex]->myTexturePaths[2].c_str(), nullptr, &texture3);
        DirectX::CreateDDSTextureFromFile(myDevice, someAnimatedElements[instanceIndex]->myTexturePaths[3].c_str(), nullptr, &texture4);

        std::array<ID3D11ShaderResourceView*, 4> textures;
        textures[0] = texture1;
        textures[1] = texture2;
        textures[2] = texture3;
        textures[3] = texture4;

        CSpriteInstance* instance = someAnimatedElements[instanceIndex]->mySpriteInstance;
        CSprite* sprite = instance->GetSprite();

        myObjectBufferData.myPosition = instance->GetPosition();
        myObjectBufferData.myColor = instance->GetColor();
        myObjectBufferData.myUVRect = instance->GetUVRect();
        myObjectBufferData.mySize = instance->GetSize();

        BindBuffer(myObjectBuffer, myObjectBufferData, "Object Buffer");

        float scrollingScale = 0.15f;
        myTextureScrollingData.scrollSpeed1 = { 0.32f, 0.2f };
        myTextureScrollingData.scrollSpeed2 = { 0.0f, -0.5f };
        myTextureScrollingData.scrollSpeed3 = { -0.31f, 0.0f };
        myTextureScrollingData.scrollSpeed4 = { 0.0f, 0.0f };
        myTextureScrollingData.uvScale1 = 1.2f;
        myTextureScrollingData.uvScale2 = 0.3f;
        myTextureScrollingData.uvScale3 = 0.7f;
        myTextureScrollingData.uvScale4 = 1.0f;
        myTextureScrollingData.scrollTimer = CTimer::Time() * scrollingScale; //..is now!
        myTextureScrollingData.opacityStrength = 0.2f;
        BindBuffer<STextureScrollingData>(myTextureScrollingBuffer, myTextureScrollingData, "Texture Scrolling Buffer");

        CSprite::SSpriteData spriteData = sprite->GetSpriteData();

        myContext->IASetPrimitiveTopology(spriteData.myPrimitiveTopology);
        myContext->IASetInputLayout(nullptr);
        myContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
        myContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

        myContext->VSSetConstantBuffers(0, 1, &myObjectBuffer);
        myContext->VSSetShader(mySpriteVertexShader, nullptr, 0);

        myContext->GSSetShader(mySpriteGeometryShader, nullptr, 0);

        myContext->PSSetSamplers(0, 1, &spriteData.mySampler);
        myContext->PSSetConstantBuffers(0, 1, &myObjectBuffer);
        myContext->PSSetConstantBuffers(1, 1, &myTextureScrollingBuffer);
        myContext->PSSetShaderResources(0, 4, &textures[0]);
        //myContext->PSSetShaderResources(0, 1, &spriteData.myTexture);
        myContext->PSSetShader(myAnimatedUIPixelShader, nullptr, 0);

        myContext->Draw(3, 0);

        //Reset Resources
        ID3D11ShaderResourceView* nullView = NULL;
        myContext->PSSetShaderResources(0, 1, &nullView);

        myContext->GSSetShader(nullptr, nullptr, 0);
    }
}
