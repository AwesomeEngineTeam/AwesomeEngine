namespace aw
{
	namespace sdk
	{
		inline void BaseApplication::Quit()
		{
			s_quit = true;
		}

		inline bool BaseApplication::ShouldQuit()
		{
			return s_quit;
		}

		void BaseApplication::Launch()
		{
			Start();

			while (!s_quit)
				Update();

			//sdk::ThreadManager::JoinAll();
			Context::GetDevice().WaitGraphicsIdle();

			End();
		}

		inline void BaseApplication::Start()
		{
			LOG_LOG("================================================================");
			LOG_LOG("=                      Application Start                       =");
			LOG_LOG("================================================================");

			//m_inputChrono.Play();
		}

		inline void BaseApplication::Update()
		{
			Time::Update();

			//if (m_inputChrono.GetElapsedTime<Chrono::Milliseconds>() >= 5)
			//{
				InputManager::Update();
				//m_inputChrono.Restart();
			//}
		}

		inline void BaseApplication::End()
		{
			sound::End();

			LOG_LOG("================================================================");
			LOG_LOG("=                      Application End                         =");
			LOG_LOG("================================================================");
		}
	}
}