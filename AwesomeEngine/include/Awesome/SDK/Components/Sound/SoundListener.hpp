#pragma once

#ifndef AWESOME_SDK_SOUND_LISTENER_GUARD
#define AWESOME_SDK_SOUND_LISTENER_GUARD

#include <Awesome/SDK/Components/Component.hpp>
#include <Awesome/Sound/Sound.hpp>

namespace aw
{
	namespace sdk
	{
		class SoundListener : public Component
		{
			AkGameObjectID m_id;
			friend class SoundSystem;

		public:
			inline SoundListener(Actor* actor);
		};
	}

	template <>
	class TypeInfo<sdk::SoundListener>
	{
	public:
		static const uint32 id = uint32(sdk::ComponentID::SoundListener);
	};
}

#include <Awesome/SDK/Components/Sound/SoundListener.inl>

#endif //Guard