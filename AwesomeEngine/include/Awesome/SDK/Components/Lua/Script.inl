namespace aw
{
	namespace sdk
	{
		template<typename T>
		inline void Script::AddOrSetBinding(const std::string& key, T&& value)
		{
			m_script.AddOrSetBinding(key, std::forward<T>(value));
		}
	}
}