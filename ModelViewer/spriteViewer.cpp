#include "spriteViewer.h"

#include "Engine.h"

#include "Canvas.h"
#include "Scene.h"
#include "SpriteInstance.h"
#include "SpriteFactory.h"
#include "AnimatedUIElement.h"
#include <Timer.h>


namespace SM = DirectX::SimpleMath;

namespace SpriteViewer
{
	CCanvas* Init()
	{
		CCanvas* canvas = new CCanvas();
		canvas->Init("Json/UI_InGame_Description.json");

		return canvas;
		//CSpriteInstance* spriteInstance = new CSpriteInstance();
		//spriteInstance->Init(CSpriteFactory::GetInstance()->GetSprite("Assets/3D/UI/Ingame/UI_test.dds"/*"tempUI.dds"*/));
		////spriteInstance->SetSize({ 1024.0f / 1000.0f, 256.0f / 1000.0f });
		////spriteInstance->SetSize({ 1024.0f / CEngine::GetInstance()->GetWindowHandler()->GetWidth(), 256.0f / CEngine::GetInstance()->GetWindowHandler()->GetWidth() });
		////spriteInstance->SetSize({ 2.0f, 2.0f });
		////spriteInstance->SetPosition({ 0.0f,-0.85f });
		//spriteInstance->SetPosition({ 0.0f,-0.82f });
		//spriteInstance->SetSize({ 0.75f, 0.75f });
		//
		//CScene::GetInstance()->AddInstance(spriteInstance);
		//
		//CAnimatedUIElement* uiElement = new CAnimatedUIElement("VFXData_UI_HealthOrb.json");
		//uiElement->SetPosition({ -0.52f, -0.82f });
		//CScene::GetInstance()->AddInstance(uiElement);
		//CAnimatedUIElement* uiElement2 = new CAnimatedUIElement("VFXData_UI_ResourceOrb.json");
		//uiElement2->SetPosition({ 0.52f, -0.82f });
		//CScene::GetInstance()->AddInstance(uiElement2);
		//CAnimatedUIElement* uiElement3 = new CAnimatedUIElement("VFXData_UI_ExperienceBar.json");
		//uiElement3->SetPosition({ 0.0f, -0.735f });
		//CScene::GetInstance()->AddInstance(uiElement3);
	}

	void Update(CCanvas& aCanvas, bool aShouldUpdate)
	{
		if (aShouldUpdate)
		{
			aCanvas.Update();
			//std::vector<CSpriteInstance*> animatedUIFrames;
			//auto animUI1 = CScene::GetInstance()->CullAnimatedUI(animatedUIFrames)[0];
			//animUI1->Level(abs(sin(CTimer::Time())));
			//auto animUI2 = CScene::GetInstance()->CullAnimatedUI(animatedUIFrames)[1];
			//animUI2->Level(abs(sin(CTimer::Time() - (3.1415f/2.0f))));
			//auto animUI3 = CScene::GetInstance()->CullAnimatedUI(animatedUIFrames)[2];
			//animUI3->Level(abs(sin(CTimer::Time() * 0.33f)));
		}
	}
}