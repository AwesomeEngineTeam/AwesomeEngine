#pragma once

#ifndef AWESOME_SDK_SPHERE_COLLIDER_GUARD
#define AWESOME_SDK_SPHERE_COLLIDER_GUARD

#include <Awesome/SDK/Components/Physical/Collider.hpp>

namespace aw
{
	namespace sdk
	{
		class SphereCollider : public Collider
		{
		protected:
			inline bool UpdateBuffering() override;

		public:
			inline SphereCollider(Actor* actor, float radius = 0.5f);
			~SphereCollider() override = default;

			std::atomic<float> radius;

			REFLECT_BEGIN(SphereCollider)
				REFLECT_MEMBER_INHERITANCE(Collider)
				REFLECT_MEMBER(radius)
			REFLECT_END()
		};
	}

	template <>
	class TypeInfo<sdk::SphereCollider>
	{
	public:
		static const uint32 id = static_cast<uint32>(sdk::ComponentID::SphereCollider);
	};
}

#include <Awesome/SDK/Components/Physical/SphereCollider.inl>

#endif // GUARD