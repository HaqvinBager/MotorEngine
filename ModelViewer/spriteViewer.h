#pragma once

#include <SimpleMath.h>
#include <map>

class CCanvas;
namespace SpriteViewer
{
	CCanvas* Init();
	void Update(CCanvas& aCanvas, bool aShouldUpdate);
}