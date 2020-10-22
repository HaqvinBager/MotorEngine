#pragma once

class CWindowHandler;
class CCamera;

class CCameraFactory
{
	friend class CEngine;
public:
	bool Init(CWindowHandler* aWindowHandler);

	CCamera* CreateCamera(float aFoV);

	static CCameraFactory* GetInstance();

private:
	CCameraFactory();
	~CCameraFactory();
private:
	static CCameraFactory* ourInstance;
	CWindowHandler* myWindowHandler;
	std::vector<CCamera*> myCameras;
};

