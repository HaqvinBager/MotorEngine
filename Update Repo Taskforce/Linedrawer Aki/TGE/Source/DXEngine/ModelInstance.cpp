#include "pch.h"
#include "ModelInstance.hpp"

namespace SM = DirectX::SimpleMath;

namespace ModelSystem
{
	ModelInstance::ModelInstance()
	{
		myModel = nullptr;
	}
	ModelInstance::~ModelInstance()
	{
		myModel = nullptr;
	}
	void ModelInstance::Init( Model * aModel )
	{
		myModel = aModel;
	}

	void ModelInstance::SetTransform(const DirectX::SimpleMath::Vector3& aPosition, const DirectX::SimpleMath::Vector3& aRotation)
	{
		SetRotation(aRotation);
		SetPosition(aPosition);
	}
	void ModelInstance::SetPosition(const DirectX::SimpleMath::Vector3& aPosition)
	{
		myTransform.Translation(aPosition);
	}
	void ModelInstance::SetRotation(const DirectX::SimpleMath::Vector3& aRotation)
	{
		SM::Vector3 tempTranslation(myTransform.Translation());
		myTransform = SM::Matrix::CreateFromYawPitchRoll( DirectX::XMConvertToRadians(aRotation.y), DirectX::XMConvertToRadians(aRotation.x), DirectX::XMConvertToRadians(aRotation.z));
		myTransform.Translation(tempTranslation);
	}
	void ModelInstance::SetScale(const DirectX::SimpleMath::Vector3& aScale)
	{
		myTransform = myTransform.CreateScale(aScale);
	}
	void ModelInstance::Move(const DirectX::SimpleMath::Vector3& aMovement)
	{
		myTransform.Translation(myTransform.Translation() + (myTransform.Right() * aMovement.x));
		myTransform.Translation(myTransform.Translation() + (myTransform.Up() * aMovement.y));
		myTransform.Translation(myTransform.Translation() + (myTransform.Forward() * aMovement.z));
	}
	void ModelInstance::Rotate(const DirectX::SimpleMath::Vector3& aRotation)
	{
		SM::Vector3 tempTranslation(myTransform.Translation());
		myTransform *= SM::Matrix::CreateFromYawPitchRoll( DirectX::XMConvertToRadians(aRotation.y), DirectX::XMConvertToRadians(aRotation.x), DirectX::XMConvertToRadians(aRotation.z));
		myTransform.Translation(tempTranslation);
	}
}
