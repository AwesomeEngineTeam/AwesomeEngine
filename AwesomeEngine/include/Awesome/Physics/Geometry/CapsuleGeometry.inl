namespace aw
{
	namespace ph
	{
		inline CapsuleGeometry::CapsuleGeometry(float radius, float halfHeight) : m_handle(radius, halfHeight)
		{
		}

		inline CapsuleGeometry::CapsuleGeometry(CapsuleGeometry&& other) : m_handle{ std::move(other.m_handle) }
		{
		}

		inline CapsuleGeometry::CapsuleGeometry(const CapsuleGeometry& other) : m_handle{ other.m_handle }
		{

		}

		inline GeometryType CapsuleGeometry::GetType() const
		{
			return GeometryType(m_handle.getType());
		}

		inline physx::PxGeometry& CapsuleGeometry::Get()
		{
			return m_handle;
		}

		inline const physx::PxGeometry& CapsuleGeometry::Get() const
		{
			return m_handle;
		}

		inline bool CapsuleGeometry::IsValid() const
		{
			return m_handle.isValid();
		}

		inline float& CapsuleGeometry::Radius()
		{
			return m_handle.radius;
		}

		inline float CapsuleGeometry::Radius() const
		{
			return m_handle.radius;
		}

		inline float& CapsuleGeometry::HalfHeight()
		{
			return m_handle.halfHeight;
		}

		inline float CapsuleGeometry::HalfHeight() const
		{
			return m_handle.halfHeight;
		}

		inline CapsuleGeometry& CapsuleGeometry::operator=(CapsuleGeometry&& rhs)
		{
			m_handle = std::move(rhs.m_handle);

			return *this;
		}

		inline CapsuleGeometry& CapsuleGeometry::operator=(const CapsuleGeometry& rhs)
		{
			m_handle = rhs.m_handle;

			return *this;
		}
	}
}