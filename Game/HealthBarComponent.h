#pragma once
#include "Behaviour.h"

class CGameObject;
class CCanvas;
class CScene;

class CHealthBarComponent : public CBehaviour
{
public:
	CHealthBarComponent(CGameObject& aParant, CScene& aScene);
	~CHealthBarComponent();

	void Awake() override;
	void Start() override;
	void Update() override;

	void OnEnable() override;
	void OnDisable() override;

	CCanvas* GetCanvas() { return myCanvas; }
	CCanvas* GetCanvas2() { return myCanvas2; }
private:
	CCanvas* myCanvas;
	CCanvas* myCanvas2;
};

