#pragma once
/*
	Launcher::Main(...) calls: 
	engine.RenderFrame()
	{ 
		myGrahicsEngine->RenderFrame();
add-->  myGraphicsEngine->RenderLines();// after so that they are drawn ontop of everything
		// uses ForwardRenderer? Needs camera
	}

	RenderLines works like Render
	but only renders the lines that exist in 
	LineManager --> Scene?

	LineFactory
	LineManager(singleton?)
	{
		container to hold all active line calls.
		passes these to renderer to render lines
	}
	
	The most common solution would be to use 
	D3D11_PRIMITIVE_TOPOLOGY_LINELIST 
	in your 
	IASetPrimitiveTopology 
	calls.

	Write a simple Vertex Shader that takes position.
	Write a simple Pixel Shader that returns desired color.

	Create DebugTools namespace to hold all Line-related functions

	What to do with the Line? Treat it as a Model? Line -> LineInstance?
	LineInstance holds a transform? allows for manip of Line
	a LineInstance can then be a set of Lines?

	
*/