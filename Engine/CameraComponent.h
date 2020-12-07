#pragma once
#include "Component.h"
#include "PerlinNoise.h"

class CGameObject;

class CCameraComponent : public CComponent
{
public:
	CCameraComponent(CGameObject& aParent, const float aFoV/*, float aNearPlane = 0.3f, float aFarPlane = 10000.0f, DirectX::SimpleMath::Vector2 aResolution = {1600.f, 900.f}*/);
	~CCameraComponent();

	void Awake() override;
	void Start() override;
	void Update() override;

	DirectX::SimpleMath::Matrix GetProjection() const { return myProjection; }

	void SetTrauma(float aValue);
	void SetStartingRotation(DirectX::SimpleMath::Vector3 aRotation);

private:
	void Shake();

private:
	DirectX::SimpleMath::Matrix myProjection;
	DirectX::SimpleMath::Vector3 myStartingRotation;
	DirectX::SimpleMath::Vector3 myMaxShakeRotation;
	PerlinNoise myNoise;
	float myTrauma;
	float myShake;
	float myShakeSpeed;
	float myDecayInSeconds;
	float myShakeTimer;
};

