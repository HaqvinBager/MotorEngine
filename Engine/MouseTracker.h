#pragma once
#include "SimpleMath.h"
#include "Camera.h"
#include "Scene.h"
#include <iostream>

//namespace MouseTracker {

class MouseTracker
{
public:
	//Checks against farplane
	static DirectX::SimpleMath::Vector4 ScreenPositionToWorldPosition(const float aX, const float aY, float aWidth, float aHeight)
	{
		CCamera* testCamera = CScene::GetInstance()->GetMainCamera();

		DirectX::SimpleMath::Matrix proj =  /*testCamera->GetView()*/testCamera->GetTransform() * testCamera->GetProjection();
		DirectX::SimpleMath::Matrix porjInvert = proj.Invert();

		float mouseX = (aX / (aWidth / 2.f) - 1.f);
		float mouseY = (-aY / (aHeight / 2.f) + 1.f);

		DirectX::SimpleMath::Vector4 mouseWorldPositionNear = DirectX::SimpleMath::Vector4(mouseX, mouseY, 2.0 * 1.0f - 1.0, 1.0f);
		DirectX::SimpleMath::Vector4 worldPosNear = DirectX::XMVector4Transform(mouseWorldPositionNear, porjInvert);
		
		//DirectX::SimpleMath::Vector4 mouseWorldPositionFar = DirectX::SimpleMath::Vector4(mouseX, mouseY, 1000.0f, 1.f);
		//DirectX::SimpleMath::Vector4 worldPosFar = DirectX::XMVector4Transform(mouseWorldPositionFar, porjInvert);

		//std::cout << "Click Far [" << worldPosFar.x << ", " <<
		//	worldPosFar.y << ", " <<
		//	worldPosFar.z << "]" << std::endl;

		worldPosNear.w = 1.f / worldPosNear.w;
		worldPosNear.x *= worldPosNear.w;
		worldPosNear.y *= worldPosNear.w;
		worldPosNear.z *= worldPosNear.w;

		std::cout << "Click Near [" << worldPosNear.x << ", " <<
			worldPosNear.y << ", " <<
			worldPosNear.z << "] --- " << std::endl;
	//	worldPosFar.w = 1.f / worldPosFar.w;
	//	worldPosFar.x *= worldPosFar.w;
	//	worldPosFar.y *= worldPosFar.w;
		
		
		return std::move(worldPosNear);
	};
};
//};



