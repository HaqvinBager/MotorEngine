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
#include "DestructibleComponent.h"
#include "VFXComponent.h"
#include "StatsComponent.h"
#include "InstancedModelComponent.h"
#include "EnemyBehavior.h"
#include "HealthBarComponent.h"
#include "ParticleEmitterComponent.h"
#include "BossBehavior.h"

#include "CollisionManager.h"
#include "LightFactory.h"
#include "VFXFactory.h"
#include "ParticleFactory.h"
#include "PointLight.h"
//#include "NavmeshLoader.h"// included in NavMeshComp
#include "animationLoader.h"
#include "Model.h"

#include "Debug.h"

#define ENGINE_SCALE 0.01f

//#define BAREBONES_SCENE
	// Used for debugging 2020 12 05
	// added if(NavMeshComp != nullptr) check in PlayerControllerComp
	// commented canvas and tokenpool in InGameState
	// commented navmesh loading in LoadLevelState
	// commented InputMapper event subscription in PlayerControllerComp

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
	aScene.AddInstance(CreateGameObject(aData.myGameObject, someModelPaths[0/*aData.myGameObject.myModelIndex*/])); //pls no more crash //Nico 09 dec
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
	}
	CGameObject* player = CreateGameObject(aData.myPlayerData, aBinModelPaths[aData.myPlayerData.myModelIndex]);
	aScene.AddInstance(player);
	aScene.AddPlayer(player);

	for (const auto& eventdata : aData.myEventData)
	{
		aScene.AddInstance(CreateGameObject(eventdata, aData.myEventStringMap.at(eventdata.myInstanceID)));
	}

	CEnemyBehavior* enemyBehavior = new CEnemyBehavior(player); // Deleted by scene
	aScene.TakeOwnershipOfAIBehavior(enemyBehavior);
	for (const auto& enemyData : aData.myEnemyData)
	{
		CGameObject* enemy = CreateGameObject(enemyData, aBinModelPaths.at(enemyData.myModelIndex), enemyBehavior, aScene);
		aScene.AddInstance(enemy);
		aScene.AddEnemies(enemy);
	}

	for (const auto& destructibleData : aData.myDestructibleData)
	{
		CGameObject* destructible = CreateGameObject(destructibleData, aBinModelPaths.at(destructibleData.myModelIndex));
		aScene.AddInstance(destructible);
		aScene.AddDestructible(destructible);
	}

	std::unordered_map<int, int> modelIndexMap;
	for (const auto& gameObjectData : aData.myGameObjects)
	{
		if (modelIndexMap.find(gameObjectData.myModelIndex) == modelIndexMap.end())
		{
			modelIndexMap[gameObjectData.myModelIndex] = 0;
		}
		modelIndexMap[gameObjectData.myModelIndex]++;
	}

	std::unordered_map<int, std::vector<Matrix>> transformIndexMap;
	for (const auto& go : aData.myGameObjects)
	{
		if (transformIndexMap.find(go.myModelIndex) == transformIndexMap.end())
		{
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

	for (int key = 0; key < aBinModelPaths.size(); ++key)
	{
		if (modelIndexMap.find(key) != modelIndexMap.end())
		{
			aScene.AddInstance(CreateGameObjectInstanced(aBinModelPaths[key], modelIndexMap[key], transformIndexMap[key]));
		}
	}

	for (auto& environmentFX : aData.myEnvironmentFXs)
	{
		aScene.AddInstance(CreateGameObject(environmentFX, aData.myEnvironmentFXStringMap.at(environmentFX.myInstanceID)));
	}

	for (auto& particleFX : aData.myParticleFXs)
	{
		aScene.AddInstance(CreateGameObject(particleFX, aData.myParticleFXStringMap.at(particleFX.myInstanceID)));
	}

	if (aData.myBossIsInScene > 0)
	{
		CBossBehavior* bossBehavior = new CBossBehavior(player);
		CGameObject* aBossGameObject = CreateGameObject(aData.myBossData, aBinModelPaths[aData.myBossData.myModelIndex]);
		aBossGameObject->AddComponent<CAIBehaviorComponent>(*aBossGameObject, bossBehavior);
		aScene.AddInstance(aBossGameObject);
	}

	return true;
}

CGameObject* CUnityFactory::CreateGameObject(const SCameraData& aData, bool addCameraController)
{
	CGameObject* gameObject = new CGameObject();
	auto camComponent = gameObject->AddComponent<CCameraComponent>(*gameObject, aData.myFieldOfView);
	if (addCameraController)
	{
		int cameraMode = static_cast<int>(aData.myStartInCameraMode);
		gameObject->AddComponent<CCameraControllerComponent>(*gameObject, aData.myFreeCamMoveSpeed, static_cast<CCameraControllerComponent::ECameraMode>(cameraMode), static_cast<char>(aData.myToggleFreeCamKey), aData.myOffset);
	}
	gameObject->myTransform->Position(aData.myPosition);
	gameObject->myTransform->Rotation(aData.myRotation);
	camComponent->SetStartingRotation(aData.myRotation);
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
	gameObject->AddComponent<CInstancedModelComponent>(*gameObject, aModelPath, InstancedID, aInstancedTransforms, (GetSuffixFromString(aModelPath) == "_AL"));
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

	gameObject->AddComponent<CCircleColliderComponent>(*gameObject, 1.f, ECollisionLayer::PLAYER, static_cast<uint64_t>(ECollisionLayer::ALL));
	gameObject->AddComponent<CStatsComponent>(*gameObject, 100.0f, 10.0f, 3.0f, 1.0f, 0.0f, 1.0f);

	std::pair<EAbilityType, unsigned int> ab1 = { EAbilityType::PlayerAbility1, 1 };
	std::pair<EAbilityType, unsigned int> ab2 = { EAbilityType::PlayerAbility2, 1 };
	std::pair<EAbilityType, unsigned int> ab3 = { EAbilityType::PlayerAbility3, 1 };
	std::pair<EAbilityType, unsigned int> ab4 = { EAbilityType::PlayerMelee, 1 };
	std::pair<EAbilityType, unsigned int> ab5 = { EAbilityType::PlayerHeavyMelee, 1 };
	std::vector<std::pair<EAbilityType, unsigned int>> abs;
	abs.emplace_back(ab1);
	abs.emplace_back(ab2);
	abs.emplace_back(ab3);
	abs.emplace_back(ab4);
	abs.emplace_back(ab5);
	gameObject->AddComponent<CAbilityComponent>(*gameObject, abs);

	AddAnimationsToGameObject(*gameObject, aModelPath, EAnimatedObject::Player);

	return gameObject;
}

CGameObject* CUnityFactory::CreateGameObject(const SEnemyData& aData, const std::string& aModelPath, IAIBehavior* aBehavior, CScene& aScene)
{
	CGameObject* gameObject = new CGameObject();
	gameObject->AddComponent<CModelComponent>(*gameObject, aModelPath);
	gameObject->AddComponent<CStatsComponent>(*gameObject, aData.myHealth, aData.myDamage, aData.myMoveSpeed, aData.myDamageCooldown, aData.myVisionRange, aData.myAttackRange, 105.0f);
	gameObject->AddComponent<CAIBehaviorComponent>(*gameObject, aBehavior);
	gameObject->AddComponent<CNavMeshComponent>(*gameObject);
	gameObject->AddComponent<CCircleColliderComponent>(*gameObject, 0.3f, ECollisionLayer::ENEMY, static_cast<uint64_t>(ECollisionLayer::PLAYERABILITY));
	gameObject->myTransform->Position(aData.myPosition);
	gameObject->myTransform->Rotation(aData.myRotation);

	std::pair<EAbilityType, unsigned int> ab1 = { EAbilityType::EnemyAbility, 1 };
	std::vector<std::pair<EAbilityType, unsigned int>> abs;
	abs.emplace_back(ab1);
	gameObject->AddComponent<CAbilityComponent>(*gameObject, abs);

	AddAnimationsToGameObject(*gameObject, aModelPath, EAnimatedObject::Enemy);

	gameObject->AddComponent<CHealthBarComponent>(*gameObject, aScene, "Json/UI_InGame_Enemy_HealthBar.json");

	return gameObject;
}

CGameObject* CUnityFactory::CreateGameObject(const SEventData& aData, const std::string anEventString)
{
	CGameObject* gameObject = new CGameObject();
	gameObject->myTransform->Position(aData.myPosition);
	gameObject->AddComponent<CCollisionEventComponent>(*gameObject,
		static_cast<EMessageType>(aData.myEvent),
		anEventString);

	gameObject->AddComponent<CRectangleColliderComponent>(*gameObject,
		aData.myColliderData.x,
		aData.myColliderData.y,
		ECollisionLayer::EVENT,
		static_cast<uint64_t>(ECollisionLayer::PLAYER));

	return gameObject;
}

CGameObject* CUnityFactory::CreateGameObject(const SDestructibleData& aData, const std::string& aModelPath)
{
	CGameObject* gameObject = new CGameObject();
	gameObject->myTransform->Position(aData.myPosition);
	gameObject->myTransform->Rotation(aData.myRotation);
	gameObject->AddComponent<CModelComponent>(*gameObject, aModelPath);
	gameObject->AddComponent<CCircleColliderComponent>(*gameObject, 0.15f, ECollisionLayer::ALL, static_cast<uint64_t>(ECollisionLayer::PLAYER));
	gameObject->AddComponent<CDestructibleComponent>(*gameObject);

	AddAnimationsToGameObject(*gameObject, aModelPath, EAnimatedObject::Destructible);

	return gameObject;
}

CGameObject* CUnityFactory::CreateGameObject(const SEnvironmentFXData& aData, std::string aEnvironmentFXName)
{
	CGameObject* gameObject = new CGameObject();
	gameObject->myTransform->Position(aData.myPosition);
	gameObject->myTransform->Rotation(aData.myRotation);
	gameObject->myTransform->Scale(aData.myScale.x);

	std::string jsonPath = "json/VFXData_";
	jsonPath.append(aEnvironmentFXName.c_str());
	jsonPath.append(".json");
	gameObject->AddComponent<CVFXComponent>(*gameObject)->Init(CVFXFactory::GetInstance()->GetVFXBaseSet({ jsonPath }));

	return gameObject;
}

CGameObject* CUnityFactory::CreateGameObject(const SParticleFXData& aData, const std::vector<std::string>& somParticleFXNames)
{
	CGameObject* gameObject = new CGameObject();
	gameObject->myTransform->Position(aData.myPosition);
	gameObject->myTransform->Rotation(aData.myRotation);

	std::vector<std::string> myjsonPaths;
	myjsonPaths.reserve(somParticleFXNames.size());
	for (const std::string& str : somParticleFXNames)
	{
		std::string jsonPath = "json/";
		jsonPath.append(str.c_str());
		jsonPath.append(".json");
		myjsonPaths.emplace_back(jsonPath);
	}

	gameObject->AddComponent<CParticleEmitterComponent>(*gameObject)->Init(CParticleFactory::GetInstance()->GetParticleSet(myjsonPaths));
	return gameObject;
	return nullptr;
}

CGameObject* CUnityFactory::CreateGameObject(const SBossData& aData, const std::string& aModelPath)
{
	CGameObject* gameObject = new CGameObject();
	gameObject->myTransform->Position(aData.myPosition);
	gameObject->myTransform->Rotation(aData.myRotation);
	gameObject->myTransform->Scale(aData.myScale.x);
	gameObject->AddComponent<CModelComponent>(*gameObject, aModelPath);
	AddAnimationsToGameObject(*gameObject, aModelPath, EAnimatedObject::Boss);
	
	gameObject->AddComponent<CCircleColliderComponent>(*gameObject, 0.5f, ECollisionLayer::BOSS, static_cast<int>(ECollisionLayer::PLAYERABILITY)); //todo more flags
	gameObject->AddComponent<CStatsComponent>(*gameObject, 10.0f, 10.0f, 3.0f, 3.0f, 20.0f, 15.0f);
	gameObject->AddComponent<CNavMeshComponent>(*gameObject);

	std::pair<EAbilityType, unsigned int> ab1 = { EAbilityType::BossAbility1, 1 };
	std::pair<EAbilityType, unsigned int> ab2 = { EAbilityType::BossAbility2, 1 };
	std::pair<EAbilityType, unsigned int> ab3 = { EAbilityType::BossAbility3, 1 };
	std::vector<std::pair<EAbilityType, unsigned int>> abs;
	abs.emplace_back(ab1);
	abs.emplace_back(ab2);
	abs.emplace_back(ab3);
	gameObject->AddComponent<CAbilityComponent>(*gameObject, abs);
	return gameObject;
}
