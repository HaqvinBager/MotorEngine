#pragma once
#include "Component.h"
#include <SimpleMath.h>

namespace DirectX {
	namespace SimpleMath {
		struct Matrix;
		struct Vector3;
	}
}

class CGameObject;
class CTransformComponent : public CComponent
{
public:
	CTransformComponent(CGameObject& aParent, DirectX::SimpleMath::Vector3 aPosition = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f));
	~CTransformComponent() override;

	void Awake() override;
	void Start() override;
	void Update() override;

	void Position(DirectX::SimpleMath::Vector3 aPosition);
	DirectX::SimpleMath::Vector3 Position() const;

	void Rotation(DirectX::SimpleMath::Vector3 aRotation);
	void Rotation(DirectX::SimpleMath::Quaternion aQuaternion);
	void Scale(float aScale);
	float Scale();
	void SetOutlineScale();
	void ResetScale();

	void Transform(DirectX::SimpleMath::Vector3 aPosition, DirectX::SimpleMath::Vector3 aRotation);
	DirectX::SimpleMath::Matrix& Transform();

	void Move(DirectX::SimpleMath::Vector3 aMovement);
	void MoveLocal(DirectX::SimpleMath::Vector3 aMovement);
	void Rotate(DirectX::SimpleMath::Vector3 aRotation);
	void Rotate(DirectX::SimpleMath::Quaternion aQuaternion);
	
	void MoveAlongPath();
	void SetPath(std::vector<DirectX::SimpleMath::Vector3>& aPath, DirectX::SimpleMath::Vector3 aFinalPosition);

	DirectX::SimpleMath::Matrix GetMatrix() const;

private:
	float myScale;
	// TODO: Move speed into "Resource Component"
	float myMoveSpeed;
	DirectX::SimpleMath::Matrix myTransform;
	std::vector<DirectX::SimpleMath::Vector3> myPath;
};