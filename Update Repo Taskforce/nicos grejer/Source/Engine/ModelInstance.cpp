#include "stdafx.h"
#include "ModelInstance.h"

namespace SM = DirectX::SimpleMath;

CModelInstance::CModelInstance()
{
	myModel = nullptr;
}

CModelInstance::~CModelInstance()
{
	myModel = nullptr;
}

void CModelInstance::Init(CModel* aModel)
{
	myModel = aModel;
}

void CModelInstance::SetTransform(SM::Vector3 aPosition, SM::Vector3 aRotation)
{
	SetRotation(aRotation);
	SetPosition(aPosition);
}

void CModelInstance::SetRotation(SM::Vector3 aRotation)
{
	SM::Vector3 translation = myTransform.Translation();
	myTransform = SM::Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(aRotation.y),
		DirectX::XMConvertToRadians(aRotation.x),
		DirectX::XMConvertToRadians(aRotation.z)
	);
	myTransform *= SM::Matrix::CreateScale(myScale);
	myTransform.Translation(translation);
}

void CModelInstance::SetPosition(SM::Vector3 aPosition)
{
	myTransform.Translation(aPosition);
}

void CModelInstance::SetScale(float aScale)
{
	myScale = aScale;
	myTransform *= SM::Matrix::CreateScale(myScale);
}

void CModelInstance::SetOutlineScale()
{
	SM::Vector3 scale;
	SM::Vector3 translation;
	SM::Quaternion rotation;
	myTransform.Decompose(scale, rotation, translation);
	myTransform = SM::Matrix::CreateFromQuaternion(rotation);
	myTransform *= SM::Matrix::CreateScale(myScale * 1.01f);
	myTransform.Translation(translation);
}

void CModelInstance::ResetScale()
{
	SM::Vector3 scale;
	SM::Vector3 translation;
	SM::Quaternion rotation;
	myTransform.Decompose(scale, rotation, translation);
	myTransform = SM::Matrix::CreateFromQuaternion(rotation);
	myTransform *= SM::Matrix::CreateScale(myScale);
	myTransform.Translation(translation);
}

void CModelInstance::Move(SM::Vector3 aMovement)
{
	myTransform.Translation(myTransform.Translation() + myTransform.Right() * aMovement.x);
	myTransform.Translation(myTransform.Translation() + myTransform.Up() * aMovement.y);
	myTransform.Translation(myTransform.Translation() - myTransform.Forward() * aMovement.z);
}

void CModelInstance::Rotate(SM::Vector3 aRotation)
{
	SM::Vector3 translation = myTransform.Translation();
	myTransform *= SM::Matrix::CreateFromYawPitchRoll(aRotation.y, aRotation.x, aRotation.z);
	myTransform.Translation(translation);
}

void CModelInstance::Scroll(DirectX::SimpleMath::Vector2 aScrollVector1, DirectX::SimpleMath::Vector2 aScrollVector2)
{
	myTextureScroll = aScrollVector1;
	myTextureScroll2 = aScrollVector2;
}
