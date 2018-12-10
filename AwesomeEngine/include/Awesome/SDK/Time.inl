namespace aw
{
	namespace sdk
	{
		inline Time::Time()
		{
			m_chrono.Play();
		}

		inline Time& Time::GetInstance()
		{
			static Time awTime;
			return awTime;
		}

		inline float Time::DeltaTime()
		{
			return GetInstance().m_deltaTime;
		}

		inline float Time::UnscaledDeltaTime()
		{
			return GetInstance().m_unscaledDeltaTime;
		}

		inline float Time::GlobalTime()
		{
			return GetInstance().m_globalTime;
		}

		inline float Time::TimeScale()
		{
			return GetInstance().m_timeScale;
		}

		inline void Time::SetTimeScale(float value)
		{
			GetInstance().m_timeScale = value;
		}

		inline void Time::Play()
		{
			GetInstance().m_chrono.Play();
		}

		inline void Time::Pause()
		{
			GetInstance().m_chrono.Pause();
		}

		inline void Time::Update()
		{
			Time& inst = GetInstance();

			inst.m_unscaledDeltaTime = static_cast<float>(inst.m_chrono.GetElapsedTime<Chrono::Seconds>());
			inst.m_deltaTime = inst.m_unscaledDeltaTime * inst.m_timeScale;
			inst.m_globalTime += inst.m_deltaTime;

			inst.m_chrono.Restart();
		}

		inline void Time::Reset()
		{
			Time& inst = GetInstance();

			inst.m_globalTime = 0.0f;
			inst.m_deltaTime = 0.0f;
			inst.m_unscaledDeltaTime = 0.0f;
			inst.m_timeScale = 1.0f;

			inst.m_chrono.Restart();
		}
	}
}