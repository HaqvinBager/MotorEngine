#include "stdafx.h"
#include "LevelLoader.h"
#include "CUnityLoader.h"
#include "Scene.h"
#include "ModelFactory.h"
#include "ModelInstance.h"
#include "Camera.h"
#include "CameraFactory.h"

CLevelLoader::CLevelLoader()
	: myUnityLoader(nullptr)
	, myScene(nullptr)
{
}

bool CLevelLoader::Init()
{
	myUnityLoader = new CUnityLoader();
	myScene = CScene::GetInstance();
	return true;
}

void CLevelLoader::CreateLevel(const std::string& aPath)
{
	//CCamera* camera = CCameraFactory::GetInstance()->CreateCamera(65.0f, 5000.0f);

	myUnityLoader->LoadModels(aPath);

	objectData = myUnityLoader->LoadGameObjects(aPath, EReadMode::EReadMode_ASCII);
	for (auto object : objectData) {
		if (object.myRelativePath.length() > 1)
		{
			CModelInstance* model = CModelFactory::GetInstance()->CreateModel(object.myRelativePath);
			model->SetTransform(DirectX::SimpleMath::Vector3(object.myPosX, object.myPosY, object.myPosZ), DirectX::SimpleMath::Vector3(object.myRotX, object.myRotY, object.myRotZ));
			//model->SetScale(DirectX::SimpleMath::Vector3(object.myScaleX, object.myScaleY, object.myScaleZ));
			//model->SetScale({ 10.0f, 10.0f, 10.0f});
			myScene->AddInstance(model);
		}
	}
//	myScene->SetMainCamera(camera);
//	myScene->AddInstance(camera);
}

void CLevelLoader::LoadNewLevel(const std::string& aPath)
{
	if (objectData.size() > 0) {
		myScene->ClearInstances();
		objectData.clear();
	}
	CreateLevel(aPath);
}
