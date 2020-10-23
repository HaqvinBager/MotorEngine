#include "stdafx.h"
#include "LineInstance.h"

#include "Line.h"

namespace SM = DirectX::SimpleMath;

CLineInstance::CLineInstance()
{
	myLine = nullptr;
}
CLineInstance::~CLineInstance()
{
	delete myLine;
	myLine = nullptr;
}
void CLineInstance::Init(CLine* aLine )
{
	myLine = aLine;
}

void CLineInstance::SetTransform(const DirectX::SimpleMath::Vector3& aPosition, const DirectX::SimpleMath::Vector3& aRotation)
{
	SetRotation(aRotation);
	SetPosition(aPosition);
}
void CLineInstance::SetPosition(const DirectX::SimpleMath::Vector3& aPosition)
{
	myTransform.Translation(aPosition);
}
void CLineInstance::SetRotation(const DirectX::SimpleMath::Vector3& aRotation)
{
	SM::Vector3 tempTranslation(myTransform.Translation());
	myTransform = SM::Matrix::CreateFromYawPitchRoll( DirectX::XMConvertToRadians(aRotation.y), DirectX::XMConvertToRadians(aRotation.x), DirectX::XMConvertToRadians(aRotation.z));
	myTransform.Translation(tempTranslation);
}
void CLineInstance::SetScale(const DirectX::SimpleMath::Vector3& aScale)
{
	myTransform = myTransform.CreateScale(aScale);
}
void CLineInstance::Move(const DirectX::SimpleMath::Vector3& aMovement)
{
	//myTransform.Translation(aMovement);
	myTransform.Translation(myTransform.Translation() + (myTransform.Right() * aMovement.x));
	myTransform.Translation(myTransform.Translation() + (myTransform.Up() * aMovement.y));
	myTransform.Translation(myTransform.Translation() + (myTransform.Forward() * aMovement.z));
}
void CLineInstance::Rotate(const DirectX::SimpleMath::Vector3& aRotation)
{
	SM::Vector3 tempTranslation(myTransform.Translation());
	myTransform *= SM::Matrix::CreateFromYawPitchRoll( DirectX::XMConvertToRadians(aRotation.y), DirectX::XMConvertToRadians(aRotation.x), DirectX::XMConvertToRadians(aRotation.z));
	myTransform.Translation(tempTranslation);
}