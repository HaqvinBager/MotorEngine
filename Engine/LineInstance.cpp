#include "stdafx.h"
#include "LineInstance.h"

#include "Line.h"

namespace SM = DirectX::SimpleMath;


const DirectX::SimpleMath::Vector3 CLineInstance::ourInactivePos = { 0xDEAD, 0xDEAD, 0xDEAD };

CLineInstance::CLineInstance()
{
	myLine = nullptr;
	myIsActive = false;
}
CLineInstance::~CLineInstance()
{
	if (myLine != nullptr)
	{
		delete myLine;
		myLine = nullptr;
	}
}
CLineInstance& CLineInstance::Init(CLine* aLine )
{
	myLine = aLine;
	myIsActive = true;
	return *this;
}

void CLineInstance::SetTransform(const DirectX::SimpleMath::Vector3& aPosition, const DirectX::SimpleMath::Vector3& aRotation)
{
	if (!myIsActive)
		return;
	SetRotation(aRotation);
	SetPosition(aPosition);
}
void CLineInstance::SetPosition(const DirectX::SimpleMath::Vector3& aPosition)
{
	if (!myIsActive)
		return;
	myTransform.Translation(aPosition);
}
void CLineInstance::SetRotation(const DirectX::SimpleMath::Vector3& aRotation)
{
	if (!myIsActive)
		return;
	SM::Vector3 tempTranslation(myTransform.Translation());
	myTransform = SM::Matrix::CreateFromYawPitchRoll( DirectX::XMConvertToRadians(aRotation.y), DirectX::XMConvertToRadians(aRotation.x), DirectX::XMConvertToRadians(aRotation.z));
	myTransform.Translation(tempTranslation);
}
void CLineInstance::SetRotation(DirectX::SimpleMath::Quaternion aQuaternion)
{
	if (!myIsActive)
		return;
	Vector3 tempTranslation = myTransform.Translation();
	Matrix tempRotation = Matrix::CreateFromQuaternion(aQuaternion);
	myTransform = tempRotation;
	myTransform.Translation(tempTranslation);
}
void CLineInstance::SetScale(const DirectX::SimpleMath::Vector3& aScale)
{
	if (!myIsActive)
		return;
	myTransform *= myTransform.CreateScale(aScale);
}
void CLineInstance::SetScale(const float aScale)
{
	if (!myIsActive)
		return;
	myTransform *= myTransform.CreateScale(aScale);
}
void CLineInstance::Move(const DirectX::SimpleMath::Vector3& aMovement)
{
	if (!myIsActive)
		return;
	//myTransform.Translation(aMovement);
	myTransform.Translation(myTransform.Translation() + (myTransform.Right() * aMovement.x));
	myTransform.Translation(myTransform.Translation() + (myTransform.Up() * aMovement.y));
	myTransform.Translation(myTransform.Translation() + (myTransform.Forward() * aMovement.z));
}
void CLineInstance::Rotate(const DirectX::SimpleMath::Vector3& aRotation)
{
	if (!myIsActive)
		return;
	SM::Vector3 tempTranslation(myTransform.Translation());
	myTransform *= SM::Matrix::CreateFromYawPitchRoll( DirectX::XMConvertToRadians(aRotation.y), DirectX::XMConvertToRadians(aRotation.x), DirectX::XMConvertToRadians(aRotation.z));
	myTransform.Translation(tempTranslation);
}

void CLineInstance::SetIsActive(const bool anIsActive)
{
	myIsActive = anIsActive;
}

bool CLineInstance::GetIsActive() const
{
	return myIsActive;
}
