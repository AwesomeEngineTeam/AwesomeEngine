namespace aw
{
	inline void RenderWindow::Create(uint32 w, uint32 h, const char* title, uint16 posX, uint16 posY, uint8 monitorId, WindowStyleBit style)
	{
		m_window.Create(w, h, title, posX, posY, monitorId, style);

		Init();
	}

	inline void RenderWindow::Create(HWND hwnd)
	{
		m_window.Create(hwnd);

		Init();
	}

	inline const RenderSurface& RenderWindow::GetSurface() const
	{
		return m_renderSurface;
	}

	inline void RenderWindow::OnSurfaceOutOfDate()
	{
		Resize(m_window.GetWidth(), m_window.GetHeight());
	}

	inline void RenderWindow::SetOnOutOfDateCallback(const std::function<void(uint32, uint32)>& callback)
	{
		m_onOutOfDate = callback;
	}

	inline bool RenderWindow::ShouldQuit()
	{
		return m_shouldQuit;
	}

	inline void RenderWindow::Quit()
	{
		m_shouldQuit = true;
	}

	inline const Window* RenderWindow::operator->() const
	{
		return &m_window;
	}

	inline Window* RenderWindow::operator->()
	{
		return &m_window;
	}

	inline const Window& RenderWindow::operator*() const
	{
		return m_window;
	}

	inline Window& RenderWindow::operator*()
	{
		return m_window;
	}
}