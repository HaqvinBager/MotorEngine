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
    mySpriteBatch->Begin();
    for (unsigned int i = 0; i < aTextInstanceList.size(); ++i) {
        CTextInstance* instance = aTextInstanceList[i];
        CText::STextData textData = instance->GetTextData()->GetTextData();

        textData.mySpriteFont->DrawString(mySpriteBatch, instance->GetText().c_str(), instance->GetPosition());
    }
    mySpriteBatch->End();
}
