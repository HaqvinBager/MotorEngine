#include "stdafx.h"
#include "UnityFactory.h"

#include "Scene.h"
#include "GameObject.h"

#include "TransformComponent.h"
#include "CameraComponent.h"
#include "ModelComponent.h"
#include "EnviromentLightComponent.h"
#include "AnimationComponent.h"
#include "PlayerControllerComponent.h"
#include "NavMeshComponent.h"
#include "PointLightComponent.h"
#include "CameraControllerComponent.h"//Could not compile 20201124 21:05 without this

#include "LightFactory.h"
#include "PointLight.h"
//#include "NavmeshLoader.h"// included in NavMeshComp

#include "animationLoader.h"

#include "Debug.h"


CUnityFactory::CUnityFactory()
{
}

CUnityFactory::~CUnityFactory()
{
}

bool CUnityFactory::FillScene(const SLoadScreenData& aData, const std::vector<std::string>& someModelPaths, CScene& aScene)
{
    CGameObject* camera = CreateGameObject(aData.myCamera, false);
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

    for (const auto& pointLightData : aData.myPointLightData)
    {
        CGameObject* pointLight = CreateGameObject(pointLightData);
        aScene.AddInstance(pointLight);
        aScene.AddInstance(pointLight->GetComponent<CPointLightComponent>()->GetPointLight());

#ifdef _DEBUG
        //Vector3 otherPosition;
        //otherPosition = pointLightData.myPosition;
        //otherPosition.x += pointLightData.myRange;
        //CDebug::GetInstance()->DrawLine(pointLightData.myPosition, otherPosition, 50.0f);
#endif
    }
    // "Assets\\3D\\Character\\CH_PL_Daughter_01_19G4_1_19\\CH_PL_Daughter_01_19G4_1_19_SK.fbx" // Animated player
    CGameObject* player = CreateGameObject(aData.myPlayerData, aBinModelPaths[aData.myPlayerData.myModelIndex]);
    aScene.AddInstance(player);

    for (const auto& gameObjectData : aData.myGameObjects)
    {
        aScene.AddInstance(CreateGameObject(gameObjectData, aBinModelPaths[gameObjectData.myModelIndex]));
    }

    aScene.InitNavMesh("Levels/NavTest_ExportedNavMesh.obj");

    return true;
}

//std::vector<CGameObject*> CUnityFactory::CreateGameObjects(const SLoadScreenData& aData, const std::vector<std::string>& someModelPaths)
//{
//    //GAmeObjects Pekare F�rsvinner fr�n Scopet n�r de l�mnar sina Create-st�llen! M�ste g�ra n�gon typ av Move semantic!
//    std::vector<CGameObject*> gameObjects;
//    gameObjects.emplace_back(CreateGameObject(aData.myGameObject, someModelPaths[aData.myGameObject.myModelIndex]));
//    gameObjects.emplace_back(CreateGameObject(aData.myCamera));
//    gameObjects.emplace_back(CreateGameObject(aData.myDirectionalLight));
//    return gameObjects;
//}

CGameObject* CUnityFactory::CreateGameObject(const SCameraData& aData, bool addCameraController)
{
    CGameObject* gameObject = new CGameObject();
    gameObject->AddComponent<CCameraComponent>(*gameObject, aData.myFieldOfView);
    if (addCameraController) {
        gameObject->AddComponent<CCameraControllerComponent>(*gameObject, aData.myFreeCamMoveSpeed, static_cast<CCameraControllerComponent::ECameraMode>(aData.myStartInCameraMode), static_cast<char>(aData.myToggleFreeCamKey), aData.myOffset);
    }
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

CGameObject* CUnityFactory::CreateGameObject(const SPointLightData& aData)
{
    CGameObject* gameObject = new CGameObject();
    gameObject->myTransform->Position(aData.myPosition);
    gameObject->AddComponent<CPointLightComponent>(*gameObject, aData.myRange, aData.myColor, aData.myIntensity);
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

CGameObject* CUnityFactory::CreateGameObject(const SPlayerData& aData, const std::string& aModelPath)
{
    CGameObject* gameObject = new CGameObject();
    gameObject->myTransform->Scale(aData.myScale.x);
    gameObject->myTransform->Position(aData.myPosition);
    gameObject->myTransform->Rotation(aData.myRotation);
    gameObject->AddComponent<CModelComponent>(*gameObject, aModelPath);
    gameObject->AddComponent<CPlayerControllerComponent>(*gameObject);
    gameObject->AddComponent<CNavMeshComponent>(*gameObject);
    AddAnimationsToGameObject(*gameObject, aModelPath);
    return gameObject;
}
