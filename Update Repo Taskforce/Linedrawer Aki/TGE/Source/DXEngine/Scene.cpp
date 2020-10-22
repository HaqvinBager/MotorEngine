#include "pch.h"
#include "Scene.hpp"

#include "ModelInstance.hpp"
#include "DirectionalLight.hpp"

Scene::Scene()
	: myMainCamera(nullptr)
	, myEnvironmentLight(nullptr)
	, myMainDirectionalLight(nullptr)
{}

Scene::~Scene()
{}

Scene& Scene::GetInstance()
{
	static Scene* myInstance = new Scene();
	return *myInstance;
}

bool Scene::Init()
{
	return true;
}

bool Scene::AddInstance( ModelSystem::ModelInstance* aModelInstance )
{
	myModels.emplace_back(aModelInstance);
	return true;
}

bool Scene::AddInstance(Camera* aCamera)
{
	myCameras.emplace_back(aCamera);
	return true;
}

bool Scene::AddInstance(EnvironmentLight* aEnvLight)
{
	myEnvLights.emplace_back(aEnvLight);
	return true;
}

bool Scene::AddInstance(DirectionalLight* aDirectionalLight)
{
	myDirLights.emplace_back(aDirectionalLight);
	return true;
}

bool Scene::AddInstance(PointLight* aPointLight)
{
	myPntLights.emplace_back(aPointLight);
	return true;
}

bool Scene::AddInstance(LineInstance* aLineInstance)
{
	myLines.emplace_back(aLineInstance);
	return false;
}

void Scene::SetMainCamera(Camera* aCamera)
{
	myMainCamera = aCamera;
}

void Scene::SetEnvironmentLight(EnvironmentLight* aEnvironmentLight)
{
	myEnvironmentLight = aEnvironmentLight;
}

void Scene::SetMainDirectionalLight(DirectionalLight* aDirectionalLight)
{
	myMainDirectionalLight = aDirectionalLight;
}

std::vector<ModelSystem::ModelInstance*> Scene::CullModels(Camera* /*aCamera*/)
{
	return myModels;
}

std::vector<LineInstance*> Scene::CullLines(Camera* /*aCamera*/)
{
	return myLines;
}

std::vector<PointLight*> Scene::CullPointLights(Camera* /*aCamera*/)
{
	return myPntLights;
}