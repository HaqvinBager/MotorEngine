#include "stdafx.h"
#include "LightFactory.h"
#include <DDSTextureLoader.h>
#include "Engine.h"
#include "EnvironmentLight.h"
#include "PointLight.h"

CLightFactory* CLightFactory::ourInstance = nullptr;
CLightFactory* CLightFactory::GetInstance()
{
    return ourInstance;
}
bool CLightFactory::Init(CEngine& anEngine)
{
    myEngine = &anEngine;
    return true;
}
CEnvironmentLight* CLightFactory::CreateEnvironmentLight(std::string aCubeMapPath)
{
	CEnvironmentLight* light = new CEnvironmentLight();
    if (!light->Init(myEngine->myFramework, aCubeMapPath))
    {
        return nullptr; //TODO INIT FAILED
    }
    
    return light;
}

CPointLight* CLightFactory::CreatePointLight() {
    CPointLight* pointLight = new CPointLight();
    if (!pointLight->Init()) {
        return nullptr;
    }

    return pointLight;
}

CLightFactory::CLightFactory()
{
    ourInstance = this;
    myEngine = nullptr;
}

CLightFactory::~CLightFactory()
{
    ourInstance = nullptr;
    myEngine = nullptr;
}
