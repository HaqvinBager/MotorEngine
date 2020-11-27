#pragma once
#include "Component.h"
#include "MessageType.h"

//class CRectangleColliderComponent;
class CGameObject;
enum class ECollisionLayer;
class CCollisionEventComponent : public CComponent
{
public:
	CCollisionEventComponent(CGameObject& aGameObject, EMessageType aMessageType, float aWidth, float aHeight, ECollisionLayer aCollisionLayer, uint64_t someCollisionFlags);
	~CCollisionEventComponent() override;

	void Awake() override;
	void Start() override;
	void Update() override;

	void Collided(CGameObject* aCollidedGameObject) override;

private:
	EMessageType myMessageType;
};