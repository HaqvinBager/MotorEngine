#include "stdafx.h"
#include "PointLightComponent.h"
#include "GameObject.h"
#include "PointLight.h"
#include "LightFactory.h"
#include "TransformComponent.h"
#include "Scene.h"

CPointLightComponent::CPointLightComponent(CGameObject& aParent, float aRange, DirectX::SimpleMath::Vector3 aColor, float anIntensity)
    : CBehaviour(aParent),
    myRange(aRange),
    myColor(aColor),
    myIntensity(anIntensity),
    myPointLight(nullptr)
{
    myPointLight = CLightFactory::GetInstance()->CreatePointLight();
    myPointLight->SetPosition(GameObject().myTransform->Position());
    myPointLight->SetColor(myColor);
    myPointLight->SetIntensity(myIntensity);
    myPointLight->SetRange(myRange);
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
