#include "stdafx.h"
#include "TextInstance.h"
#include "Engine.h"
#include "Scene.h"

namespace SM = DirectX::SimpleMath;

CTextInstance::CTextInstance() : myTextData(nullptr)
{
    CEngine::GetInstance()->GetActiveScene().AddInstance(this);
}

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
    myPosition = aPosition * CEngine::GetInstance()->GetWindowHandler()->GetResolution();
}

void CTextInstance::SetColor(DirectX::SimpleMath::Vector4 aColor)
{
    myColor = aColor;
}

void CTextInstance::SetPivot(DirectX::SimpleMath::Vector2 aPivot)
{
    myPivot = aPivot;
}
