#pragma once

#ifndef AWESOME_WINDOWING_INPUTMANAGER_GUARD
#define AWESOME_WINDOWING_INPUTMANAGER_GUARD

#include <Awesome/Core/Types.hpp>

#include <Awesome/Maths/Vector2.hpp>

#include <Awesome/Windowing/Input/KeyCodes.hpp>

#include <Awesome/Windowing/Window.hpp>
#include <Awesome/Windowing/Config.hpp>

#include <array>
#include <stack>

#define MOUSE_CAPTURED_FLAG_BIT (1UL << 5)

namespace aw
{
	enum class MouseButtonBit : uint8
	{
		Left = MK_LBUTTON,
		Right = MK_RBUTTON,
		Middle = MK_MBUTTON
	};

	enum class MouseButton : uint8
	{
		Left = 0,
		Right,
		Middle
	};

	static const uint8 doubleClickBit = 0x0080;

	class InputManager
	{
		friend class Window;

		enum class State : uint8
		{
			Up,
			Released,
			Down,
			Pressed
		};

		InputManager() = delete;
		~InputManager() = delete;

		AWESOME_WINDOWING_API static std::array<char, 256> s_keyStates;
		AWESOME_WINDOWING_API static std::stack<KeyCode> s_changedState;
		AWESOME_WINDOWING_API static std::array<char, 3> s_mouseStates;
		AWESOME_WINDOWING_API static std::stack<MouseButton> s_changedMouseState;

		AWESOME_WINDOWING_API static MousePos s_mouseDelta;
		AWESOME_WINDOWING_API static MousePos s_mousePos;
		//AWESOME_WINDOWING_API static uint8 s_mouseState;
		AWESOME_WINDOWING_API static int32 s_wheelDelta;
		AWESOME_WINDOWING_API static bool s_isMouseFrozen;
		AWESOME_WINDOWING_API static int32 s_mouseFrozenX;
		AWESOME_WINDOWING_API static int32 s_mouseFrozenY;

		AWESOME_WINDOWING_API static Window* s_focusedWindow;

		inline static void RegisterKeyReleasedEvent(KeyCode);
		inline static void RegisterKeyDownEvent(KeyCode);
		inline static void RegisterKeyPressedEvent(KeyCode);

		inline static void RegisterMouseMoveRelativeEvent(MousePos newPos);
		inline static void RegisterMouseMoveDeltaEvent(int relativeX, int relativeY);
		inline static void RegisterMouseClickEvent(uint8 buttons);
		inline static void RegisterMouseReleaseEvent(uint8 buttons);
        inline static void RegisterMouseDoubleClickEvent();
		inline static void RegisterMouseWheelEvent(int32 wheelDelta);
		
	public:
		inline static bool GetKeyDown(KeyCode key);
		inline static bool GetKeyPressed(KeyCode key);

		inline static bool GetKeyUp(KeyCode key);
		inline static bool GetKeyReleased(KeyCode key);

		inline static MousePos GetMouseDelta();

		inline static MousePos GetMousePosition();

		inline static MousePos GetMouseRelativePosition(const Window&);
		inline static MousePos GetMouseRelativePosition();

		inline static void SetMousePosition(int32 x, int32 y);
		inline static void SetMousePosition(const MousePos&);

		inline static void SetMouseRelativePosition(int32 x, int32 y, const Window&);
		inline static void SetMouseRelativePosition(int32 x, int32 y);

		inline static void SetMouseRelativePosition(MousePos, const Window&);
		inline static void SetMouseRelativePosition(MousePos);

		inline static void SetMouseCaptured(bool isCaptured);
		inline static bool IsMouseCaptured();

		inline static void FreezeMouse(int32 x, int32 y);
		inline static void FreeMouse();
		inline static bool IsMouseFrozen();

		inline static bool GetMouseDown(MouseButton button);
		inline static bool GetMouseUp(MouseButton button);

		inline static bool GetMouseReleased(MouseButton button);
		inline static bool GetMousePressed(MouseButton button);

		inline static bool GetMouseDoubleClicked();

		inline static int32 GetMouseWheelDelta();

		inline static void ShowMouse();
		inline static void HideMouse();

		inline static void SetFocusedWindow(Window* window);
		inline static uint32 GetFocusedWindowID();
		inline static Vector2 GetFocusedWindowDimensions();

		inline static void Update();
	};
}

#include <Awesome/Windowing/Input/InputManager.inl>

#endif //GUARD