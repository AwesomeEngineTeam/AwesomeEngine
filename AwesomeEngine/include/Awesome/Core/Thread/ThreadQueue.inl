namespace aw
{
	template <uint16 num>
	inline ThreadQueue<num>::ThreadQueue(ThreadQueue&& other) :
		m_queue{ std::move(other.m_queue) }
	{
		for (int i = 0; i < num; i++)
		{
			m_threads[i] = std::move(other.m_threads[i]);
			m_isRuns[i] = std::move(other.m_isRuns[i]);
		}
	}
	template <uint16 num>
	inline ThreadQueue<num>::~ThreadQueue()
	{
		Clear();

		JoinAll();
	}

	template <uint16 num>
	inline int16 ThreadQueue<num>::FindIdle() const
	{
		for (uint16 i = 0; i < num; i++)
		{
			if (!m_isRuns[i])
				return i;
		}

		return -1;
	}
	template <uint16 num>
	inline void ThreadQueue<num>::UpdateQueue(uint16 index)
	{
		m_quMut.lock();

		if (m_queue.size() <= 0u)
		{
			m_isRuns[index] = false;
			m_quMut.unlock();
			return;
		}

		std::function<void(int)> func = m_queue.front();
		m_queue.pop();

		m_quMut.unlock();

		m_threads[index].Detach();
		m_threads[index] = Thread([this, func, index]() { func(index); });
	}

	template <uint16 num>
	inline void ThreadQueue<num>::Clear()
	{
		m_quMut.lock();

		while (m_queue.size() > 0)
			m_queue.pop();

		m_quMut.unlock();
	}
	template <uint16 num>
	inline void ThreadQueue<num>::JoinAll()
	{
	START:

		for (uint16 i = 0; i < num; i++)
			m_threads[i].Join();

		std::unique_lock<std::mutex> lk(m_quMut);

		for (uint16 i = 0; i < num; i++)
		{
			if (m_isRuns[i] == true)
				goto START;
		}
	}

	template <uint16 num>
	template <typename R, typename... Args>
	inline TResult<R(Args...)> ThreadQueue<num>::Push(R(*func)(Args...), const Args&... args)
	{
		std::function<R(Args...)> qFunc(func);
		return Push(qFunc, args...);
	}
	template <uint16 num>
	template <typename R, typename... Args>
	inline TResult<R(Args...)> ThreadQueue<num>::Push(const std::function<R(Args...)>& func, const Args&... args)
	{
		if (!func)
			return TResult<R(Args...)>();

		TResult<R(Args...)> result(func);
		std::function<void(int)> qFunc([this, result, args...](uint16 index)
		{
			result(args...);
			UpdateQueue(index);
		});

		std::unique_lock<std::mutex> lk(m_quMut);

		int16 qIndex = FindIdle();

		if (qIndex < 0)
			m_queue.push(qFunc);
		else
		{
			m_isRuns[qIndex] = true;
			lk.unlock();

			m_threads[qIndex].Join();
			m_threads[qIndex] = Thread(qFunc, qIndex);
		}

		return result;
	}
}