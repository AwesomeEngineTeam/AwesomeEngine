#pragma once

#ifndef AWESOME_CORE_CHRONO_GUARD
#define AWESOME_CORE_CHRONO_GUARD

#include <Awesome/Core/Config.hpp>
#include <Awesome/Core/Types.hpp>

#include <chrono>

namespace aw
{
	class Chrono
	{
		std::chrono::time_point<std::chrono::steady_clock> m_timeStamp;
		decltype(m_timeStamp)::duration m_elapsedTime{ decltype(m_elapsedTime)::zero() };

		using clock = decltype(m_timeStamp)::clock;
		using duration = decltype(m_elapsedTime);

		bool paused = true;

	public:
		Chrono() = default;
		~Chrono() = default;

		inline void Play();
		inline void Pause();

		inline void Restart();

		template<typename Format>
		inline constexpr double GetElapsedTime();

		typedef std::nano Nanoseconds;
		typedef std::micro Microseconds;
		typedef std::milli Milliseconds;
		typedef std::ratio<1> Seconds;
	};
}

#include <Awesome/Core/Chrono.inl>

#endif //Guard