#pragma once

#ifndef AWESOME_SDK_HEIGHT_FIELD_COLLIDER_GUARD
#define AWESOME_SDK_HEIGHT_FIELD_COLLIDER_GUARD

#include <Awesome/SDK/Components/Physical/Collider.hpp>

namespace aw
{
	namespace sdk
	{
		class HeightFieldCollider : public Collider
		{
		public:
			inline HeightFieldCollider(Actor* actor, Vector3 scale);
			~HeightFieldCollider() override = default;
		};
	}

	template <>
	class TypeInfo<sdk::HeightFieldCollider>
	{
	public:
		static const uint32 id = static_cast<uint32>(sdk::ComponentID::HeightField);
	};
}

#include <Awesome/SDK/Components/Physical/HeightFieldCollider.inl>

#endif // GUARD