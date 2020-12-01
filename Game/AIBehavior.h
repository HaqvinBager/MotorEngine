#pragma once

class CGameObject;

class IAIBehavior
{
public:
	virtual ~IAIBehavior() = 0;

	virtual void Update(CGameObject* aParent) = 0;
	virtual void Collided(CGameObject* aGameObject) = 0;

	//virtual void Init()
private:
};

