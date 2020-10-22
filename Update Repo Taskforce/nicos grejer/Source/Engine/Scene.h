#pragma once
#include <vector>
#include <array>

class CModelInstance;
class CCamera;
class CEnvironmentLight;
class CPointLight;
class CParticleInstance;
class CVFXInstance;

class CScene
{
public:
	CScene();
	~CScene();

	bool Init();

	bool AddInstance(CModelInstance* aModelInstance);
	bool AddInstance(CCamera* aCamera);
	bool AddInstance(CEnvironmentLight* anEnvironmentLight);
	bool AddInstance(CPointLight* aPointLight);
	bool AddInstance(CParticleInstance* aParticleInstance);
	bool AddInstance(CVFXInstance* aVFXInstance);
	std::vector<CModelInstance*> CullModels(CCamera* aCamera);
	std::pair<unsigned int, std::array<CPointLight*, 8>> CullLights(CModelInstance* anInstance);
	std::vector<CParticleInstance*> CullParticles(CCamera* aCamera);
	std::vector<CVFXInstance*> CullVFX(CCamera* aCamera);

	void SetModelToOutline(CModelInstance* aModelInstance);
	static CScene* GetInstance();

public:
	CCamera* GetMainCamera() const { return myMainCamera; }
	void SetMainCamera(CCamera* aCamera) { myMainCamera = aCamera; }
	CEnvironmentLight* GetEnvironmentLight() const { return myEnvironmentLight; }
	std::vector<CPointLight*> GetPointLights() const { return myPointLights; }

	CModelInstance* GetModelToOutline() const { return myModelToOutline; }

private:
	static CScene* ourInstance;
	std::vector<CModelInstance*> myModels;
	std::vector<CCamera*> myCameras;
	std::vector<CPointLight*> myPointLights;
	std::vector<CParticleInstance*> myParticles;
	std::vector<CVFXInstance*> myVFXInstances;
	CCamera* myMainCamera;
	CEnvironmentLight* myEnvironmentLight;

	CModelInstance* myModelToOutline;
};

