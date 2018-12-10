namespace aw
{
	namespace ph
	{
		inline BoxGeometry::BoxGeometry(Vector3 halfScale) : m_handle{ halfScale.y, halfScale.x, halfScale.z } // Switch to physX landmarks.
		{
		}

		inline BoxGeometry::BoxGeometry(BoxGeometry&& other) : m_handle(std::move(other.m_handle))
		{
		}

		inline BoxGeometry::BoxGeometry(const BoxGeometry& other) : m_handle(other.m_handle)
		{
		}

		inline GeometryType BoxGeometry::GetType() const
		{
			return GeometryType(m_handle.getType());
		}

		inline physx::PxGeometry& BoxGeometry::Get()
		{
			return m_handle;
		}

		inline const physx::PxGeometry& BoxGeometry::Get() const
		{
			return m_handle;
		}

		inline bool BoxGeometry::IsValid() const
		{
			return m_handle.isValid();
		}

		inline Vector3 BoxGeometry::GetHalfScale() const
		{
			// Switch to aw landmarks.
			return Vector3(m_handle.halfExtents.y, m_handle.halfExtents.x, m_handle.halfExtents.z);
		}

		inline void BoxGeometry::SetHalfScale(Vector3 halfScale)
		{
			// Switch to physX landmarks.
			m_handle.halfExtents = { halfScale.y, halfScale.x, halfScale.z };
		}

		inline BoxGeometry& BoxGeometry::operator=(BoxGeometry&& rhs)
		{
			m_handle = std::move(rhs.m_handle);

			return *this;
		}

		inline BoxGeometry& BoxGeometry::operator=(const BoxGeometry& rhs)
		{
			m_handle = rhs.m_handle;

			return *this;
		}
	}
}