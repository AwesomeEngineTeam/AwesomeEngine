namespace aw
{
	namespace sdk
	{
		template<typename R, typename... Args>
		auto ThreadManager::Push(const std::function<R(Args...)>& func, const Args&... args)
		{
			return s_threadQueue.Push(func, args...);
		}
	}
}