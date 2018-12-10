#pragma once

#ifndef AWESOME_SDK_DIRECTIONAL_LIGHT_GUARD
#define AWESOME_SDK_DIRECTIONAL_LIGHT_GUARD

#include <Awesome/SDK/Components/Render/Light.hpp>

namespace aw
{
	namespace sdk
	{
		class DirectionalLight : public Light
		{
			friend class RenderSystem;

		protected:
		public:
			DirectionalLight() = delete;
			inline explicit DirectionalLight(Actor* actor);
			DirectionalLight(DirectionalLight&&) = delete;
			DirectionalLight(const DirectionalLight&) = delete;
			~DirectionalLight() override = default;

			inline DirectionalLight& operator=(DirectionalLight&&) = delete;
			inline DirectionalLight& operator=(const DirectionalLight&) = delete;

			REFLECT_BEGIN(DirectionalLight)
				REFLECT_MEMBER_INHERITANCE(Light)
			REFLECT_END()
		};
	}

	template <>
	class TypeInfo<sdk::DirectionalLight>
	{
	public:
		static const uint32 id = uint32(sdk::ComponentID::DirectionalLight);
	};
}

#include <Awesome/SDK/Components/Render/DirectionalLight.inl>

#endif // GUARD