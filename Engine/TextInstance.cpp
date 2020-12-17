#include "stdafx.h"
#include "TextInstance.h"
#include "Scene.h"
#include "Engine.h"
#include "WindowHandler.h"

namespace SM = DirectX::SimpleMath;

CTextInstance::CTextInstance(CScene& aScene, bool aAddToScene) 
    : myTextData(nullptr)
    , myShouldRender(true)
{
    if (aAddToScene)
       aScene.AddInstance(this);
}

CTextInstance::CTextInstance()
    : myTextData(nullptr)
{}

CTextInstance::~CTextInstance()
{
    myTextData = nullptr;
}

bool CTextInstance::Init(CText* aText)
{
    myTextData = aText;
    if (!myTextData) {
        return false;
    }

    myScale = { 1.0f, 1.0f };
    //myScale *= CEngine::GetInstance()->GetWindowHandler()->GetResolutionScale();

    return true;
}

void CTextInstance::SetText(std::string aString)
{
    myText = aString;
}

void CTextInstance::SetPosition(DirectX::SimpleMath::Vector2 aPosition)
{
    aPosition /= 2.0f;
    aPosition.x += 0.5f;
    aPosition.y += 0.5f;
    DirectX::SimpleMath::Vector2 standardRes = { 1920.0f, 1080.0f };
    myPosition = aPosition* standardRes /*CEngine::GetInstance()->GetWindowHandler()->GetResolution()*/;
}

void CTextInstance::SetGameObjectPosition(DirectX::SimpleMath::Vector2 aPosition)
{
    aPosition /= 2.0f;
    aPosition.x += 0.5f;
    aPosition.y += 0.5f;
    DirectX::SimpleMath::Vector2 standardRes = { 1920.0f, 1080.0f };
    myGameObjectPosition = aPosition * standardRes /*CEngine::GetInstance()->GetWindowHandler()->GetResolution()*/;
}

void CTextInstance::SetColor(DirectX::SimpleMath::Vector4 aColor)
{
    myColor = aColor;
}

void CTextInstance::SetPivot(DirectX::SimpleMath::Vector2 aPivot)
{
    myPivot = aPivot;
}

void CTextInstance::SetScale(DirectX::SimpleMath::Vector2 aScale)
{
    myScale = aScale;
}

void CTextInstance::SetShouldRender(bool aShouldRender)
{
    myShouldRender = aShouldRender;
}
