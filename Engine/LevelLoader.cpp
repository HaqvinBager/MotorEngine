#include "stdafx.h"
#include "LevelLoader.h"
#include "CUnityLoader.h"
#include "Scene.h"
#include "ModelFactory.h"
#include "Camera.h"
#include "CameraFactory.h"
#include "CapsuleColliderComponent.h"
#include "ModelComponent.h"
#include "CameraComponent.h"
#include "EnviromentLightComponent.h"
#include "PlayerControllerComponent.h"
#include "PointLightComponent.h"

#include <rapidjson\document.h>

#include "Engine.h"

CLevelLoader::CLevelLoader()
	: myUnityLoader(nullptr)
	//, myScene(nullptr)
{
}

bool CLevelLoader::Init(const unsigned int alevelIndex)
{
	myUnityLoader = new CUnityLoader();
	//myScene = CScene::GetInstance();

	std::ifstream t("Levels/json.txt");
	assert(t.is_open());
	std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	rapidjson::Document document;
	document.Parse(str.c_str());

	rapidjson::Value::ConstMemberIterator root = document.MemberBegin();
	rapidjson::Value::ConstMemberIterator oneStepDown = document.FindMember("Levels");

	rapidjson::Value& results = document["Levels"];

	std::string s = results[alevelIndex]["LevelName"].GetString();
	
	myScenes.emplace_back(new CScene());

	LoadNewLevel("Levels/" + s);

	return true;
}

void CLevelLoader::CreateLevel(const std::string& aPath)
{	
	LevelData* levelData = myUnityLoader->LoadLevelBinary(aPath);

	CGameObject* camera = new CGameObject();
	CCameraComponent* cameraComponent = camera->AddComponent<CCameraComponent>(*camera, levelData->myCameraData.myFieldOfView);
	CTransformComponent* cameraTransform = camera->myTransform;
	cameraTransform->Position({ levelData->myCameraData.myPosX, levelData->myCameraData.myPosY, levelData->myCameraData.myPosZ});
	cameraTransform->Rotation({ levelData->myCameraData.myRotX, levelData->myCameraData.myRotY, levelData->myCameraData.myRotZ });
	myScenes.back()->SetMainCamera(cameraComponent);
	myScenes.back()->AddInstance(camera);

	CGameObject* environmentLight = new CGameObject();
	environmentLight->AddComponent<CEnviromentLightComponent>(*environmentLight, 
		DirectX::SimpleMath::Vector4(
			levelData->myEnviromentData.myColorR, 
			levelData->myEnviromentData.myColorG, 
			levelData->myEnviromentData.myColorB, 
			levelData->myEnviromentData.myIntensity),
		DirectX::SimpleMath::Vector3(
			levelData->myEnviromentData.myDirectionX, 
			levelData->myEnviromentData.myDirectionY, 
			levelData->myEnviromentData.myDirectionZ));
	//maybe problemo in forwardrenderer - 20-11-11
	myScenes.back()->AddInstance(environmentLight);

	std::vector<PointLightDataRaw>& pointLights = levelData->myPointLightData;
	for (unsigned int i = 0; i < pointLights.size(); ++i)
	{
		CGameObject* pointLightGameObject = new CGameObject();
		pointLightGameObject->myTransform->Position({levelData->myPointLightData[i].myPosX, levelData->myPointLightData[i].myPosY, levelData->myPointLightData[i].myPosZ});
		pointLightGameObject->AddComponent<CPointLightComponent>(*pointLightGameObject,
			levelData->myPointLightData[i].myRange,
			DirectX::SimpleMath::Vector4(levelData->myPointLightData[i].myColorR,
			levelData->myPointLightData[i].myColorG, 
			levelData->myPointLightData[i].myColorB, 
			levelData->myPointLightData[i].myIntensity));
		myScenes.back()->AddInstance(pointLightGameObject);
	}

	CGameObject* playerGameObject = new CGameObject();
	playerGameObject->myTransform->Position({ levelData->myPlayerData.myPosX, levelData->myPlayerData.myPosY,levelData->myPlayerData.myPosZ });
	playerGameObject->myTransform->Rotation({ levelData->myPlayerData.myRotX, levelData->myPlayerData.myRotX, levelData->myPlayerData.myRotX });
	playerGameObject->AddComponent<CModelComponent>(*playerGameObject, levelData->myModelPaths[levelData->myPlayerData.myModelIndex]);
	playerGameObject->AddComponent<CPlayerControllerComponent>(*playerGameObject);
	myScenes.back()->AddInstance(playerGameObject);

	for (const auto &object : levelData->myModelData)
	{
		CGameObject* gameObject = new CGameObject();
		gameObject->myTransform->Scale(object.myScaleX);
		gameObject->myTransform->Rotation({object.myRotX, object.myRotY, object.myRotZ});
		gameObject->myTransform->Position({ object.myPosX, object.myPosY, object.myPosZ });
		gameObject->AddComponent<CModelComponent>(*gameObject, levelData->myModelPaths[object.myModelIndex]);

		myScenes.back()->AddInstance(gameObject);
	}

	CEngine::GetInstance()->AddScene(myScenes.back());
}

void CLevelLoader::LoadNewLevel(const std::string& aPath)
{
	//Fix 2020-11-11
	if (objectData.size() > 0) {
		//myScene->ClearScene();
		objectData.clear();
	}
	CreateLevel(aPath);
}
