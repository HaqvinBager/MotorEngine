#pragma once
#include "Behaviour.h"

class CGameObject;
class CPointLight;

class CPointLightComponent : public CBehaviour
{
public:
	CPointLightComponent(CGameObject& aParent, float aRange, DirectX::SimpleMath::Vector3 aColorAndIntensity, float anIntensity);
	~CPointLightComponent();

	void Awake() override;
	void Start() override;
	void Update() override;

	void OnEnable() override;
	void OnDisable() override;

	CPointLight* GetPointLight() const;

private:
	CPointLight* myPointLight;
	float myRange;
	Vector3 myColor;
	float myIntensity;
};

