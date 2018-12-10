#pragma once

#ifndef AWESOME_SDK_RIGIDBODY_GUARD
#define AWESOME_SDK_RIGIDBODY_GUARD

#include <Awesome/SDK/Components/Physical/PhysicalComponent.hpp>

namespace aw
{
	namespace sdk
	{
		class Rigidbody : public PhysicalComponent
		{
		private:
			//inline void UpdateRadius();
			inline void SetRadius(float f);
			//inline void UpdateHalfHeight();
			inline void SetHalfHeight(float f);
			//inline void UpdateHalfScale();
			inline void SetHalfScale(const Vector3& v);

			struct Force
			{
				Vector3 force;
				ph::ForceMode mode;

				Force(Vector3 force, ph::ForceMode mode) : force{ force }, mode{ mode }
				{
				}
			};

			std::vector<Force> m_linearForces;
			std::vector<Force> m_angularForces;

			std::mutex linearForcesMutex;
			std::mutex angularForcesMutex;

		protected:
			inline UniformTransform GetUpdatedGlobalTransform() override;
			inline bool UpdateBuffering() override;

			std::atomic_bool isKinematic = false; 

		public:
			std::atomic_bool useGravity = true;
			std::atomic<ph::CollisionDetection> collisionDetectionMode = ph::CollisionDetection::Discreet;

			std::atomic<float> mass = 10.0f;

			Vector3 linearVelocity = Vector3::Zero();
			Vector3 angularVelocity = Vector3::Zero();

			std::atomic<float> radius = 0.5f;
			std::atomic<float> halfHeight = 0.5f;
			Vector3 halfScale = Vector3(0.5f);

			std::atomic_uint8_t lockAxisMask = 0u;

			#if defined(WITH_EDITOR)
				std::atomic<ph::GeometryType> geometryType;
				inline void SetGeometry(ph::GeometryType type);
			#else
				const ph::GeometryType geometryType;
			#endif

			inline Rigidbody(Actor* actor, const ph::Geometry& geo = ph::CapsuleGeometry(), const ph::Material& mat = ph::Material(0.5f, 0.5f, 0.2f), float mass = 10.0f, bool useGravity = true);
			~Rigidbody() override = default;

			inline bool IsKinematic() const;
			inline void SetKinematic(bool b = true);

			inline void AddForce(const Vector3& force, ph::ForceMode mode);
			inline void AddTorque(const Vector3& torque, ph::ForceMode mode);

			REFLECT_BEGIN(Rigidbody)
				REFLECT_MEMBER_INHERITANCE(PhysicalComponent)
				REFLECT_MEMBER(isKinematic)
				REFLECT_MEMBER(useGravity)
				REFLECT_MEMBER(radius)
				REFLECT_MEMBER(halfHeight)
				REFLECT_MEMBER(halfScale)
				REFLECT_MEMBER(isKinematic)
				REFLECT_MEMBER(geometryType)
			REFLECT_END()
		};
	}

	template <>
	class TypeInfo<sdk::Rigidbody>
	{
	public:
		static const uint32 id = static_cast<uint32>(sdk::ComponentID::Rigidbody);
	};
}

#include <Awesome/SDK/Components/Physical/Rigidbody.inl>

#endif // GUARD