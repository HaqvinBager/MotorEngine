#pragma once
#include "SimpleMath.h"
#include "Engine.h"
#include "Scene.h"
#include "CameraComponent.h"

class CUtility
{
public:
	static DirectX::SimpleMath::Vector2 WorldToScreen(DirectX::SimpleMath::Vector3 aWorldPos) {
		DirectX::SimpleMath::Vector2 screen = DirectX::XMVector3Project(aWorldPos, 0, 0, CEngine::GetInstance()->GetWindowHandler()->GetResolution().x, CEngine::GetInstance()->GetWindowHandler()->GetResolution().y, 0, 1, CEngine::GetInstance()->GetActiveScene().GetMainCamera()->GetProjection(), CEngine::GetInstance()->GetActiveScene().GetMainCamera()->GetViewMatrix(), DirectX::XMMatrixIdentity());

		screen.x /= CEngine::GetInstance()->GetWindowHandler()->GetResolution().x;
		screen.y /= CEngine::GetInstance()->GetWindowHandler()->GetResolution().y;
		screen.x -= 0.5f;
		screen.y -= 0.5f;
		screen.x *= 2.0f;
		screen.y *= 2.0f;

		return screen;
	}

	//Maybe add ScreenToWorld?
};

