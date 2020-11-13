#include "stdafx.h"
#include "PointLightComponent.h"
#include "GameObject.h"
#include "PointLight.h"
#include "LightFactory.h"
#include "TransformComponent.h"
#include "Scene.h"

CPointLightComponent::CPointLightComponent(CGameObject& aParent, float aRange, DirectX::SimpleMath::Vector4 aColorAndIntensity)
	: CBehaviour(aParent)
{
	myPointLight = CLightFactory::GetInstance()->CreatePointLight();
	myPointLight->SetPosition(GameObject().myTransform->Position());
	myPointLight->SetColor({aColorAndIntensity.x, aColorAndIntensity.y, aColorAndIntensity.z});
	myPointLight->SetIntensity(aColorAndIntensity.w);
	myPointLight->SetRange(aRange);
	CScene::GetInstance()->AddInstance(myPointLight);
}

CPointLightComponent::~CPointLightComponent()
{
}

void CPointLightComponent::Awake()
{
}

void CPointLightComponent::Start()
{
}

void CPointLightComponent::Update()
{
}

void CPointLightComponent::OnEnable()
{
}

void CPointLightComponent::OnDisable()
{
}
