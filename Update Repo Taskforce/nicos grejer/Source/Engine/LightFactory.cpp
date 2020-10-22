#include "stdafx.h"
#include "LightFactory.h"
#include "Engine.h"
#include "EnvironmentLight.h"
#include "PointLight.h"

CLightFactory* CLightFactory::ourInstance = nullptr;

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

bool CLightFactory::Init(CEngine& anEngine)
{
    myEngine = &anEngine;
    return true;
}

CEnvironmentLight* CLightFactory::CreateEnvironmentLight(std::string aFilePath)
{
    CEnvironmentLight* light = new CEnvironmentLight();
    if (!light->Init(myEngine->myFramework, aFilePath)) {
        return nullptr;
    }
    //light->Init(myEngine->myFramework, aFilePath);
    return light;
}

CPointLight* CLightFactory::CreatePointLight()
{
    CPointLight* light = new CPointLight();
    if (!light->Init()) {
        return nullptr;
    }
    return light;
}

CLightFactory* CLightFactory::GetInstance()
{
    return ourInstance;
}
