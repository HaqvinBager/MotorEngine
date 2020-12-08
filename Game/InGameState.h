#pragma once
#include "State.h"
#include "StateStack.h"
#include "InputObserver.h"

class CTokenPool;
class CCanvas;
class CGameObject;
class CMouseSelection;
class CColliderPushManager;

class CInGameState : public CState, public IInputObserver
{
public:
	CInGameState(CStateStack& aStateStack, const CStateStack::EState aState = CStateStack::EState::InGame);
	~CInGameState() override;

	void Awake() override;
	void Start() override;
	void Stop() override;
	void Update() override;

	void ReceiveEvent(const EInputEvent aEvent) override;
private:
	CCanvas* myCanvas;
	CTokenPool* myTokenPool;
	CGameObject* myEnemy;
	CGameObject* myTestBoss;
	CMouseSelection* mySelection;
	CColliderPushManager* myColliderPusher;
};