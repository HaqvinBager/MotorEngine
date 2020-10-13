#pragma once
#include "Component.h"

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
	CTransformComponent(CGameObject& aParent);
	~CTransformComponent() override;

	void Awake() override;
	void Start() override;
	void Update() override;

	void Position(DirectX::SimpleMath::Vector3& aPosition);
	DirectX::SimpleMath::Vector3 Position() const;

	void Rotation(DirectX::SimpleMath::Vector3& aRotation);
	void Scale(float aScale);
	float Scale();

	void Transform(DirectX::SimpleMath::Vector3& aPosition, DirectX::SimpleMath::Vector3& aRotation);
	DirectX::SimpleMath::Matrix& Transform();

	void Move(DirectX::SimpleMath::Vector3 aMovement);
	void Rotate(DirectX::SimpleMath::Vector3 aRotation);

private:
	float myScale;
	DirectX::SimpleMath::Matrix myTransform;
};