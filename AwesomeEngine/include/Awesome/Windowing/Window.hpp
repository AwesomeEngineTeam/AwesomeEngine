#pragma once

#ifndef AWESOME_WINDOWING_WINDOW_GUARD
#define AWESOME_WINDOWING_WINDOW_GUARD

#include <Awesome/Windowing/Config.hpp>
#include <Awesome/Windowing/Enums.hpp>

#include <Awesome/Maths/Vector2.hpp>

#include <windows.h>

#include <atomic>
#include <functional>
#include <set>

namespace aw
{
	struct MousePos
	{
		int32 x, y;

		operator Vector2() const
		{
			return { static_cast<float>(x), static_cast<float>(y) };
		}
	};

	class Window
	{
		friend class Context;
		friend class RenderWindow;

	public:
		struct MonitorInfo
		{
			int top = 0;
			int bottom = 0;
			int left = 0;
			int right = 0;

			bool IsInside(int x, int y, bool relative = true) const
			{
				if (relative == true)
					return (left + x < right) && (left + x >= left) && (top + y < bottom) && (top + y >= top);
				else
					return (x >= left) && (x < right) && (y >= top) && (y < bottom);
			}

			MonitorInfo(RECT rect) : top{ rect.top }, bottom{ rect.bottom }, left{ rect.left }, right{ rect.right }
			{
			}
		};

	private:
		static std::set<MonitorInfo> s_monitors;

		HWND m_handle{ NULL };
		HINSTANCE m_instance{ NULL };
		static const char* s_className;

		AWESOME_WINDOWING_API static uint32 s_count;

		uint32 m_id;

		uint32 m_width{ 0 };
		uint32 m_height{ 0 };

		uint32 m_rectW{ 0 };
		uint32 m_rectH{ 0 };

		std::function<void()> m_onDestroyFunc;
		std::function<void()> m_onCloseFunc;
		std::function<void(uint32, uint32)> m_onResizeFunc;
		std::function<void()> m_onMinimizeFunc;
		std::function<void()> m_onMaximizeFunc;

		bool m_isMinimized{ false };
		bool m_isFocused{ true };

		std::atomic_bool m_shouldResize{ true };

		static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);

	public:
		AWESOME_WINDOWING_API static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		inline Window();
		AWESOME_WINDOWING_API ~Window();

		AWESOME_WINDOWING_API static void Init();
		AWESOME_WINDOWING_API static void UnInit();

		AWESOME_WINDOWING_API void Update();

		AWESOME_WINDOWING_API void Create(uint32 w, uint32 h, const char* title = "Awesome Engine", uint16 posX = 0, uint16 posY = 0, uint8 monitorId = 0, WindowStyleBit style = WindowStyleBit::Default);
		AWESOME_WINDOWING_API void Create(HWND);
		AWESOME_WINDOWING_API void Destroy();
		AWESOME_WINDOWING_API void Close();

		inline void SetDestroyCallback(std::function<void()>);
		inline void SetCloseCallback(std::function<void()>);
		inline void SetResizeCallback(std::function<void(uint32, uint32)>);
		inline void SetMinimizeCallback(std::function<void()>);
		inline void SetMaximizeCallback(std::function<void()>);

		AWESOME_WINDOWING_API static void KeyDownEvent(WPARAM wParam, LPARAM lParam);
		AWESOME_WINDOWING_API static void KeyUpEvent(WPARAM wParam, LPARAM lParam);
		AWESOME_WINDOWING_API static void InputEvent(WPARAM wParam, LPARAM lParam);
		AWESOME_WINDOWING_API void MouseMoveEvent(WPARAM wParam, LPARAM lParam);
		AWESOME_WINDOWING_API void SizeEvent(WPARAM wParam, LPARAM lParam);

		inline uint32 GetID() const;

		inline uint32 GetWidth() const;
		inline uint32 GetHeight() const;

		inline uint32 GetRectW() const;
		inline uint32 GetRectH() const;

		inline bool IsMinimized() const;
		inline bool ShouldResize() const;

		inline const HWND GetHandle() const;
		inline HINSTANCE GetInstance() const;

		inline void SetFocused();
	};

	inline bool operator<(const Window::MonitorInfo& lhs, const Window::MonitorInfo& rhs)
	{
		return lhs.left < rhs.left;
	}
}

#include <Awesome/Windowing/Window.inl>

#endif //GUARD