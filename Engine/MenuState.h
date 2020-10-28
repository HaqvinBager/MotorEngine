#pragma once
#include "State.h"

class CSpriteInstance;

class CMenuState : public CState {
public:
	CMenuState(CStateStack& aStateStack);
	~CMenuState() override;

	void Awake() override;
	void Start() override;
	void Update() override;
private:

	CSpriteInstance* myBackground;
};