#pragma once
#include "SimpleMath.h"

using namespace DirectX;

class CCamera
{
public:
	CCamera();
	~CCamera();
	//En rolig kommentar
	bool Init(float aFoV, DirectX::SimpleMath::Vector2 aResolution, float aFarPlane = 1000.0f);
	void SetTransform(DirectX::SimpleMath::Vector3 aPosition, DirectX::SimpleMath::Vector3 aRotation);
	void SetPosition(DirectX::SimpleMath::Vector3 aRotation);
	void SetRotation(DirectX::SimpleMath::Vector3 aPosition);
	void Move(DirectX::SimpleMath::Vector3 aMovement);
	void Rotate(DirectX::SimpleMath::Vector3 aRotation);

public:
	DirectX::SimpleMath::Matrix GetTransform() { return myTransform; }
	DirectX::SimpleMath::Matrix GetProjection() { return myProjection; }
	DirectX::SimpleMath::Vector4 GetPosition()
	{
		DirectX::SimpleMath::Vector4 position;
		position.x = myTransform.Translation().x;
		position.y = myTransform.Translation().y;
		position.z = myTransform.Translation().z;
		position.w = 1.0f;
		return position;
	}

private:
	DirectX::SimpleMath::Matrix myTransform;
	DirectX::SimpleMath::Matrix myProjection;
};