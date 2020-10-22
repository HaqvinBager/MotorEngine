#include "pch.h"
#include "LightFactory.hpp"

#include "DirectX11Framework.hpp"
#include "EnvironmentLight.hpp"
#include "DirectionalLight.hpp"
#include "PointLight.hpp"
//DirectXTK
#include "DDSTextureLoader.h"
#include "SimpleMath.h"

#if _DEBUG
#include <iostream>
#endif

LightFactory* LightFactory::ourInstance = nullptr;

LightFactory::LightFactory()
	: myDevice(nullptr)
{}

LightFactory::~LightFactory()
{}

void LightFactory::Create()
{
	if (!ourInstance)
	{
		ourInstance = new LightFactory();
	}
}

LightFactory& LightFactory::GetInstance()
{
	assert(ourInstance != nullptr && "No instance Created!");
	return *ourInstance;
}

bool LightFactory::Init(DirectX11Framework* aFramework)
{
	if (!aFramework)
	{
		assert(false && "DirectX11Framework was null, make sure that you init after the framework has been ini!");
		return false;
	}
	myDevice = aFramework->GetDevice();

	return true;
}

void LightFactory::Destroy()
{
	if (ourInstance)
	{
		for (auto it = myPointLights.begin(); it != myPointLights.end(); ++it)
		{
			delete it->second;
			it->second = nullptr;
		}
		myPointLights.clear();
		SAFE_DELETE(ourInstance)
	}
}

EnvironmentLight* LightFactory::CreateEnvironmentLight(const wchar_t* aCubeMapPath)
{	
	HRESULT hResult;
	
	ID3D11ShaderResourceView* cubeMapShaderResourceView = nullptr;
	hResult = DirectX::CreateDDSTextureFromFile(myDevice, aCubeMapPath, nullptr, &cubeMapShaderResourceView);
	if (FAILED(hResult))
	{
		assert(false && "Failed to load cubemap!");
		return nullptr;
	}
	
	return (new EnvironmentLight(cubeMapShaderResourceView));
}

DirectionalLight* LightFactory::CreateDirectionalLight(const DirectX::SimpleMath::Vector3& aDirection, const DirectX::SimpleMath::Vector4& aColor)
{
	DirectionalLight* dirLight = new DirectionalLight();
	dirLight->SetDirection(aDirection);
	dirLight->SetColorAndIntensity(aColor);
	return dirLight;
}

PointLight* LightFactory::CreatePointLight(const DirectX::SimpleMath::Vector4& aColorAndIntensity, const float& aRange, const std::string& aName)
{
	if (myPointLights.count(aName) != 0)
	{
#if _DEBUG
		std::cout << "Returning PointLight with name: " << aName << std::endl;
#endif
		return myPointLights[aName];
	}
#if _DEBUG
	std::cout << "Creating new PointLight with name: " << aName << std::endl;
#endif
	PointLight* pointLight = new PointLight();
	pointLight->SetColorAndIntensity(aColorAndIntensity);
	pointLight->SetRange(aRange);
	myPointLights.emplace(aName, pointLight);
	return pointLight;
}

PointLight* LightFactory::GetPointLight(const std::string& aName)
{
	if (myPointLights.count(aName) != 0)
	{
#if _DEBUG
		std::cout << "Returning PointLight with name: " << aName << std::endl;
#endif
		return myPointLights[aName];
	}

#if _DEBUG
	std::cout << "PointLight with name: " << aName << ", not found. Creating one with color: White, range: 1 " << std::endl;
#endif
	return CreatePointLight({1.f,1.f,1.f,1.f}, 1.f, aName);
}
