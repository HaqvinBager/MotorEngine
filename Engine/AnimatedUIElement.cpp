#include "stdafx.h"
#include "AnimatedUIElement.h"
#include "SpriteFactory.h"
#include "SpriteInstance.h"
#include "Engine.h"
#include "WindowHandler.h"

CAnimatedUIElement::CAnimatedUIElement() : mySpriteInstance(nullptr), myLevel(1.0f)
{
    mySpriteInstance = new CSpriteInstance();
    mySpriteInstance->Init(CSpriteFactory::GetInstance()->GetSprite("Assets/3D/UI/Ingame/UI_IN_OrbBase.dds"));
    UINT windowWidth = CEngine::GetInstance()->GetWindowHandler()->GetWidth();
    mySpriteInstance->SetSize({ 128.0f / windowWidth, 128.0f / windowWidth });
    mySpriteInstance->SetPosition({ -0.45f, -0.80f });

    myTexturePaths[0] = L"VFXCloud.dds";
    myTexturePaths[1] = L"VFXCloud.dds";
    myTexturePaths[2] = L"VFXCloud.dds";
    myTexturePaths[3] = L"MaskWithoutAlpha.dds";
}

CAnimatedUIElement::~CAnimatedUIElement()
{
    delete mySpriteInstance;
    mySpriteInstance = nullptr;

    //for (auto& texture : myTextures) 
    //{
    //    texture->Release();
    //}

    //myTextures.clear();
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
