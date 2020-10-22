#pragma once
#include "SimpleMath.h"
#include "TransformComponent.h"

using namespace DirectX;

class CTransformComponent;
class CCamera
{
public:
	CCamera();
	~CCamera();
	//En rolig kommentar
	//EN till Rolig kommentar!! Waoow
	bool Init(float aFoV, DirectX::SimpleMath::Vector2 aResolution, float aFarPlane = 1000.0f);
	void SetTransform(DirectX::SimpleMath::Vector3 aPosition, DirectX::SimpleMath::Vector3 aRotation);
	void SetPosition(DirectX::SimpleMath::Vector3 aRotation);
	void SetRotation(DirectX::SimpleMath::Vector3 aPosition);
	void Move(DirectX::SimpleMath::Vector3 aMovement);
	void Rotate(DirectX::SimpleMath::Vector3 aRotation);

	/// <summary>
	/// Temporary for Showcase!
	/// </summary>
	void Update()
	{
		SetPosition(myTarget->Position() + myOffset);
		SetRotation(myDirection);
	}

	/// <summary>
	/// Temporary for Showcase!
	/// </summary>
	void SetTarget(CTransformComponent* aTarget, DirectX::SimpleMath::Vector3 aOffset, DirectX::SimpleMath::Vector3 aDirection)
	{
		myTarget = aTarget;
		myOffset = aOffset;
		myDirection = aDirection;
	}

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

	DirectX::SimpleMath::Matrix GetView()
	{
		SimpleMath::Vector3 eye = myTransform.Forward();
		SimpleMath::Vector3 at = myTransform.Invert().Translation();
		SimpleMath::Vector3 up = myTransform.Up();// DirectX::SimpleMath::Vector3::Up;
		return DirectX::XMMatrixLookAtLH(eye, at, up);
	}

private:
	DirectX::SimpleMath::Matrix myTransform;
	DirectX::SimpleMath::Matrix myProjection;

private:

	/// <summary>
	/// Temporary for Showcase!
	/// </summary>
	CTransformComponent* myTarget;
	DirectX::SimpleMath::Vector3 myOffset;
	DirectX::SimpleMath::Vector3 myDirection;
};