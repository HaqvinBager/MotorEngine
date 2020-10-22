#pragma once

#include <vector>

namespace CommonUtilities
{
	class InputHandler;
}

class EnvironmentLight;
class DirectionalLight;
class Camera;
class LineInstance;
namespace ModelSystem{
	class ModelInstance;
}

class Game
{
public:
	Game();
	~Game();

	bool Init();
	bool Update(const float dt, CommonUtilities::InputHandler& aInputHandler);

private:
	void InitcallBack();
	void UpdateCallBack();

	// Temp
	std::vector<ModelSystem::ModelInstance*> myModels;
	std::vector<LineInstance*>				 myLines;
	LineInstance* myGrid;
	Camera* myCamera;
	EnvironmentLight* myEnvironmentLight;
	DirectionalLight* myDirectionalLight;
	// !Temp
};

