#include "stdafx.h"
#include "Camera.h"

using namespace DirectX::SimpleMath;

CCamera::CCamera()
	: myTarget(nullptr) //gör rätt 
//Okej jag gör väl rätt då Hahaha roligt fyfan bästa skiten ever omfg
{
}

CCamera::~CCamera()
{
}

bool CCamera::Init(float aFoV, Vector2 aResolution, float aFarPlane)
{
	myTransform = Matrix();
	myProjection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(aFoV), (aResolution.x / aResolution.y), 0.05f, aFarPlane);
	return true;
}

void CCamera::SetTransform(Vector3 aPosition, Vector3 aRotation)
{
	SetRotation(aRotation);
	SetPosition(aPosition);
}

void CCamera::SetPosition(Vector3 aPosition)
{
	myTransform.Translation(aPosition);
}

void CCamera::SetRotation(Vector3 aRotation)
{
	Vector3 tempTranslation = myTransform.Translation();
	
	Matrix tempRotation = Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(aRotation.y), 
		DirectX::XMConvertToRadians(aRotation.x), 
		DirectX::XMConvertToRadians(aRotation.z)
	);

	myTransform = tempRotation;
	myTransform.Translation(tempTranslation);
}

void CCamera::Move(Vector3 aMovement)
{
	myTransform.Translation(myTransform.Translation() + myTransform.Right() * aMovement.x);
	myTransform.Translation(myTransform.Translation() + myTransform.Up() * aMovement.y);
	myTransform.Translation(myTransform.Translation() - myTransform.Forward() * aMovement.z);
}

void CCamera::Rotate(Vector3 aRotation) {
	Vector3 tempTranslation = myTransform.Translation();
	Matrix tempRotation = Matrix::CreateFromYawPitchRoll(aRotation.y, aRotation.x, aRotation.z);
	myTransform *= tempRotation;
	myTransform.Translation(tempTranslation);
}