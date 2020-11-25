#include "stdafx.h"
#include "VFXComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"

namespace SM = DirectX::SimpleMath;
#define ENGINE_SCALE 0.01f

CVFXComponent::CVFXComponent(CGameObject& aParent): CComponent(aParent) {
	SetScale(1.0f);
	myTransform.Translation(GameObject().myTransform->Position());
}

CVFXComponent::~CVFXComponent() {
    myVFXBases.clear();
}

void CVFXComponent::Awake() {
}

void CVFXComponent::Start() {
}

void CVFXComponent::Update() 
{
	SetPosition(GameObject().myTransform->Position());
	DirectX::SimpleMath::Vector3 scale;
	DirectX::SimpleMath::Quaternion quat;
	DirectX::SimpleMath::Vector3 translation;
	GameObject().myTransform->GetMatrix().Decompose(scale, quat, translation);
	SetRotation(quat);
}

bool CVFXComponent::Init(std::vector<CVFXBase*> someVFXBases) {
	myVFXBases = someVFXBases;
	if (someVFXBases.empty()) {
		return false;
	}

	return true;
}

void CVFXComponent::SetTransform(DirectX::SimpleMath::Vector3 aPosition, DirectX::SimpleMath::Vector3 aRotation) {
    SetRotation(aRotation);
    SetPosition(aPosition);
}

void CVFXComponent::SetRotation(DirectX::SimpleMath::Vector3 aRotation) {
	SM::Vector3 translation = myTransform.Translation();
	myTransform = SM::Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(aRotation.y),
		DirectX::XMConvertToRadians(aRotation.x),
		DirectX::XMConvertToRadians(aRotation.z)
	);
	myTransform *= SM::Matrix::CreateScale(myScale * ENGINE_SCALE);
	myTransform.Translation(translation);
}

void CVFXComponent::SetRotation(DirectX::SimpleMath::Quaternion aQuaternion)
{
	SM::Vector3 translation = myTransform.Translation();
	myTransform = SM::Matrix::CreateFromQuaternion(
		aQuaternion
	);
	myTransform *= SM::Matrix::CreateScale(myScale * ENGINE_SCALE);
	myTransform.Translation(translation);
}

void CVFXComponent::SetPosition(DirectX::SimpleMath::Vector3 aPosition) {
	myTransform.Translation(aPosition);
}

void CVFXComponent::SetScale(float aScale) {
	myScale = aScale;
	SM::Vector3 scale;
	SM::Vector3 translation;
	SM::Quaternion rotation;
	myTransform.Decompose(scale, rotation, translation);
	myTransform = SM::Matrix::CreateFromQuaternion(rotation);
	myTransform *= SM::Matrix::CreateScale(myScale * ENGINE_SCALE);
	myTransform.Translation(translation);
}

void CVFXComponent::Move(DirectX::SimpleMath::Vector3 aMovement) {
	myTransform.Translation(myTransform.Translation() + myTransform.Right() * aMovement.x);
	myTransform.Translation(myTransform.Translation() + myTransform.Up() * aMovement.y);
	myTransform.Translation(myTransform.Translation() - myTransform.Forward() * aMovement.z);
}

void CVFXComponent::Rotate(DirectX::SimpleMath::Vector3 aRotation) {
	SM::Vector3 translation = myTransform.Translation();
	myTransform *= SM::Matrix::CreateFromYawPitchRoll(aRotation.y, aRotation.x, aRotation.z);
	myTransform.Translation(translation);
}

void CVFXComponent::Rotate(DirectX::SimpleMath::Quaternion aQuaternion)
{
	SM::Vector3 translation = myTransform.Translation();
	myTransform *= SM::Matrix::CreateFromQuaternion(aQuaternion);
	myTransform.Translation(translation);
}
