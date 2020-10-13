#include "stdafx.h"
#include "Scene.h"
#include "ModelInstance.h"
#include "EnvironmentLight.h"
#include "ModelInstance.h"

CScene* CScene::ourInstance = nullptr;

CScene* CScene::GetInstance()
{
	return ourInstance;
}

CScene::CScene()
{
	ourInstance = this;
	myMainCamera = nullptr;
}

CScene::~CScene()
{
	ourInstance = nullptr;
	myMainCamera = nullptr;
}


bool CScene::Init()
{
	return true;
}


void CScene::SetMainCamera(CCamera* aCamera)
{
	myMainCamera = aCamera;
}

CCamera* CScene::GetMainCamera()
{
	return myMainCamera;
}

CEnvironmentLight* CScene::GetEnvironmentLight()
{
	return myEnvironmentLights[0];
}

std::vector<CModelInstance*> CScene::CullModels(CCamera* /*aMainCamera*/)
{

	return myModelInstances;
}

std::vector<CGameObject*> CScene::CullGameObjects(CCamera* /*aMainCamera*/)
{
	return myGameObjects;
}

bool CScene::AddInstance(CModelInstance* aModel)
{
	myModelInstances.emplace_back(aModel);
	return true;
}

bool CScene::AddInstance(CCamera* aCamera)
{
	myCameras.emplace_back(aCamera);
	return true;
}

bool CScene::AddInstance(CEnvironmentLight* anEnvironmentLight)
{
	myEnvironmentLights.emplace_back(anEnvironmentLight);
	return true;
}

bool CScene::AddInstance(CGameObject* aGameObject)
{
	myGameObjects.emplace_back(aGameObject);
	return true;
}

bool CScene::ClearInstances()
{
	for (auto models : myModelInstances) {
		delete models;
		models = nullptr;
	}
	myModelInstances.clear();
	return true;
}
