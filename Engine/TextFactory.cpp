#include "stdafx.h"
#include "TextFactory.h"
#include "DirectXFramework.h"
#include "Text.h"
#include <SpriteFont.h> 

CTextFactory* CTextFactory::ourInstance = nullptr;

bool CTextFactory::Init(CDirectXFramework* aFramework)
{
    if (!aFramework) {
        return false;
    }

    myFramework = aFramework;
    return true;
}

CText* CTextFactory::LoadText(std::string aFontAndSize)
{
    std::string narrowString = aFontAndSize + ".spritefont";

#pragma warning(suppress : 4244)
    std::wstring wideString(narrowString.begin(), narrowString.end());
    
    CText::STextData textData;
    textData.mySpriteFont = new DirectX::SpriteFont(myFramework->GetDevice(), wideString.c_str());
    
    CText* text = new CText();
    text->Init(textData);
    
    myTextMap.emplace(aFontAndSize, text);
    
    return text;
}

CText* CTextFactory::GetText(std::string aFontAndSize)
{
    if (myTextMap.find(aFontAndSize) == myTextMap.end())
    {
        return LoadText(aFontAndSize);
    }
    return myTextMap.at(aFontAndSize);
}

CTextFactory* CTextFactory::GetInstance()
{
    return ourInstance;
}

CTextFactory::CTextFactory()
{
    ourInstance = this;
    myFramework = nullptr;
}

CTextFactory::~CTextFactory()
{
    ourInstance = nullptr;
    myFramework = nullptr;
}
