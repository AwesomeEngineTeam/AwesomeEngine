namespace aw
{
	inline GUIDebug::GUIDebug()
	{
		Logger::AddStream(&m_outputStream);
	}
}