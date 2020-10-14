#include "WinInclude.h"
#include "Engine.h"
#include "Input.h"

#include <SimpleMath.h>
#include <Scene.h>
#include <CameraFactory.h>
#include <Camera.h>
#include <Model.h>
#include <ModelFactory.h>
#include <ModelInstance.h>
#include <LightFactory.h>
#include <EnvironmentLight.h>
#include <Timer.h>
#include "LevelLoader.h"

#include <Animation.h>

using namespace CommonUtilities;
namespace SM = DirectX::SimpleMath;

#define USE_CONSOLE_COMMAND
void InitConsole()
{
#pragma warning( push )
#pragma warning( disable : 4996 )
#pragma warning( disable : 6031 )
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
#pragma warning( pop )
}

void CloseConsole()
{
#pragma warning( push )
#pragma warning( disable : 4996 )
	fclose(stdin); 
	fclose(stdout);
	fclose(stderr);
#pragma warning( pop )
}

void InitModels()
{
	CScene* scene = CScene::GetInstance();

	CCamera* camera = CCameraFactory::GetInstance()->CreateCamera(65.0f, 5000.0f);
	camera->SetPosition({ 0,0,0 });
	scene->AddInstance(camera);
	scene->SetMainCamera(camera);

	CLightFactory* lightFactory = CLightFactory::GetInstance();
	CEnvironmentLight* environmentLight = lightFactory->CreateEnvironmentLight("Yokohama2.dds");

	CModel* cubeModel = CModelFactory::GetInstance()->GetCube();
	CModelInstance* cube = new CModelInstance();
	cube->Init(cubeModel);
	scene->AddInstance(cube);

	CModelInstance* model = CModelFactory::GetInstance()->CreateModel("Model/Chest/Particle_Chest.fbx", { 0.025f, 0.025f, 0.025f });
	model->SetPosition({ 12.5f, 0.0f, 15.0f });
	environmentLight->SetDirection(SM::Vector3(0, 1, -1));
	environmentLight->SetColor(SM::Vector3(0.8f, 0.8f, 0.8f));

	scene->AddInstance(model);
	scene->AddInstance(environmentLight);
}

void Update()
{

}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	hInstance;
	hPrevInstance;
	lpCmdLine;
	nShowCmd;

#ifdef USE_CONSOLE_COMMAND
	InitConsole();
#endif

	CWindowHandler::SWindowData windowData;
	windowData.myX = 100;
	windowData.myY = 100;
	windowData.myWidth = 1280;
	windowData.myHeight = 720;
	
	CEngine engine;
	bool shouldRun = engine.Init(windowData);
	if (!shouldRun)
		return 1;
	
	InitModels();

	MSG windowMessage = { 0 };
	while (shouldRun)
	{
		while (PeekMessage(&windowMessage, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&windowMessage);
			DispatchMessage(&windowMessage);
	
			if (windowMessage.message == WM_QUIT)
			{
				shouldRun = false;
			}
		}
	
		engine.BeginFrame();
		Update();
		engine.RenderFrame();
		engine.EndFrame();
		//Input::GetInstance()->update();
	}

#ifdef USE_CONSOLE_COMMAND
	CloseConsole();
#endif
	return 0;
}