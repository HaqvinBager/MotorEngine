#pragma once
#include "State.h"
#include "InputObserver.h"

class CTokenPool;
class CCanvas;
class CGameObject;
class CInGameState : public CState, public IInputObserver
{

public:
	CInGameState(CStateStack& aStateStack);
	~CInGameState() override;

	void Awake() override;
	void Start() override;
	void Update() override;

	void ReceiveEvent(const EInputEvent aEvent) override;
	void MakeSceneActive() override;

private:
	CCanvas* myCanvas;
	CTokenPool* myTokenPool;
	CGameObject* myEnemy;
};