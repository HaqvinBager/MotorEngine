#include "stdafx.h"
#include "TransformComponent.h"
#include <SimpleMath.h>
#include "Engine.h"
#include "Scene.h"
#include "AStar.h"

#include "AnimationComponent.h"

#define ENGINE_SCALE 0.01f
using namespace DirectX::SimpleMath;

CTransformComponent::CTransformComponent(CGameObject& aParent, DirectX::SimpleMath::Vector3 aPosition): myScale(1.0f), CComponent(aParent), myMoveSpeed(3.0f)
{
	Scale(1.0f);
	Position(aPosition);
	myStartPosition = {0.0f,0.0f,0.0f};
}

CTransformComponent::~CTransformComponent()
{
}

void CTransformComponent::Awake()
{
}

void CTransformComponent::Start()
{
	myStartPosition = Position();
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

const DirectX::SimpleMath::Vector3 CTransformComponent::StartPosition() const
{
	return myStartPosition;
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

void CTransformComponent::Rotation(DirectX::SimpleMath::Quaternion aQuaternion)
{
	Vector3 tempTranslation = myTransform.Translation();

	Matrix tempRotation = Matrix::CreateFromQuaternion(
		aQuaternion
	);
	myTransform = tempRotation;
	myTransform *= Matrix::CreateScale(myScale * ENGINE_SCALE);
	myTransform.Translation(tempTranslation);
}

DirectX::SimpleMath::Quaternion CTransformComponent::Rotation()
{
	DirectX::SimpleMath::Vector3 translation;
	DirectX::SimpleMath::Vector3 scale;
	GetMatrix().Decompose(scale, myQuat, translation);
	return myQuat;
}

void CTransformComponent::Scale(float aScale)
{
	myScale = aScale;
	ResetScale();
}

float CTransformComponent::Scale()
{
	return myScale;
}

void CTransformComponent::SetOutlineScale()
{
	Vector3 scale;
	Vector3 translation;
	Quaternion rotation;
	myTransform.Decompose(scale, rotation, translation);
	myTransform = Matrix::CreateFromQuaternion(rotation);
	myTransform *= Matrix::CreateScale((myScale * ENGINE_SCALE) * 0.5f);
	myTransform.Translation(translation);
}

void CTransformComponent::ResetScale()
{
	Vector3 scale;
	Vector3 translation;
	Quaternion rotation;
	myTransform.Decompose(scale, rotation, translation);
	myTransform = Matrix::CreateFromQuaternion(rotation);
	myTransform *= Matrix::CreateScale(myScale * ENGINE_SCALE);
	myTransform.Translation(translation);
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
	myTransform.Translation(myTransform.Translation() + aMovement);
}

void CTransformComponent::MoveLocal(DirectX::SimpleMath::Vector3 aMovement)
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
}

void CTransformComponent::Rotate(DirectX::SimpleMath::Quaternion aQuaternion)
{
	Vector3 tempTranslation = myTransform.Translation();
	Matrix tempRotation = Matrix::CreateFromQuaternion(aQuaternion);
	myTransform *= tempRotation;
	myTransform.Translation(tempTranslation);
}

void CTransformComponent::MoveAlongPath()
{
	// Astar returns backwards path. Because we cannot swap path nodes,
	// we go from the back to the front in the path.

	if (GameObject().GetComponent<CAnimationComponent>()->IsUsingAbility())
	{
		myPath.clear();
		return;
	}

	DirectX::SimpleMath::Matrix lookAt;

	size_t pathSize = myPath.size();
	if (pathSize > 0) {

		GameObject().GetComponent<CAnimationComponent>()->MovingState();

		DirectX::SimpleMath::Vector3 newPos;
		DirectX::SimpleMath::Vector3 dir;

		float epsilon = 0.05f;

		dir = (myPath[pathSize - 1] - this->Position());
		dir.Normalize();
		
		this->Move(dir * myMoveSpeed * CTimer::Dt());

		Rotation({ 0, DirectX::XMConvertToDegrees(atan2f(dir.x, dir.z)) + 180.f, 0 });

		if (DirectX::SimpleMath::Vector3::DistanceSquared(this->Position(), myPath[pathSize - 1]) < epsilon) {
			myPath.pop_back();
		}
	}
}

void CTransformComponent::SetPath(std::vector<DirectX::SimpleMath::Vector3>& aPath, DirectX::SimpleMath::Vector3 aFinalPosition)
{
	if (aPath.empty()) {
		return;
	}

	myPath.clear();
	myPath.emplace_back(aFinalPosition);
	for (unsigned int i = 0; i < aPath.size(); ++i) {
		myPath.emplace_back(aPath[i]);
	}
	//myPath.emplace_back(this->Position());
}

void CTransformComponent::ClearPath()
{
	myPath.clear();
}

float CTransformComponent::MovementSpeed() const
{
	return myMoveSpeed;
}

void CTransformComponent::MovementSpeed(float aMovementSpeed)
{
	myMoveSpeed = aMovementSpeed;
}

DirectX::SimpleMath::Matrix CTransformComponent::GetMatrix() const
{
	return myTransform;
}
