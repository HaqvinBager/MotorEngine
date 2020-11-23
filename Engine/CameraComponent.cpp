#include "stdafx.h"
#include "CameraComponent.h"

CCameraComponent::CCameraComponent(CGameObject& aParent, const float /*aFoV*//*, float aNearPlane, float aFarPlane, DirectX::SimpleMath::Vector2 aResolution*/)
	: CComponent(aParent)
{
	myProjection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(65.0f), (1600.0f / 900.0f), 10.0f, 10000.0f);
}

CCameraComponent::~CCameraComponent()
{
}

void CCameraComponent::Awake()
{
}

void CCameraComponent::Start()
{
}

void CCameraComponent::Update()
{
}
