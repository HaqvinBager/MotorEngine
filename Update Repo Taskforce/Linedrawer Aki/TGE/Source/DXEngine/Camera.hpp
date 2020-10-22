#pragma once

#include "SimpleMath.h"
//#include "../DirectXTK/Inc/SimpleMath.h"

class Camera
{
public:
	Camera();
	~Camera();

	bool Init(const float aFov, const DirectX::SimpleMath::Vector2& aResolution);
	void SetTransform(const DirectX::SimpleMath::Vector3& aPosition, const DirectX::SimpleMath::Vector3& aRotation);
	void SetRotation(const DirectX::SimpleMath::Vector3& aRotation);
	// Temp: all values are negated ( * -1 ) 
	void SetPosition(const DirectX::SimpleMath::Vector3& aPosition);
	void Move(const DirectX::SimpleMath::Vector3& aMovement);
	void Rotate(const DirectX::SimpleMath::Vector3& aRotation);

public:
	DirectX::SimpleMath::Matrix GetTransform(){ return myTransform; }
	DirectX::SimpleMath::Matrix GetProjection(){ return myProjection; }

private:
	DirectX::SimpleMath::Matrix myTransform;
	DirectX::SimpleMath::Matrix myProjection;

};