#pragma once
#include "WinInclude.h"
#include <Windowsx.h>
#include <bitset>
#include <memory>
#include <stack>

namespace CommonUtilities {
	class Input {
	public:

		static Input* GetInstance();

		enum class MouseButton {
			Left = 0,
			Right = 1,
			Middle = 2,
			Mouse4 = 3,
			Mouse5 = 4

		};//regular enum was easier to use.

		Input();

		bool update_events(UINT message, WPARAM wParam, LPARAM lParam);
		void update();
		

		bool move_left();
		bool move_right();
		bool move_up();
		bool MoveDown();

		bool IsKeyDown(WPARAM wParam);
		bool IsKeyPressed(WPARAM wParam);
		bool IsKeyReleased(WPARAM wParam);

		int MouseX();//x coordiantes in app window
		int MouseY();//y coordiantes in app window
		int MouseScreenX();
		int MouseScreenY();
		int MouseDeltaX();
		int MouseDeltaY();
		int MouseWheel();
		void SetMouseScreenPosition(int x, int y);
		bool IsMouseDown(MouseButton mouse_button);
		bool IsMousePressed(MouseButton mouse_button);
		bool IsMouseReleased(MouseButton mouse_button);

	private:
		int _mouse_x;
		int _mouse_y;
		int _mouse_screen_x;
		int _mouse_screen_y;
		int _mouse_x_last;
		int _mouse_y_last;
		int _mouse_wheel;//positive = away from user, negative = towards user

		std::bitset<5> _mouse_button_last;
		std::bitset<5> _mouse_button;

		std::bitset<256> _key_down_last;
		std::bitset<256> _key_down;
	};
}