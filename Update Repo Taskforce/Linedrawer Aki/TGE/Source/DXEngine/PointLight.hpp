#pragma once

#include "SimpleMath.h"

class PointLight
{
public:
	PointLight();
	~PointLight();

	// W value in Vector4 will be used for intensity.
	void SetColorAndIntensity(const DirectX::SimpleMath::Vector4& aColor);
	void SetColor(const DirectX::SimpleMath::Vector3& aColor);
	void SetIntensity(const float& anIntensity);
	void SetRange(const float& aRange);

public:
	const DirectX::SimpleMath::Vector4& GetColorAndIntensity() { return myColor; }
	const DirectX::SimpleMath::Vector3& GetColor() { return DirectX::SimpleMath::Vector3( myColor.x, myColor.y, myColor.z ); }
	const float& GetIntensity() { return myColor.w; }
	const float& GetRange() { return myRange; }

private:
	DirectX::SimpleMath::Vector4 myColor;
	float myRange;
};

