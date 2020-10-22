#include "stdafx.h"
#include <array>
#include "Engine.h"
#include "WindowHandler.h"
#include "DirectXFramework.h"
#include "Scene.h"
#include "Camera.h"
#include "Timer.h"
#include "ModelFactory.h"
#include "CameraFactory.h"
#include "LightFactory.h"
#include "EnvironmentLight.h"
#include "LightFactory.h"
#include "RenderManager.h"
#include "ParticleFactory.h"
#include "VFXFactory.h"

#pragma comment(lib, "d3d11.lib")

CEngine::CEngine()
{
	myWindowHandler = new CWindowHandler();
	myFramework = new CDirectXFramework();
	myScene = new CScene();
	myEngineTimer = new CTimer();
	myModelFactory = new CModelFactory();
	myCameraFactory = new CCameraFactory();
	myLightFactory = new CLightFactory();
	myParticleFactory = new CParticleFactory();
	myVFXFactory = new CVFXFactory();
}

CEngine::~CEngine()
{
	delete myWindowHandler;
	myWindowHandler = nullptr;
	delete myFramework;
	myFramework = nullptr;
	delete myScene;
	myScene = nullptr;
	delete myEngineTimer;
	myEngineTimer = nullptr;
	delete myModelFactory;
	myModelFactory = nullptr;
	delete myCameraFactory;
	myCameraFactory = nullptr;
	delete myLightFactory;
	myLightFactory = nullptr;
	delete myRenderManager;
	myRenderManager = nullptr;
	//TODO: change to emitterfactory
	delete myParticleFactory;
	myParticleFactory = nullptr;
	delete myVFXFactory;
	myVFXFactory = nullptr;
}

bool CEngine::Init(CWindowHandler::SWindowData& someWindowData)
{
	if (!myWindowHandler->Init(someWindowData))
	{
		return false;
	}

	if (!myFramework->Init(myWindowHandler))
	{
		return false;
	}

	if (!myModelFactory->Init(*this)) {
		return false;
	}

	if (!myCameraFactory->Init(myWindowHandler)) {
		return false;
	}

	if (!myScene->Init()) {
		return false;
	}

	if (!myLightFactory->Init(*this)) {
		return false;
	}

	myRenderManager = new CRenderManager(*myScene);

	if (!myRenderManager->Init(myFramework, myWindowHandler)) {
		return false;
	}

	if (!myParticleFactory->Init(myFramework)) {
		return false;
	}

	if (!myVFXFactory->Init(*this)) {
		return false;
	}

	return true;
}

float CEngine::BeginFrame()
{
	return myEngineTimer->Mark();
}

void CEngine::RenderFrame()
{
	myRenderManager->Render();
}

void CEngine::EndFrame()
{
	myFramework->EndFrame();
}
