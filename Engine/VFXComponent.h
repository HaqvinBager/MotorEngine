#pragma once
#include "Component.h"
#include <SimpleMath.h>

class CVFXBase;
class CGameObject;
class CVFXComponent : public CComponent {

public:
	CVFXComponent(CGameObject& aParent);
	~CVFXComponent() override;
	
	void Awake() override;
	void Start() override;
	void Update() override;

	bool Init(std::vector<CVFXBase*> someVFXBases);
	void SetTransform(DirectX::SimpleMath::Vector3 aPosition, DirectX::SimpleMath::Vector3 aRotation);
	void SetRotation(DirectX::SimpleMath::Vector3 aRotation);
	void SetRotation(DirectX::SimpleMath::Quaternion aQuaternion);
	void SetPosition(DirectX::SimpleMath::Vector3 aPosition);
	void SetScale(float aScale);
	void Move(DirectX::SimpleMath::Vector3 aMovement);
	void Rotate(DirectX::SimpleMath::Vector3 aRotation);
	void Rotate(DirectX::SimpleMath::Quaternion aQuaternion);

public:
	std::vector<CVFXBase*> GetVFXBases() { return myVFXBases; }
	DirectX::SimpleMath::Matrix GetTransform() { return myTransform; }

private:
	std::vector<CVFXBase*> myVFXBases;
	DirectX::SimpleMath::Matrix myTransform;
	float myScale = 1.0f;
};

