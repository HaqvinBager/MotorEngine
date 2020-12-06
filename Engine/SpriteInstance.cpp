#include "stdafx.h"
#include "SpriteInstance.h"
#include "Sprite.h"
#include "Scene.h"
#include "Engine.h"
#include "WindowHandler.h"

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
	this->SetSize({ 1.0f, 1.0f });
	myRenderOrder = ERenderOrder::ForegroundLayer;

	return true;
}

void CSpriteInstance::SetSize(DirectX::SimpleMath::Vector2 aSize)
{
	//Sprites are assumed to be the correct native size in a 1920x1080 resolution
	CSprite::SSpriteData mySpriteData = mySprite->GetSpriteData();
	mySize = mySpriteData.myDimensions;
	mySize /= 1080.0f;
	mySize *= aSize;
}

void CSpriteInstance::SetShouldRender(bool aBool)
{
	myShouldRender = aBool;
}

void CSpriteInstance::SetRenderOrder(ERenderOrder aRenderOrder)
{
	myRenderOrder = aRenderOrder;
}

/// <summary>
///The y-position is flipped in this function so that we go from Shader space 
///([-1, -1] in the lower left corner) to Shader space with [-1, -1] in the 
/// upper left corner. I think this is more intuitive, but might as well 
/// revert this.
/// </summary>
/// <param name="aPosition"></param>
void CSpriteInstance::SetPosition(DirectX::SimpleMath::Vector2 aPosition)
{
	myPosition.x = aPosition.x;
	myPosition.y -= aPosition.y;
}

void CSpriteInstance::SetColor(DirectX::SimpleMath::Vector4 aColor)
{
	myColor = aColor;
}

void CSpriteInstance::SetUVRect(DirectX::SimpleMath::Vector4 aUVRect)
{
	myUVRect = aUVRect;
}
