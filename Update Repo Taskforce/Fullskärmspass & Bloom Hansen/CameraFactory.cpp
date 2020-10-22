#include "stdafx.h"
#include "CameraFactory.h"
#include "Camera.h"
#include "WindowHandler.h"

CCameraFactory* CCameraFactory::GetInstance() {
	static CCameraFactory* instance = new CCameraFactory();
	return instance;
}

CCameraFactory::CCameraFactory() : myWindowHandler(nullptr) {
}

CCameraFactory::~CCameraFactory() {
}

bool CCameraFactory::Init(CWindowHandler* aWindowHandler) {
	if (!aWindowHandler) {
		return false;
	}

	myWindowHandler = aWindowHandler;
	return true;
}

CCamera* CCameraFactory::CreateCamera(float aFov, float aNearPlane, float aFarPlane) {
	CCamera* camera = new CCamera();
	float width = myWindowHandler->GetWidth();
	float height = myWindowHandler->GetHeight();
	camera->Init(aFov, aNearPlane, aFarPlane, SM::Vector2(width, height));
	myCameras.push_back(camera);
	return camera;
}
