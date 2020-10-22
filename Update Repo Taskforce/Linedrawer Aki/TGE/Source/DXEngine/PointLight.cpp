#include "pch.h"
#include "PointLight.hpp"

PointLight::PointLight()
	: myRange(0.0f)
{}

PointLight::~PointLight()
{}

void PointLight::SetColorAndIntensity(const DirectX::SimpleMath::Vector4 & aColor)
{
	myColor = aColor;
}

void PointLight::SetColor(const DirectX::SimpleMath::Vector3 & aColor)
{
	myColor.x = aColor.x;
	myColor.y = aColor.y;
	myColor.z = aColor.z;
}

void PointLight::SetIntensity(const float& anIntensity)
{
	myColor.w = anIntensity;
}

void PointLight::SetRange(const float& aRange)
{
	myRange = aRange;
}
