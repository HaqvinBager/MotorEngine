#include "stdafx.h"
#include "Camera.h"
#include <DirectXMath.h>

namespace SM = DirectX::SimpleMath;

CCamera::CCamera()
{
}

CCamera::~CCamera()
{
}

bool CCamera::Init(float aFoV, DirectX::SimpleMath::Vector2 aResolution)
{
    myProjection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(aFoV), aResolution.x / aResolution.y, 0.1f, 500.0f);
    myTransform = SM::Matrix();
    return true;
}

void CCamera::SetTransform(DirectX::SimpleMath::Vector3 aPosition, DirectX::SimpleMath::Vector3 aRotation)
{
    SetRotation(aRotation);
    SetPosition(aPosition);
}

void CCamera::SetRotation(DirectX::SimpleMath::Vector3 aRotation)
{
    SM::Vector3 translation = myTransform.Translation();
    myTransform = SM::Matrix::CreateFromYawPitchRoll(
        DirectX::XMConvertToRadians(aRotation.y), 
        DirectX::XMConvertToRadians(aRotation.x),
        DirectX::XMConvertToRadians(aRotation.z)
    );
    myTransform.Translation(translation);
}

void CCamera::SetPosition(DirectX::SimpleMath::Vector3 aPosition)
{
    myTransform.Translation(aPosition);
}

void CCamera::Move(DirectX::SimpleMath::Vector3 aMovement)
{
    myTransform.Translation(myTransform.Translation() + myTransform.Right() * aMovement.x);
    myTransform.Translation(myTransform.Translation() + myTransform.Up() * aMovement.y);
    myTransform.Translation(myTransform.Translation() - myTransform.Forward() * aMovement.z);
}

void CCamera::Rotate(DirectX::SimpleMath::Vector3 aRotation)
{
    SM::Vector3 translation = myTransform.Translation();
    myTransform *= SM::Matrix::CreateFromYawPitchRoll(aRotation.y, aRotation.x, aRotation.z);
    myTransform.Translation(translation);
    
}
