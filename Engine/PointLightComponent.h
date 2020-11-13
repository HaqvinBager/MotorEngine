#pragma once
#include "Behaviour.h"

class CGameObject;
class CPointLight;

class CPointLightComponent : public CBehaviour
{
public:
	CPointLightComponent(CGameObject& aParent, float aRange = 10.f, DirectX::SimpleMath::Vector4 aColorAndIntensity = {1.f, 1.f, 1.f, 1.f});
	~CPointLightComponent();

	void Awake() override;
	void Start() override;
	void Update() override;

	void OnEnable() override;
	void OnDisable() override;

private:
	CPointLight* myPointLight;
};

