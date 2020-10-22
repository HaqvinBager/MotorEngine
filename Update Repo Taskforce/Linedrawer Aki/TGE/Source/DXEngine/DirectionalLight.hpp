#pragma once

//namespace DirectX
//{
//	namespace SimpleMath
//	{
//		struct Vector3;
//		struct Vector4;
//	}
//}
#include <SimpleMath.h>// Since GetDirection and -Color return Vector4 can't forward declare
class DirectionalLight
{
public:
	DirectionalLight();
	~DirectionalLight();

	void SetDirection(const DirectX::SimpleMath::Vector3& aDirection);
	void SetColor(const DirectX::SimpleMath::Vector3& aColor);
	void SetIntensity(const float& anIntensity);
	void SetColorAndIntensity(const DirectX::SimpleMath::Vector4& aColor);

	void Rotate(const DirectX::SimpleMath::Vector3& aRotation);

public:
	const DirectX::SimpleMath::Vector4& GetDirection() { return myDirection; }
	const DirectX::SimpleMath::Vector4& GetColor() { return myColor; }

private:
	DirectX::SimpleMath::Vector4 myDirection;
	DirectX::SimpleMath::Vector4 myColor;
};

