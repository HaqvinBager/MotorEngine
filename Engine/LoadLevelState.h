#pragma once
#include "State.h"

class CLoadLevelState : public CState {

public:
	CLoadLevelState(CStateStack& aStateStack);
	~CLoadLevelState() override;

	void Awake() override;
	void Start() override;
	void Update() override;
private:

};


