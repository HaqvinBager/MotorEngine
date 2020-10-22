#include "pch.h"
#include "Engine.hpp"

#include "Timer.hpp"
#include "GraphicsEngine.hpp"
//#include "Model.hpp"
//#include "ModelInstance.hpp"
#include "ModelFactory.hpp"
#include "Camera.hpp"
#include "CameraFactory.hpp"
#include "LightFactory.hpp"
#include "LineFactory.hpp"

//#include "SimpleMath.h"
namespace SM = DirectX::SimpleMath;
namespace MS = ModelSystem;

Engine::Engine()
{
	myGraphicsEngine	= new GraphicsEngine();
	myEngineTimer		= new Timer();
	myDeltaTime			= 0.0f;
}
Engine::~Engine()
{
	ModelSystem::ModelFactory::GetInstance().Destroy();
	CameraFactory::GetInstance().Destroy();
	LightFactory::GetInstance().Destroy();
	LineFactory::GetInstance().Destroy();
	SAFE_DELETE(myGraphicsEngine)
	SAFE_DELETE(myEngineTimer)
}

bool Engine::Init()
{
	WindowHandler::SWindowData windowData;
	windowData.myX      = 100;
	windowData.myY      = 100;
	windowData.myWidth  = 1280;
	windowData.myHeight = 720;
	bool shouldRun = myGraphicsEngine->Init(windowData);
	if( !shouldRun )
	{
		assert(false && "Engine initialization failed!");
		return false;
	}
	
	ModelSystem::ModelFactory::Create();
	ModelSystem::ModelFactory& modelFactory = ModelSystem::ModelFactory::GetInstance();
	modelFactory.Init(myGraphicsEngine->GetDXFramework());

	LightFactory::Create();
	LightFactory::GetInstance().Init(myGraphicsEngine->GetDXFramework());

	CameraFactory::Create();
	CameraFactory::GetInstance().Init(myGraphicsEngine->GetWindowHandler());

	LineFactory::Create();
	LineFactory::GetInstance().Init(myGraphicsEngine->GetDXFramework());

	return true;
}

//void Engine::Update()
//{
//	myGraphicsEngine->BeginFrame()
//	myGraphicsEngine->EndFrame();
//}
float Engine::BeginFrame()
{
	myGraphicsEngine->BeginFrame();
	myDeltaTime = myEngineTimer->Mark();
	return myDeltaTime;
}
void Engine::RenderFrame()
{
	myGraphicsEngine->RenderFrame();
}
void Engine::EndFrame()
{
	myGraphicsEngine->EndFrame();
}

const float Engine::GetDeltaTime() const
{
	return myDeltaTime;
}

WindowHandler* Engine::GetWindowHandler()
{
	return myGraphicsEngine->GetWindowHandler();
}
