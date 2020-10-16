#include "stdafx.h"
#include "Scene.h"
#include "ModelInstance.h"
#include "EnvironmentLight.h"
#include "ModelInstance.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Camera.h"

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

std::vector<CModelInstance*> CScene::CullModels(CCamera* aMainCamera)
{
	using namespace DirectX::SimpleMath;
	Vector3 cameraPosition = aMainCamera->GetTransform().Translation();
	std::vector<CModelInstance*> culledModelInstances;
	for (auto& gameObject : myModelInstances)
	{
		float distanceToCameraSquared = Vector3::DistanceSquared(gameObject->GetTransform().Translation(), cameraPosition);
		if (distanceToCameraSquared < 1500.0f)
		{
			culledModelInstances.emplace_back(gameObject);
		}
	}
	return culledModelInstances;
}

std::vector<CGameObject*> CScene::CullGameObjects(CCamera* aMainCamera)
{
	using namespace DirectX::SimpleMath;
	Vector3 cameraPosition = aMainCamera->GetTransform().Translation();
	std::vector<CGameObject*> culledGameObjects;
	for (auto& gameObject : myGameObjects)
	{
		float distanceToCameraSquared = Vector3::DistanceSquared(gameObject->GetComponent<CTransformComponent>()->Position(), cameraPosition);
		if (distanceToCameraSquared < 1500.0f)
		{
			culledGameObjects.emplace_back(gameObject);
		}
	}
	return culledGameObjects;
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
