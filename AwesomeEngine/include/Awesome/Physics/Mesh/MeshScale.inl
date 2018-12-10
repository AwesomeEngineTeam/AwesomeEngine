namespace aw
{
	namespace ph
	{
		inline MeshScale::MeshScale(float scale) : m_handle(scale)
		{
		}

		inline MeshScale::MeshScale(const Vector3& scale, const Quaternion& rot) :
			m_handle(reinterpret_cast<const physx::PxVec3&>(scale), physx::PxQuat(rot.x, rot.y, rot.z, rot.w))
		{
		}

		inline MeshScale::MeshScale(MeshScale&& other) : m_handle{ std::move(other.m_handle) }
		{
		}

		inline MeshScale::MeshScale(const MeshScale& other) : m_handle{ other.m_handle }
		{
		}

		inline const physx::PxMeshScale& MeshScale::Get() const
		{
			return m_handle;
		}

		inline Vector3& MeshScale::Scale()
		{
			return reinterpret_cast<Vector3&>(m_handle.scale);
		}

		inline const Vector3& MeshScale::Scale() const
		{
			return reinterpret_cast<const Vector3&>(m_handle.scale);
		}

		inline Quaternion MeshScale::GetRotation() const
		{
			return Quaternion(m_handle.rotation.w, m_handle.rotation.x, m_handle.rotation.y, m_handle.rotation.z);
		}

		inline void MeshScale::SetRotation(const Quaternion& rotation)
		{
			m_handle.rotation = physx::PxQuat(rotation.x, rotation.y, rotation.z, rotation.w);
		}

		inline  MeshScale& MeshScale::operator=(MeshScale&& rhs)
		{
			m_handle = std::move(rhs.m_handle);

			return *this;
		}

		inline MeshScale& MeshScale::operator=(const MeshScale& rhs)
		{
			m_handle = rhs.m_handle;

			return *this;
		}
	}
}