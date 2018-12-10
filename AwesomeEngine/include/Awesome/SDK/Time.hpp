#pragma once

#ifndef AWESOME_SDK_TIME_GUARD
#define AWESOME_SDK_TIME_GUARD

#include <Awesome/Core/Chrono.hpp>

namespace aw
{
	namespace sdk
	{
		class Time
		{
			Chrono m_chrono;

			float m_globalTime = 0.0f;
			float m_deltaTime = 0.0f;
			float m_unscaledDeltaTime = 0.0f;
			float m_timeScale = 1.0f;

			inline Time();

			static inline Time& GetInstance();

		public:
			~Time() = default;

			inline static float DeltaTime();
			inline static float UnscaledDeltaTime();
			inline static float GlobalTime();
			inline static float TimeScale();

			inline static void SetTimeScale(float value);

			inline static void Play();
			inline static void Pause();
			inline static void Update();
			inline static void Reset();
		};
	}
}

#include <Awesome/SDK/Time.inl>

#endif // GUARD