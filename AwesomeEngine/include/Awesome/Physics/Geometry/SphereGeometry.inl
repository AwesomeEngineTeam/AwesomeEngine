namespace aw
{
	namespace ph
	{
		inline SphereGeometry::SphereGeometry(float radius) : m_handle(radius)
		{
		}

		inline SphereGeometry::SphereGeometry(SphereGeometry&& other) : m_handle{std::move(other.m_handle)}
		{
		}

		inline SphereGeometry::SphereGeometry(const SphereGeometry& other) : m_handle{ other.m_handle }
		{
		}

		inline GeometryType SphereGeometry::GetType() const
		{
			return GeometryType(m_handle.getType());
		}

		inline physx::PxGeometry& SphereGeometry::Get()
		{
			return m_handle;
		}

		inline const physx::PxGeometry& SphereGeometry::Get() const
		{
			return m_handle;
		}

		inline bool SphereGeometry::IsValid() const
		{
			return m_handle.isValid();
		}

		inline float& SphereGeometry::Radius()
		{
			return m_handle.radius;
		}

		inline float SphereGeometry::Radius() const
		{
			return m_handle.radius;
		}

		inline SphereGeometry& SphereGeometry::operator=(SphereGeometry&& rhs)
		{
			m_handle = std::move(rhs.m_handle);
			
			return *this;
		}

		inline SphereGeometry& SphereGeometry::operator=(const SphereGeometry& rhs)
		{
			m_handle = rhs.m_handle;

			return *this;
		}
	}
}