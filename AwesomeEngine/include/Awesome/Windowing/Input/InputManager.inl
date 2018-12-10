namespace aw
{
	inline bool InputManager::GetKeyDown(KeyCode key)
	{
		return s_keyStates[static_cast<int>(key)] >= static_cast<char>(State::Down);
	}

	inline bool InputManager::GetKeyUp(KeyCode key)
	{
		return s_keyStates[static_cast<int>(key)] <= static_cast<char>(State::Released);
	}

	inline bool InputManager::GetKeyReleased(KeyCode key)
	{
		return s_keyStates[static_cast<int>(key)] == static_cast<char>(State::Released);
	}

	inline bool InputManager::GetKeyPressed(KeyCode key)
	{
		return s_keyStates[static_cast<int>(key)] == static_cast<char>(State::Pressed);
	}

	inline MousePos InputManager::GetMouseDelta()
	{
		return s_mouseDelta;
	}

	inline MousePos InputManager::GetMousePosition()
	{
		POINT pos;
//		GetPhysicalCursorPos(&pos);
		GetCursorPos(&pos);
		return { pos.x, pos.y };
	}

	inline MousePos InputManager::GetMouseRelativePosition(const Window& window)
	{
		POINT pos;
//		GetPhysicalCursorPos(&pos);
		GetCursorPos(&pos);
		ScreenToClient(window.GetHandle(), &pos);
		return { pos.x, pos.y };
	}

	inline MousePos InputManager::GetMouseRelativePosition()
	{
		if(s_focusedWindow != nullptr)
			return GetMouseRelativePosition(*s_focusedWindow);

		LOG_WARNING("No Focused Window set to InputManager, yet trying to retrieve relative position from it");
		return { 0, 0 };
	}

	inline void InputManager::SetMousePosition(int32 x, int32 y)
	{
		//SetPhysicalCursorPos(x, y);
		SetCursorPos(x, y);
	}

	inline void InputManager::SetMousePosition(const MousePos& pos)
	{
		SetMousePosition(pos.x, pos.y);
	}

	inline void InputManager::SetMouseRelativePosition(int32 x, int32 y, const Window& window)
	{
		POINT pos{ x, y };
		ClientToScreen(window.GetHandle(), &pos);

		SetMousePosition(pos.x, pos.y);
	}

	inline void InputManager::SetMouseRelativePosition(int32 x, int32 y)
	{
		if (s_focusedWindow != nullptr)
		{
			SetMouseRelativePosition(x, y, *s_focusedWindow);
			return;
		}

		LOG_WARNING("No Focused Window set to InputManager, yet trying to set relative position from it");
	}

	inline void InputManager::SetMouseRelativePosition(MousePos pos, const Window& window)
	{
		ClientToScreen(window.GetHandle(), reinterpret_cast<LPPOINT>(&pos));

		SetMousePosition(pos.x, pos.y);
	}

	inline void InputManager::SetMouseRelativePosition(MousePos pos)
	{
		if (s_focusedWindow != nullptr)
		{
			SetMouseRelativePosition(pos, *s_focusedWindow);
			return;
		}

		LOG_WARNING("No Focused Window set to InputManager, yet trying to set relative position from it");
	}

	inline void InputManager::SetMouseCaptured(bool isCaptured)
	{
		//s_mouseState ^= (static_cast<unsigned long>(-int8(isCaptured)) ^ s_mouseState) & MOUSE_CAPTURED_FLAG_BIT;//The cast is here to make this two's complement independant
		//ShowCursor(!isCaptured);

		LOG_ERROR("mouse capture not handled");
	}

	inline bool InputManager::IsMouseCaptured()
	{
		//return bool(s_mouseState & MOUSE_CAPTURED_FLAG_BIT);

		LOG_ERROR("mouse capture not handled");

		return false;
	}

	inline void InputManager::FreezeMouse(int32 x, int32 y)
	{
		s_isMouseFrozen = true;
		s_mouseFrozenX = x;
		s_mouseFrozenY = y;
	}

	inline void InputManager::FreeMouse()
	{
		s_isMouseFrozen = false;
	}

	inline bool InputManager::IsMouseFrozen()
	{
		return s_isMouseFrozen;
	}

	inline bool InputManager::GetMouseDown(MouseButton button)
	{
		return s_mouseStates[static_cast<int>(button)] >= static_cast<char>(State::Down);
	}

	inline bool InputManager::GetMouseUp(MouseButton button)
	{
		return s_mouseStates[static_cast<int>(button)] <= static_cast<char>(State::Released);
	}

	inline bool InputManager::GetMouseReleased(MouseButton button)
	{
		return s_mouseStates[static_cast<int>(button)] == static_cast<char>(State::Released);
	}

	inline bool InputManager::GetMousePressed(MouseButton button)
	{
		return s_mouseStates[static_cast<int>(button)] == static_cast<char>(State::Pressed);
	}

	//inline bool InputManager::IsMouseButtonDown(MouseButton button)
	//{
	//	return s_mouseState & static_cast<uint8>(button);
	//}

	//inline bool InputManager::IsMouseButtonPressed(MouseButton button)
	//{

	//}

	//inline bool InputManager::IsMouseButtonReleased(MouseButton button)
	//{

	//}

    inline bool InputManager::GetMouseDoubleClicked()
    {
        //return s_mouseState & static_cast<uint8>(doubleClickBit);
		LOG_ERROR("double click not handled");

		return false;
    }

	inline int32 InputManager::GetMouseWheelDelta()
	{
		return s_wheelDelta;
	}

	inline void InputManager::ShowMouse()
	{
		::ShowCursor(true);
	}

	inline void InputManager::HideMouse()
	{
		::ShowCursor(false);
	}

	inline void InputManager::SetFocusedWindow(Window* window)
	{
		s_focusedWindow = window;
	}

	inline uint32 InputManager::GetFocusedWindowID()
	{
		if (s_focusedWindow == nullptr)
			return 0;//because there is at least the main window
		
		return s_focusedWindow->GetID();
	}

	inline Vector2 InputManager::GetFocusedWindowDimensions()
	{
		if (s_focusedWindow != nullptr)
			return { s_focusedWindow->GetWidth(), s_focusedWindow->GetHeight() };

		return {};
	}

	inline void InputManager::Update()
	{
		while(!s_changedState.empty())
		{
			--s_keyStates[static_cast<int>(s_changedState.top())];
			s_changedState.pop();
		}

		while (!s_changedMouseState.empty())
		{
			--s_mouseStates[static_cast<int>(s_changedMouseState.top())];
			s_changedMouseState.pop();
		}

		/*if ((s_mouseState & MOUSE_CAPTURED_FLAG_BIT) == MOUSE_CAPTURED_FLAG_BIT)
		{
			SetMouseRelativePosition(s_focusedWindow->GetRectW() / 2, s_focusedWindow->GetRectH() / 2);
		}
		else */if (s_isMouseFrozen)
		{
			SetMousePosition(s_mouseFrozenX, s_mouseFrozenY);
		}

        //s_mouseState &= ~(doubleClickBit);
		s_wheelDelta = 0;
		s_mouseDelta = {};
	}

	inline void InputManager::RegisterKeyReleasedEvent(KeyCode key)
	{
		s_keyStates[static_cast<int>(key)] = static_cast<char>(State::Released);
		s_changedState.push(key);
	}

	inline void InputManager::RegisterKeyDownEvent(KeyCode key)
	{
		s_keyStates[static_cast<int>(key)] = static_cast<char>(State::Down);
	}

	inline void InputManager::RegisterKeyPressedEvent(KeyCode key)
	{
		s_keyStates[static_cast<int>(key)] = static_cast<char>(State::Pressed);
		s_changedState.push(key);
	}

	inline void InputManager::RegisterMouseMoveRelativeEvent(MousePos newPos)
	{
		s_mouseDelta.x += s_mousePos.x - newPos.x;
		s_mouseDelta.y += s_mousePos.y - newPos.y;

		s_mousePos = newPos;
	}

	inline void InputManager::RegisterMouseMoveDeltaEvent(int relativeX, int relativeY)
	{
		s_mouseDelta.x += relativeX;
		s_mouseDelta.y += relativeY;
	}

	inline void InputManager::RegisterMouseClickEvent(uint8 buttons)
	{
		if ((buttons & static_cast<uint8>(MouseButtonBit::Left)) == static_cast<uint8>(MouseButtonBit::Left))
		{
			s_mouseStates[static_cast<uint8>(MouseButton::Left)] = static_cast<uint8>(State::Pressed);
			s_changedMouseState.push(MouseButton::Left);
		}
		if ((buttons & static_cast<uint8>(MouseButtonBit::Right)) == static_cast<uint8>(MouseButtonBit::Right))
		{
			s_mouseStates[static_cast<uint8>(MouseButton::Right)] = static_cast<uint8>(State::Pressed);
			s_changedMouseState.push(MouseButton::Right);
		}
		if ((buttons & static_cast<uint8>(MouseButtonBit::Middle)) == static_cast<uint8>(MouseButtonBit::Middle))
		{
			s_mouseStates[static_cast<uint8>(MouseButton::Middle)] = static_cast<uint8>(State::Pressed);
			s_changedMouseState.push(MouseButton::Middle);
		}
	}

	inline void InputManager::RegisterMouseReleaseEvent(uint8 buttons)
	{
		if ((buttons & static_cast<uint8>(MouseButtonBit::Left)) == static_cast<uint8>(MouseButtonBit::Left))
		{
			s_mouseStates[static_cast<uint8>(MouseButton::Left)] = static_cast<uint8>(State::Released);
			s_changedMouseState.push(MouseButton::Left);
		}
		if ((buttons & static_cast<uint8>(MouseButtonBit::Right)) == static_cast<uint8>(MouseButtonBit::Right))
		{
			s_mouseStates[static_cast<uint8>(MouseButton::Right)] = static_cast<uint8>(State::Released);
			s_changedMouseState.push(MouseButton::Right);
		}
		if ((buttons & static_cast<uint8>(MouseButtonBit::Middle)) == static_cast<uint8>(MouseButtonBit::Middle))
		{
			s_mouseStates[static_cast<uint8>(MouseButton::Middle)] = static_cast<uint8>(State::Released);
			s_changedMouseState.push(MouseButton::Middle);
		}
	}

    inline void InputManager::RegisterMouseDoubleClickEvent()
    {
		LOG_ERROR("double click not handled!");
        //s_mouseState |= doubleClickBit;
    }

	inline void InputManager::RegisterMouseWheelEvent(int32 wheelDelta)
	{
		s_wheelDelta += wheelDelta;
	}
}