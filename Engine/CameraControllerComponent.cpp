#include "stdafx.h"
#include "CameraControllerComponent.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "Engine.h"
#include "Scene.h"
#include "GameObject.h"

CCameraControllerComponent::CCameraControllerComponent(CGameObject& aGameObject, const float aCameraMoveSpeed /*, ECameraMode aCameraMode, char aToggleFreeCam*/) 
	: CComponent(aGameObject),
	myCameraMoveSpeed(aCameraMoveSpeed),
	myCamera(nullptr)
{
	
}

CCameraControllerComponent::~CCameraControllerComponent()
{
}

void CCameraControllerComponent::Awake()
{
	myCamera = CEngine::GetInstance()->GetActiveScene().GetMainCamera();
}

void CCameraControllerComponent::Start()
{

}

void CCameraControllerComponent::Update()
{
	//std::cout << "Camera Controller Update" << std::endl;
	const float dt = CTimer::Dt();
	//float cameraMoveSpeed = 25.0f;
	float verticalMoveSpeedModifier = 1.5f;
	DirectX::SimpleMath::Vector3 camera_movement_input(0, 0, 0);
	camera_movement_input.z = Input::GetInstance()->IsKeyDown('W') ? myCameraMoveSpeed : camera_movement_input.z;
	camera_movement_input.z = Input::GetInstance()->IsKeyDown('S') ? -myCameraMoveSpeed : camera_movement_input.z;
	camera_movement_input.x = Input::GetInstance()->IsKeyDown('D') ? myCameraMoveSpeed : camera_movement_input.x;
	camera_movement_input.x = Input::GetInstance()->IsKeyDown('A') ? -myCameraMoveSpeed : camera_movement_input.x;
	camera_movement_input.y = Input::GetInstance()->IsKeyDown('R') ? myCameraMoveSpeed * verticalMoveSpeedModifier : camera_movement_input.y;
	camera_movement_input.y = Input::GetInstance()->IsKeyDown('F') ? -myCameraMoveSpeed * verticalMoveSpeedModifier : camera_movement_input.y;

	float rotationSpeed = 1.0f;
	float camera_rotation_input = 0;
	camera_rotation_input = Input::GetInstance()->IsKeyDown('Q') ? -rotationSpeed : camera_rotation_input;
	camera_rotation_input = Input::GetInstance()->IsKeyDown('E') ? rotationSpeed : camera_rotation_input;

	myCamera->GameObject().myTransform->MoveLocal(camera_movement_input * myCameraMoveSpeed * dt);
	myCamera->GameObject().myTransform->Rotate({ 0, camera_rotation_input * rotationSpeed * dt, 0 });
}