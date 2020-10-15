#include "stdafx.h"
#include "TransformComponent.h"
#include <SimpleMath.h>

#define ENGINE_SCALE 0.01f
using namespace DirectX::SimpleMath;

CTransformComponent::CTransformComponent(CGameObject& aParent, DirectX::SimpleMath::Vector3 aPosition) : myScale(ENGINE_SCALE), CComponent(aParent)
{
	Position(aPosition);
}

CTransformComponent::~CTransformComponent()
{
}

void CTransformComponent::Awake()
{
}

void CTransformComponent::Start()
{
}

void CTransformComponent::Update()
{
}

void CTransformComponent::Position(DirectX::SimpleMath::Vector3 aPosition)
{
	myTransform.Translation(aPosition);
}

DirectX::SimpleMath::Vector3 CTransformComponent::Position() const
{
	return myTransform.Translation();
}

void CTransformComponent::Rotation(DirectX::SimpleMath::Vector3 aRotation)
{
	Vector3 tempTranslation = myTransform.Translation();

	Matrix tempRotation = Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(aRotation.y),
		DirectX::XMConvertToRadians(aRotation.x),
		DirectX::XMConvertToRadians(aRotation.z)
	);
	myTransform = tempRotation;
	myTransform *= Matrix::CreateScale(myScale * ENGINE_SCALE);
	myTransform.Translation(tempTranslation);
}

void CTransformComponent::Scale(float aScale)
{
	myScale = aScale;
	myTransform *= Matrix::CreateScale(aScale * ENGINE_SCALE);
}

float CTransformComponent::Scale()
{
	return myScale;
}

void CTransformComponent::Transform(DirectX::SimpleMath::Vector3 aPosition, DirectX::SimpleMath::Vector3 aRotation)
{
	Rotation(aRotation);
	Position(aPosition);
}

DirectX::SimpleMath::Matrix& CTransformComponent::Transform()
{
	return myTransform;
}

void CTransformComponent::Move(DirectX::SimpleMath::Vector3 aMovement)
{
	myTransform.Translation(myTransform.Translation() + myTransform.Right() * aMovement.x);
	myTransform.Translation(myTransform.Translation() + myTransform.Up() * aMovement.y);
	myTransform.Translation(myTransform.Translation() - myTransform.Forward() * aMovement.z);
}

void CTransformComponent::Rotate(DirectX::SimpleMath::Vector3 aRotation)
{
	Vector3 tempTranslation = myTransform.Translation();
	Matrix tempRotation = Matrix::CreateFromYawPitchRoll(aRotation.y, aRotation.x, aRotation.z);
	myTransform *= tempRotation;
	myTransform.Translation(tempTranslation);


	//Vector3 tempTranslation = myTransform.Translation();
	//
	//Matrix tempRotation = Matrix::CreateFromYawPitchRoll(
	//	DirectX::XMConvertToRadians(aRotation.y),
	//	DirectX::XMConvertToRadians(aRotation.x),
	//	DirectX::XMConvertToRadians(aRotation.z)
	//);
	//
	//myTransform = tempRotation;
	//myTransform.Translation(tempTranslation);
}
