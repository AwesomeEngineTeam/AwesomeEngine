#pragma once

#ifndef AWESOME_CORE_TRESULT_GUARD
#define AWESOME_CORE_TRESULT_GUARD

#include <memory>
#include <future>
#include <functional>

namespace aw
{
	template <typename T>
	class TResult;

	template <typename R, typename... Args>
	class TResult<R(Args...)>
	{
		mutable std::shared_ptr<std::packaged_task<R(Args...)>> m_handle = nullptr;
		std::shared_future<R> m_result;

	public:
		TResult() = default;
		inline TResult(std::function<R(Args...)> func);
		inline TResult(TResult&& other);
		inline TResult(const TResult& other);
		~TResult() = default;

		inline R Get();
		inline bool IsValid() const;
		inline void Wait() const;

		inline void operator()(Args... args) const;

		inline TResult& operator=(TResult&&) = delete;
		inline TResult& operator=(const TResult&) = delete;
	};
}

#include <Awesome/Core/Thread/TResult.inl>

#endif // GUARD