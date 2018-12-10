#pragma once

#ifndef AWESOME_SDK_BASE_APPLICATION_GUARD
#define AWESOME_SDK_BASE_APPLICATION_GUARD

#include <Awesome/Core/Debug.hpp>

#include <Awesome/SDK/Time.hpp>
#include <Awesome/SDK/Config.hpp>
#include <Awesome/SDK/Scene/SceneManager.hpp>
#include <Awesome/SDK/ThreadManager.hpp>

#include <Awesome/Windowing/Input/InputManager.hpp>

namespace aw
{
	namespace sdk
	{
		class BaseApplication
		{
			static inline std::atomic_bool s_quit = false;

		protected:
			std::atomic_bool m_shouldUpdateSound = true;
			std::atomic_bool m_shouldUpdateScripts = false;
			std::atomic_bool m_shouldUpdateGraphics = true;

			Chrono m_inputChrono{};

		public:
			BaseApplication() = default;
			BaseApplication(BaseApplication&&) = delete;
			BaseApplication(const BaseApplication&) = delete;
			virtual ~BaseApplication() = default;

			inline void Launch();
			static inline void Quit();

			static inline bool ShouldQuit();

			inline virtual void Start();
			inline virtual void Update();
			inline virtual void End();

			BaseApplication& operator=(BaseApplication&&) = delete;
			BaseApplication& operator=(const BaseApplication&) = delete;
		};
	}
}

#include <Awesome/SDK/BaseApplication.inl>

#endif // GUARD