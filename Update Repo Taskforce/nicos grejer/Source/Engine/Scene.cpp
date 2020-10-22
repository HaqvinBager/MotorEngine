#include "stdafx.h"
#include "Scene.h"
#include "Camera.h"
#include "EnvironmentLight.h"
#include "PointLight.h"
#include "ModelInstance.h"
#include "ParticleInstance.h"

CScene* CScene::ourInstance = nullptr;

CScene::CScene()
{
	ourInstance = this;
	myMainCamera = nullptr;
	myEnvironmentLight = nullptr;
}

CScene::~CScene()
{
	ourInstance = nullptr;
	myMainCamera = nullptr;
	myEnvironmentLight = nullptr;
}

bool CScene::Init()
{
	return true;
}

bool CScene::AddInstance(CModelInstance* aModelInstance)
{
	myModels.push_back(aModelInstance);
	return true;
}

bool CScene::AddInstance(CCamera* aCamera)
{
	myCameras.push_back(aCamera);
	return true;
}

bool CScene::AddInstance(CEnvironmentLight* anEnvironmentLight)
{
	myEnvironmentLight = anEnvironmentLight;
	return true;
}

bool CScene::AddInstance(CPointLight* aPointLight) {
	myPointLights.push_back(aPointLight);
	return true;
}

bool CScene::AddInstance(CParticleInstance* aParticleInstance)
{
	myParticles.push_back(aParticleInstance);
	return true;
}

bool CScene::AddInstance(CVFXInstance* aVFXInstance) {
	myVFXInstances.push_back(aVFXInstance);
	return true;
}

std::vector<CModelInstance*> CScene::CullModels(CCamera* /*aCamera*/)
{
	return myModels;
}

std::pair<unsigned int, std::array<CPointLight*, 8>> CScene::CullLights(CModelInstance* anInstance)
{
	std::pair<unsigned int, std::array<CPointLight*, 8>> pointLightPair;
	UINT counter = 0;

	for (unsigned int i = 0; i < myPointLights.size() && counter < 8; ++i) {
		float distSqr = DirectX::SimpleMath::Vector3::DistanceSquared(myPointLights[i]->GetPosition(), anInstance->GetTransform().Translation());
		float range = myPointLights[i]->GetRange();
		if (distSqr < (range * range)) {
			pointLightPair.second[counter] = myPointLights[i];
			counter++;
		}
	}
	pointLightPair.first = counter;
	return pointLightPair;
}

std::vector<CParticleInstance*> CScene::CullParticles(CCamera* aCamera)
{
	for (int i = 0; i < myParticles.size(); ++i) {
		myParticles[i]->Update(CTimer::Dt(), { aCamera->GetPosition().x, aCamera->GetPosition().y, aCamera->GetPosition().z });
	}
	return myParticles;
}

std::vector<CVFXInstance*> CScene::CullVFX(CCamera* /*aCamera*/) 
{
	return myVFXInstances;
}

void CScene::SetModelToOutline(CModelInstance* aModelInstance) {
	if (myModelToOutline) {
		myModels.emplace_back(std::move(myModelToOutline));
	}
	auto it = std::find(myModels.begin(), myModels.end(), aModelInstance);
	if (it != myModels.end()) {
		std::swap(*it, myModels.back());
		myModelToOutline = myModels.back();
		myModels.pop_back();
	}
	else {
		myModelToOutline = aModelInstance;
	}
}

CScene* CScene::GetInstance()
{
	return ourInstance;
}
