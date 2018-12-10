namespace aw
{
	template<typename Function, typename... Args>
	inline Thread::Thread(Function&& func, Args&&... args) :
		m_handle{ func, args... }
	{
	}
	inline Thread::Thread(Thread&& other) : m_handle{ std::move(other.m_handle) }
	{
	}
	inline Thread::~Thread()
	{
		Join();
	}

	inline uint32 Thread::GetId() const
	{
		std::thread::id index = m_handle.get_id();
		return reinterpret_cast<const uint32&>(index);
	}
	inline bool Thread::IsEmpty() const
	{
		return !m_handle.joinable();
	}

	inline void Thread::Join()
	{
		if (m_handle.joinable())
			m_handle.join();
	}
	inline void Thread::Detach()
	{
		if (m_handle.joinable())
			m_handle.detach();
	}

	inline uint32 Thread::HardwareConcurrency()
	{
		return std::thread::hardware_concurrency();
	}

	inline Thread& Thread::operator=(Thread&& rhs)
	{
		ASSERT(!IsEmpty(), "op= Thread must be Empty! Call Join or Detach!");

		m_handle = std::move(rhs.m_handle);

		return *this;
	}
}