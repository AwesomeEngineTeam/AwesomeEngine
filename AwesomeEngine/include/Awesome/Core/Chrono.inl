namespace aw
{
	inline void Chrono::Play()
	{
		if(paused == true)
			m_timeStamp = clock::now();

		paused = false;
	}

	inline void Chrono::Pause()
	{
		if(paused == false)
			m_elapsedTime += clock::now() - m_timeStamp;

		paused = true;
	}

	inline void Chrono::Restart()
	{
		m_elapsedTime = duration::zero();
		m_timeStamp = clock::now();

		paused = false;
	}

	template<typename Format>
	inline constexpr double Chrono::GetElapsedTime()
	{
		if (paused == false)
		{
			m_elapsedTime += clock::now() - m_timeStamp;
			m_timeStamp = clock::now();
		}

		return std::chrono::duration_cast<std::chrono::duration<double, Format>>(m_elapsedTime).count();
	}
}