#include "pch.h"
#include "GraphicsEngine.hpp"

#include "Camera.hpp"
#include "EnvironmentLight.hpp"
#include "DirectionalLight.hpp"

#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

typedef std::array<float, 4> ClearColor;
#define ENGINE_CLEAR_COLOR_ORANGEORANGE		{1.0f, 0.55f, 0.255f, 1}
#define ENGINE_CLEAR_COLOR_GREYDARK			{0.22f, 0.22f, 0.22f, 1}
#define ENGINE_CLEAR_COLOR_GREYDARK_WARM	{0.28f, 0.25f, 0.22f, 1}

GraphicsEngine::GraphicsEngine()
{}

GraphicsEngine::~GraphicsEngine()
{}

bool GraphicsEngine::Init(WindowHandler::SWindowData someWindowData)
{
	if(!myWindowHandler.Init(someWindowData))
	{
		return false;
	}

	if(!myDXFramework.Init(&myWindowHandler))
	{
		return false;
	}

	myRenderer.Init(&myDXFramework);

	if (!Scene::GetInstance().Init())
	{
		return false;
	}

	return true;
}

void GraphicsEngine::BeginFrame()
{
	ClearColor clearColor = ENGINE_CLEAR_COLOR_GREYDARK;
	myDXFramework.BeginFrame(clearColor);
}
void GraphicsEngine::RenderFrame()
{
	EnvironmentLight* environmentLight						= Scene::GetInstance().GetEnvironmentLight();
	DirectionalLight* dirLight								= Scene::GetInstance().GetMainDirectionalLight();
	Camera* mainCamera										= Scene::GetInstance().GetMainCamera();
	std::vector<ModelSystem::ModelInstance*> modelsToRender = Scene::GetInstance().CullModels(mainCamera);
	std::vector<LineInstance*> linesToRender				= Scene::GetInstance().CullLines(mainCamera);

	myRenderer.Render(environmentLight, dirLight, mainCamera, modelsToRender);
	myRenderer.RenderLines(mainCamera, linesToRender);
}
void GraphicsEngine::EndFrame()
{
	myDXFramework.EndFrame();
}

DirectX11Framework* GraphicsEngine::GetDXFramework()
{
	return &myDXFramework;
}

WindowHandler* GraphicsEngine::GetWindowHandler()
{
	return &myWindowHandler;
}

//Scene* GraphicsEngine::GetScene()
//{
//	return &myScene;
//}
