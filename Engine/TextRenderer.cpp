#include "stdafx.h"
#include "TextRenderer.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include "DirectXFramework.h"
#include "Text.h"
#include "TextInstance.h"

CTextRenderer::CTextRenderer() : myContext(nullptr), mySpriteBatch(nullptr)
{
}

CTextRenderer::~CTextRenderer()
{
}

bool CTextRenderer::Init(CDirectXFramework* aFramework)
{
    myContext = aFramework->GetContext();
    if (!myContext) {
        return false;
    }

    mySpriteBatch = new DirectX::SpriteBatch(myContext);
    return true;
}

void CTextRenderer::Render(std::vector<CTextInstance*>& aTextInstanceList)
{
    DirectX::SimpleMath::Vector2 dropShadowOffset = { 2.0f, 2.0f };
    DirectX::SimpleMath::Vector4 dropShadowColor = { 0.0f, 0.0f, 0.0f, 1.0f };

    CTextInstance* instance = nullptr;
    CText::STextData textData;

    DirectX::SimpleMath::Vector2 dimensions;

    mySpriteBatch->Begin();
    for (unsigned int i = 0; i < aTextInstanceList.size(); ++i) {
        instance = aTextInstanceList[i];
        textData = instance->GetTextData()->GetTextData();
        
        DirectX::XMVECTOR result = textData.mySpriteFont->MeasureString(instance->GetText().c_str());
        dimensions = { DirectX::XMVectorGetX(result), DirectX::XMVectorGetY(result) };
        dimensions *= instance->GetPivot() * instance->GetScale();

        dropShadowColor.w = instance->GetColor().w;
        textData.mySpriteFont->DrawString(mySpriteBatch, instance->GetText().c_str(), (instance->GetPosition() + instance->GetGameObjectPosition()) + dropShadowOffset - dimensions, dropShadowColor, 0.0f, { 0.0f, 0.0f }, instance->GetScale());
        textData.mySpriteFont->DrawString(mySpriteBatch, instance->GetText().c_str(), (instance->GetPosition() + instance->GetGameObjectPosition()) - dimensions, instance->GetColor(), 0.0f, { 0.0f, 0.0f }, instance->GetScale());
    }
    mySpriteBatch->End();
}
