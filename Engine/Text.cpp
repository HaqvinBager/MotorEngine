#include "stdafx.h"
#include "Text.h"
#include <SpriteFont.h>

CText::CText() : myTextData()
{
}

CText::~CText()
{
}

bool CText::Init(STextData someData)
{
    myTextData = someData;
    return true;
}

CText::STextData& CText::GetTextData()
{
    return myTextData;
}
