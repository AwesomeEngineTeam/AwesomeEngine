#pragma once

#ifndef AWESOME_SDK_THREAD_MANAGER_GUARD
#define AWESOME_SDK_THREAD_MANAGER_GUARD

#include <Awesome/Core/Thread/ThreadQueue.hpp>

namespace aw
{
	namespace sdk
	{
		class ThreadManager
		{
			static ThreadQueue<7> s_threadQueue;
		public:
			ThreadManager() = delete;

			template <typename R, typename... Args>
			static auto Push(const std::function<R(Args...)>&, const Args&...);

			static void JoinAll();
		};
	}
}

#include <Awesome/SDK/ThreadManager.inl>

#endif //GUARD