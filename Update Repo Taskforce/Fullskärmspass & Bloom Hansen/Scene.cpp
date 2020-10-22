#include "stdafx.h"
#include "Scene.h"
#include "Camera.h"
#include "EnvironmentLight.h"
#include "PointLight.h"
#include <array>

CScene* CScene::GetInstance() {
    static CScene* instance = new CScene();
    return instance;
}

bool CScene::AddInstance(CModelInstance* aModelInstance) {
    myModels.push_back(aModelInstance);
    return true;
}

bool CScene::AddInstance(CCamera* aCamera) {
    myCameras.push_back(aCamera);
    return true;
}

bool CScene::AddInstance(CEnvironmentLight* aEnvironmentLight) {
    myEnvironmentLight = aEnvironmentLight;
    return true;
}

bool CScene::AddInstance(CPointLight* aPointLight) {
    myPointLights.push_back(aPointLight);
    return true;
}

void CScene::SetMainCamera(CCamera* aCamera) {
    myMainCamera = aCamera;
}

CCamera* CScene::GetMainCamera() {
    return myMainCamera;
}

CEnvironmentLight* CScene::GetEnvironmentLight() {
    return myEnvironmentLight;
}

std::vector<CModelInstance*> CScene::CullModels(CCamera* aCamera) {
    //TODO: lookin kinda empty here
    return myModels;
}

std::pair<unsigned int, std::array<CPointLight*, 8>> CScene::CullLights(CModelInstance* aModelInstance) {
    std::pair<unsigned int, std::array<CPointLight*, 8>> pointLightPair;
    UINT counter = 0;

    for (UINT i = 0; i < myPointLights.size() && counter < 8; ++i) {
        float distanceSquared = DirectX::SimpleMath::Vector3::DistanceSquared(myPointLights[i]->GetPosition(), aModelInstance->GetTransform().Translation());
        float range = myPointLights[i]->GetRange();
        if (distanceSquared < (range * range)) {
            pointLightPair.second[counter] = myPointLights[i];
            ++counter;
        }
    }

    pointLightPair.first = counter;
    return pointLightPair;
}

CScene::CScene() : myMainCamera(nullptr), myEnvironmentLight(nullptr) {
}

CScene::~CScene() {
}