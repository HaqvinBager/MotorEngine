#pragma once
#include "SimpleMath.h"
#include "Camera.h"
#include "Scene.h"
#include "Input.h"
#include <iostream>

//namespace MouseTracker {

class MouseTracker
{
public:

	static DirectX::SimpleMath::Vector3 ScreenPositionToWorldPosition(unsigned int aWidth, unsigned int aHeight/*const float aX, const float aY, float aWidth, float aHeight*/)
	{
		DirectX::SimpleMath::Ray ray = MouseTracker::WorldSpacePick(aWidth, aHeight);
		//The picking ray is checked against the floor of the world, **ASSUMING Y TO BE 0**
		DirectX::SimpleMath::Plane worldPlane = DirectX::SimpleMath::Plane(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Up);
		
		float distToPlane = 0.0f;
		DirectX::SimpleMath::Vector3 worldPos{ 0.0f, 0.0f, 0.0f };

		if (ray.Intersects(worldPlane, distToPlane)) {
			worldPos = ray.position + ray.direction * distToPlane;
		}

		return worldPos;

	//	CCamera* testCamera = CScene::GetInstance()->GetMainCamera();

	//	DirectX::SimpleMath::Matrix proj =  /*testCamera->GetView()*/testCamera->GetTransform() * testCamera->GetProjection();
	//	DirectX::SimpleMath::Matrix porjInvert = proj.Invert();

	//	float mouseX = (aX / (aWidth / 2.f) - 1.f);
	//	float mouseY = (-aY / (aHeight / 2.f) + 1.f);

	//	DirectX::SimpleMath::Vector4 mouseWorldPositionNear = DirectX::SimpleMath::Vector4(mouseX, mouseY, 2.0 * 1.0f - 1.0, 1.0f);
	//	DirectX::SimpleMath::Vector4 worldPosNear = DirectX::XMVector4Transform(mouseWorldPositionNear, porjInvert);
	//	
	//	//DirectX::SimpleMath::Vector4 mouseWorldPositionFar = DirectX::SimpleMath::Vector4(mouseX, mouseY, 1000.0f, 1.f);
	//	//DirectX::SimpleMath::Vector4 worldPosFar = DirectX::XMVector4Transform(mouseWorldPositionFar, porjInvert);

	//	//std::cout << "Click Far [" << worldPosFar.x << ", " <<
	//	//	worldPosFar.y << ", " <<
	//	//	worldPosFar.z << "]" << std::endl;

	//	worldPosNear.w = 1.f / worldPosNear.w;
	//	worldPosNear.x *= worldPosNear.w;
	//	worldPosNear.y *= worldPosNear.w;
	//	worldPosNear.z *= worldPosNear.w;

	//	std::cout << "Click Near [" << worldPosNear.x << ", " <<
	//		worldPosNear.y << ", " <<
	//		worldPosNear.z << "] --- " << std::endl;
	////	worldPosFar.w = 1.f / worldPosFar.w;
	////	worldPosFar.x *= worldPosFar.w;
	////	worldPosFar.y *= worldPosFar.w;
	//	
	//	
	//	return std::move(worldPosNear);
	};

	static DirectX::SimpleMath::Ray WorldSpacePick(unsigned int aWidth, unsigned int aHeight) {
		float mouseX = static_cast<float>(CommonUtilities::Input::GetInstance()->MouseX());
		float mouseY = static_cast<float>(CommonUtilities::Input::GetInstance()->MouseY());

		CCamera* cam = CScene::GetInstance()->GetMainCamera();

		float xV = (((2 * mouseX) / aWidth) - 1) / cam->GetProjection()._11;
		float yV = (-((2 * mouseY) / aHeight) + 1) / cam->GetProjection()._22;

		DirectX::SimpleMath::Vector3 target = cam->GetPosition() - cam->GetTransform().Forward();

		DirectX::SimpleMath::Matrix viewMatrix = DirectX::XMMatrixLookAtLH(cam->GetPosition(), target, cam->GetTransform().Up());
		DirectX::SimpleMath::Matrix viewMatrixInv = viewMatrix.Invert();

		DirectX::SimpleMath::Vector4 origin = { 0.0f, 0.0f, 0.0f, 1.0f };
		DirectX::SimpleMath::Vector4 dir = { xV, yV, 1.0f, 0.0f };

		origin = DirectX::XMVector4Transform(origin, viewMatrixInv);
		dir = DirectX::XMVector4Transform(dir, viewMatrixInv);
		dir.Normalize();

		return DirectX::SimpleMath::Ray({ origin.x, origin.y, origin.z }, { dir.x, dir.y, dir.z });
	}
};
//};



