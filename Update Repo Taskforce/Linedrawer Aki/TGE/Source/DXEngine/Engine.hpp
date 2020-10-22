#pragma once
class GraphicsEngine;

class Timer;
class WindowHandler;
class Engine
{
public:
	Engine();
	~Engine();

	bool Init();
	//void Update();
	float BeginFrame();
	void RenderFrame();
	void EndFrame();

	const float GetDeltaTime() const;

	// Temp
	WindowHandler* GetWindowHandler();
	// !Temp

private:
	GraphicsEngine* myGraphicsEngine;
	Timer* myEngineTimer;
	float myDeltaTime;
};

