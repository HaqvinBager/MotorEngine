#include "pch.h"
#include "DirectionalLight.hpp"

#include "SimpleMath.h"
namespace SM = DirectX::SimpleMath;

DirectionalLight::DirectionalLight()
{}

DirectionalLight::~DirectionalLight()
{}

void DirectionalLight::SetDirection(const DirectX::SimpleMath::Vector3& aDirection)
{
	myDirection = SM::Vector4(aDirection.x, aDirection.y, aDirection.z, 0.0f);
	myDirection.Normalize();
}

void DirectionalLight::SetColor(const DirectX::SimpleMath::Vector3& aColor)
{
	myColor.x = aColor.x;
	myColor.y = aColor.y;
	myColor.z = aColor.z;
}

void DirectionalLight::SetIntensity(const float& anIntensity)
{
	myColor.w = anIntensity;
}

void DirectionalLight::SetColorAndIntensity(const DirectX::SimpleMath::Vector4& aColor)
{
	myColor = aColor;
}

void DirectionalLight::Rotate(const DirectX::SimpleMath::Vector3 & aRotation)
{
	//myDirection = SM::Vector4(DirectX::XMConvertToRadians(aRotation.x), DirectX::XMConvertToRadians(aRotation.y), DirectX::XMConvertToRadians(aRotation.z), DirectX::XMConvertToRadians(0.0f));
	SM::Vector3 dir3(myDirection.x, myDirection.y, myDirection.z);
	dir3 = DirectX::XMVector3Rotate(dir3, DirectX::XMQuaternionRotationRollPitchYaw(  DirectX::XMConvertToRadians(aRotation.x)
																					, DirectX::XMConvertToRadians(aRotation.y)
																					, DirectX::XMConvertToRadians(aRotation.z)));
	myDirection.x = dir3.x;
	myDirection.y = dir3.y;
	myDirection.z = dir3.z;
	myDirection.Normalize();
}
