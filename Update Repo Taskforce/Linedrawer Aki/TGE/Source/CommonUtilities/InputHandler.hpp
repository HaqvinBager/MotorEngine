#pragma once
#include <Windows.h>
#include <Windowsx.h>
#include <bitset>

using std::bitset;

namespace CommonUtilities {
	class InputHandler {
	public:

		enum class EMouseButton {
			Left	= 0,
			Right	= 1,
			Middle	= 2,
			Mouse4	= 3,
			Mouse5	= 4

		};

		InputHandler();

		bool UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam );
		void Update();

		bool IsKeyDown(WPARAM wParam);
		bool IsKeyPressed(WPARAM wParam);
		bool IsKeyReleased(WPARAM wParam);
		
		int MouseWindowX();
		int MouseWindowY();
		int MouseScreenX();
		int MouseScreenY();
		int MouseDeltaX();
		int MouseDeltaY();
		int MouseWheel();
		void SetMousePosition(int aX , int aY);
		bool IsMouseDown (EMouseButton aMouseButton);
		bool IsMousePressed (EMouseButton aMouseButton);
		bool IsMouseReleased (EMouseButton aMouseButton);

	private:
		int myMouseX;
		int myMouseY;
		int myMouseScreenX;
		int myMouseScreenY;
		int myMouseXLast;
		int myMouseYLast;
		int myMouseWheel;//positive = away from user, negative = towards user

		bitset<5> myMouseButtonLast;
		bitset<5> myMouseButton;

		bitset<256> myKeyDownLast;
		bitset<256> myKeyDown;
	};
}