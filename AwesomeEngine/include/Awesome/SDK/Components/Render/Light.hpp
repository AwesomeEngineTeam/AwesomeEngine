#pragma once

#ifndef AWESOME_SDK_LIGHT_GUARD
#define AWESOME_SDK_LIGHT_GUARD

#include <Awesome/Core/Color.hpp>
#include <Awesome/SDK/Components/Component.hpp>

namespace aw
{
	namespace sdk
	{
		class Light : public Component
		{
			friend class RenderSystem;

		public:
			Color3f color{ Color3f::White() };
			float intensity{ 16.0f };

		protected:
			inline Light(Actor* actor, uint32 typeId, const std::string& name = defaultName);

		public:
			Light() = delete;
			inline explicit Light(Actor* actor);
			Light(Light&&) = delete;
			Light(const Light&) = delete;
			~Light() override = default;

			inline Light& operator=(Light&&) = delete;
			inline Light& operator=(const Light&) = delete;

			REFLECT_BEGIN(Light)
				REFLECT_MEMBER_INHERITANCE(Component)
				REFLECT_MEMBER(color)
				REFLECT_MEMBER(intensity)
			REFLECT_END()
		};
	}

	template <>
	class TypeInfo<sdk::Light>
	{
	public:
		static const uint32 id = uint32(sdk::ComponentID::Light);
	};
}

#include <Awesome/SDK/Components/Render/Light.inl>

#endif // GUARD
