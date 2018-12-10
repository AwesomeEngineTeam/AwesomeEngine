#pragma once

#ifndef AWESOME_SDK_POINT_LIGHT_GUARD
#define AWESOME_SDK_POINT_LIGHT_GUARD

#include <Awesome/SDK/Components/Render/Light.hpp>

namespace aw
{
	namespace sdk
	{
		class PointLight : public Light
		{
			friend class RenderSystem;

		protected:
		public:
			PointLight() = delete;
			inline explicit PointLight(Actor* actor);
			PointLight(PointLight&&) = delete;
			PointLight(const PointLight&) = delete;
			~PointLight() override = default;

			inline PointLight& operator=(PointLight&&) = delete;
			inline PointLight& operator=(const PointLight&) = delete;

			REFLECT_BEGIN(PointLight)
				REFLECT_MEMBER_INHERITANCE(Light)
			REFLECT_END()
		};
	}

	template <>
	class TypeInfo<sdk::PointLight>
	{
	public:
		static const uint32 id = uint32(sdk::ComponentID::PointLight);
	};
}

#include <Awesome/SDK/Components/Render/PointLight.inl>

#endif // GUARD
