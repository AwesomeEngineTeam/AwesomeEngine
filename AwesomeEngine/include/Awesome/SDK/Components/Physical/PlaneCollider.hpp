#pragma once

#ifndef AWESOME_SDK_PLANE_COLLIDER_GUARD
#define AWESOME_SDK_PLANE_COLLIDER_GUARD

#include <Awesome/Physics/Geometry/PlaneGeometry.hpp>
#include <Awesome/SDK/Components/Physical/Collider.hpp>

namespace aw
{
	namespace sdk
	{
		class PlaneCollider : public Collider
		{
		public:
			inline PlaneCollider(Actor* actor);
			~PlaneCollider() override = default;

			REFLECT_BEGIN(PlaneCollider)
				REFLECT_MEMBER_INHERITANCE(Collider)
			REFLECT_END()
		};
	}

	template <>
	class TypeInfo<sdk::PlaneCollider>
	{
	public:
		static const uint32 id = static_cast<uint32>(sdk::ComponentID::PlaneCollider);
	};
}

#include <Awesome/SDK/Components/Physical/PlaneCollider.inl>

#endif // GUARD