#pragma once
#include "WindowHandler.hpp"
#include "DirectX11Framework.hpp"
#include "ForwardRenderer.hpp"
#include "Scene.hpp"

class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();

	bool Init(WindowHandler::SWindowData someWindowData);
	void BeginFrame();
	void RenderFrame();
	void EndFrame();

	DirectX11Framework* GetDXFramework();
	WindowHandler*		GetWindowHandler();
	//Scene* GetScene();

private:
	WindowHandler				myWindowHandler;
	DirectX11Framework			myDXFramework;
	Rendering::ForwardRenderer	myRenderer;
	//Scene						myScene;
};