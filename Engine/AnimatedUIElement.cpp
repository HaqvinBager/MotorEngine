#include "stdafx.h"
#include "AnimatedUIElement.h"
#include "SpriteFactory.h"
#include "SpriteInstance.h"
#include "Engine.h"
#include "WindowHandler.h"

#include "rapidjson\document.h"
#include "rapidjson\istreamwrapper.h"

CAnimatedUIElement::CAnimatedUIElement() : mySpriteInstance(nullptr), myLevel(1.0f)
{
    mySpriteInstance = new CSpriteInstance();
    mySpriteInstance->Init(CSpriteFactory::GetInstance()->GetSprite("Assets/3D/UI/Ingame/UI_IN_OrbFrame.dds"));
    UINT windowWidth = CEngine::GetInstance()->GetWindowHandler()->GetWidth();
    mySpriteInstance->SetSize({ 200.0f / windowWidth, 200.0f / windowWidth });
    mySpriteInstance->SetPosition({ -0.45f, -0.50f });
    myData = CSpriteFactory::GetInstance()->GetVFXSprite("VFXData_UI_HealthOrb.json");
}

CAnimatedUIElement::~CAnimatedUIElement()
{
    delete mySpriteInstance;
    mySpriteInstance = nullptr;
}

void CAnimatedUIElement::Level(float aLevel)
{
    myLevel = aLevel;
}

float CAnimatedUIElement::Level() const
{
    return myLevel;
}

CSpriteInstance* CAnimatedUIElement::GetInstance() const
{
    return mySpriteInstance;
}

SAnimatedSpriteData* CAnimatedUIElement::GetVFXBaseData()
{
    return myData;
}
