#pragma once

#include <SimpleMath.h>
#include <map>

class CCanvas;
class CScene;
namespace SpriteViewer
{
	CCanvas* Init(CScene* aScene);
	void Update(CCanvas& aCanvas, bool aShouldUpdate);
}