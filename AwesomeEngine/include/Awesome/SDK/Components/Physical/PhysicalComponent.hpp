#pragma once

#ifndef AWESOME_SDK_PHYSICAL_COMPONENT_GUARD
#define AWESOME_SDK_PHYSICAL_COMPONENT_GUARD

#include <Awesome/Physics/Body/RigidbodyDynamic.hpp>
#include <Awesome/Physics/Body/RigidbodyStatic.hpp>

#include <Awesome/SDK/Components/Component.hpp>

namespace aw
{
	namespace sdk
	{
		enum class CollisionLayer : uint32
		{
			NoCollision = 0,
			Layer01 = (1 << 0),
			Layer02 = (1 << 1),
			Layer03 = (1 << 2),
			Layer04 = (1 << 3),
			Layer05 = (1 << 4),
			Layer06 = (1 << 5),
			Layer07 = (1 << 6),
			Layer08 = (1 << 7),
			Layer09 = (1 << 8),
			Layer10 = (1 << 9),
			Layer11 = (1 << 10),
			Layer12 = (1 << 11),
			Layer13 = (1 << 12),
			Layer14 = (1 << 13),
			Layer15 = (1 << 14),
			Layer16 = (1 << 15),
			Layer17 = (1 << 16),
			Layer18 = (1 << 17),
			Layer19 = (1 << 18),
			Layer20 = (1 << 19),
			Layer21 = (1 << 20),
			Layer22 = (1 << 21),
			Layer23 = (1 << 22),
			Layer24 = (1 << 23),
			Layer25 = (1 << 24),
			Layer26 = (1 << 25),
			Layer27 = (1 << 26),
			Layer28 = (1 << 27),
			Layer29 = (1 << 28),
			Layer30 = (1 << 29),
			Layer31 = (1 << 30),
			Layer32 = (1 << 31),
			All = ~0u,
		};

		class PhysicalComponent : public Component
		{
			friend class PhysicalSystem;

		protected:
			ph::Rigidbody* m_body = nullptr;

			ph::Shape m_shape;

		public:
			std::atomic<CollisionLayer> collisionLayer = CollisionLayer::Layer01;
			std::atomic_uint32_t filterMask = ~0u;

			ph::Material material;

			PhysicalComponent() = delete;
			PhysicalComponent(PhysicalComponent&&) = delete;
			PhysicalComponent(const PhysicalComponent&) = delete;
			inline ~PhysicalComponent() override;

			inline static void TriggerCallback(void* comp1, void* comp2);
			inline static void CollisionCallback(void* comp1, void* comp2);

		protected:
			inline virtual UniformTransform GetUpdatedGlobalTransform();
			inline virtual bool UpdateBuffering();

			void OnTriggerCallback(Component* other);
			void OnCollisionCallback(Component* other);

			inline PhysicalComponent(Actor* actor, uint32 typeId, bool isStatic, bool useGravity, const ph::Material& mat, const std::string& name, const UniformTransform& localTransform = UniformTransform::Identity());

			REFLECT_BEGIN(PhysicalComponent)
				REFLECT_MEMBER_INHERITANCE(Component)
				REFLECT_MEMBER(collisionLayer)
				REFLECT_MEMBER(filterMask)
				REFLECT_MEMBER(material)
			REFLECT_END()
		};
	}
}

#include <Awesome/SDK/Components/Physical/PhysicalComponent.inl>

#endif // GUARD