#pragma once
#include "Behaviour.h"

namespace DirectX {
	namespace SimpleMath {
		struct Vector3;
	}
}

class CRectangleColliderComponent : public CBehaviour
{
	friend class CIntersectionManager;

public:
	CRectangleColliderComponent(CGameObject& aParent, float aWidth, float aHeight, bool isStatic);
	~CRectangleColliderComponent() override;

	void Awake() override;
	void Start() override;
	void Update()override;
	void Collided(CGameObject* aCollidedGameObject) override;

	void OnEnable() override;
	void OnDisable() override;


private:
	bool myIsStatic;
	float myWidth;
	float myHeight;
	DirectX::SimpleMath::Vector3 myMin;
	DirectX::SimpleMath::Vector3 myMax;
	DirectX::SimpleMath::Vector3 myPosition;
};


