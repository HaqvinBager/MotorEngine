#include "pch.h"
#include "EnvironmentLight.hpp"

EnvironmentLight::EnvironmentLight(ID3D11ShaderResourceView* aCubeMap)
	: myCubeMap(aCubeMap)
{}

EnvironmentLight::~EnvironmentLight()
{}

//void EnvironmentLight::SetDirection(const DirectX::SimpleMath::Vector4& aDirection)
//{
//	myDirection = aDirection;
//}
//
//void EnvironmentLight::SetDirectionAndNormalize(const DirectX::SimpleMath::Vector4& aDirection)
//{
//	myDirection = aDirection;
//	myDirection.Normalize();
//}

void EnvironmentLight::SetColor(const DirectX::SimpleMath::Vector4& aColor)
{
	myColor = aColor;
}
