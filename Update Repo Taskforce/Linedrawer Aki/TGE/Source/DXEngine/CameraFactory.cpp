#include "pch.h"
#include "CameraFactory.hpp"

#include "Camera.hpp"
#include "WindowHandler.hpp"

namespace SM = DirectX::SimpleMath;

CameraFactory* CameraFactory::ourInstance = nullptr;

bool CameraFactory::Create()
{
    assert(ourInstance == nullptr && "CameraFactory already created!");
    if (!ourInstance)
    {
        ourInstance = new CameraFactory();
        return true;
    }
    return false;
}

CameraFactory& CameraFactory::GetInstance()
{
    assert(ourInstance != nullptr && "Instance has not been created. Create and Init one!");
    return *ourInstance;
}

void CameraFactory::Destroy()
{
    assert(ourInstance != nullptr && "No instance exists to Destroy!");
    if( ourInstance )
    {
        SAFE_DELETE(ourInstance)
    }
}

bool CameraFactory::Init(WindowHandler* aWindowHandler)
{
    assert(aWindowHandler != nullptr && "WindowHandler is NULL!");
    if (!aWindowHandler)
    {
        return false;
    }
    myWindowHandler = aWindowHandler;
    
    return true;
}

Camera* CameraFactory::CreateCamera(const float& aFov)
{
    Camera* camera = new Camera();
    camera->Init(aFov, SM::Vector2(static_cast<float>(myWindowHandler->GetWidth()), static_cast<float>(myWindowHandler->GetHeight())));
    myCameras.emplace_back(camera);

    return camera;
}

CameraFactory::CameraFactory()
    : myWindowHandler(nullptr)
{}
CameraFactory::~CameraFactory()
{
    for (auto i = 0; i < myCameras.size(); ++i)
    {
        delete myCameras[i];
        myCameras[i] = nullptr;
    }
}
