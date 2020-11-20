#include "stdafx.h"
#include "EnviromentLightComponent.h"
#include "GameObject.h"
#include "EnvironmentLight.h"
#include "LightFactory.h"
#include "TransformComponent.h"

CEnviromentLightComponent::CEnviromentLightComponent(CGameObject& aParent, DirectX::SimpleMath::Vector3 aColor, float anIntensity, DirectX::SimpleMath::Vector3 aDirection)
	: CComponent(aParent)
{
	myEnvironmentLight = CLightFactory::GetInstance()->CreateEnvironmentLight("Yokohama2.dds");
	myEnvironmentLight->SetColor(aColor);
	myEnvironmentLight->SetIntensity(anIntensity);
	aParent.myTransform->Transform().Forward(aDirection);
	myEnvironmentLight->SetDirection(aParent.myTransform->Transform().Forward());
}

CEnviromentLightComponent::~CEnviromentLightComponent()
{
}

void CEnviromentLightComponent::Awake()
{
}

void CEnviromentLightComponent::Start()
{
}

void CEnviromentLightComponent::Update()
{
}
