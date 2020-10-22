#pragma once
#include "..\DirectXTK\Inc\SimpleMath.h"
#include <vector>

namespace SM = DirectX::SimpleMath;

class CCamera;
class CWindowHandler;

class CCameraFactory {
public:
	static CCameraFactory* GetInstance();

	CCameraFactory();
	~CCameraFactory();
	bool Init(CWindowHandler* aWindowHandler);
	CCamera* CreateCamera(float aFov, float aNearPlane, float aFarPlane);

private:
	CWindowHandler* myWindowHandler;
	std::vector<CCamera*> myCameras;
};

