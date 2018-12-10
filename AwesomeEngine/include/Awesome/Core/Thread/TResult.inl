namespace aw
{
	template <typename R, typename... Args>
	inline TResult<R(Args...)>::TResult(std::function<R(Args...)> func)
	{
		if (!func)
			return;

		m_handle = std::make_shared<std::packaged_task<R(Args...)>>(std::packaged_task<R(Args...)>([func](Args... fArgs) { return func(fArgs...); }));
		m_result = m_handle->get_future();
	}
	template <typename R, typename... Args>
	inline TResult<R(Args...)>::TResult(TResult&& other) :
		m_handle{ std::move(other.m_handle) },
		m_result{ std::move(other.m_result) }
	{
	}
	template <typename R, typename... Args>
	inline TResult<R(Args...)>::TResult(const TResult& other) :
		m_handle{ other.m_handle },
		m_result{ other.m_result }
	{
	}

	template <typename R, typename... Args>
	inline R TResult<R(Args...)>::Get()
	{
		if (IsValid())
			return m_result.get();

		return R();
	}
	template <typename R, typename... Args>
	inline bool TResult<R(Args...)>::IsValid() const
	{
		return m_result.valid();
	}
	template <typename R, typename... Args>
	inline void TResult<R(Args...)>::Wait() const
	{
		if (IsValid())
			m_result.wait();
	}

	template <typename R, typename... Args>
	inline void TResult<R(Args...)>::operator()(Args... args) const
	{
		if (m_handle)
			m_handle->operator()(args...);
	}
}