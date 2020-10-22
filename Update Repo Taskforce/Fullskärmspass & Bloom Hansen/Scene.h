#pragma once
#include <vector>
#include "ModelInstance.h"

class CCamera;
class CEnvironmentLight;
class CPointLight;

class CScene {
public:
    static CScene* GetInstance();

    bool AddInstance(CModelInstance* aModelInstance);
    bool AddInstance(CCamera* aCamera);
    bool AddInstance(CEnvironmentLight* aEnvironmentLight);
    bool AddInstance(CPointLight* aPointLight);
    void SetMainCamera(CCamera* aCamera);
    CCamera* GetMainCamera();
    CEnvironmentLight* GetEnvironmentLight();
    std::vector<CModelInstance*> CullModels(CCamera* aCamera);
    std::pair<unsigned int, std::array<CPointLight*, 8>> CullLights(CModelInstance* aModelInstance);

private:
    CScene();
    ~CScene();

    std::vector<CModelInstance*> myModels;
    std::vector<CCamera*> myCameras;
    std::vector<CPointLight*> myPointLights;
    CEnvironmentLight* myEnvironmentLight;
    CCamera* myMainCamera;
};