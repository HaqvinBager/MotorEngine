#pragma once
#include "Behaviour.h"

class CDestructibleComponent : public CBehaviour// dont forget to C it
{
public:
	CDestructibleComponent(CGameObject& aParent);

	void Awake() override;
	void Start() override;
	void Update() override;

	void Collided(CGameObject* aCollidedGameObject) override;

	void OnEnable() override;
	void OnDisable() override;

private:

};

