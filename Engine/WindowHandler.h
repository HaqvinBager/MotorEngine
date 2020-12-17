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
	friend class CEngine;

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

	const HWND GetWindowHandle() const;
	DirectX::SimpleMath::Vector2 GetResolution();
	const float GetResolutionScale() const;
	void SetWindowTitle(std::string aString);

private:
	bool Init(CWindowHandler::SWindowData someWindowData);
	void SetInternalResolution();
	void SetResolution(DirectX::SimpleMath::Vector2 aResolution);

private:
	CWindowHandler::SWindowData myWindowData;
	HWND myWindowHandle;
	DirectX::SimpleMath::Vector2* myResolution;
	float myResolutionScale;
};

