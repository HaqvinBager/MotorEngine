#pragma once

/// To do:
/// Change how Singleton is created; i.e -> static Scene* myInstance = new Scene(); to something like the one used in IronWrought.

#include <vector>

class Camera;
class EnvironmentLight;
class DirectionalLight;
class PointLight;
class LineInstance;
namespace ModelSystem
{
	class ModelInstance;
}
class Scene
{
public:
	Scene operator=(const Scene& other) = delete;
	Scene(const Scene& other)			= delete;
	static Scene& GetInstance();
	//void Destroy();

	bool Init();

	bool AddInstance(ModelSystem::ModelInstance* aModelInstance);
	bool AddInstance(Camera* aCamera);
	bool AddInstance(EnvironmentLight* aEnvironmentLight);
	bool AddInstance(DirectionalLight* aDirectionalLight);
	bool AddInstance(PointLight* aPointLight);
	bool AddInstance(LineInstance* aLineInstance);

	void SetMainCamera(Camera* aCamera);
	void SetEnvironmentLight(EnvironmentLight* aEnvironmentLight);
	void SetMainDirectionalLight(DirectionalLight* aDirectionalLight);
	
	std::vector<ModelSystem::ModelInstance*> CullModels(Camera* aCamera);
	std::vector<LineInstance*> CullLines(Camera* aCamera);
	std::vector<PointLight*> CullPointLights(Camera* aCamera);

public:
	Camera* GetMainCamera() { return myMainCamera; }
	EnvironmentLight* GetEnvironmentLight() { return myEnvironmentLight; };
	DirectionalLight* GetMainDirectionalLight() { return myMainDirectionalLight; }

private:
	Scene();
	~Scene();

	std::vector<ModelSystem::ModelInstance*> myModels;
	std::vector<Camera*>					 myCameras;
	std::vector<EnvironmentLight*>			 myEnvLights;
	std::vector<DirectionalLight*>			 myDirLights;
	std::vector<PointLight*>				 myPntLights;
	std::vector<LineInstance*>				 myLines;

	Camera*				myMainCamera;
	EnvironmentLight*	myEnvironmentLight;
	DirectionalLight*	myMainDirectionalLight;
};