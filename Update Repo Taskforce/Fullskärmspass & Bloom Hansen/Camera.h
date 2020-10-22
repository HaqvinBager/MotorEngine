#pragma once
#include "..\DirectXTK\Inc\SimpleMath.h"

namespace SM = DirectX::SimpleMath;

class CCamera {
public:
	CCamera();
	~CCamera();
	bool Init(float aFov, float aNearPlane, float aFarPlane, SM::Vector2 aResolution);
	void SetTransform(SM::Vector3 aPosition, SM::Vector3 aRotation);
	void SetRotation(SM::Vector3 aRotation);
	void SetPosition(SM::Vector3 aPosition);
	void Move(SM::Vector3 aMovement);
	void Rotate(SM::Vector3 aRotation);
	SM::Matrix GetTransform();
	SM::Matrix GetProjection();

private:
	SM::Matrix myTransform;
	SM::Matrix myProjection;
};

