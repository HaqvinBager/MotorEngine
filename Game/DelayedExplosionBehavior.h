#pragma once
#include "AbilityBehavior.h"

class CDelayedExplosionBehavior:
    public IAbilityBehavior
{
public:
    CDelayedExplosionBehavior(float aDuration, float aDelay, float aResourceCost, CGameObject* aParent);
    ~CDelayedExplosionBehavior();

    void Update(CGameObject* aParent) override;
    void Init(CGameObject* aCaster) override;

private:
    CGameObject* myCaster;
    CGameObject* myParent;
    float myDelay;
    float myResourceCost;
};

