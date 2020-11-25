#include "stdafx.h"
#include "CameraFactory.h"
#include "Camera.h"
#include "WindowHandler.h"
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

CCameraFactory* CCameraFactory::ourInstance = nullptr;
CCameraFactory* CCameraFactory::GetInstance()
{
	return ourInstance;
}

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

CCamera* CCameraFactory::CreateCamera(float aFieldOfView, float aFarPlane)
{
	CCamera* camera = new CCamera();
	float width = myWindowHandler->GetResolution().x;
	float height = myWindowHandler->GetResolution().y;
	camera->Init(aFieldOfView, Vector2(width, height), aFarPlane);
	myCameras.push_back(camera);
	return camera;
}