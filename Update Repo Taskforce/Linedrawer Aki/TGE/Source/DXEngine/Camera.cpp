#include "pch.h"
#include "Camera.hpp"

namespace DirX = DirectX;
namespace SM = DirectX::SimpleMath;

Camera::Camera()
{}
Camera::~Camera()
{}

bool Camera::Init(const float aFov, const DirectX::SimpleMath::Vector2& aResolution)
{
	myProjection = DirX::XMMatrixPerspectiveFovLH(DirX::XMConvertToRadians(aFov), aResolution.x / aResolution.y, 0.1f, 100.0f);

	return true;
}
void Camera::SetTransform(const DirectX::SimpleMath::Vector3& aPosition, const DirectX::SimpleMath::Vector3& aRotation)
{
	SetRotation(aRotation);
	SetPosition(aPosition);
}

void Camera::SetRotation(const DirectX::SimpleMath::Vector3& aRotation)
{
	SM::Vector3 tempTranslation(myTransform.Translation());
	myTransform = SM::Matrix::CreateFromYawPitchRoll( DirX::XMConvertToRadians(aRotation.y), DirX::XMConvertToRadians(aRotation.x), DirX::XMConvertToRadians(aRotation.z));
	myTransform.Translation(tempTranslation);
}

void Camera::SetPosition(const DirectX::SimpleMath::Vector3& aPosition)
{
	// -1 * => Temp solution, ACTUAL SOLUTION? NO? YES?
	myTransform.Translation(aPosition);
}

void Camera::Move(const DirectX::SimpleMath::Vector3& aMovement)
{
	// Gives incorrect movement after rotation, probably missed something, fix later?
	myTransform.Translation(myTransform.Translation() + (myTransform.Right() * aMovement.x));
	myTransform.Translation(myTransform.Translation() + (myTransform.Up() * aMovement.y));
	myTransform.Translation(myTransform.Translation() + (myTransform.Forward() * -aMovement.z));
	//myTransform._41 += -aMovement.x;
	//myTransform._42 += -aMovement.y;
	//myTransform._43 += -aMovement.z;
}

void Camera::Rotate(const DirectX::SimpleMath::Vector3& aRotation)
{
	SM::Vector3 tempTranslation(myTransform.Translation());
	myTransform *= SM::Matrix::CreateFromYawPitchRoll( DirX::XMConvertToRadians(aRotation.y), DirX::XMConvertToRadians(aRotation.x), DirX::XMConvertToRadians(aRotation.z));
	myTransform.Translation(tempTranslation);
}

