#pragma once

#include "SimpleMath.h"

class PointLight;
class PointLightInstance
{
public:
	PointLightInstance(PointLight* aPointLight);
	~PointLightInstance();

	void SetPosition(const DirectX::SimpleMath::Vector4& aPosition);

public:
	//const PointLight& GetPointLight() { return *myPointLight; }
	PointLight* GetPointLight() { return myPointLight; }
	const DirectX::SimpleMath::Vector4& GetPosition() { return myPosition; }

private:
	DirectX::SimpleMath::Vector4 myPosition;
	PointLight* myPointLight;
};

