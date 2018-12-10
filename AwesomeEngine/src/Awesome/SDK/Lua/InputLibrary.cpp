#include <Awesome/SDK/Lua/Libraries.hpp>

namespace aw
{
	namespace sdk
	{
		namespace lua
		{
			namespace input
			{
				const luwra::MemberMap Functions
				{
					{ "GetKeyDown", LUWRA_WRAP(InputManager::GetKeyDown) },
					{ "GetKeyUp", LUWRA_WRAP(InputManager::GetKeyUp) },

					{ "GetKeyPressed", LUWRA_WRAP(InputManager::GetKeyPressed) },
					{ "GetKeyReleased", LUWRA_WRAP(InputManager::GetKeyReleased) },

					{ "GetMouseDelta", LUWRA_WRAP(InputManager::GetMouseDelta) },

					{ "GetMousePosition", LUWRA_WRAP_OVERLOAD(InputManager::GetMouseRelativePosition, MousePos()) },
					{ "GetAbsoluteMousePosition", LUWRA_WRAP(InputManager::GetMousePosition) },
					{ "SetMousePosition", LUWRA_WRAP_OVERLOAD(InputManager::SetMouseRelativePosition, void(int32, int32)) },
					{ "SetMousePositionV", LUWRA_WRAP_OVERLOAD(InputManager::SetMouseRelativePosition, void(MousePos)) },

					{ "SetMouseCaptured", LUWRA_WRAP(InputManager::SetMouseCaptured) },
					{ "IsMouseCaptured", LUWRA_WRAP(InputManager::IsMouseCaptured) },

					{ "FreezeMouse", LUWRA_WRAP(InputManager::FreezeMouse) },
					{ "FreeMouse", LUWRA_WRAP(InputManager::FreeMouse) },
					{ "IsMouseFrozen", LUWRA_WRAP(InputManager::IsMouseFrozen) },

					{ "GetMouseDown", LUWRA_WRAP(InputManager::GetMouseDown) },
					{ "GetMouseUp", LUWRA_WRAP(InputManager::GetMouseUp) },

					{ "GetMousePressed", LUWRA_WRAP(InputManager::GetMousePressed) },
					{ "GetMouseReleased", LUWRA_WRAP(InputManager::GetMouseReleased) },
					
					{ "GetMouseWheelDelta", LUWRA_WRAP(InputManager::GetMouseWheelDelta) },
					
					{ "ShowMouse", LUWRA_WRAP(InputManager::ShowMouse) },
					{ "HideMouse", LUWRA_WRAP(InputManager::HideMouse) },

					{ "GetFocusedWindowDimensions", LUWRA_WRAP(InputManager::GetFocusedWindowDimensions) },

					{ "GetMouseDoubleClicked", LUWRA_WRAP(InputManager::GetMouseDoubleClicked) }
				};

				const luwra::MemberMap KeyCodes
				{
					{ "Alt", KeyCode::Alt },
					{ "Control", KeyCode::Control },
					{ "LShift", KeyCode::LShift },
					{ "RShift", KeyCode::RShift },
					{ "Esc", KeyCode::Esc },
					{ "Space", KeyCode::Space },
					{ "Backspace", KeyCode::Backspace },
					{ "Enter", KeyCode::Enter },
					{ "A", KeyCode::A },
					{ "B", KeyCode::B },
					{ "C", KeyCode::C },
					{ "D", KeyCode::D },
					{ "E", KeyCode::E },
					{ "F", KeyCode::F },
					{ "G", KeyCode::G },
					{ "H", KeyCode::H },
					{ "I", KeyCode::I },
					{ "J", KeyCode::J },
					{ "K", KeyCode::K },
					{ "L", KeyCode::L },
					{ "M", KeyCode::M },
					{ "N", KeyCode::N },
					{ "O", KeyCode::O },
					{ "P", KeyCode::P },
					{ "Q", KeyCode::Q },
					{ "R", KeyCode::R },
					{ "S", KeyCode::S },
					{ "T", KeyCode::T },
					{ "U", KeyCode::U },
					{ "V", KeyCode::V },
					{ "W", KeyCode::W },
					{ "X", KeyCode::X },
					{ "Y", KeyCode::Y },
					{ "Z", KeyCode::Z },
					{ "_0", KeyCode::_0 },
					{ "_1", KeyCode::_1 },
					{ "_2", KeyCode::_2 },
					{ "_3", KeyCode::_3 },
					{ "_4", KeyCode::_4 },
					{ "_5", KeyCode::_5 },
					{ "_6", KeyCode::_6 },
					{ "_7", KeyCode::_7 },
					{ "_8", KeyCode::_8 },
					{ "_9", KeyCode::_9 },
					{ "Up", KeyCode::Up },
					{ "Down", KeyCode::Down },
					{ "Left", KeyCode::Left },
					{ "Right", KeyCode::Right },
					{ "Numpad0", KeyCode::Numpad0 },
					{ "Numpad1", KeyCode::Numpad1 },
					{ "Numpad2", KeyCode::Numpad2 },
					{ "Numpad3", KeyCode::Numpad3 },
					{ "Numpad4", KeyCode::Numpad4 },
					{ "Numpad5", KeyCode::Numpad5 },
					{ "Numpad6", KeyCode::Numpad6 },
					{ "Numpad7", KeyCode::Numpad7 },
					{ "Numpad8", KeyCode::Numpad8 },
					{ "Numpad9", KeyCode::Numpad9 },
					{ "NumpadMultiply", KeyCode::NumpadMultiply }
				};

				const luwra::MemberMap MouseButtons
				{
					{ "Left", MouseButton::Left },
					{ "Right", MouseButton::Right },
					{ "Middle", MouseButton::Middle }
				};
			}
		}
	}
}