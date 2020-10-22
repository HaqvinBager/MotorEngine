#pragma once

#include <map>

namespace DirectX
{
	namespace SimpleMath
	{
		struct Vector3;
		struct Vector4;
	}
}

class DirectX11Framework;
struct ID3D11Device;

class EnvironmentLight;
class DirectionalLight;
class PointLight;

#define DEFAULT_POINTLIGHT "PointLight"

class LightFactory
{
public:
	static void			 Create();
	static LightFactory& GetInstance();
	bool				 Init(DirectX11Framework* aFramework);
	void				 Destroy();

	EnvironmentLight* CreateEnvironmentLight(const wchar_t* aCubeMapPath);
	DirectionalLight* CreateDirectionalLight(const DirectX::SimpleMath::Vector3& aDirection, const DirectX::SimpleMath::Vector4& aColor);
	PointLight* CreatePointLight(const DirectX::SimpleMath::Vector4& aColorAndIntensity, const float& aRange, const std::string& aName = DEFAULT_POINTLIGHT);

	PointLight* GetPointLight(const std::string& aName);

private:
	LightFactory();
	~LightFactory();
	static LightFactory* ourInstance;
	
	ID3D11Device* myDevice;

	std::map<std::string, PointLight*> myPointLights;
};

