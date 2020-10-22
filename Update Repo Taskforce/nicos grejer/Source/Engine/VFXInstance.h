#pragma once
#include "SimpleMath.h"

class CVFXBase;

class CVFXInstance {
public:
	CVFXInstance();
	~CVFXInstance();

	bool Init(CVFXBase* aVFXBase);
	void SetTransform(DirectX::SimpleMath::Vector3 aPosition, DirectX::SimpleMath::Vector3 aRotation);
	void SetRotation(DirectX::SimpleMath::Vector3 aRotation);
	void SetPosition(DirectX::SimpleMath::Vector3 aPosition);
	void SetScale(float aScale);
	void Move(DirectX::SimpleMath::Vector3 aMovement);
	void Rotate(DirectX::SimpleMath::Vector3 aRotation);
	void Scroll(DirectX::SimpleMath::Vector2 aScrollVector1, DirectX::SimpleMath::Vector2 aScrollVector2);

public:
	CVFXBase* GetVFXBase() { return myVFXBase; }
	DirectX::SimpleMath::Matrix GetTransform() { return myTransform; }
	DirectX::SimpleMath::Vector2 GetTextureScroll() { return myTextureScroll; }
	DirectX::SimpleMath::Vector2 GetTextureScroll2() { return myTextureScroll2; }

private:
	CVFXBase* myVFXBase;
	DirectX::SimpleMath::Matrix myTransform;
	DirectX::SimpleMath::Vector2 myTextureScroll;
	DirectX::SimpleMath::Vector2 myTextureScroll2;
	float myScale = 1.0f;
};

