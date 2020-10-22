#pragma once

#include <vector>

class WindowHandler;
class Camera;

class CameraFactory
{
public:
	CameraFactory operator=(const CameraFactory& other) = delete;
	CameraFactory(const CameraFactory& other)			= delete;

	static bool Create();
	static CameraFactory& GetInstance();
	void   Destroy();
	bool   Init(WindowHandler* aWindowHandler);

	Camera* CreateCamera(const float& aFov);

public:
	const int GetNrOfCameras(){ return static_cast<int>(myCameras.size()); }

private:
	CameraFactory();
	~CameraFactory();
	
	static CameraFactory*	ourInstance;
	WindowHandler*			myWindowHandler;
	std::vector<Camera*>	myCameras;
};

