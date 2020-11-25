#pragma once
#include "WinInclude.h"
#include <string>

namespace DirectX {
	namespace SimpleMath {
		struct Vector2;
	}
}

class CWindowHandler
{
public:
	struct SWindowData
	{
		UINT myX;
		UINT myY;
		UINT myWidth;
		UINT myHeight;
	};

	static LRESULT CALLBACK WinProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

	CWindowHandler();
	~CWindowHandler();

	bool Init(CWindowHandler::SWindowData someWindowData);
	const HWND GetWindowHandle() const;

	DirectX::SimpleMath::Vector2 GetResolution();
	void SetResolution();

	void SetWindowTitle(std::string aString);
	//void HandleInput(float dt);

private:
	UINT GetWidth() const;
	UINT GetHeight() const;
	
private:
	CWindowHandler::SWindowData myWindowData;
	HWND myWindowHandle;
	DirectX::SimpleMath::Vector2* myResolution;
	//CKeyboard myKeyboard;
	//CMouse myMouse;
};

