#pragma once

#ifndef AWESOME_CORE_THREAD_QUEUE_GUARD
#define AWESOME_CORE_THREAD_QUEUE_GUARD

#include <queue>

#include <mutex>
#include <atomic>

#include <Awesome/Core/Thread/TResult.hpp>
#include <Awesome/Core/Thread/Thread.hpp>

namespace aw
{
	template <uint16 num = 1>
	class ThreadQueue
	{
		std::mutex m_quMut;

		Thread m_threads[num];
		bool m_isRuns[num]{ false };

		std::queue<std::function<void(int)>> m_queue;

		inline int16 FindIdle() const;
		inline void UpdateQueue(uint16 index);

	public:
		ThreadQueue() = default;
		inline ThreadQueue(ThreadQueue&& other);
		ThreadQueue(const ThreadQueue&) = delete;
		inline ~ThreadQueue();

		inline void Clear();
		inline void JoinAll();

		template <typename R, typename... Args>
		inline TResult<R(Args...)> Push(R(*func)(Args...), const Args&... args);
		template <typename R, typename... Args>
		inline TResult<R(Args...)> Push(const std::function<R(Args...)>& func, const Args&... args);
	};
}

#include <Awesome/Core/Thread/ThreadQueue.inl>

#endif // GUARD