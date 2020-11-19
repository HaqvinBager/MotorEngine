#include "stdafx.h"
#include "EnviromentLightComponent.h"
#include "GameObject.h"
#include "EnvironmentLight.h"
#include "LightFactory.h"
#include "TransformComponent.h"
#include "Scene.h"

CEnviromentLightComponent::CEnviromentLightComponent(CGameObject& aParent, DirectX::SimpleMath::Vector4 aColorAndIntensity, DirectX::SimpleMath::Vector3 aDirection)
	: CComponent(aParent)
{
	myEnvironmentLight = CLightFactory::GetInstance()->CreateEnvironmentLight("Assets/Cubemaps/Yokohama2.dds");
	myEnvironmentLight->SetColor({aColorAndIntensity.x, aColorAndIntensity.y, aColorAndIntensity.z});
	myEnvironmentLight->SetIntensity(aColorAndIntensity.w);
	//not sure if this work! We want to set the direction of this transform!
	aParent.myTransform->Transform().Forward(aDirection);
	myEnvironmentLight->SetDirection(aParent.myTransform->Transform().Forward());
	CScene::GetInstance()->AddInstance(myEnvironmentLight);
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
