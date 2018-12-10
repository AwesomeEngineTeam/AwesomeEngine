namespace aw
{
	inline Window::Window() : m_id{s_count++}
	{
	}

	inline const HWND Window::GetHandle() const
	{
		return m_handle;
	}

	inline HINSTANCE Window::GetInstance() const
	{
		return m_instance;
	}

	inline void Window::SetDestroyCallback(std::function<void()> callback)
	{
		m_onDestroyFunc = callback;
	}

	inline void Window::SetCloseCallback(std::function<void()> callback)
	{
		m_onCloseFunc = callback;
	}

	inline void Window::SetResizeCallback(std::function<void(uint32, uint32)> callback)
	{
		m_onResizeFunc = callback;
	}

	inline void Window::SetMinimizeCallback(std::function<void()> callback)
	{
		m_onMinimizeFunc = callback;
	}

	inline void Window::SetMaximizeCallback(std::function<void()> callback)
	{
		m_onMaximizeFunc = callback;
	}

	inline uint32 Window::GetID() const
	{
		return m_id;
	}

	inline uint32 Window::GetWidth() const
	{
		return m_width;
	}

	inline uint32 Window::GetHeight() const
	{
		return m_height;
	}

	inline uint32 Window::GetRectW() const
	{
		return m_rectW;
	}

	inline uint32 Window::GetRectH() const
	{
		return m_rectH;
	}

	inline void Window::SetFocused()
	{
		SetFocus(m_handle);
		m_isFocused = true;
	}

	inline bool Window::IsMinimized() const
	{
		return m_isMinimized;
	}

	inline bool Window::ShouldResize() const
	{
		return m_shouldResize;
	}
}