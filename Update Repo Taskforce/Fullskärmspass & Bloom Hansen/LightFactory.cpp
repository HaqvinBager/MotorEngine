#include "stdafx.h"
#include "LightFactory.h"
#include "EnvironmentLight.h"
#include "PointLight.h"

CLightFactory* CLightFactory::GetInstance() {
	static CLightFactory* instance = new CLightFactory();
	return instance;
}

CLightFactory::CLightFactory() {
}

CLightFactory::~CLightFactory() {
}

bool CLightFactory::Init(ID3D11Device* aDevice) {
	myDevice = aDevice;
	return true;
}

CEnvironmentLight* CLightFactory::CreateEnvironmentLight(std::string aFilePath) {
	CEnvironmentLight* environmentLight = new CEnvironmentLight();
	if (!environmentLight->Init(aFilePath, myDevice)) {
		return nullptr;
	}

	return environmentLight;
}

CPointLight* CLightFactory::CreatePointLight() {
	CPointLight* pointLight = new CPointLight();
	if (!pointLight->Init()) {
		return nullptr;
	}

	return pointLight;
}
