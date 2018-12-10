#pragma once

#ifndef AWESOME_SDK_BOX_COLLIDER_GUARD
#define AWESOME_SDK_BOX_COLLIDER_GUARD

#include <Awesome/SDK/Components/Physical/Collider.hpp>

#include <Awesome/Physics/Geometry/BoxGeometry.hpp>

namespace aw
{
	namespace sdk
	{
		class BoxCollider : public Collider
		{
		protected:
			inline bool UpdateBuffering() override;

		public:
			inline BoxCollider(Actor* actor, Vector3 halfScale = Vector3(0.5f));
			~BoxCollider() override = default;

			Vector3 halfScale;

			REFLECT_BEGIN(BoxCollider)
				REFLECT_MEMBER_INHERITANCE(Collider)
				REFLECT_MEMBER(halfScale)
			REFLECT_END()
		};
	}

	template <>
	class TypeInfo<sdk::BoxCollider>
	{
	public:
		static const uint32 id = static_cast<uint32>(sdk::ComponentID::BoxCollider);
	};
}

#include <Awesome/SDK/Components/Physical/BoxCollider.inl>

#endif // GUARD