#pragma once
#include "SimpleMath.h"
#include "Camera.h"
#include "Scene.h"
#include "Input.h"
#include "CameraComponent.h"
#include <iostream>
#include "Engine.h"
#include "WindowHandler.h"
#include "Debug.h"
#include "Utility.h"
#include "NavmeshLoader.h"
#include "Scene.h"

//namespace MouseTracker {

class MouseTracker
{
public:

	static DirectX::SimpleMath::Vector3 ScreenPositionToWorldPosition(/*const float aX, const float aY, float aWidth, float aHeight*/)
	{
		DirectX::SimpleMath::Ray ray = MouseTracker::WorldSpacePick();
		STriangle* triangle = nullptr; 
		for (unsigned int i = 0; i < CEngine::GetInstance()->GetActiveScene().GetNavMesh()->myTriangles.size(); ++i) {
			triangle = CEngine::GetInstance()->GetActiveScene().GetNavMesh()->myTriangles[i];
			float dist = 0; 
			if (ray.Intersects(triangle->myVertexPositions[0], triangle->myVertexPositions[1], triangle->myVertexPositions[2], dist)) {
				DirectX::SimpleMath::Vector3 finalPosition = ray.position + ray.direction * dist; 
				return finalPosition;
			}
		}

		//The picking ray is checked against the floor of the world, **ASSUMING Y TO BE 0**
		//DirectX::SimpleMath::Plane worldPlane = DirectX::SimpleMath::Plane(DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Up);
		//
		//float distToPlane = 0.f;
		//DirectX::SimpleMath::Vector3 worldPos{ 0.0f, 0.0f, 0.0f };
		//
		//if (ray.Intersects(worldPlane, distToPlane)) {
		//	worldPos = ray.position + ray.direction * distToPlane;
		//}

		return Vector3();

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

	static DirectX::SimpleMath::Ray WorldSpacePick() {
		UINT width = static_cast<UINT>(CEngine::GetInstance()->GetWindowHandler()->GetResolution().x);
		UINT height = static_cast<UINT>(CEngine::GetInstance()->GetWindowHandler()->GetResolution().y);

		float mouseX = static_cast<float>(Input::GetInstance()->MouseX());
		float mouseY = static_cast<float>(Input::GetInstance()->MouseY());

		CCameraComponent* cam = CEngine::GetInstance()->GetActiveScene().GetMainCamera();
		CTransformComponent* camTransform = cam->GameObject().myTransform;

		float xV = (((2 * mouseX) / width) - 1) / cam->GetProjection()._11;
		float yV = (-((2 * mouseY) / height) + 1) / cam->GetProjection()._22;

		DirectX::SimpleMath::Vector3 target = camTransform->Position() - camTransform->Transform().Forward();

		//DirectX::SimpleMath::Matrix viewMatrix = DirectX::XMMatrixLookAtLH(camTransform->Position(), target, camTransform->Transform().Up());
		DirectX::SimpleMath::Matrix viewMatrix = cam->GetViewMatrix();
		DirectX::SimpleMath::Matrix viewMatrixInv = viewMatrix.Invert();

		DirectX::SimpleMath::Vector4 origin = { 0.0f, 0.0f, 0.0f, 1.0f };
		DirectX::SimpleMath::Vector4 dir = { xV, yV, 1.0f, 0.0f };

		origin = DirectX::XMVector4Transform(origin, viewMatrixInv);
		dir = DirectX::XMVector4Transform(dir, viewMatrixInv);
		dir.Normalize();
		//CDebug::GetInstance()->DrawLine({ origin.x + dir.x * 100, origin.y + dir.y * 100, origin.z + dir.z * 100} , camTransform->Position(), 20.f);

		return DirectX::SimpleMath::Ray({ origin.x, origin.y, origin.z }, { dir.x, dir.y, dir.z });
	}
};
//};
