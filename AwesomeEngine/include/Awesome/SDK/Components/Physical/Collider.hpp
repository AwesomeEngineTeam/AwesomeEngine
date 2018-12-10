#pragma once

#ifndef AWESOME_SDK_COLLIDER_GUARD
#define AWESOME_SDK_COLLIDER_GUARD

#include <Awesome/Physics/Body/RigidbodyStatic.hpp>

#include <Awesome/SDK/Components/Physical/PhysicalComponent.hpp>

namespace aw
{
	namespace sdk
	{
		class Collider : public PhysicalComponent
		{
		protected:
			inline Collider(Actor* actor, uint32 typeId, const ph::Geometry& geo, const std::string& name);

			inline bool UpdateBuffering() override;

		public:
			std::atomic_bool isTrigger = false;

			Collider() = delete;
			~Collider() override = default;

			REFLECT_BEGIN(Collider)
				REFLECT_MEMBER_INHERITANCE(PhysicalComponent)
				REFLECT_MEMBER(isTrigger)
			REFLECT_END()
		};
	}

	template <>
	class TypeInfo<sdk::Collider>
	{
	public:
		static const uint32 id = static_cast<uint32>(sdk::ComponentID::Collider);
	};
}

#include <Awesome/SDK/Components/Physical/Collider.inl>

#endif // GUARD