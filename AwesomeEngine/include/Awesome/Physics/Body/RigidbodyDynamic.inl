namespace aw
{
	namespace ph
	{
		inline RigidbodyDynamic::RigidbodyDynamic(const TransformBase& tr, void* userData, bool useGravity)
		{
			Create(tr, userData);
			SetUseGravity(useGravity);
		}

		inline RigidbodyDynamic::RigidbodyDynamic(const TransformBase& tr, const Geometry& geo, const Material& mat, float mass, void* userData, bool useGravity)
		{
			Create(tr, userData);
			CreateShape(geo, mat);
			SetUseGravity(useGravity);
		}

		inline RigidbodyDynamic::RigidbodyDynamic(RigidbodyDynamic&& other) : Rigidbody(std::move(other))
		{
		}

		inline bool RigidbodyDynamic::IsSleeping() const
		{
			return reinterpret_cast<physx::PxRigidDynamic*>(m_handle)->isSleeping();
		}

		inline bool RigidbodyDynamic::IsKinematic() const
		{
			return reinterpret_cast<physx::PxRigidDynamic*>(m_handle)->getRigidBodyFlags().isSet(physx::PxRigidBodyFlag::eKINEMATIC);
		}

		inline void RigidbodyDynamic::SetKinematic(bool b)
		{
			reinterpret_cast<physx::PxRigidDynamic*>(m_handle)->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, b);
		}

		inline void RigidbodyDynamic::SetLockAxis(uint8 lockMask)
		{
			reinterpret_cast<physx::PxRigidDynamic*>(m_handle)->setRigidDynamicLockFlags(static_cast<physx::PxRigidDynamicLockFlags>(lockMask));
		}

		inline float RigidbodyDynamic::GetMass() const
		{
			return reinterpret_cast<physx::PxRigidDynamic*>(m_handle)->getMass();
		}

		inline void RigidbodyDynamic::SetMass(float mass)
		{
			reinterpret_cast<physx::PxRigidDynamic*>(m_handle)->setMass(mass);
		}

		inline Vector3 RigidbodyDynamic::GetLinearVelocity() const
		{
			auto temp = reinterpret_cast<physx::PxRigidDynamic*>(m_handle)->getLinearVelocity();
			return reinterpret_cast<const Vector3&>(temp);
		}

		inline void RigidbodyDynamic::SetLinearVelocity(const Vector3& vel)
		{
			reinterpret_cast<physx::PxRigidDynamic*>(m_handle)->setLinearVelocity(reinterpret_cast<const physx::PxVec3&>(vel));
		}

		inline Vector3 RigidbodyDynamic::GetAngularVelocity() const
		{
			auto temp = reinterpret_cast<physx::PxRigidDynamic*>(m_handle)->getAngularVelocity();
			return reinterpret_cast<const Vector3&>(temp);
		}

		inline void RigidbodyDynamic::SetAngularVelocity(const Vector3& vel)
		{
			reinterpret_cast<physx::PxRigidDynamic*>(m_handle)->setAngularVelocity(reinterpret_cast<const physx::PxVec3&>(vel));
		}

		inline void RigidbodyDynamic::SetCollisionDetection(CollisionDetection mode)
		{
			reinterpret_cast<physx::PxRigidDynamic*>(m_handle)->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, mode == CollisionDetection::Continuous);
		}

		inline void RigidbodyDynamic::AddForce(const Vector3& force, ForceMode mode)
		{
			reinterpret_cast<physx::PxRigidDynamic*>(m_handle)->addForce(reinterpret_cast<const physx::PxVec3&>(force), physx::PxForceMode::Enum(mode));
		}
		inline void RigidbodyDynamic::AddTorque(const Vector3& torque, ForceMode mode)
		{
			reinterpret_cast<physx::PxRigidDynamic*>(m_handle)->addTorque(reinterpret_cast<const physx::PxVec3&>(torque), physx::PxForceMode::Enum(mode));
		}

		inline RigidbodyDynamic& RigidbodyDynamic::operator=(RigidbodyDynamic&& rhs)
		{
			Rigidbody::operator=(std::move(rhs));

			return *this;
		}
	}
}