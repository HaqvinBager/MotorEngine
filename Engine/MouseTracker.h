#pragma once
#include "SimpleMath.h"
#include "Camera.h"
#include "Scene.h"

namespace MouseTracker {
	//Checks against farplane
	static DirectX::SimpleMath::Vector4 ScreenPositionToWorldPosition(const float &aX, const float &aY, const float &aWidth, const float &aHeight) {
		CCamera* testCamera = CScene::GetInstance()->GetMainCamera();
		DirectX::SimpleMath::Matrix proj = testCamera->GetTransform() * testCamera->GetProjection();
		DirectX::SimpleMath::Matrix porjInvert = proj.Invert();

		float mouseX = (aX / (aWidth / 2.f) - 1.f);
		float mouseY = (-aY / (aHeight / 2.f) + 1.f);

		DirectX::SimpleMath::Vector4 mouseWorldPosition = DirectX::SimpleMath::Vector4(mouseX, mouseY, 1.f, 1.f);

		DirectX::SimpleMath::Vector4 worldPos = DirectX::XMVector4Transform(mouseWorldPosition, porjInvert);
		worldPos.w = 1.f / worldPos.w;

		worldPos.x *= worldPos.w;
		worldPos.y *= worldPos.w;
		return std::move(worldPos);
	};
}

