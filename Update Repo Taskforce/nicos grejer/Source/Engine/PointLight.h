#pragma once
#include "SimpleMath.h"

class CPointLight
{
public:
	CPointLight();
	~CPointLight();

	bool Init();
public:
	void SetPosition(DirectX::SimpleMath::Vector3 aPosition) { myPosition = aPosition; }
	void SetColor(DirectX::SimpleMath::Vector3 aColor) { myColor = aColor; }
	void SetRange(float aRange) { myRange = aRange; }
	void SetIntensity(float anIntensity) { myIntensity = anIntensity; }

	const DirectX::SimpleMath::Vector3 GetPosition() const { return myPosition; }
	const DirectX::SimpleMath::Vector3 GetColor() const { return myColor; }
	const float GetRange() const { return myRange; }
	const float GetIntensity() const { return myIntensity; }
private:
	DirectX::SimpleMath::Vector3 myPosition;
	DirectX::SimpleMath::Vector3 myColor;
	float myRange;
	float myIntensity;
};

