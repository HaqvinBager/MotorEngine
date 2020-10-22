#include "stdafx.h"
#include "Camera.h"
#include <DirectXMath.h>

CCamera::CCamera() {
}

CCamera::~CCamera() {
}

bool CCamera::Init(float aFov, float aNearPlane, float aFarPlane, SM::Vector2 aResolution) { 
	myProjection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(aFov), aResolution.x / aResolution.y, aNearPlane, aFarPlane);
	return true;
}

void CCamera::SetTransform(SM::Vector3 aPosition, SM::Vector3 aRotation) {
	SetRotation(aRotation);
	SetPosition(aPosition);
}

void CCamera::SetRotation(SM::Vector3 aRotation) {
	SM::Vector3 position = myTransform.Translation();
	myTransform = SM::Matrix::CreateFromYawPitchRoll(aRotation.y, aRotation.x, aRotation.z);
	myTransform.Translation(position);
}

void CCamera::SetPosition(SM::Vector3 aPosition) {
	myTransform.Translation(-aPosition);
}

void CCamera::Move(SM::Vector3 aMovement) { 
	myTransform.Translation(myTransform.Translation() + myTransform.Right() * aMovement.x);
	myTransform.Translation(myTransform.Translation() + myTransform.Up() * aMovement.y);
	myTransform.Translation(myTransform.Translation() - myTransform.Forward() * aMovement.z);
}

void CCamera::Rotate(SM::Vector3 aRotation) { //TODO: fix this, its weird
	SM::Vector3 position = myTransform.Translation();
	myTransform *= SM::Matrix::CreateFromYawPitchRoll(aRotation.y, aRotation.x, aRotation.z);
	myTransform.Translation(position);
}

SM::Matrix CCamera::GetTransform() {
	return myTransform;
}

SM::Matrix CCamera::GetProjection() {
	return myProjection;
}
