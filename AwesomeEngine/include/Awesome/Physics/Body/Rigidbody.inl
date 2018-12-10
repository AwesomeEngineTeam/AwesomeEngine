namespace aw
{
	namespace ph
	{
		inline Rigidbody::Rigidbody(Rigidbody&& other) : m_handle{ std::move(other.m_handle) }
		{
			other.m_handle = nullptr;
		}

		inline Rigidbody::~Rigidbody()
		{
			Destroy();
		}

		inline physx::PxRigidActor* Rigidbody::Get()
		{
			return m_handle;
		}

		inline const physx::PxRigidActor* Rigidbody::Get() const
		{
			return m_handle;
		}

		inline physx::PxActor* Rigidbody::GetAsActor()
		{
			return m_handle;
		}

		inline const physx::PxActor* Rigidbody::GetAsActor() const
		{
			return m_handle;
		}

		inline void Rigidbody::SetGlobalTransform(TransformBase tr)
		{
			// Awesome to physX reference
			tr.SetRotation(tr.GetRotation() * Quaternion(M_PI2, Vector3(0.0f, 0.0f, 1.0f)));

			Quaternion rot = tr.GetRotation();
			Vector3 pos = tr.GetPosition();

			physx::PxTransform pxTr(reinterpret_cast<const physx::PxVec3&>(pos), physx::PxQuat(rot.x, rot.y, rot.z, rot.w));
			m_handle->setGlobalPose(pxTr);
		}

		inline bool Rigidbody::GetUseGravity() const
		{
			return !m_handle->getActorFlags().isSet(physx::PxActorFlag::eDISABLE_GRAVITY);
		}

		inline void Rigidbody::SetUseGravity(bool b)
		{
			m_handle->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !b);
		}

		inline bool Rigidbody::IsActive() const
		{
			return !m_handle->getActorFlags().isSet(physx::PxActorFlag::eDISABLE_SIMULATION);
		}

		inline void Rigidbody::SetActive(bool b)
		{
			m_handle->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !b);
		}

		template <typename T>
		inline T* Rigidbody::GetUserData()
		{
			return reinterpret_cast<T*>(m_userData);
		}

		template <typename T>
		inline const T* Rigidbody::GetUserData() const
		{
			return reinterpret_cast<const T*>(m_userData);
		}

		inline sdk::PhysicalComponent* Rigidbody::GetPhysicalComponent()
		{
			return reinterpret_cast<sdk::PhysicalComponent*>(m_userData);
		}

		inline const sdk::PhysicalComponent* Rigidbody::GetPhysicalComponent() const
		{
			return reinterpret_cast<const sdk::PhysicalComponent*>(m_userData);
		}

		inline void Rigidbody::Destroy()
		{
			if (!m_handle)
				return;

			ClearShapes();

			m_handle->release();
			//delete[] reinterpret_cast<void**>(m_handle->userData);

			m_handle = nullptr;
		}

		inline Rigidbody& Rigidbody::operator=(Rigidbody&& rhs)
		{
			Destroy();

			m_handle = rhs.m_handle;
			rhs.m_handle = nullptr;

			return *this;
		}
	}
}