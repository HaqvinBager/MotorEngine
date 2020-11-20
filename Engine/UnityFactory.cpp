#include "stdafx.h"
#include "UnityFactory.h"

#include "Scene.h"
#include "GameObject.h"

#include "TransformComponent.h"
#include "CameraComponent.h"
#include "ModelComponent.h"
#include "EnviromentLightComponent.h"
#include "CameraControllerComponent.h"

#include "LightFactory.h"


CUnityFactory::CUnityFactory()
{
}

CUnityFactory::~CUnityFactory()
{
}

bool CUnityFactory::FillScene(const SLoadScreenData& aData, const std::vector<std::string>& someModelPaths, CScene& aScene)
{
    CGameObject* camera = CreateGameObject(aData.myCamera);
    aScene.AddInstance(camera);
    aScene.SetMainCamera(camera->GetComponent<CCameraComponent>());
    CGameObject* envLight = CreateGameObject(aData.myDirectionalLight);
    aScene.AddInstance(envLight);
    aScene.SetEnvironmentLight(envLight->GetComponent<CEnviromentLightComponent>()->GetEnviromentLight());
    aScene.AddInstance(CreateGameObject(aData.myGameObject, someModelPaths[aData.myGameObject.myModelIndex]));
    return true;
}

bool CUnityFactory::FillScene(const SInGameData& aData, const std::vector<std::string>& aBinModelPaths, CScene& aScene)
{
    CGameObject* camera = CreateGameObject(aData.myCamera);
    aScene.AddInstance(camera);
    aScene.SetMainCamera(camera->GetComponent<CCameraComponent>());

    CGameObject* envLight = CreateGameObject(aData.myDirectionalLight);
    aScene.AddInstance(envLight);
    aScene.SetEnvironmentLight(envLight->GetComponent<CEnviromentLightComponent>()->GetEnviromentLight());

    //for (const auto& pointLightData : aData.myPointLightData)
    //{
    //    aScene.AddInstance(CreateGameObject(pointLightData));
    //}

    for (const auto& gameObjectData : aData.myGameObjects)
    {
        aScene.AddInstance(CreateGameObject(gameObjectData, aBinModelPaths[gameObjectData.myModelIndex]));
    }

    return true;
}

//std::vector<CGameObject*> CUnityFactory::CreateGameObjects(const SLoadScreenData& aData, const std::vector<std::string>& someModelPaths)
//{
//    //GAmeObjects Pekare Försvinner från Scopet när de lämnar sina Create-ställen! Måste göra någon typ av Move semantic!
//    std::vector<CGameObject*> gameObjects;
//    gameObjects.emplace_back(CreateGameObject(aData.myGameObject, someModelPaths[aData.myGameObject.myModelIndex]));
//    gameObjects.emplace_back(CreateGameObject(aData.myCamera));
//    gameObjects.emplace_back(CreateGameObject(aData.myDirectionalLight));
//    return gameObjects;
//}

CGameObject* CUnityFactory::CreateGameObject(const SCameraData& aData)
{
    CGameObject* gameObject = new CGameObject();
    gameObject->AddComponent<CCameraComponent>(*gameObject, aData.myFieldOfView);
    gameObject->AddComponent<CCameraControllerComponent>(*gameObject, 25.0f);
    gameObject->myTransform->Position(aData.myPosition);
    gameObject->myTransform->Rotation(aData.myRotation);
    return std::move(gameObject);
}

CGameObject* CUnityFactory::CreateGameObject(const SDirectionalLightData& aData)
{
    CGameObject* gameObject = new CGameObject();
    gameObject->AddComponent<CEnviromentLightComponent>(*gameObject, aData.myColor, aData.myIntensity, aData.myDirection);
    return std::move(gameObject);
}

CGameObject* CUnityFactory::CreateGameObject(const SGameObjectData& aData, const std::string& aModelPath)
{
    CGameObject* gameObject = new CGameObject();
    gameObject->AddComponent<CModelComponent>(*gameObject, aModelPath);
    gameObject->myTransform->Scale(aData.myScale.x);
    gameObject->myTransform->Position(aData.myPosition);
    gameObject->myTransform->Rotation(aData.myRotation);
    return std::move(gameObject);
}

CGameObject* CUnityFactory::CreateGameObject(const SPlayerData& /*aData*/)
{
    return nullptr;
}
