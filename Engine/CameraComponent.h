#pragma once
#include "Component.h"

class CGameObject;

class CCameraComponent : public CComponent
{
public:
	CCameraComponent(CGameObject& aParent, float aFoV = 60.0f, DirectX::SimpleMath::Vector2 aResolution = {1600.f, 900.f}, float aNearPlane = 0.3f, float aFarPlane = 10000.0f);
	~CCameraComponent();

	void Awake() override;
	void Start() override;
	void Update() override;

	DirectX::SimpleMath::Matrix GetProjection() const { return myProjection; }

private:
	DirectX::SimpleMath::Matrix myProjection;
};

