#pragma once
#include <d3d11.h>
#include <string>

class CEnvironmentLight;
class CPointLight;

class CLightFactory {
public:
	static CLightFactory* GetInstance();

	CLightFactory();
	~CLightFactory();
	bool Init(ID3D11Device* aDevice);
	CEnvironmentLight* CreateEnvironmentLight(std::string aFilePath);
	CPointLight* CreatePointLight();

private:
	ID3D11Device* myDevice;
};

