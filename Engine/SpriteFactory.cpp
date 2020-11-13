#include "stdafx.h"
#include "SpriteFactory.h"
#include "Sprite.h"
#include "DDSTextureLoader.h"
#include "AnimatedUIElement.h"

CSpriteFactory* CSpriteFactory::ourInstance = nullptr;

CSpriteFactory::CSpriteFactory() : myVertexShader(nullptr), myPixelShader(nullptr), myGeometryShader(nullptr)
{
	ourInstance = this;
	myFramework = nullptr;
}

CSpriteFactory::~CSpriteFactory()
{
	ourInstance = nullptr;
	myFramework = nullptr;
}

bool CSpriteFactory::Init(CDirectXFramework* aFramework)
{
	if (!aFramework) {
		return false;
	}

	myFramework = aFramework;

	//Start Shaders
	std::ifstream vsFile;
	vsFile.open("SpriteVertexShader.cso", std::ios::binary);
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	ENGINE_HR_MESSAGE(myFramework->GetDevice()->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &myVertexShader), "Sprite Vertex Shader could not be created.");
	vsFile.close();

	std::ifstream psFile;
	psFile.open("SpritePixelShader.cso", std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	ENGINE_HR_MESSAGE(myFramework->GetDevice()->CreatePixelShader(psData.data(), psData.size(), nullptr, &myPixelShader), "Sprite Pixel Shader could not be created.");
	psFile.close();

	std::ifstream gsFile;
	gsFile.open("SpriteGeometryShader.cso", std::ios::binary);
	std::string gsData = { std::istreambuf_iterator<char>(gsFile), std::istreambuf_iterator<char>() };
	ENGINE_HR_MESSAGE(myFramework->GetDevice()->CreateGeometryShader(gsData.data(), gsData.size(), nullptr, &myGeometryShader), "Sprite Geometry Shader could not be created.");
	gsFile.close();
	//End Shaders

	return true;
}

CSprite* CSpriteFactory::LoadSprite(std::string aTexturePath)
{
	//Start Sampler
	ID3D11SamplerState* sampler;
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	ENGINE_HR_MESSAGE(myFramework->GetDevice()->CreateSamplerState(&samplerDesc, &sampler), "Sampler could not be created.");
	//End Sampler

	ID3D11ShaderResourceView* shaderResourceView = GetShaderResourceView(myFramework->GetDevice(), aTexturePath);

	CSprite* sprite = new CSprite();
	if (!sprite) {
		return nullptr;
	}

	CSprite::SSpriteData spriteData;

	spriteData.myVertexShader = myVertexShader;
	spriteData.myPixelShader = myPixelShader;
	spriteData.myGeometryShader = myGeometryShader;
	spriteData.mySampler = sampler;
	spriteData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	spriteData.myTexture = shaderResourceView;

	sprite->Init(spriteData);
	return sprite;
}

//CSprite* CSpriteFactory::LoadVFXSprite(std::string aTexturePath)
//{
//	//Start Sampler
//	ID3D11SamplerState* sampler;
//	D3D11_SAMPLER_DESC samplerDesc = {};
//	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//	ENGINE_HR_MESSAGE(myFramework->GetDevice()->CreateSamplerState(&samplerDesc, &sampler), "Sampler could not be created.");
//	//End Sampler
//
//	ID3D11ShaderResourceView* shaderResourceView = GetShaderResourceView(myFramework->GetDevice(), aTexturePath);
//
//	ID3D11ShaderResourceView* textureOneShaderResourceView = GetShaderResourceView(myFramework->GetDevice(), vfxBaseData.texturePathOne);
//	ID3D11ShaderResourceView* textureTwoShaderResourceView = GetShaderResourceView(myFramework->GetDevice(), vfxBaseData.texturePathTwo);
//	ID3D11ShaderResourceView* textureThreeShaderResourceView = GetShaderResourceView(myFramework->GetDevice(), vfxBaseData.texturePathThree);
//	ID3D11ShaderResourceView* textureFourShaderResourceView = GetShaderResourceView(myFramework->GetDevice(), vfxBaseData.texturePathMask);
//
//	CSprite* sprite = new CSprite();
//	if (!sprite) {
//		return nullptr;
//	}
//
//	SAnimatedSpriteData spriteData;
//
//	spriteData.myVertexShader = myVertexShader;
//	spriteData.myPixelShader = myPixelShader;
//	spriteData.myGeometryShader = myGeometryShader;
//	spriteData.mySampler = sampler;
//	spriteData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
//	spriteData.myTexture = shaderResourceView;
//
//	sprite->Init(spriteData);
//	return sprite;
//}

CSprite* CSpriteFactory::GetSprite(std::string aTexturePath)
{
	if (mySpriteMap.find(aTexturePath) == mySpriteMap.end())
	{
		return LoadSprite(aTexturePath);
	}
	return mySpriteMap.at(aTexturePath);
}

//CSprite* CSpriteFactory::GetVFXSprite(std::string aTexturePath)
//{
//	if (myVFXSpriteMap.find(aTexturePath) == myVFXSpriteMap.end())
//	{
//		return LoadVFXSprite(aTexturePath);
//	}
//	return myVFXSpriteMap.at(aTexturePath);
//}

ID3D11ShaderResourceView* CSpriteFactory::GetShaderResourceView(ID3D11Device* aDevice, std::string aTexturePath) {
	ID3D11ShaderResourceView* shaderResourceView;

	wchar_t* widePath = new wchar_t[aTexturePath.length() + 1];
	std::copy(aTexturePath.begin(), aTexturePath.end(), widePath);
	widePath[aTexturePath.length()] = 0;

	////==ENABLE FOR TEXTURE CHECKING==
	//ENGINE_HR_MESSAGE(DirectX::CreateDDSTextureFromFile(aDevice, widePath, nullptr, &shaderResourceView), aTexturePath.append(" could not be found.").c_str());
	////===============================

	//==DISABLE FOR TEXTURE CHECKING==
	HRESULT result;
	result = DirectX::CreateDDSTextureFromFile(aDevice, widePath, nullptr, &shaderResourceView);
	if (FAILED(result))
		DirectX::CreateDDSTextureFromFile(aDevice, L"ErrorTexture.dds", nullptr, &shaderResourceView);
	//================================

	delete[] widePath;
	return shaderResourceView;
}

CSpriteFactory* CSpriteFactory::GetInstance()
{
	return ourInstance;
}

