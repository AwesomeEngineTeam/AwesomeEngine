namespace aw
{
	namespace ph
	{
		inline PlaneGeometry::PlaneGeometry(PlaneGeometry&& other) : m_handle{ std::move(other.m_handle) }
		{
		}

		inline PlaneGeometry::PlaneGeometry(const PlaneGeometry& other) : m_handle{ other.m_handle }
		{
		}

		inline GeometryType PlaneGeometry::GetType() const
		{
			return GeometryType(m_handle.getType());
		}

		inline physx::PxGeometry& PlaneGeometry::Get()
		{
			return m_handle;
		}

		inline const physx::PxGeometry& PlaneGeometry::Get() const
		{
			return m_handle;
		}

		inline bool PlaneGeometry::IsValid() const
		{
			return m_handle.isValid();
		}

		inline PlaneGeometry& PlaneGeometry::operator=(PlaneGeometry&& rhs)
		{
			m_handle = std::move(rhs.m_handle);

			return *this;
		}

		inline PlaneGeometry& PlaneGeometry::operator=(const PlaneGeometry& rhs)
		{
			m_handle = rhs.m_handle;

			return *this;
		}
	}
}