#include "stdafx.h"
#include "CameraComponent.h"

CCameraComponent::CCameraComponent(CGameObject& aParent, float aFoV, DirectX::SimpleMath::Vector2 aResolution, float aNearPlane, float aFarPlane) 
	: CComponent(aParent)
{
	myProjection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(aFoV), (aResolution.x / aResolution.y), aNearPlane, aFarPlane);
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
