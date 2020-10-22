#include "stdafx.h"
#include "CameraFactory.h"
#include "Camera.h"
#include "SimpleMath.h"

CCameraFactory* CCameraFactory::ourInstance = nullptr;

CCameraFactory::CCameraFactory()
{
	ourInstance = this;
	myWindowHandler = nullptr;
}

CCameraFactory::~CCameraFactory()
{
	ourInstance = nullptr;
	myWindowHandler = nullptr;
}

bool CCameraFactory::Init(CWindowHandler* aWindowHandler)
{
	if (!aWindowHandler) {
		return false;
	}

	myWindowHandler = aWindowHandler;
	return true;
}

CCamera* CCameraFactory::CreateCamera(float aFoV)
{
	CCamera* camera = new CCamera();
	float width = (float)myWindowHandler->GetWidth();
	float height = (float)myWindowHandler->GetHeight();
	camera->Init(aFoV, DirectX::SimpleMath::Vector2(width, height));
	myCameras.push_back(camera);
	return camera;
}

CCameraFactory* CCameraFactory::GetInstance()
{
	return ourInstance;
}
