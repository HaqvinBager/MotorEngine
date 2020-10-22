#include "WinInclude.h"
#include "Engine.h"
#include "Input.h"
#include "Game.h"

#ifdef _DEBUG
#pragma comment(lib, "Game_Debug.lib")
#endif // _DEBUG
#ifdef NDEBUG
#pragma comment(lib, "Game_Release.lib")
#endif // NDEBUG

using namespace CommonUtilities;

#ifdef _DEBUG
#define USE_CONSOLE_COMMAND
#endif
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

	CGame game;
	game.Init();

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
		game.Update();
		engine.RenderFrame();
		engine.EndFrame();
		Input::GetInstance()->update();
	}

#ifdef USE_CONSOLE_COMMAND
	CloseConsole();
#endif
	return 0;
}

	/*CScene* scene = CScene::GetInstance();
	CModelFactory* modelFactory = CModelFactory::GetInstance();
	CCameraFactory* cameraFactory = CCameraFactory::GetInstance();

	cameraFactory->Init(engine.GetWindowHandler());
	modelFactory->Init(engine);


	CCamera* camera = cameraFactory->CreateCamera(65.0f);
	camera->SetTransform(Vector3(0.0f, 0.0f, -5.0f), Vector3(0.0f, 0.0f, 0.0f));
	scene->AddInstance(camera);
	scene->SetMainCamera(camera);

	CModel* model = modelFactory->LoadModel("Model/enemy_robot/", "enemy_robot.fbx");
	CModelInstance* model_instance_enemy = new CModelInstance();
	model_instance_enemy->Init(model);
	scene->AddInstance(model_instance_enemy);
	model_instance_enemy->SetPosition(Vector3(2, -1, 7));

	CModel* cube = modelFactory->GetCube();
	CModelInstance* cubeModelInstance = new CModelInstance();
	cubeModelInstance->Init(cube);
	scene->AddInstance(cubeModelInstance);
	cubeModelInstance->SetPosition(Vector3(0, 0, 7));


	CModelInstance* cubeModelInstance_1 = new CModelInstance();
	cubeModelInstance_1->Init(cube);
	scene->AddInstance(cubeModelInstance_1);
	cubeModelInstance_1->SetPosition(Vector3(-2, 0, 7));*/

	//const int count = 100;
	//CModelInstance* cube_instances = new CModelInstance[count];
	//for (int i = 0; i < count; ++i) {
	//	cube_instances[i].Init(cube);
	//	scene->AddInstance(&cube_instances[i]);
	//	cube_instances[i].SetPosition(Vector3(-10.0f + (i * 0.5f), 0.0f, (i * 0.8f)));
	//}

		//const auto dt = engine.BeginFrame();
		//static float duration = 0.0f;
		//duration += dt;
		//
		//Vector3 camera_movement_input(0, 0, 0);
		//camera_movement_input.z = Input::GetInstance()->IsKeyDown('W') ? 1.0f : camera_movement_input.z;
		//camera_movement_input.x = Input::GetInstance()->IsKeyDown('A') ? -1.0f : camera_movement_input.x;
		//camera_movement_input.z = Input::GetInstance()->IsKeyDown('S') ? -1.0f : camera_movement_input.z;
		//camera_movement_input.x = Input::GetInstance()->IsKeyDown('D') ? 1.0f : camera_movement_input.x;
		//camera_movement_input.y = Input::GetInstance()->IsKeyDown('Q') ? 1.0f : camera_movement_input.y;
		//camera_movement_input.y = Input::GetInstance()->IsKeyDown('E') ? -1.0f : camera_movement_input.y;
		//camera->Move(camera_movement_input * dt);

		//if (Input::GetInstance()->IsMouseDown(Input::MouseButton::Left)) {
		//	Vector2 mouse_movement_input(0, 0);
		//	mouse_movement_input.x = (float)Input::GetInstance()->MouseDeltaX();
		//	mouse_movement_input.y = (float)Input::GetInstance()->MouseDeltaY();
		//	camera->Rotate({ mouse_movement_input.x * dt, mouse_movement_input.y * dt, 0.0f });
		//}

		////camera->Rotate()


		//Vector2 uvoffset = cubeModelInstance->GetUVOffset();
		//if (Input::GetInstance()->IsKeyDown('U')) {
		//	cubeModelInstance->SetUVOffset(Vector2(duration, uvoffset.y));
		//}

		//if (Input::GetInstance()->IsKeyDown('V')) {		
		//	cubeModelInstance->SetUVOffset(Vector2(uvoffset.x, duration));
		//}

		//	
		//cubeModelInstance->Rotate(Vector3(sinf(duration) * dt, sinf(duration) * dt, cosf(duration) * dt));
		////cubeModelInstance->SetPosition(Vector3(cosf(duration) * dt, 0.0f, 7.0f));

		//cubeModelInstance_1->Rotate(Vector3(cosf(duration) * dt, cosf(duration) * dt, sinf(duration) * dt));
		//cubeModelInstance_1->SetPosition(Vector3(cosf(duration) * 15.0f, sinf(duration) * 15.0f, 25.0f));

		////cubeModelInstance_1->Rotate(Vector3(cosf(duration) * dt, cosf(duration) * dt, sinf(duration) * dt));

	//	game.Update();
	//	engine.RenderFrame();
	//	engine.EndFrame();
	//	Input::GetInstance()->update();
	//}
	//return 0;
//}