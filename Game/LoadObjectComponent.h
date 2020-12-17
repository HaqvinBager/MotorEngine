#pragma once
#include "Behaviour.h"

class CLoadObjectComponent : public CBehaviour
{
public:
	CLoadObjectComponent(CGameObject& aParent);
	~CLoadObjectComponent();

	void Awake() override;
	void Start() override;
	void Update() override;

	void OnEnable() override;
	void OnDisable() override;
private:
	bool myShrink;
};

