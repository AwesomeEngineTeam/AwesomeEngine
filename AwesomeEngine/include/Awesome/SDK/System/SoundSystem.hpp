#pragma once

#ifndef AWESOME_SDK_SOUND_SYSTEM_GUARD
#define AWESOME_SDK_SOUND_SYSTEM_GUARD

#include <Awesome/SDK/Components/Sound/SoundListener.hpp>
#include <Awesome/SDK/Components/Sound/SoundEmitter.hpp>

#include <vector>

namespace aw
{
	namespace sdk
	{
		class SoundSystem
		{
			static AkGameObjectID s_count;

			static std::vector<SoundListener*> s_listeners;
			static std::vector<SoundEmitter*> s_emitters;

			static std::mutex s_mutex;

			static inline AkSoundPosition GetPosition(Component* component);

			static AWESOME_SDK_API void GarbageCollect();

		public:
			SoundSystem() = delete;
			virtual ~SoundSystem() = default;

			static inline bool Register(Component*);
			//static inline void Unregister(Component*);

			static AWESOME_SDK_API void Start(std::atomic_bool& shouldUpdate);
			static inline void Update();
		};
	}
}

#include <Awesome/SDK/System/SoundSystem.inl>

#endif //GUARD