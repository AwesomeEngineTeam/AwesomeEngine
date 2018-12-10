#pragma once

#ifndef AWESOME_PHYSICS_RIGIDBODY_DYNAMIC_GUARD
#define AWESOME_PHYSICS_RIGIDBODY_DYNAMIC_GUARD

#include <physX/PxRigidDynamic.h>

#include <Awesome/Physics/Body/Rigidbody.hpp>

namespace aw
{
	namespace ph
	{
		enum class CollisionDetection : uint32
		{
			Discreet = 0,
			Continuous = 1,
		};

		enum class LockAxis : uint8
		{
			LockNone = 0,
			LockLinearX = physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X,
			LockLinearY = physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y,
			LockLinearZ = physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z,
			LockAngularX = physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X,
			LockAngularY = physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y,
			LockAngularZ = physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z,
			LockAllLinear = 7,
			LockAllAngular = 56,
			LockAll = 63,
		};

		class RigidbodyDynamic : public Rigidbody
		{
		public:
			RigidbodyDynamic() = default;
			inline RigidbodyDynamic(const TransformBase& tr, void* userData, bool useGravity = true);
			inline RigidbodyDynamic(const TransformBase& tr, const Geometry& geo, const Material& mat, float mass, void* userData, bool useGravity = true);
			inline RigidbodyDynamic(RigidbodyDynamic&& other);
			RigidbodyDynamic(const RigidbodyDynamic&) = delete;
			~RigidbodyDynamic() = default;

			inline bool IsSleeping() const;
			inline bool IsKinematic() const;
			inline void SetKinematic(bool b = true);

			inline void SetLockAxis(uint8 lockMask);

			inline float GetMass() const;
			inline void SetMass(float mass);
			inline Vector3 GetLinearVelocity() const;
			inline void SetLinearVelocity(const Vector3& vel);
			inline Vector3 GetAngularVelocity() const;
			inline void SetAngularVelocity(const Vector3& vel);
			inline void SetCollisionDetection(CollisionDetection mode);

			inline void AddForce(const Vector3& force, ForceMode mode);
			inline void AddTorque(const Vector3& torque, ForceMode mode);

			AWESOME_PHYSICS_API void Create(const TransformBase& tr, void* userData) override;

			inline RigidbodyDynamic& operator=(RigidbodyDynamic&& rhs);
			RigidbodyDynamic& operator=(const RigidbodyDynamic& rhs) = delete;
		};
	}
}

#include <Awesome/Physics/Body/RigidbodyDynamic.inl>

#endif // GUARD