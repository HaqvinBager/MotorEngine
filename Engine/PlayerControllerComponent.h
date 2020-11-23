#pragma once
#include "Behaviour.h"

class CPlayerControllerComponent : public CBehaviour
{
public:
	CPlayerControllerComponent(CGameObject& aParent);
	~CPlayerControllerComponent() override;
	void Awake() override;
	void Start() override;
	void Update() override;

	void OnEnable() override;
	void OnDisable() override;

private:

};