#include "stdafx.h"
#include <array>
#include "Engine.h"

#include "WindowHandler.h"
#include "DirectXFramework.h"
#include "ForwardRenderer.h"
#include "Scene.h"
#include "Camera.h"
#include "Timer.h"
#include "ModelFactory.h"
#include "CameraFactory.h"
#include "EnvironmentLight.h"
#include "LightFactory.h"
#include "RenderManager.h"
#include "ParticleFactory.h"
#include "VFXFactory.h"
#include "LineFactory.h"
#include "SpriteFactory.h"
#include "TextFactory.h"
#include "InputMapper.h"
#include <rapidjson\document.h>
#include <string>
#include "Debug.h"
#include <ScreenGrab.h>
#include <wincodec.h>
#include "EnemyFactory.h"
#include "DL_Debug.h"
#include "MainSingleton.h"
#include "AudioManager.h"
#include <string>

#include "StateStack.h"

#pragma comment(lib, "runtimeobject.lib")
#pragma comment(lib, "d3d11.lib")

CEngine* CEngine::ourInstance = nullptr;

CEngine::CEngine()
{
	ourInstance = this;

	myScene = new CScene();
	myTimer = new CTimer();
	myWindowHandler = new CWindowHandler();
	myFramework = new CDirectXFramework();
	myForwardRenderer = new CForwardRenderer();
	myModelFactory = new CModelFactory();
	myCameraFactory = new CCameraFactory();
	myLightFactory = new CLightFactory();
	myParticleFactory = new CParticleFactory();
	myVFXFactory = new CVFXFactory();
	myLineFactory = new CLineFactory();
	mySpriteFactory = new CSpriteFactory();
	myTextFactory = new CTextFactory();
	myInputMapper = new CInputMapper();
	myDebug = new CDebug();
	myRenderManager = nullptr;
	myEnemyFactory = new CEnemyFactory();
	myMainSingleton = new CMainSingleton();
	// Audio Manager must be constructed after main singleton, since it subscribes to postmaster messages
	myAudioManager = new CAudioManager();
	//myActiveScene = 0; //muc bad
	myActiveState = CStateStack::EState::MainMenu;
	myInGameScene = nullptr;
}

CEngine::~CEngine()
{
	delete myScene;
	myScene = nullptr;
	delete myWindowHandler;
	myWindowHandler = nullptr;
	delete myFramework;
	myFramework = nullptr;
	delete myTimer;
	myTimer = nullptr;

	delete myModelFactory;
	myModelFactory = nullptr;
	delete myCameraFactory;
	myCameraFactory = nullptr;
	delete myLightFactory;
	myLightFactory = nullptr;
	delete myRenderManager;
	myRenderManager = nullptr;

	delete myParticleFactory;
	myParticleFactory = nullptr;
	delete myVFXFactory;
	myVFXFactory = nullptr;
	delete myLineFactory;
	myLineFactory = nullptr;
	delete mySpriteFactory;
	mySpriteFactory = nullptr;
	delete myTextFactory;
	myTextFactory = nullptr;
	delete myInputMapper;
	myInputMapper = nullptr;

	delete myDebug;
	myDebug = nullptr;

	delete myEnemyFactory;
	myEnemyFactory = nullptr;

	// Audio Manager must be destroyed before main singleton, since it unsubscribes from postmaster messages
	delete myAudioManager;
	myAudioManager = nullptr;

	delete myMainSingleton;
	myMainSingleton = nullptr;


	ourInstance = nullptr;
}

bool CEngine::Init(CWindowHandler::SWindowData& someWindowData)
{
	ENGINE_ERROR_BOOL_MESSAGE(myWindowHandler->Init(someWindowData), "Window Handler could not be initialized.");
	ENGINE_ERROR_BOOL_MESSAGE(myFramework->Init(myWindowHandler), "Framework could not be initialized.");
	myWindowHandler->SetResolution();
	ENGINE_ERROR_BOOL_MESSAGE(myModelFactory->Init(*this), "Model Factory could not be initiliazed.");
	ENGINE_ERROR_BOOL_MESSAGE(myCameraFactory->Init(myWindowHandler), "Camera Factory could not be initialized.");
	ENGINE_ERROR_BOOL_MESSAGE(myScene->Init(), "Scene could not be initialized.");
	myRenderManager = new CRenderManager();
	ENGINE_ERROR_BOOL_MESSAGE(myRenderManager->Init(myFramework, myWindowHandler), "RenderManager could not be initialized.");
	ENGINE_ERROR_BOOL_MESSAGE(myLightFactory->Init(*this), "Light Factory could not be initialized.");
	ENGINE_ERROR_BOOL_MESSAGE(myParticleFactory->Init(myFramework), "Particle Factory could not be initialized.");
	ENGINE_ERROR_BOOL_MESSAGE(myVFXFactory->Init(myFramework), "VFX Factory could not be initialized.");
	ENGINE_ERROR_BOOL_MESSAGE(myLineFactory->Init(myFramework), "Line Factory could not be initialized.");
	ENGINE_ERROR_BOOL_MESSAGE(mySpriteFactory->Init(myFramework), "Sprite Factory could not be initialized.");
	ENGINE_ERROR_BOOL_MESSAGE(myTextFactory->Init(myFramework), "Text Factory could not be initialized.");
	ENGINE_ERROR_BOOL_MESSAGE(myInputMapper->Init(), "InputMapper could not be initialized");

	InitWindowsImaging();
	return true;
}

float CEngine::BeginFrame()
{
#ifdef _DEBUG
	std::string fpsString = std::to_string((1.0f / CTimer::Dt()));
	size_t decimalIndex = fpsString.find_first_of('.');
	fpsString = fpsString.substr(0, decimalIndex);
	myWindowHandler->SetWindowTitle("IronWrought | FPS: " + fpsString);
#endif // _DEBUG

	std::array<float, 4> clearColor = { 0.5f, 0.5f, 0.5f, 1.0f };
	myFramework->BeginFrame(clearColor);

#ifdef _DEBUG
	myDebug->Update();
	//CDebug::GetInstance()->Update();
#endif

	return myTimer->Mark();
}

void CEngine::RenderFrame()
{
	if (myActiveState == CStateStack::EState::InGame)
	{
		myRenderManager->Render(*myInGameScene);
	}
	else
	{
		myRenderManager->Render(*mySceneMap[myActiveState]);
	}
	//if(myScenes.size() > 0 && myActiveScene < myScenes.size())
	//	myRenderManager->Render(*myScenes[myActiveScene]);
}

void CEngine::EndFrame()
{
	myFramework->EndFrame();
}

CWindowHandler* CEngine::GetWindowHandler()
{
	return myWindowHandler;
}

void CEngine::InitWindowsImaging()
{
#if (_WIN32_WINNT >= 0x0A00 /*_WIN32_WINNT_WIN10*/)
	Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);
	if (FAILED(initialize))
		// error
#else
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr))
		// error
#endif
		return;
}

#include <DbgHelp.h>
#include <strsafe.h>

void CEngine::CrashWithScreenShot(std::wstring &aSubPath)
{
	DL_Debug::CDebug::GetInstance()->CopyToCrashFolder(aSubPath);

	aSubPath += L"\\screenshot.bmp";
	HRESULT hr = CoInitialize(nullptr);
	hr = SaveWICTextureToFile(myFramework->GetContext(), myFramework->GetBackbufferTexture(),
		GUID_ContainerFormatBmp, aSubPath.c_str(),
		&GUID_WICPixelFormat16bppBGR565);

	if (FAILED(hr)) {
		return;
	}
	CoUninitialize();
}

CEngine* CEngine::GetInstance()
{
	return ourInstance;
}

const CStateStack::EState CEngine::AddScene(const CStateStack::EState aState, CScene* aScene)
{
	if (aState == CStateStack::EState::InGame)
	{
		if (myInGameScene != nullptr)
		{
			delete myInGameScene;
			myInGameScene = nullptr;
		}
		myInGameScene = aScene;
		return aState;
	}

	auto it = mySceneMap.find(aState);
	if (it/*mySceneMap.find(aState)*/ != mySceneMap.end())
	{
		std::cout << __FUNCTION__ << " Deleteing scene for " << static_cast<int>(aState) << std::endl;
		delete it->second;
		it->second = nullptr;
		//delete mySceneMap[aState];
		//mySceneMap[aState] = nullptr;
		mySceneMap.erase(it);
	}
	mySceneMap[aState] = aScene;

	return aState;
}

//void CEngine::PopBackScene()
//{
//	myScenes.back()->ClearScene();
//	myScenes.pop_back();
//}

void CEngine::SetActiveScene(const CStateStack::EState aState)
{
	myActiveState = aState;
}

//void CEngine::SetActiveScene(CScene* aScene)
//{
//	for (unsigned int i = 0; i < myScenes.size(); ++i)
//	{
//		if (myScenes[i] == aScene)
//		{
//			myActiveScene = i;
//			std::cout << "Active Scene Index: " << i << std::endl;
//		}
//	}
//}

CScene& CEngine::GetActiveScene()
{
	if (myActiveState == CStateStack::EState::InGame)
	{
		return *myInGameScene;
	}
	return *mySceneMap[myActiveState];
	//return *myScenes[myActiveScene];
}

//unsigned int CEngine::ScenesSize()
//{
//	return static_cast<unsigned int>(myScenes.size() - 1);
//}
