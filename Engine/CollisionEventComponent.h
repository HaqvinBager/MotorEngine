#pragma once
#include "Component.h"
#include "MessageType.h"

//class CRectangleColliderComponent;
class CGameObject;
enum class ECollisionLayer;

class CCollisionEventComponent : public CComponent
{
public:
	CCollisionEventComponent(CGameObject& aGameObject, EMessageType aMessageType, std::string anEventString, float aWidth, float aHeight, ECollisionLayer aCollisionLayer, uint64_t someCollisionFlags);
	~CCollisionEventComponent() override;

	void Awake() override;
	void Start() override;
	void Update() override;

	void Collided(CGameObject* aCollidedGameObject) override;

	const std::string& GetEventMessage() const;

private:
	EMessageType myMessageType;
	std::string myTextMessage;
};