#pragma once

#ifndef AWESOME_SDK_SCRIPT_SYSTEM_GUARD
#define AWESOME_SDK_SCRIPT_SYSTEM_GUARD

#include <Awesome/SDK/Components/Lua/Script.hpp>

#include <vector>

namespace aw
{
	namespace sdk
	{
		class ScriptSystem
		{
			static std::vector<Script*> s_scripts;
			static std::vector<std::function<void()>> s_events;

			static std::mutex s_mutex;
			static std::mutex s_eventMutex;

			static AWESOME_SDK_API void GarbageCollect();

			static inline void ProcessEvents();

		public:
			ScriptSystem() = delete;
			virtual ~ScriptSystem() = default;

			static inline bool Register(Component*);
			//static inline void Unregister(Component*);

			static AWESOME_SDK_API void Start(std::atomic_bool& shouldUpdate);

			static inline void OnSceneStart();
			static inline void Update(float elapsedTime);

			static inline void AddEvent(const std::function<void()>& event);
		};
	}
}

#include <Awesome/SDK/System/ScriptSystem.inl>

#endif