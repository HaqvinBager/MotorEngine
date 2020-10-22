#include "pch.h"
#include "PointLightInstance.hpp"

PointLightInstance::PointLightInstance(PointLight* aPointLight)
	: myPointLight(aPointLight)
{}

PointLightInstance::~PointLightInstance()
{
	myPointLight = nullptr;
}

void PointLightInstance::SetPosition(const DirectX::SimpleMath::Vector4& aPosition)
{
	myPosition = aPosition;
}
