#pragma once

#ifndef AWESOME_SDK_CAPSULE_COLLIDER_GUARD
#define AWESOME_SDK_CAPSULE_COLLIDER_GUARD

#include <Awesome/SDK/Components/Physical/Collider.hpp>

namespace aw
{
	namespace sdk
	{
		class CapsuleCollider : public Collider
		{
		protected:
			inline bool UpdateBuffering() override;

		public:
			inline CapsuleCollider(Actor* actor, float radius = 0.5f, float halfHeight = 0.5f);
			~CapsuleCollider() override = default;

			std::atomic<float> radius;
			std::atomic<float> halfHeight;

			REFLECT_BEGIN(CapsuleCollider)
				REFLECT_MEMBER_INHERITANCE(Collider)
				REFLECT_MEMBER(radius)
				REFLECT_MEMBER(halfHeight)
			REFLECT_END()
		};
	}

	template <>
	class TypeInfo<sdk::CapsuleCollider>
	{
	public:
		static const uint32 id = static_cast<uint32>(sdk::ComponentID::CapsuleCollider);
	};
}

#include <Awesome/SDK/Components/Physical/CapsuleCollider.inl>

#endif // GUARD