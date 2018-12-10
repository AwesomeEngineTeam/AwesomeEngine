namespace aw
{
	namespace sdk
	{
		inline WindowApplication::WindowApplication(uint32 width, uint32 height, const char* title, uint16 posX, uint16 posY, uint8 monitorId, WindowStyleBit style) :
			BaseApplication(),
			m_mainWindow{ std::make_unique<RenderWindow>() }
		{
			m_mainWindow->Create(width, height, title, posX, posY, monitorId, style);
		}

		inline void WindowApplication::Start()
		{
			BaseApplication::Start();
		}

		inline void WindowApplication::Update()
		{
			BaseApplication::Update();

			auto& window = *m_mainWindow.get();
			if (InputManager::GetKeyDown(KeyCode::Esc) || window.ShouldQuit() == true)
				Quit();
			else if(InputManager::GetFocusedWindowID() == (*m_mainWindow)->GetID())
				(*m_mainWindow)->Update();

			//Context::GetDevice().WaitGraphicsIdle();
		}

		inline void WindowApplication::End()
		{
			BaseApplication::End();

			if (m_mainWindow)
			{
				m_mainWindow->Quit();
				m_mainWindow.reset();
			}
		}
	}
}