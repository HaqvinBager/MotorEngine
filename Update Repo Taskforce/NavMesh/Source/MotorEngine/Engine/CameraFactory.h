#pragma once
#include <vector>

class CCamera;
class CWindowHandler;

class CCameraFactory {
	friend class CEngine;

public:
	static CCameraFactory* GetInstance();
	bool Init(CWindowHandler* aWindowHandler);
	CCamera* CreateCamera(float aFieldOfView, float aFarPlane = 1000.0f);

private:
	CCameraFactory();
	~CCameraFactory();

private:
	CWindowHandler* myWindowHandler;
	std::vector<CCamera*> myCameras;
	static CCameraFactory* ourInstance;
};