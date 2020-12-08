#pragma once
#include "Behaviour.h"

class DestructibleComponent : public CBehaviour// dont forget to C it
{
public:
	DestructibleComponent(CGameObject& aParent);

	void Awake() override;
	void Start() override;
	void Update() override;

	void Collided(CGameObject* aCollidedGameObject) override;

	void OnEnable() override;
	void OnDisable() override;

private:

};

