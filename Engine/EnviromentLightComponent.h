#pragma once
#include "Component.h"

class CGameObject;
class CEnvironmentLight;

class CEnviromentLightComponent : public CComponent
{
public:
	CEnviromentLightComponent(CGameObject& aParent, DirectX::SimpleMath::Vector3 aColor = { 1.f, 1.f, 1.f }, float anIntensity =  1.0f, DirectX::SimpleMath::Vector3 aDirection = { 0.f, 0.f, 1.f });
	~CEnviromentLightComponent();

	void Awake() override;
	void Start() override;
	void Update() override;

	CEnvironmentLight* GetEnviromentLight() const { return myEnvironmentLight; }

private:
	CEnvironmentLight* myEnvironmentLight;
};

