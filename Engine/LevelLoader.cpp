#include "stdafx.h"
#include "LevelLoader.h"
#include "CUnityLoader.h"
#include "Scene.h"
#include "ModelFactory.h"
#include "Camera.h"
#include "CameraFactory.h"
#include "CapsuleColliderComponent.h"
#include "ModelComponent.h"

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
	//CCamera* camera = CCameraFactory::GetInstance()->CreateCamera(65.0f, 5000.0f);
	
	std::string modelPath = aPath;
	modelPath.append("_bin_modelPaths.txt");
	myUnityLoader->LoadModels(modelPath);

	objectData = myUnityLoader->LoadGameObjects(aPath + "_bin.bin", EReadMode::EReadMode_Binary);
	//for (auto object : objectData) {
	//	if (object.myRelativePath.length() > 1)
	//	{
	//		CModelInstance* model = CModelFactory::GetInstance()->CreateModel(object.myRelativePath);
	//		model->SetTransform(DirectX::SimpleMath::Vector3(object.myPosX, object.myPosY, object.myPosZ), DirectX::SimpleMath::Vector3(object.myRotX, object.myRotY, object.myRotZ));
	//		//model->SetScale(DirectX::SimpleMath::Vector3(object.myScaleX, object.myScaleY, object.myScaleZ));
	//		//model->SetScale({ 10.0f, 10.0f, 10.0f});
	//		myScene->AddInstance(model);
	//	}
	//}

	for (auto object : objectData)
	{
		CGameObject* gameObject = new CGameObject();
		CTransformComponent* transform = gameObject->AddComponent<CTransformComponent>(*gameObject);
		transform->Scale(object.myScaleX);
		transform->Position({ object.myPosX, object.myPosY, object.myPosZ });
		gameObject->AddComponent<CModelComponent>(*gameObject, object.myRelativePath);

		//if (object.myColliderHeight > 0.0f)
		//{
		//	gameObject->AddComponent<CCapsuleColliderComponent>(*gameObject, object.myColliderRadius, object.myColliderHeight);
		//}

		myScene->AddInstance(gameObject);
	}
//	myScene->SetMainCamera(camera);
//	myScene->AddInstance(camera);
}

void CLevelLoader::LoadNewLevel(const std::string& aPath)
{
	if (objectData.size() > 0) {
		myScene->ClearScene();
		objectData.clear();
	}
	CreateLevel(aPath);
}
