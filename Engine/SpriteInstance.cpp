#include "stdafx.h"
#include "SpriteInstance.h"
#include "Sprite.h"

CSpriteInstance::CSpriteInstance()
{
	mySprite = nullptr;
}

CSpriteInstance::~CSpriteInstance()
{
}

bool CSpriteInstance::Init(CSprite* aSprite)
{
	if (!aSprite) {
		return false;
	}

	mySprite = aSprite;

	return true;
}

void CSpriteInstance::SetSize(DirectX::SimpleMath::Vector2 aSize)
{
	mySize = aSize;
}

void CSpriteInstance::SetShouldRender(bool aBool)
{
	myShouldRender = aBool;
}

void CSpriteInstance::SetPosition(DirectX::SimpleMath::Vector2 aPosition)
{
	myPosition.x = aPosition.x;
	myPosition.y = aPosition.y;
}

void CSpriteInstance::SetColor(DirectX::SimpleMath::Vector4 aColor)
{
	myColor = aColor;
}

void CSpriteInstance::SetUVRect(DirectX::SimpleMath::Vector4 aUVRect)
{
	myUVRect = aUVRect;
}
