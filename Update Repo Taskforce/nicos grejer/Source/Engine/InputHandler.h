#pragma once

#include <bitset>

#include "Windows.h"
#include "Windowsx.h"
#include <Xinput.h>
#include <array>
#include <queue>
#include <tuple>

using std::bitset;

namespace CommonUtilities {
	class InputHandler {
	public:
		enum class MouseButton {
			Left = 0,
			Right = 1,
			Middle = 2,
			Mouse4 = 3,
			Mouse5 = 4,
		};

		enum class XButton {
			DPAD_UP = 0x0001,
			DPAD_DOWN = 0x0002,
			DPAD_LEFT = 0x0004,
			DPAD_RIGHT = 0x0008,
			START = 0x0010,
			BACK = 0x0020,
			LEFT_THUMB = 0x0040,
			RIGHT_THUMB = 0x0080,
			LEFT_SHOULDER = 0x0100,
			RIGHT_SHOULDER = 0x0200,
			A = 0x1000,
			B = 0x2000,
			X = 0x4000,
			Y = 0x8000
		};
		
		enum class GamePadIndex{one = 0,two,three,four};

		bool update_events(UINT message, WPARAM wParam, LPARAM lParam);
		void update();

		bool is_key_pressed(WPARAM wParam) const;
		bool is_key_down(WPARAM wParam) const;
		bool is_key_released(WPARAM wParam) const;

		float scaled_mouse_x() const;
		float scaled_mouse_y() const;
		float scaled_mouse_delta_x() const;
		float scaled_mouse_delta_y() const;
		int mouse_x() const;
		int mouse_y() const;
		int mouse_delta_x() const;
		int mouse_delta_y() const;
		int mouse_screen_x() const;
		int mouse_screen_y() const;
		int mouse_wheel() const;
		void set_mouse_screen_position(int x, int y);
		bool is_mouse_pressed(MouseButton mouse_button) const;
		bool is_mouse_down(MouseButton mouse_button) const;
		bool is_mouse_released(MouseButton mouse_button) const;
		
		XINPUT_GAMEPAD *x_state_controll(const GamePadIndex index);
		bool x_check_connection_controll(const GamePadIndex index);
		bool x_refresh_controll(const GamePadIndex index);
		bool x_is_connected(const GamePadIndex index) const;
		bool x_is_pressed(const GamePadIndex index, const XButton button) const;
		bool x_is_down(const GamePadIndex index, const XButton button) const;
		bool x_is_released(const GamePadIndex index, const XButton button) const;
		bool x_is_left_trigger_pressed(const GamePadIndex controll) const;
		float x_left_trigger(const GamePadIndex index) const;
		float x_right_trigger(const GamePadIndex index) const;
		float x_left_stick_x(const GamePadIndex index) const;
		float x_left_stick_y(const GamePadIndex index) const;
		float x_right_stick_x(const GamePadIndex index) const;
		float x_right_stick_y(const GamePadIndex index) const;

		void set_window_scale(float scale_x, float scale_y);
		void set_window(HWND *window_handle);

		void set_as_main();

		static InputHandler *main();

	private:

		struct Controlls {
			int _x_button_last;
			int _x_controller_id = -1;
			float _x_deadzone = 0.05f;
			float _x_left_stick_x = 0.0f;
			float _x_left_stick_y = 0.0f;
			float _x_right_stick_x = 0.0f;
			float _x_right_stick_y = 0.0f;
			float _x_left_trigger = 0.0f;
			float _x_right_trigger = 0.0f;
			int _x_check_interval = 0;
		};

		HWND *_window_handle = nullptr;

		const int X_CHECK_INTERVAL = 60;
		const float X_TRIGGER_THRESHOLD = 0.9f;

		std::array<XINPUT_STATE, XUSER_MAX_COUNT> _controllers_XINPUT;
		std::array<Controlls, XUSER_MAX_COUNT> _controllers_controlls;

		float _window_scale_x = 640.0f;
		float _window_scale_y = 360.0f;
		int _mouse_wheel = 0;
		int _mouse_x = 0;
		int _mouse_y = 0;
		int _mouse_x_last = 0;
		int _mouse_y_last = 0;
		int _mouse_screen_x = 0;
		int _mouse_screen_y = 0;
		std::bitset<5> _mouse_down_last;
		std::bitset<5> _mouse_down;
		std::bitset<256> _key_down_last;
		std::bitset<256> _key_down;

		static InputHandler *main_handler;

	};
}
