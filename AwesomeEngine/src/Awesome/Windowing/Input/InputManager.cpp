#include <Awesome/Windowing/Input/InputManager.hpp>

namespace aw
{
	std::array<char, 256> InputManager::s_keyStates;
	std::stack<KeyCode> InputManager::s_changedState;
	std::array<char, 3> InputManager::s_mouseStates;
	std::stack<MouseButton> InputManager::s_changedMouseState;

	MousePos InputManager::s_mouseDelta{};
	MousePos InputManager::s_mousePos{};
	//uint8 InputManager::s_mouseState;
	int32 InputManager::s_wheelDelta = 0;
	bool InputManager::s_isMouseFrozen = false;
	int32 InputManager::s_mouseFrozenX = 0;
	int32 InputManager::s_mouseFrozenY = 0;

	Window* InputManager::s_focusedWindow = nullptr;
}