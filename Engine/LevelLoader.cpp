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

#include <rapidjson\document.h>

CLevelLoader::CLevelLoader()
	: myUnityLoader(nullptr)
	, myScene(nullptr)
{
}

bool CLevelLoader::Init()
{
	myUnityLoader = new CUnityLoader();
	myScene = CScene::GetInstance();

	std::ifstream t("Levels/json.txt");
	assert(t.is_open());
	std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	rapidjson::Document document;
	document.Parse(str.c_str());

	rapidjson::Value::ConstMemberIterator root = document.MemberBegin();
	rapidjson::Value::ConstMemberIterator oneStepDown = document.FindMember("Levels");

	rapidjson::Value& results = document["Levels"];

	std::string s = results[0]["LevelName"].GetString();

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
	myScene->SetMainCamera(cameraComponent);
	myScene->AddInstance(camera);

	CGameObject* environmentLight = new CGameObject();
	CEnviromentLightComponent* environmentLightComponent = environmentLight->AddComponent<CEnviromentLightComponent>(*environmentLight, 
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
	myScene->AddInstance(environmentLight);
	myScene->AddInstance(environmentLightComponent->GetEnviromentLight());

	CGameObject* playerGameObject = new CGameObject();
	playerGameObject->myTransform->Position({ levelData->myPlayerData.myPosX, levelData->myPlayerData.myPosY,levelData->myPlayerData.myPosZ });
	playerGameObject->myTransform->Rotation({ levelData->myPlayerData.myRotX, levelData->myPlayerData.myRotX, levelData->myPlayerData.myRotX });
	playerGameObject->AddComponent<CModelComponent>(*playerGameObject, levelData->myModelPaths[levelData->myPlayerData.myModelIndex]);
	playerGameObject->AddComponent<CPlayerControllerComponent>(*playerGameObject);
	myScene->AddInstance(playerGameObject);

	for (const auto &object : levelData->myModelData)
	{
		CGameObject* gameObject = new CGameObject();
		gameObject->myTransform->Scale(object.myScaleX);
		gameObject->myTransform->Rotation({object.myRotX, object.myRotY, object.myRotZ});
		gameObject->myTransform->Position({ object.myPosX, object.myPosY, object.myPosZ });
		gameObject->AddComponent<CModelComponent>(*gameObject, levelData->myModelPaths[object.myModelIndex]);

		myScene->AddInstance(gameObject);
	}
}

void CLevelLoader::LoadNewLevel(const std::string& aPath)
{
	//Fix 2020-11-11
	if (objectData.size() > 0) {
		myScene->ClearScene();
		objectData.clear();
	}
	CreateLevel(aPath);
}
