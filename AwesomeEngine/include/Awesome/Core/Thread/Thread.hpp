#pragma once

#ifndef AWESOME_CORE_THREAD_GUARD
#define AWESOME_CORE_THREAD_GUARD

#include <thread>

#include <Awesome/Core/Logger/Logger.hpp>

namespace aw
{
	class Thread
	{
		std::thread m_handle;

	public:
		Thread() = default;
		template<typename Function, typename... Args>
		inline Thread(Function&& func, Args&&... args);
		inline Thread(Thread&& other);
		Thread(const Thread&) = delete;
		inline ~Thread();

		inline uint32 GetId() const;
		inline bool IsEmpty() const;

		inline void Join();
		inline void Detach();

		static inline uint32 HardwareConcurrency();

		inline Thread& operator=(Thread&& rhs);
		Thread& operator=(const Thread&) = delete;
	};
}

#include <Awesome/Core/Thread/Thread.inl>

#endif // GUARD