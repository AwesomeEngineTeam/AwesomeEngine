#include <Awesome/Windowing/Window.hpp>

#include <Awesome/Core/Logger/Logger.hpp>
#include <Awesome/Core/Chrono.hpp>
#include <Awesome/Windowing/Input/InputManager.hpp>

#include <windowsx.h>

// Inspiré de Nazara Engine par Jérôme Leclercq

namespace aw
{
	std::set<Window::MonitorInfo> Window::s_monitors;

	const char* Window::s_className = "Awesome Window";
	uint32 Window::s_count = 0;

	Window::~Window()
	{
		Destroy();
	}

	BOOL CALLBACK Window::MonitorEnumProc(HMONITOR hMonitor,
		HDC      hdcMonitor,
		LPRECT   lprcMonitor,
		LPARAM   dwData)
	{
		MONITORINFO info;
		info.cbSize = sizeof(info);

		if (GetMonitorInfo(hMonitor, &info))
			s_monitors.emplace(info.rcWork);

		return TRUE;  // continue enumerating
	}

	void Window::Init()
	{
		EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProc, 0);

		WNDCLASSEX windowClass{};

		windowClass.hInstance = GetModuleHandle(nullptr);
		windowClass.lpfnWndProc = WindowProc;
		windowClass.lpszClassName = s_className;
		windowClass.style = CS_DBLCLKS; // Gestion du double-clic
		windowClass.cbSize = sizeof(WNDCLASSEX);

		ASSERT(RegisterClassEx(&windowClass) == 0, "Failed to register window class. Error : %lu", GetLastError());

//		ASSERT(SetProcessDPIAware() == 0, "Failed to set DPI-awareness");

		LOG_LOG("Window initialized !");
	}

	void Window::UnInit()
	{
		UnregisterClass(s_className, GetModuleHandle(nullptr));
		LOG_LOG("Window uninitialized !");
	}

	void Window::Update()
	{
		MSG msg = {};

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		//if (GetMessage(&msg, m_handle, 0, 0))
		//if (GetMessage(&msg, nullptr, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void Window::Create(uint32 w, uint32 h, const char* title, uint16 posX, uint16 posY, uint8 monitorId, WindowStyleBit style)
	{
		if (m_handle != nullptr)
			Destroy();

		m_width = w;
		m_height = h;
		//bool async = (style & WindowStyleBit::Threaded) != WindowStyleBit::None;
		bool fullscreen = (style & WindowStyleBit::Fullscreen) != WindowStyleBit::None;
		DWORD win32Style, win32StyleEx;

		if (fullscreen)
		{
			DEVMODE win32Mode;
			memset(&win32Mode, 0, sizeof(DEVMODE));
			win32Mode.dmBitsPerPel = 4;
			win32Mode.dmPelsWidth = m_width;
			win32Mode.dmPelsHeight = m_height;
			win32Mode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
			win32Mode.dmSize = sizeof(DEVMODE);

			if (ChangeDisplaySettings(&win32Mode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			{
				LOG_ERROR("Failed to change display settings for fullscreen, this video mode is not supported by your computer");
				fullscreen = false;
			}
		}

		if (fullscreen)
		{
			win32Style = WS_CLIPCHILDREN | WS_POPUP;

			// Pour cacher la barre des tâches
			// http://msdn.microsoft.com/en-us/library/windows/desktop/ff700543(v=vs.85).aspx
			win32StyleEx = WS_EX_APPWINDOW;
		}
		else
		{
			win32Style = WS_VISIBLE;
			if ((style & WindowStyleBit::Titlebar) != WindowStyleBit::None)
			{
				win32Style |= WS_CAPTION | WS_MINIMIZEBOX;
				if ((style & WindowStyleBit::Closable) != WindowStyleBit::None)
					win32Style |= WS_SYSMENU;

				if ((style & WindowStyleBit::Resizable) != WindowStyleBit::None)
					win32Style |= WS_MAXIMIZEBOX | WS_SIZEBOX;
			}
			else
				win32Style |= WS_POPUP;

			win32StyleEx = 0;
		}

		if (monitorId > s_monitors.size() - 1)
			monitorId = uint8(s_monitors.size() - 1);

		auto monitor = s_monitors.begin();
		std::advance(monitor, monitorId);

		if (monitor->IsInside(posX, posY) == false)
		{
			posX = uint16(monitor->left);
			posY = uint16(monitor->top);
		}

		m_instance = GetModuleHandle(nullptr);
		m_handle = CreateWindowEx(win32StyleEx, s_className, title, win32Style, monitor->left + posX, monitor->top + posY, m_width, m_height, nullptr, nullptr, m_instance, this);
		ASSERT(m_handle == nullptr, "Failed to create window. Error : %lu", GetLastError());

		SetPropW(m_handle, L"userPointer", this);

		RECT rect;
		GetClientRect(m_handle, &rect);

		m_rectW = rect.right - rect.left;
		m_rectH = rect.bottom - rect.top;

#ifndef HID_USAGE_PAGE_GENERIC
	#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
	#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

		RAWINPUTDEVICE Rid[1];
		Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
		Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
		Rid[0].dwFlags = RIDEV_INPUTSINK;
		Rid[0].hwndTarget = m_handle;
		RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));

		SetForegroundWindow(m_handle);
		ShowWindow(m_handle, SW_SHOW);

		InputManager::SetFocusedWindow(this);
	}

	void Window::Create(HWND hwnd)
	{
		if (m_handle != nullptr)
			Destroy();

		m_instance = GetModuleHandle(nullptr);
		m_handle = hwnd;

		SetPropW(m_handle, L"userPointer", this);

		RECT rect;
		GetClientRect(m_handle, &rect);

		m_rectW = rect.right - rect.left;
		m_rectH = rect.bottom - rect.top;

		GetWindowRect(m_handle, &rect);

		m_width = rect.right - rect.left;
		m_height = rect.bottom - rect.top;

#ifndef HID_USAGE_PAGE_GENERIC
	#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
	#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

		RAWINPUTDEVICE Rid[1];
		Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
		Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
		Rid[0].dwFlags = RIDEV_INPUTSINK;
		Rid[0].hwndTarget = m_handle;
		RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));

		ShowWindow(m_handle, SW_SHOW);

		InputManager::SetFocusedWindow(this);
	}

	void Window::Destroy()
	{
		if (m_onDestroyFunc)
			m_onDestroyFunc();

		DestroyWindow(m_handle);
		m_handle = nullptr;
	}

	void Window::Close()
	{
		if (m_onCloseFunc)
			m_onCloseFunc();


	}

	LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		auto win = reinterpret_cast<Window*>(GetPropW(hwnd, L"userPointer"));

		if (win)
		{
			switch (uMsg)
			{
			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			{
				/*
				if (win->m_isFocused)
				{
					if ((lParam & (1u << 30u)) == 0u)
						InputManager::RegisterKeyPressedEvent(static_cast<KeyCode>(wParam));
					else
						InputManager::RegisterKeyDownEvent(static_cast<KeyCode>(wParam));
				}
				//*/
				KeyDownEvent(wParam, lParam);

				return 0;
			}

			case WM_SYSKEYUP:
			case WM_KEYUP:
			{
				//if (win->m_isFocused)
					//InputManager::RegisterKeyReleasedEvent(static_cast<KeyCode>(wParam));
				KeyUpEvent(wParam, lParam);

				return 0;
			}

			case WM_SETFOCUS:
			{
				win->m_isFocused = true;

				LOG_LOG("WM_SETFOCUS");

				//InputManager::SetMouseCaptured(true);
				InputManager::SetFocusedWindow(win);
				return 0;
			}

			case WM_KILLFOCUS:
			{
				win->m_isFocused = false;

				LOG_LOG("WM_KILLFOCUS");

				InputManager::SetMouseCaptured(false);
				InputManager::FreeMouse();
				InputManager::ShowMouse();
				return 0;
			}

			case WM_CLOSE:
			{
				//DestroyWindow(hwnd);//TODO Manage window closing during runtime + Make a main window responsible of calling the application closure.
				//reinterpret_cast<Window*>(GetPropW(hwnd, L"userPointer"))->Close();
				win->Close();
				return 0;
			}

			case WM_DESTROY:
			{
				//DestroyWindow(hwnd);
				//reinterpret_cast<Window*>(GetPropW(hwnd, L"userPointer"))->Destroy();
				win->Destroy();
				PostQuitMessage(0);
				return 0;
			}

			case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hwnd, &ps);

				FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

				EndPaint(hwnd, &ps);
				return 0;
			}

			case WM_MOUSEWHEEL:
			{
				//if (win->m_isFocused)
					InputManager::RegisterMouseWheelEvent(GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
				return 0;
			}

			/*case WM_INPUT:
			{
				InputEvent(wParam, lParam);

				return 0;
			}*/

			case WM_MOUSEMOVE:
			{
				if (win != nullptr)
				{
					win->InputEvent(wParam, lParam);
				}

				return 0;
			}

			case WM_LBUTTONUP:
			case WM_LBUTTONDOWN:
			case WM_RBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_MBUTTONDOWN:
			{
				//if (win->m_isFocused)
					InputManager::RegisterMouseClickEvent(wParam & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON));
				return 0;
			}

			case WM_LBUTTONDBLCLK:
			{
				//if (win->m_isFocused)
				{
					InputManager::RegisterMouseClickEvent(wParam & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON));
					InputManager::RegisterMouseDoubleClickEvent();
				}

				return 0;
			}

			case WM_SIZE:
			{
				if (win != nullptr)
				{
					win->SizeEvent(wParam, lParam);
				}

				return 0;
			}

			default:
				break;
			}
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	void Window::KeyDownEvent(WPARAM wParam, LPARAM lParam)
	{
		if ((lParam & (1u << 30u)) == 0u)
			InputManager::RegisterKeyPressedEvent(static_cast<KeyCode>(wParam));
		else
			InputManager::RegisterKeyDownEvent(static_cast<KeyCode>(wParam));
	}

	void Window::KeyUpEvent(WPARAM wParam, LPARAM lParam)
	{
		InputManager::RegisterKeyReleasedEvent(static_cast<KeyCode>(wParam));
	}

	void Window::InputEvent(WPARAM wParam, LPARAM lParam)
	{
		UINT dwSize;

		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));
		std::vector<BYTE> lpb(dwSize);

		if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb.data(), &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
			LOG_ERROR("GetRawInputData does not return correct size !\n");

		auto raw = reinterpret_cast<RAWINPUT*>(lpb.data());

		if (raw->header.dwType == RIM_TYPEKEYBOARD)
		{
			const RAWKEYBOARD& keyboard = raw->data.keyboard;

			if ((keyboard.Flags & RI_KEY_BREAK) == RI_KEY_BREAK)
			{
				LOG_LOG("The key is up");
			}
			else if ((keyboard.Flags & RI_KEY_MAKE) == RI_KEY_MAKE)
			{
				LOG_LOG("The key is down");
			}

			LOG_LOG("MakeCode %d", keyboard.MakeCode);
		}
		else if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			const RAWMOUSE& mouse = raw->data.mouse;

			InputManager::RegisterMouseMoveDeltaEvent(mouse.lLastX, mouse.lLastY);

			uint16 lbutton = mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN;
			uint16 rbutton = (mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) >> 1;
			uint16 mbutton = mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN;

			uint16 buttons = lbutton | rbutton | mbutton;

			InputManager::RegisterMouseClickEvent(buttons & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON));


			lbutton = (mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) >> 1;
			rbutton = (mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) >> 2;
			mbutton = (mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP) >> 1;

			buttons = lbutton | rbutton | mbutton;

			InputManager::RegisterMouseReleaseEvent(buttons & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON));
		}
	}

	void Window::MouseMoveEvent(WPARAM wParam, LPARAM lParam)
	{
		MousePos mp{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

		InputManager::RegisterMouseMoveRelativeEvent(mp);
	}

	void Window::SizeEvent(WPARAM wParam, LPARAM lParam)
	{
		switch (wParam)
		{
			case SIZE_MINIMIZED:
			{
				m_isMinimized = true;

				if (m_onMinimizeFunc)
					m_onMinimizeFunc();

				break;
			}

			case SIZE_MAXIMIZED:
			{
				if (m_onMaximizeFunc)
					m_onMaximizeFunc();
				// Do not break; here
			}
			default:
			{
				m_isMinimized = false;

				m_width = LOWORD(lParam);
				m_height = HIWORD(lParam);

				m_shouldResize = true;

				if (m_onResizeFunc)
					m_onResizeFunc(m_width, m_height);

				break;
			}
		}
	}
}