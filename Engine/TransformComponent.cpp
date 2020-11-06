#include "stdafx.h"
#include "TransformComponent.h"
#include <SimpleMath.h>

#define ENGINE_SCALE 0.01f
using namespace DirectX::SimpleMath;

CTransformComponent::CTransformComponent(CGameObject& aParent, DirectX::SimpleMath::Vector3 aPosition) : myScale(1.0f), CComponent(aParent), myMoveSpeed(3.0f)
{
	Scale(1.0f);
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
	myTransform *= Matrix::CreateScale((myScale * ENGINE_SCALE) * 1.01f);
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
	//myTransform.Translation(myTransform.Translation() + myTransform.Right() * aMovement.x);
	//myTransform.Translation(myTransform.Translation() + myTransform.Up() * aMovement.y);
	//myTransform.Translation(myTransform.Translation() - myTransform.Forward() * aMovement.z);
	myTransform.Translation(myTransform.Translation() + aMovement);
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

void CTransformComponent::MoveAlongPath()
{
	// Astar returns backwards path. Because we cannot swap path nodes, 
	// we go from the back to the front in the path.
	size_t pathSize = myPath.size();
	if (pathSize > 0) {

		DirectX::SimpleMath::Vector3 newPos;
		DirectX::SimpleMath::Vector3 dir;

		float epsilon = 0.005f;

		//if (pathSize > 1) {
			dir = (myPath[pathSize - 1] - this->Position());
			dir.Normalize();
			this->Move(dir * myMoveSpeed * CTimer::Dt());
			
			if (DirectX::SimpleMath::Vector3::DistanceSquared(this->Position(), myPath[pathSize - 1]) < epsilon) {
				myPath.pop_back();
			}
		//}
		//else if (myPath.size() == 1) {
		//	dir = myPath[0] - this->Position();
		//	dir.Normalize();
		//	this->Move(dir * myMoveSpeed * CTimer::Dt());

		//	if (DirectX::SimpleMath::Vector3::DistanceSquared(this->Position(), myPath[0]) < epsilon) {
		//		myPath.pop_back();
		//	}
		//}
	}
}

void CTransformComponent::SetPath(std::vector<DirectX::SimpleMath::Vector3>& aPath, DirectX::SimpleMath::Vector3 aFinalPosition)
{
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
