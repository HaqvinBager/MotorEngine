#include "stdafx.h"
#include "UnityFactory.h"

#include "Scene.h"
#include "GameObject.h"
#include "Engine.h"

#include "ModelComponent.h"
#include "AnimationComponent.h"
#include "TransformComponent.h"
#include "PlayerControllerComponent.h"
#include "AbilityComponent.h"
#include "NavMeshComponent.h"
#include "AIBehaviorComponent.h"
#include "CameraComponent.h"
#include "CameraControllerComponent.h"//Could not compile 20201124 21:05 without this
#include "EnviromentLightComponent.h"
#include "PointLightComponent.h"
#include "CollisionEventComponent.h"
#include "RectangleColliderComponent.h"
#include "CircleColliderComponent.h"

#include "CollisionManager.h"
#include "LightFactory.h"
#include "PointLight.h"
#include "StatsComponent.h"
#include "EnemyBehavior.h"
#include "InstancedModelComponent.h"
//#include "NavmeshLoader.h"// included in NavMeshComp
#include "animationLoader.h"
#include "HealthBarComponent.h"
#include "Model.h"

#include "Debug.h"

#define ENGINE_SCALE 0.01f


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
	aScene.AddPlayer(player);

	for (const auto& eventdata : aData.myEventData)
	{
	    aScene.AddInstance(CreateGameObject(eventdata, aData.myEventStringMap.at(eventdata.myInstanceID)));
	}

	CEnemyBehavior* enemyBehavior = new CEnemyBehavior(player);
	for (const auto& enemyData : aData.myEnemyData) {
		CGameObject* enemy = CreateGameObject(enemyData, aBinModelPaths[enemyData.myModelIndex], enemyBehavior);
			aScene.AddInstance(enemy);
			aScene.AddEnemies(enemy);
	}

	std::unordered_map<int, int> modelIndexMap;
	for (const auto& gameObjectData : aData.myGameObjects)
	{
		if (modelIndexMap.find(gameObjectData.myModelIndex) == modelIndexMap.end()) {
			modelIndexMap[gameObjectData.myModelIndex] = 0;
		}
		modelIndexMap[gameObjectData.myModelIndex]++;
	}

	std::unordered_map<int, std::vector<Matrix>> transformIndexMap;
	for (const auto& go : aData.myGameObjects) {
		if (transformIndexMap.find(go.myModelIndex) == transformIndexMap.end()) {
			transformIndexMap[go.myModelIndex].reserve(modelIndexMap[go.myModelIndex]);
		}
		Matrix transform = { };
		//Scale
		Vector3 scale;
		Vector3 translation;
		Quaternion rotation;
		transform.Decompose(scale, rotation, translation);
		transform = Matrix::CreateFromQuaternion(rotation);
		transform *= Matrix::CreateScale(go.myScale.x * ENGINE_SCALE);
		transform.Translation(translation);

		//Position
		transform.Translation(go.myPosition);

		//Rotation
		Vector3 tempTranslation = transform.Translation();

		DirectX::SimpleMath::Matrix tempRotation = Matrix::CreateFromYawPitchRoll(
			DirectX::XMConvertToRadians(go.myRotation.y),
			DirectX::XMConvertToRadians(go.myRotation.x),
			DirectX::XMConvertToRadians(go.myRotation.z)
		);

		transform = tempRotation;
		transform *= Matrix::CreateScale(go.myScale.x * ENGINE_SCALE);
		transform.Translation(tempTranslation);


		transformIndexMap[go.myModelIndex].emplace_back(transform);
	}

	for (int key = 0; key < aBinModelPaths.size(); ++key) {
		if (modelIndexMap.find(key) != modelIndexMap.end()) {
			aScene.AddInstance(CreateGameObjectInstanced(aBinModelPaths[key], modelIndexMap[key], transformIndexMap[key]));
		}
	}

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
        int cameraMode = static_cast<int>(aData.myStartInCameraMode);
        gameObject->AddComponent<CCameraControllerComponent>(*gameObject, aData.myFreeCamMoveSpeed, static_cast<CCameraControllerComponent::ECameraMode>(cameraMode), static_cast<char>(aData.myToggleFreeCamKey), aData.myOffset);
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

CGameObject* CUnityFactory::CreateGameObjectInstanced(const std::string& aModelPath, int InstancedID, std::vector<DirectX::SimpleMath::Matrix> aInstancedTransforms)
{
	CGameObject* gameObject = new CGameObject();
	gameObject->AddComponent<CInstancedModelComponent>(*gameObject, aModelPath, InstancedID, aInstancedTransforms);
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

	gameObject->AddComponent<CRectangleColliderComponent>(*gameObject, 1.f, 1.f, ECollisionLayer::PLAYER , static_cast<uint64_t>(ECollisionLayer::ALL));
    gameObject->AddComponent<CCircleColliderComponent>(*gameObject, 0.3f, ECollisionLayer::PLAYER, static_cast<uint64_t>(ECollisionLayer::ALL));
    gameObject->AddComponent<CStatsComponent>(*gameObject, 100.0f, 10.0f, 3.0f, 0.0f, 0.0f, 1.0f);

    std::pair<EAbilityType, unsigned int> ab1 = { EAbilityType::PlayerAbility1, 1 };
    std::pair<EAbilityType, unsigned int> ab2 = { EAbilityType::PlayerAbility2, 1 };
    std::pair<EAbilityType, unsigned int> ab3 = { EAbilityType::PlayerAbility3, 1 };
	std::pair<EAbilityType, unsigned int> ab4 = {EAbilityType::PlayerAbility4 , 1};
	std::pair<EAbilityType, unsigned int> ab5 = {EAbilityType::PlayerAbility5 , 1};
    std::vector<std::pair<EAbilityType, unsigned int>> abs;
    abs.emplace_back(ab1);
    abs.emplace_back(ab2);
    abs.emplace_back(ab3);
    abs.emplace_back(ab4);
    abs.emplace_back(ab5);
    gameObject->AddComponent<CAbilityComponent>(*gameObject, abs);

    AddAnimationsToGameObject(*gameObject, aModelPath);
	gameObject->GetComponent<CAnimationComponent>()->SetStateIDs(EPlayerAnimationID::Idle, EPlayerAnimationID::Run, EPlayerAnimationID::Dead);

    return gameObject;
}

CGameObject* CUnityFactory::CreateGameObject(const SEnemyData& aData, const std::string& aModelPath, IAIBehavior* aBehavior)
{
    CGameObject* gameObject = new CGameObject();
    gameObject->AddComponent<CModelComponent>(*gameObject, aModelPath);
    gameObject->AddComponent<CStatsComponent>(*gameObject, aData.myHealth, aData.myDamage, aData.myMoveSpeed, aData.myDamageCooldown, aData.myVisionRange, aData.myAttackRange);
    gameObject->AddComponent<CAIBehaviorComponent>(*gameObject, aBehavior);
    gameObject->AddComponent<CNavMeshComponent>(*gameObject);
	gameObject->AddComponent<CCircleColliderComponent>(*gameObject, 0.3f, ECollisionLayer::ENEMY, static_cast<uint64_t>(ECollisionLayer::PLAYER));
    gameObject->myTransform->Position(aData.myPosition);
    gameObject->myTransform->Rotation(aData.myRotation);

	std::pair<EAbilityType, unsigned int> ab1 = { EAbilityType::EnemyAbility, 1 };
	std::vector<std::pair<EAbilityType, unsigned int>> abs;
	abs.emplace_back(ab1);
	gameObject->AddComponent<CAbilityComponent>(*gameObject, abs);

	AddAnimationsToGameObject(*gameObject, aModelPath);
	gameObject->GetComponent<CAnimationComponent>()->SetStateIDs(EEnemyAnimationID::Idle, EEnemyAnimationID::Walk, EEnemyAnimationID::Dead);
	gameObject->AddComponent<CHealthBarComponent>(*gameObject, "Json/UI_InGame_Enemy_HealthBar.json");

	return gameObject;
}

CGameObject* CUnityFactory::CreateGameObject(const SEventData& aData, const std::string anEventString)
{
    CGameObject* gameObject = new CGameObject();
    gameObject->myTransform->Position(aData.myPosition);
    gameObject->AddComponent<CCollisionEventComponent>(*gameObject, 
		static_cast<EMessageType>(aData.myEvent),
		anEventString,
		aData.myColliderData.x, 
		aData.myColliderData.y, 
		ECollisionLayer::EVENT, 
		static_cast<uint64_t>(ECollisionLayer::PLAYER));
    return gameObject;
}
