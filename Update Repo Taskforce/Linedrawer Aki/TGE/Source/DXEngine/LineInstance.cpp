#include "pch.h"
#include "LineInstance.hpp"

#include "Line.hpp"

namespace SM = DirectX::SimpleMath;

LineInstance::LineInstance()
{
	myLine = nullptr;
}
LineInstance::~LineInstance()
{
	delete myLine;
	myLine = nullptr;
}
void LineInstance::Init(Line* aLine )
{
	myLine = aLine;
}

void LineInstance::SetTransform(const DirectX::SimpleMath::Vector3& aPosition, const DirectX::SimpleMath::Vector3& aRotation)
{
	SetRotation(aRotation);
	SetPosition(aPosition);
}
void LineInstance::SetPosition(const DirectX::SimpleMath::Vector3& aPosition)
{
	myTransform.Translation(aPosition);
}
void LineInstance::SetRotation(const DirectX::SimpleMath::Vector3& aRotation)
{
	SM::Vector3 tempTranslation(myTransform.Translation());
	myTransform = SM::Matrix::CreateFromYawPitchRoll( DirectX::XMConvertToRadians(aRotation.y), DirectX::XMConvertToRadians(aRotation.x), DirectX::XMConvertToRadians(aRotation.z));
	myTransform.Translation(tempTranslation);
}
void LineInstance::SetScale(const DirectX::SimpleMath::Vector3& aScale)
{
	myTransform = myTransform.CreateScale(aScale);
}
void LineInstance::Move(const DirectX::SimpleMath::Vector3& aMovement)
{
	//myTransform.Translation(aMovement);
	myTransform.Translation(myTransform.Translation() + (myTransform.Right() * aMovement.x));
	myTransform.Translation(myTransform.Translation() + (myTransform.Up() * aMovement.y));
	myTransform.Translation(myTransform.Translation() + (myTransform.Forward() * aMovement.z));
}
void LineInstance::Rotate(const DirectX::SimpleMath::Vector3& aRotation)
{
	SM::Vector3 tempTranslation(myTransform.Translation());
	myTransform *= SM::Matrix::CreateFromYawPitchRoll( DirectX::XMConvertToRadians(aRotation.y), DirectX::XMConvertToRadians(aRotation.x), DirectX::XMConvertToRadians(aRotation.z));
	myTransform.Translation(tempTranslation);
}